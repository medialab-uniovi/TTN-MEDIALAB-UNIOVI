#include "Arduino.h"
#include "LoRaWan_APP.h"
#include <Wire.h>
#include <VL53L0X.h>

VL53L0X sensor;
VL53L0X sensor2;

#define DELAYVAL 500

#define pin1 GPIO1 //pin XSHUT1
#define pin2 GPIO2 //pun XSHUT2

int state = 0; // defines the state in wich the sensors are
int previous_state =0; // defines the previous state
int counter = 0; // variable used to count the number of people
int batteryVoltage; // variable to read and store the baterry voltage
int d_max = 2000;
int d_range = 1300;

// Uncomment this line to use long range mode. This
// increases the sensitivity of the sensor and extends its
// potential range, but increases the likelihood of getting
// an inaccurate reading because of reflections from objects
// other than the intended target. It works best in dark
// conditions.

#define LONG_RANGE


// Uncomment ONE of these two lines to get
// - higher speed at the cost of lower accuracy OR
// - higher accuracy at the cost of lower speed

#define HIGH_SPEED
//#define HIGH_ACCURACY



/***********************************/
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
uint8_t appKey[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x800, 0x00, 0x00, 0x00, 0x00, 0x800, 0x00, 0x00, 0x00, 0x00, 0x00 };

/* ABP para*/
uint8_t nwkSKey[] = { 0x91, 0x7C, 0x77, 0x9D, 0xBE, 0xF9, 0xCF, 0xD1, 0xF4, 0x05, 0x2C, 0x6B, 0x6C, 0x09, 0x46, 0x34 };
uint8_t appSKey[] = { 0xCB, 0x04, 0x29, 0x8F, 0xF6, 0xA9, 0x81, 0x16, 0xFC, 0x1C, 0x42, 0x84, 0x05, 0xD0, 0x25, 0xF4 };
uint32_t devAddr =  ( uint32_t )0x26013FF5;

/*LoraWan channelsmask, default channels 0-7*/ 
uint16_t userChannelsMask[6]={ 0x00FF,0x0000,0x0000,0x0000,0x0000,0x0000 };

/*LoraWan region, select in arduino IDE tools*/
LoRaMacRegion_t loraWanRegion = ACTIVE_REGION;

/*LoraWan Class, Class A and Class C are supported*/
DeviceClass_t  loraWanClass = LORAWAN_CLASS;

/*the application data transmission duty cycle.  value in [ms].*/
uint32_t appTxDutyCycle = 900000; // sends data each 2 minutes
                                  // If data transmission duty cycle is too small (less than 30s)
                                  // it will maybe interfeer with people counter
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

    appDataSize = 4;
    appData[0] = highByte(counter);
    appData[1] = lowByte (counter);
    appData[2] = highByte(batteryVoltage);
    appData[3] = lowByte (batteryVoltage);
}


void setup() {

	
  
  /**Define I2C direction for the VL53L0X sensors**/
  pinMode(Vext, OUTPUT);
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
  delay(500);
  Wire.begin();
  Serial.begin (9600);
  digitalWrite(pin1, HIGH);
  delay(150);
  Serial.println("00");
  sensor.init(true);
  Serial.println("01");
  delay(100);
  sensor.setAddress((uint8_t)22);
  Serial.println("02");
  digitalWrite(pin2, HIGH);
  delay(150);
  sensor2.init(true);
  Serial.println("03");
  delay(100);
  sensor2.setAddress((uint8_t)25);
  Serial.println("04");
  Serial.println("addresses set");
  sensor.init();
  sensor.setTimeout(500);
  sensor.startContinuous();
  sensor2.init();
  sensor2.setTimeout(500);
  sensor2.startContinuous();

#if defined LONG_RANGE
  // lower the return signal rate limit (default is 0.25 MCPS)
  sensor.setSignalRateLimit(0.1);
  // increase laser pulse periods (defaults are 14 and 10 PCLKs)
  sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
   sensor2.setSignalRateLimit(0.1);
  // increase laser pulse periods (defaults are 14 and 10 PCLKs)
  sensor2.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  sensor2.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
#endif

#if defined HIGH_SPEED
  // reduce timing budget to 20 ms (default is about 33 ms)
  sensor.setMeasurementTimingBudget(20000);
  sensor2.setMeasurementTimingBudget(20000);
#elif defined HIGH_ACCURACY
  // increase timing budget to 200 ms
  sensor.setMeasurementTimingBudget(200000);
  sensor2.setMeasurementTimingBudget(200000);
#endif

    /********************/


   /*********setup LORA****/

	boardInitMcu();
	Serial.begin(115200);
#if(AT_SUPPORT)
	enableAt();
#endif
	deviceState = DEVICE_STATE_INIT;
	LoRaWAN.ifskipjoin();
}

