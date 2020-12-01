# SANITIZER


This project is a hydroalcoholic gel dispenser device developed by University of Oviedo.
The aim of the project is to be able to record the number times the device has been used.

It dispenses gel whether a person puts their hands in the hole that the device has enabled. The device is designed to be placed near some entrance or exit for anyone to use it.

How it works? A IR sensor detects if the hands have been put into the hole, and then the microcontroller activates the gel dispenser pump through an H-Bridge. The device records the number of times the device has been used and data is sent via LoRaWAN protocol using The Things Network after a fixed time.

## SOFTWARE

You need to install [PlatformIO](https://platformio.org/) extension for Visual Studio Code to upload the code to board.

The code can be structured into two parts :

- Reading data from IR Sensor : this part must stay the same as the original code.

- Sending data via LoRaWAN : whether if you use OTAA or ABP activation method. You must change the parameters given by your TTN console account in order to send the data correctly.  
	
## HARDWARE	
	
* CubeCell Dev Board (AB01), you can find it here: https://heltec.org/project/htcc-ab01/

* IR Sensor (GP2Y0A41SK0F) (1 uds): [Amazon example link](https://www.amazon.es/HILABEE-Proximidad-Infrarrojo-Distancia-GP2Y0A41SK0F/dp/B07TF7NP8N/ref=sr_1_2?__mk_es_ES=%C3%85M%C3%85%C5%BD%C3%95%C3%91&dchild=1&keywords=GP2Y0A41SK0F&qid=1606842972&sr=8-2)

* L9110s H-Bridge (1 uds): [Amazon example link](https://www.amazon.es/TECNOIOT-L9110S-H-Bridge-Stepper-Controller/dp/B07KFLZKP1/ref=sr_1_1?__mk_es_ES=%C3%85M%C3%85%C5%BD%C3%95%C3%91&dchild=1&keywords=L9110S+H-Bridge&qid=1606843173&sr=8-1)

* Water pump 6V (1 uds): [Amazon example link](https://www.amazon.es/Solomi-Bomba-dosificaci%C3%B3n-perist%C3%A1ltica-Aquarium/dp/B07SZBTY84/ref=sr_1_10?__mk_es_ES=%C3%85M%C3%85%C5%BD%C3%95%C3%91&dchild=1&keywords=bomba+de+agua+6v&qid=1606843741&sr=8-10)

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
Below you cand find tutorials to learn how to do it:

[Device registration](https://www.thethingsnetwork.org/docs/devices/registration.html)

#### IMPORTANT NOTE :

We used ABP method, because OTAA didn't work properly, so we recommended to use ABP until device is located for production.