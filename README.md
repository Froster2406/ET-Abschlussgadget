# HS20-23 ET Abschlussgadget

Als Teil der Abschlusszeitung gibt es auch dieses Jahr wieder ein kleines Gadget kostenfrei dazu. Ganz im Namen des Überthemas "ET im PRENpunkt" mit der Anlehung an "Familien im Brennpunkt", kannst auch Du herausfinden, wie viel ET in Dir steckt. Aber nur wenn Du so asozial wie ein ET'ler bist, schaltet sich die geheime Zusatzfunktion frei.

## Verwendung

* Beiligende Batterie einlegen
* Schalter auf Platine in ON-Zustand versetzten
* Finger auf "Drück mich!" legen
* Anzeige ablesen

<details>
<summary>only for real engineers</summary>
<br>
You deem yourself a true engineer? Great!<br>
Tag along as we dive deeper into how you can reprogram the microcontroller. <br>

## general information
* [Reference Manual](https://ww1.microchip.com/downloads/aemDocuments/documents/MCU08/ProductDocuments/DataSheets/ATtiny212-214-412-414-416-DataSheet-DS40002287A.pdf)
* There are two pins (J1 & J2) on the side of the PCB than can be freely addressed, without interfering with the main functionality of the board


## preresquisites
* USB to serial converter | USB to TTL converter <br>
`Any converter (3.3V or 5V) off of Aliexpress, Ebay, Mouser, etc. will do.`
* 1kOhm resistor <br>
* Wires to connect the PCB to the converter <br>
* [Microchip Studio](https://www.microchip.com/en-us/tools-resources/develop/microchip-studio) <br>
* [Python](https://www.python.org/downloads/) installed on your machine <br>
* Python package: [pymcuprog](https://pypi.org/project/pymcuprog/) <br>
`pip install pymcuprog`<br>

## wiring diagram 
https://github.com/microchip-pic-avr-tools/pymcuprog#serial-port-updi-pyupdi

## reprogramming
* Connect the PCB to the USB to serial converter according to the wiring diagram
* Adjust the code to your liking and build it with F7
* Navigate to the python-programmer script <br>
`cd <YOUR_PATH>\ET-Abschlussgadget\Software\Programmer Python`
* Run the programming command <br>
`python attiny414.py <COM_PORT> <ABSOLUTE_PATH_TO_HEX_FILE_YOU_COMPILED_WITH_MICROCHIP_STUDIO>` <br>
*COM_PORT: If unknown, the available ones will be printed in the console*

* Power cycle the PCB

</details>
