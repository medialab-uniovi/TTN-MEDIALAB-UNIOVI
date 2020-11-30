# PAX counter


This project is a people counter device developed by University of Oviedo. The aim of the project is to be able to registrate the capacity in the different places inside the buildings of the university.


It determines whether a person comes in or out the place where it's installed. The sensor is designed to be placed near some entrance or exit, fixed in the wall so it can count the number of people inside that room. The data is sent via LoRaWAN protocol using The Things Network.

How it works?  With 2 Time of Flight (ToF) sensors and a microcontroller the system can detect if someone walk through the infrared ray of the devices, then depending on which sensor detects first, can be determinated if the person comes in or out the room.

## SOFTWARE

To run the programme is necessary some Arduino compatible (ARDUINO IDE or PLATFORM IO work properly) software as well as the following libraries:

		#include "LoRaWan_APP.h"
		#include <Wire.h>
		#include <VL53L0X.h>
		
All necessary libraries are included in .zip file (.pio > libdeps).


The code can be structured into two parts :

	- Reading data from VL53L0X sensor : this part must stay the same as the original code.

	- Sending data via LoRaWAN : whether if you use OTAA or ABP, you should change the parameters given by your TTN account in order to send the data correctly.  
	
## HARDWARE USED	
	
The board used for this code is CubeCell Dev Board (AB01), you can find it here : https://heltec.org/project/htcc-ab01/

For data reading, VL53L0X Time of Flight Sensors (2 uds) : https://www.amazon.es/TECNOIOT-VL53L0X-Flight-Distance-GY-VL53L0XV2/dp/B07RKXRZWX/ref=sr_1_5?dchild=1&keywords=vl53l0x&qid=1606135848&sr=8-5

<img src="https://heltec.org/wp-content/uploads/2020/04/board002.png" width="150" height="150"/><img src="https://naylampmechatronics.com/1620-large_default/sensor-vl53l0x-de-tof.jpg" width="150" height="150"/>







## MAKE THE PROJECT WORK

	1. If you are using PLATFORM IO download and unzip project folder and open it using the program.
	2. Then you should go here:
			/* OTAA para*/
			uint8_t devEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
			uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
			uint8_t appKey[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x800, 0x00, 0x00, 0x00, 0x00, 0x800, 0x00, 0x00, 0x00, 0x00, 0x00 };

			/* ABP para*/
			uint8_t nwkSKey[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
			uint8_t appSKey[] = { 0x00 ,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00, 0x00 };
			uint32_t devAddr =  ( uint32_t )0x00000000;
		
			
	And change the values depending on the activation method you're using.
	
	3. Once you've change this parameters then you can build the code and upload ir to your board. It should work.



	1. If you're using ARDUINO IDE you need to upload the libraries and boards
	2. Tools > Boards > Boards Manager > Search and Install Cubecell Development Framework
	3. Dowload the libraries folder on this project and Sketch > Include Library > Add .ZIP Library
	4. Now you must opne .ino file and change OTAA or ABP keys the same as PLATFORM IO method.
	5. Once you've change this parameters then you can build the code and upload ir to your board. It should work.


## REGISTER ANY DEVICE ON TTN

In order to receive data via LoRaWAN, you need to register the device in TTN Network. Here you cand find good tutorials about how to do it.

1. Getting started with TTN : https://www.thethingsnetwork.org/docs/devices/uno/quick-start.html#get-your-device-eui
2. Device registration: https://www.thethingsnetwork.org/docs/devices/registration.html

## UNIVERSAL PCB

To make the connections easier we've designed a PCB board, that can be used in other projects apart from this one.

<img src= https://user-images.githubusercontent.com/75246223/100598694-5fc92880-32ff-11eb-98ab-012b1e58ebb6.png width="400" height="350" />
