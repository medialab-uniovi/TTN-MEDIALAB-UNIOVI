#include "LoRaWan_APP.h"
#include "Arduino.h"

/*
 * set LoraWan_RGB to Active,the RGB active in loraWan
 * RGB red means sending;
 * RGB purple means joined done;
 * RGB blue means RxWindow1;
 * RGB yellow means RxWindow2;
 * RGB green means received done;
 */

/* OTAA para*/
uint8_t devEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t appKey[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

/* ABP para*/
uint8_t nwkSKey[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t appSKey[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint32_t devAddr =  ( uint32_t )0x00000000;

/*LoraWan channelsmask, default channels 0-7*/ 
uint16_t userChannelsMask[6]={ 0x00FF,0x0000,0x0000,0x0000,0x0000,0x0000 };

/*LoraWan region, select in arduino IDE tools*/
LoRaMacRegion_t loraWanRegion = ACTIVE_REGION;

/*LoraWan Class, Class A and Class C are supported*/
DeviceClass_t  loraWanClass = LORAWAN_CLASS;

/*the application data transmission duty cycle.  value in [ms].*/
uint32_t appTxDutyCycle = 15 * 60 * 1000;

/*OTAA or ABP*/
bool overTheAirActivation = LORAWAN_NETMODE;

/*ADR enable*/
bool loraWanAdr = LORAWAN_ADR;

/* set LORAWAN_Net_Reserve ON, the node could save the network info to flash, when node reset not need to join again */
bool keepNet = LORAWAN_NET_RESERVE;

/* Indicates if the node is sending confirmed or unconfirmed messages */
bool isTxConfirmed = LORAWAN_UPLINKMODE;

/* Application port */
uint8_t appPort = 2;
/*!
* Number of trials to transmit the frame, if the LoRaMAC layer did not
* receive an acknowledgment. The MAC performs a datarate adaptation,
* according to the LoRaWAN Specification V1.0.2, chapter 18.4, according
* to the following table:
*
* Transmission nb | Data Rate
* ----------------|-----------
* 1 (first)       | DR
* 2               | DR
* 3               | max(DR-1,0)
* 4               | max(DR-1,0)
* 5               | max(DR-2,0)
* 6               | max(DR-2,0)
* 7               | max(DR-3,0)
* 8               | max(DR-3,0)
*
* Note, that if NbTrials is set to 1 or 2, the MAC will not decrease
* the datarate, in case the LoRaMAC layer did not receive an acknowledgment
*/
uint8_t confirmedNbTrials = 4;

const int LED_PIN = RGB;
const int PIN_DIR_MOT = GPIO1; // Pin IB2 del bridge
const int PIN_VEL_MOT = GPIO2; // Pin IA2 del bridge PWM
const int TIEMPO_DISPENSADO = 1000; // tiempo en ms
const int TOLERANCIA_SENSOR = 600;
const int TOLERANCIA_MISMA_PERSONA = 100;

unsigned int usos = 0;
int batteryVoltage;
float batteryLevel;
bool misma_persona = false;
int valorIR = 0;

/* Prepares the payload of the frame */
static void prepareTxFrame( uint8_t port )
{
	/*appData size is LORAWAN_APP_DATA_MAX_SIZE which is defined in "commissioning.h".
	*appDataSize max value is LORAWAN_APP_DATA_MAX_SIZE.
	*if enabled AT, don't modify LORAWAN_APP_DATA_MAX_SIZE, it may cause system hanging or failure.
	*if disabled AT, LORAWAN_APP_DATA_MAX_SIZE can be modified, the max value is reference to lorawan region and SF.
	*for example, if use REGION_CN470, 
	*the max value for different DR can be found in MaxPayloadOfDatarateCN470 refer to DataratesCN470 and BandwidthsCN470 in "RegionCN470.h".
	*/
	batteryVoltage = getBatteryVoltage();
	batteryLevel = (BoardGetBatteryLevel() / 254.0);
	appDataSize = 4; //AppDataSize max value is 64
	appData[0] = highByte(usos);
	appData[1] = lowByte(usos);
	appData[2] = highByte(batteryVoltage);
	appData[3] = lowByte(batteryVoltage);
	//appData[4] = round(batteryLevel*100);

	Serial.print("El Higienizador se ha usado: ");
    Serial.print(usos);
	Serial.print(" veces [ Nivel Tensión Batería: ");
    Serial.print(batteryVoltage);
	Serial.print(" mV, Nivel Batería: ");
    Serial.print(batteryLevel*100);
    Serial.println(" %]");
}

unsigned int Dispensar()
{
  usos++;
  digitalWrite(LED_PIN,HIGH);
  unsigned long now = millis();
  while(millis()-now<TIEMPO_DISPENSADO){
      Serial.println("Dispensando Gel Hidroalcohólico");
      digitalWrite(PIN_DIR_MOT,HIGH);
      digitalWrite(PIN_VEL_MOT,LOW);
      delay(1000);
  }
  digitalWrite(LED_PIN, LOW);
  digitalWrite(PIN_DIR_MOT, LOW);
  digitalWrite(PIN_VEL_MOT, LOW);
  return usos;
}

unsigned int LeerIR()
{
  unsigned int IR = analogRead(ADC);
  delay(50);
  //Serial.println(IR);
  return IR;
}

void setup() {
	boardInitMcu();
	Serial.begin(115200);
	pinMode(LED_PIN,OUTPUT);
	pinMode(PIN_DIR_MOT,OUTPUT);
	pinMode(PIN_VEL_MOT,OUTPUT);
	digitalWrite(PIN_DIR_MOT, LOW);
	digitalWrite(PIN_VEL_MOT, LOW);

#if(AT_SUPPORT)
	enableAt();
#endif
	deviceState = DEVICE_STATE_INIT;
	LoRaWAN.ifskipjoin();
}

void loop()
{
  valorIR = LeerIR();
  //Serial.println(valorIR);
  if(valorIR > TOLERANCIA_SENSOR){
    if(not misma_persona){
      usos = Dispensar();
      misma_persona = true;
      Serial.println("Mismo usuario");
    }
  }
  else if(valorIR < TOLERANCIA_SENSOR-TOLERANCIA_MISMA_PERSONA){
    misma_persona = false;
  }

	switch( deviceState )
	{
		case DEVICE_STATE_INIT:
		{
#if(LORAWAN_DEVEUI_AUTO)
			LoRaWAN.generateDeveuiByChipID();
#endif
#if(AT_SUPPORT)
			getDevParam();
#endif
			printDevParam();
			LoRaWAN.init(loraWanClass,loraWanRegion);
			deviceState = DEVICE_STATE_JOIN;
			break;
		}
		case DEVICE_STATE_JOIN:
		{
			LoRaWAN.join();
			break;
		}
		case DEVICE_STATE_SEND:
		{
			prepareTxFrame( appPort );
			LoRaWAN.send();
			deviceState = DEVICE_STATE_CYCLE;
			break;
		}
		case DEVICE_STATE_CYCLE:
		{
			// Schedule next packet transmission
			txDutyCycleTime = appTxDutyCycle + randr( 0, APP_TX_DUTYCYCLE_RND );
			LoRaWAN.cycle(txDutyCycleTime);
			deviceState = DEVICE_STATE_SLEEP;
			break;
		}
		case DEVICE_STATE_SLEEP:
		{
			//LoRaWAN.sleep(); El dispositivo se comportará como Clase C debido a la lectura IR
			break;
		}
		default:
		{
			deviceState = DEVICE_STATE_INIT;
			break;
		}
	}
}
