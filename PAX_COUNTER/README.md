# PAX counter


This project is a people counter device developed by University of Oviedo. 
The aim of the project is to be able to record the capacity in the different places inside the buildings of the university.


It determines whether a person comes in or out the place where it's placed. The device is designed to be placed near some entrance or exit, fixed in the wall so it can count the number of people inside that room. The data is sent via LoRaWAN protocol using The Things Network.

How it works?  With 2 Time of Flight (ToF) sensors and a microcontroller, system can detect if someone walk through the infrared ray of the device, then depending on which sensor detects first, can be determinated if the person comes in or out the room.

<p align="center">
  <img width="400" height="400" src="https://github.com/medialab-uniovi/TTN-MEDIALAB-UNIOVI/blob/main/PAX_COUNTER/images/pax_counter_enclosure.jpg"/>
  <img width="400" height="400" src="https://github.com/medialab-uniovi/TTN-MEDIALAB-UNIOVI/blob/main/PAX_COUNTER/images/pax_counter.png"/>
</p>

## SOFTWARE

You need to install [PlatformIO](https://platformio.org/) extension for Visual Studio Code to upload the code to board.

The code can be structured into two parts :

- Reading data from VL53L0X sensor : this part must stay the same as the original code.

- Sending data via LoRaWAN : whether if you use OTAA or ABP activation method. You must change the parameters given by your TTN console account in order to send the data correctly.  
	
## HARDWARE	
	
* CubeCell Dev Board (AB01), you can find it here: https://heltec.org/project/htcc-ab01/

* VL53L0X Time of Flight Sensors (2 uds) : [Amazon example link](https://www.amazon.es/TECNOIOT-VL53L0X-Flight-Distance-GY-VL53L0XV2/dp/B07RKXRZWX/ref=sr_1_5?dchild=1&keywords=vl53l0x&qid=1606135848&sr=8-5)

## RUN THE PROJECT

	1. Download the project and open its folder using Visual Studio Code
	2. Check you have installed PlatformIO extension
	2. Go to /src/main.cpp and check for:

			/* OTAA para*/
			uint8_t devEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
			uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
			uint8_t appKey[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x800, 0x00, 0x00, 0x00, 0x00, 0x800, 0x00, 0x00, 0x00, 0x00, 0x00 };

			/* ABP para*/
			uint8_t nwkSKey[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
			uint8_t appSKey[] = { 0x00 ,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00, 0x00 };
			uint32_t devAddr =  ( uint32_t )0x00000000;
		
			
	And change accordngly the keys values depending on the activation method you're using.
	
	3. Once you've change this parameters then you can build the code and upload it to your board.

	4. Setup the payload decode function in the Payload Formats tab inside the Application where you have registered your device.

## REGISTER THE DEVICE ON TTN

In order to receive data via LoRaWAN, you need to register the device in TTN Network. 
Below you cand find a tutorial to learn how to do it:

[Device registration](https://www.thethingsnetwork.org/docs/devices/registration.html)

#### IMPORTANT NOTE :

We used ABP method, because OTAA didn't work properly, so we recommended to use ABP until device is located for production.
