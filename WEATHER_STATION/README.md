# Mini Weather Station


This project is a slightly modified fork from this [opensource project](https://github.com/chrisys/mini-lora-weatherstation)
Credits to Chris [Crocker-White](https://github.com/chrisys) original author 😉

The aim of the project is to be able to record environmental data (temperature, relative humidity & pressure) there where it will be placed (indoors or outdoors).

The data is sent via LoRaWAN protocol using The Things Network.

How it works?  An environmental sensor (BME280) and a microcontroller board powered by a LiPo battery reads the environmental data each a fixed period of time (can be configured via code) and send the data via LoRaWAN.

<p align="center">
  <img width="400" height="600" src="https://github.com/medialab-uniovi/TTN-MEDIALAB-UNIOVI/blob/main/WEATHER_STATION/images/mini_lora_weather_station.png"/>
</p>

## SOFTWARE

You need to install [PlatformIO](https://platformio.org/) extension for Visual Studio Code to upload the code to board.

The code can be structured into two parts :

- Reading data from BME280 sensor : this part must stay the same as the original code.

- Sending data via LoRaWAN : whether if you use OTAA or ABP activation method. You must change the parameters given by your TTN console account in order to send the data correctly.  
	
## HARDWARE	
	
* CubeCell Dev Board (AB01), you can find it here: https://heltec.org/project/htcc-ab01/

* BME280 sensor (1 uds) : [Amazon example link](https://www.amazon.es/AZDelivery-el%C3%A9ctrico-Temperatura-Impresi%C3%B3n-Raspberry/dp/B07D8T4HP6/ref=sr_1_6?__mk_es_ES=%C3%85M%C3%85%C5%BD%C3%95%C3%91&dchild=1&keywords=bme280&qid=1606994282&sr=8-6)

* 3.7V LiPo cell (we're using a 1800mAh) with a micro JST connector

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
