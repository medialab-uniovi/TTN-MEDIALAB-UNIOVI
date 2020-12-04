# MediaLab Uniovi - THE THINGS NETWORK - 📡

The aim of this project is to join [The Things Network](https://www.thethingsnetwork.org/) initative and experiment with different **Internet of Things** use cases.

We have placed some LoRa Gateways at the EPI Gijón Campus (University of Oviedo) enabling anyone to connect their IoT projects through sending data over LoRa technology.

This repository will list & contain the different devices that have been developed:

- **PAX COUNTER 👥:** a device that records the capacity of the different rooms within the building. Data is sent via The Things Network (TTN)

- **SANITIZER 🧼:** a device that dispenses hydroalcoholic gel and records the number of times it has been used. Data is sent via The Things Network (TTN)

- **MINI WEATHER STATION 💧🌡️🎈** : a basic battery powered LoRa mini weather station that records the temperature, humidity & pressure. Data is sent via The Things Network (TTN)

    ***___Note:___*** this project is a slightly modified fork from this [opensource project](https://github.com/chrisys/mini-lora-weatherstation). Credits to [Chris Crocker-White](https://github.com/chrisys) original author 😉

You can find all the information related to the projects in their respectives folders. 

## Universal PCB Adapter for CubeCell Dev-Board ⚙️

In addition, to **make easier the way people can prototype different IoT devices as solutions to a variety of use cases**, we've designed an **Universal PCB** that can be used with all the projects listed and it also provides **additional pins** to connect different sensors or modules.

The Universal PCB has been desgined to use with the [Heltec CubeCell Dev-Board](https://heltec.org/project/htcc-ab01/) which is compatible with **Arduino IDE & PlatformIO** and includes a **built in battery management 🔋 & solar charging ☀️** functionality, as well as **ultra low power consumption**, making it a good choice to prototype LoRa-connected devices almost anywhere we want.

<p align="center">
  <img width="350" height="350" src="https://github.com/medialab-uniovi/TTN-MEDIALAB-UNIOVI/blob/main/images/1st_pcb_version_developed.png"/>
  <img width="350" height="350" src="https://github.com/medialab-uniovi/TTN-MEDIALAB-UNIOVI/blob/main/images/universal_pcb_cubecell_dev_board.png"/>
</p>

You can find source files inside **PCB_FILES** folder.

# Official Websites 🌐

You can find more information about the initiative & projects being developed on the official website:

## http://www.medialab-uniovi.es/pro-holalora.php

Data is displayed in real time and accessible through the following control panel that has been enabled for the **public access** from any device with a web browser:

## http://sensores.medialab-uniovi.es/ui/

# License 📋

Copyright (c) 2020 medialab-uniovi
