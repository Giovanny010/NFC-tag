# PEE20_NFC_Example

Deze repository bevat een library en voorbeeldprogramma om NFC tags uit te lezen middels een `MFRC522` NFC lezer en de `MPS430G2553`, bedoeld voor studenten die het project PEE20 volgen.

## Tag informatie

De data op de tags wordt uitgelezen middels de A sleutel `0xBABEBABEBABE`. De tags kunnen worden beschreven met de B sleutel, welke niet openbaar wordt gemaakt.

## Voorbeeld programma

Het voorbeeldprogramma kijkt elke seconde of er een tag aanwezig is binnen het bereik van de lezer en leest de kleurwaarde uit. De kleurwaarde en extra informatie wordt via de UART gestuurd zodat deze zichtbaar wordt op een seriele monitor.

---

# Android App

Er is een Android App (apk) beschikbaar welke dezelfde functionaliteit bevat als de robot moet krijgen:

* Tags kunnen worden uitgelezen
* De kleurwaarde (`R`, `G` of `B`) wordt weergegeven
* Het getal kan worden uitgelezen en wordt weergegeven
* Het getal wordt opgeteld bij het totaal
* Bij een reset signaal (**CLEAR** knop) wordt het totaal op nul gezet.

Daarnaast bevat de App de volgende functies:

* **WRITE** knop: De geselecteerde data (R/G/B en het getal) kan op een tag worden geschreven op een tag die is voorzien van de juiste A en B sleutels.
* Menu --> **Zero all Data**: De data in alle sectoren kan worden gewist op een tag die is voorzien van de juiste A en B sleutels. Deze optie kan enige seconden in beslag nemen.
* Menu --> **Write Keys**: De juiste A en B sleutels kunnen in alle sectoren van een tag met fabriekssleutels (`0xFFFFFFFFFFFF`) worden geschreven. Daarnaast worden de toegangsbits zo gezet dat de data in elke sector kan worden gelezen met de bovengenoemde A sleutel, en kan worden beschreven met de geheime B sleutel. Deze optie kan enige seconden in beslag nemen.
* Menu --> **Set to factory default**: Een tag die is voorzien van (een combinatie van) bekende A en B sleutels kan worden teruggezet naar fabrieksinstellingen. Deze optie kan enige seconden in beslag nemen.



