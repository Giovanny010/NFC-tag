/*
 * PEE20 NFC Example
 * Roy Bakker
 *
 * Pin connections:
 *
 * P1.1 UART RX (UART Jumper on MSPEXPG2 should be oriented horizontally)
 * P1.2 UART TX (UART Jumper on MSPEXPG2 should be oriented horizontally)
 *
 * MSP        RC522
 * P1.3  <-->  RST
 * P1.4  <-->  SDA    (SPI CS)
 * P1.5  <-->  SCK    (SPI CLK)
 * P1.6  <-->  MISO   (SPI MISO)
 * P1.7  <-->  MOSI   (SPI MOSI)
 * VCC   <-->  VCC
 * GND   <-->  GND
 *
 * P2.0 <--> LED1 (no card present)
 * P2.1 <--> LED2 (card present)
 */

#include <msp430.h>
#include <SPI.h>
#include <Mfrc522.h>
#include <UART.h>

#define RC522_CS_PIN  BIT4 // P1.4
#define RC522_RST_PIN BIT3 // P1.3
#define LED_OK        BIT1 // P2.1
#define LED_ERR       BIT0 // P2.0

/* Delay in ms. Assume clock 1 MHz. */
void delay(int ms){
    int i;
    for(i = 0; i < ms; i++)
    {
         __delay_cycles(1000);
    }
}

/**
 * main.c
 */
int main(void)
{
    /* Stop watchdog timer */
	WDTCTL = WDTPW | WDTHOLD;

    /* Set DCO to 1 MHz (this code will not work with other frequency!) */
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL  = CALDCO_1MHZ;

    /* Configure LEDS */
    P2DIR |= (LED_OK | LED_ERR);

    /* Init SPI P1.5, 1.6, 1.7 */
    SPI_begin();

    /* Init RC522 */
    RC522_init(RC522_CS_PIN, RC522_RST_PIN);

    /* Init UART */
    UART_init();

    /*
     * Setting antenna gain to maximum (7) can read cards up to approximately 5cm.
     * Default can read cards up to approximately 1cm.
     * */
    RC522_setAntennaGain(7);

    uint16_t i = 0;

    while(1){
        /* Try detecting cards */
        UART_printf("\r\nChecking for cards: %u\r\n", i);
        i++;

        /* Make space to store response */
        uint8_t str[MAX_LEN];

        /* Clear LEDS */
        P2OUT &= ~(LED_OK | LED_ERR);

        /* Make sure we see the LED blinking */
        delay(200);

        /* See if there are cards present */
        if(RC522_request(PICC_REQALL, str) == MI_OK){

            /* Can we communicate with the first detected card? */
            if(RC522_anticoll(str) == MI_OK){
                UART_printf("--Found cardID: 0x%y%y%y%y%y\r\n", str[0], str[1], str[2], str[3], str[4]);
                P2OUT |= LED_OK;

                /* Select card to be active */
                uint8_t l = RC522_selectTag(str);

                /* See if we can authenticate to read */
                uint8_t recvData[18];

                uint8_t keyA[6] = {0xBA, 0xBE, 0xBA, 0xBE, 0xBA, 0xBE};
                if(RC522_auth(PICC_AUTHENT1A, 1, keyA, str) == MI_OK){
                    UART_printf("------Auth OK\r\n");

                    /* See if we can actually read data from the card. (block 1) */
                    if(RC522_readBlock(1, recvData) == MI_OK){
                        /* Color data is in block 1 of sector 0 */
                        UART_printf("--------READ color value from sector 1 byte 0: %c\r\n", recvData[0]);
                    }

                    /* Stop authentication */
                    RC522_stopCrypto();
                    UART_printf("------Exit Auth state\r\n");
                }
                else{
                    UART_printf("------Auth failed!\r\n");
                }

                /* Put card into sleep mode */
                RC522_halt();
                UART_printf("----Card put to sleep\r\n");
            }
            else{
                UART_printf("----Could not communicate with card\r\n");
            }
        }
        else{
            UART_printf("--No card present\r\n");
            P2OUT |= LED_ERR;
        }

        /* Keep led on */
        delay(800);
    }
	
	return 0;
}
