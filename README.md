# Mechatronics Challenge

## Goal
Design a prototype can output GPS coordinates and accelerometer data via serial USB.

## Instructions
1. Clone this repo to your computer and then push to your own Github account. Don't fork it.
```sh
$ git clone git@github.com:voyage/mechatronics-challenge.git
```
2. Put your deliverables for **Part 1** in `electrical` and `src` folders and **Part 2** in the `mechanical` folder.
3. Delete `deliverables.txt` in each folder as they are just dummy files.
4. Modify the `README.MD` file at the root of your repo to explain your approach.
5. Once you've completed the challenge, please send an e-mail to alan@voyage.auto with the link to your Github repo containing the solution.

### Part 1: Electrical/Coding
-The wiring diagram does not illustrate a power supply. This is because it is imagined that the installation would be connected via USB already so that data can be read in the serial monitor.

-The accelerometer has a resistor on the SPI Clock, MISO and MOSI lines. These are to prevent errors when reprogramming the ARM chip on the Photon as this is done over SPI. In lieu of resistors it would also be acceptable to ensure nothing is connector to the microcontroller when it is programmed. 

-The GPS RX and TX lines connect from the GPS shield to the microcontrollers RX and TX lines respectively. While this may sound counterintuitive, the RX and TX are swapped on the Carloop shield where they connect to U1. This is shown on the GPS shield's schematic.

-The setup() function attempts to begin the carloop object and set up the accelerometers. If the accelerometer fails to set-up it will be retried four times. If it still fails there will be a serial message to mention as much and the program will stop there

-The loop function attempts to take an accelerometer reading every 10ms. Every second (or 100 accelerometer readings) it also takes a GPS reading. This is because the max frequency for GPS readings is around 1hZ. By seperating the two readings we have stopped the GPS reading from blocking the accelerometer reading. This way, if the WITH_LOCK(carloop.gps()){} function only operates at 1hZ we will still read the accelerometer at 100hZ.

-This project uses the Carloop library and a third party library for the accelerometers. Local sources are included in /src/libraries_used/ These are for reference, in actual use, libraries should be taken from their repos to ensure that they're up to date. The link for the accelerometer repo is included in the source code as a comment.

-Date and time are output over serial as well as the GPS and accelerometer information. This is to aid future analytical work. To be explicit, the dates are formatted in the US standard as MM/DD/YYYY. This can be easily changed.

### Part 2: Mechanical
-The enclosure is envisioned to be made from ABS plastic due to small manufacturing quantity and non-conductive nature of plasitic. Due to the relatively poor tolerances of additive machining, components have been designed with clearances in mind. Final fitment may require additional hand finishing depending on the resolution used during the build of parts.

-The breadboard is fitted to the based of the enclosure. It is envisioned that this is bonded with a standard adhesive like 9323.

-The enclosure is not designed to be A/V mounted as this would impact on accelerometer readings. If A/V mounting for the Photon is desired the Photon and Carloop can be mounted seperately with the carloop rigidly mounted.

-I was unable to find a CAD model or dimensions of the Carloop GPS shield. Based on the pin pitch being the same as the Photon it should fit in the enclosure. However it would be a very quick job to measure the actual hardware and create a CAD model. From there this assumption can be tested and the enclosure can be enlarged if needed.

-I have added a brace behind the Photon. The purpose of this is to support the board when the USB cable is connected. The front of the enclosure will react to the loading of removing the USB cable. This way, the load is not put through the header pins.

-The enclosure is not water proof but a simple O-ring and o-ring groove around the lid and a move to an autosport style connector could easily change this. It is likely though that 3D printing would struggle to support the tolerances required and roughness guidelines for the o-ring.

-Mounting lugs are not included on the enclosure as it is unknown how it will attach in situ, these would be relatively simple to add prior to manufacturing.

-The lid is keyed and can only be fitted in one orientation. Therefore it is possible to lable the USB side as well as the orientation of the accelerometers and have this be reliable.

## Remarks
If you prefer to use other microcontroller, accelerometer or GPS unit, feel free to do so.