void loop()
{
  

	/*****Measure distance*****/
  unsigned int distance1 = sensor.readRangeSingleMillimeters();
  if (sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }

       // if (distance1 < 8000){
        
       // Serial.println(distance1);
        // } 

  unsigned int distance2 = sensor2.readRangeSingleMillimeters();  
  if (sensor2.timeoutOccurred()) { Serial.print(" TIMEOUT"); }

        //if (distance2 < 8000){
        
       // Serial.println(distance2);
        //} 

/***************************************************/


  /***********State machine***********/
  
  switch (state)
  {
  case 0: //No one walking through the door  
        
          
          if((distance1 < d_range)&& (distance1 != 0)) 
          { //coming in
            state = 1;                 
          }          
          else if((distance2 < d_range)&& (distance2 != 0)) 
          { //coming out
            state = 2;                
          }                              
          else state = 0;          
          break;
               
  case 1: //Someone comes in

          
          previous_state = 1;
          
          if ((distance2 < d_range) && (distance2 != 0))
          {
          state = 4;  
          }
          else if((distance1 < d_range )&&(distance2 > d_max))
          {
          state = 3;
          }
          else if((distance1 > d_range) && (distance2 > d_max) )
          {  
          state = 0;              
          }         
          break;

  case 2: //Someone comes out

          
          previous_state = 2;
          
          if ((distance1 < d_range) && (distance1 != 0))
          {
          state = 5;  
          }
          else if((distance2 < d_range )&&(distance1 > d_max))
          {
          state = 3;
          }
          else if((distance1 > d_max) && (distance2 > d_max) )
          {  
          state = 0;              
          }         
          break;

  case 3: // Someone is stopped in front of the door

 
          if ((distance2 < d_range) && (distance1>d_max) && (previous_state == 1))
          { 
          previous_state = 3;           
          state = 4;           
          }
          if ((distance1 < d_range) && (distance2>d_max) && (previous_state == 2))
          {          
          previous_state = 3;
          state = 5;           
          }
          if((distance1 > d_max) && (distance2 > d_max) )
          {  
          state = 0;             
          }  
          break;
      
  case 4:  //Inside
          
          if ((previous_state==3)|| (previous_state==1))
          {
          counter++;
          previous_state = 0;
          }
          else if((distance1 > d_max) && (distance2 > d_max) )
          {  
          state = 0;              
          }

          Serial.print("Aforo:");
          Serial.println (counter);
          
          break;
  case 5:  //Outside
           
          if (((previous_state==3)|| (previous_state==2))&&(counter != 0))
          {
          counter--;
          previous_state = 0;
          }
          else if((distance1 > d_max) && (distance2 > d_max) )
          {  
          state = 0;              
          }
          Serial.print("Aforo:");
          Serial.println (counter);  
          break; 


  }

 /**********/

	switch( deviceState )
	{
		case DEVICE_STATE_INIT:
		{
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
			//LoRaWAN.sleep();
			break;
		}
		default:
		{
			deviceState = DEVICE_STATE_INIT;
			break;
		}
	}
}
