/* controll logic for "Intelligent Power Distribution and Load Balancing using Arduino"
Author: Shane Chin 
*/
#include <Irms_Calc.h>  //https://github.com/Warlib1975/Irms_calc
#include <LiquidCrystal.h>

const int sensorIn = A0;
const int sensorIn1 = A1;
const int sensorIn2 = A2;
const int sensorIn3 = A3;

const int relayPin[]={7,8,9,6};// output pins where 4 relays will be connected

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


ACS712_Irms acs712;
ACS712_Irms acs712_1;
ACS712_Irms acs712_2;
ACS712_Irms acs712_3;



void setup() {
  Serial.begin(9600);
  for(int i=0; i<4; i++)
  {
    pinMode(relayPin[i], OUTPUT);   
    digitalWrite(relayPin[i], LOW);// initial relay status to be ON
  }
  acs712.ADCSamples = 1024.0; //1024 samples
  acs712_1.ADCSamples = 1024.0;
  acs712_2.ADCSamples = 1024.0;
  acs712_3.ADCSamples = 1024.0;
  acs712.mVperAmp = scaleFactor::ACS712_20A; // use 100 for 20A Module and 66 for 30A Module and 185 for 5A Module
  acs712_1.mVperAmp = scaleFactor::ACS712_20A;
  acs712_2.mVperAmp = scaleFactor::ACS712_20A;
  acs712_3.mVperAmp = scaleFactor::ACS712_20A;
  acs712.maxADCVolt = 5.0; //5 Volts
  acs712_1.maxADCVolt = 5.0;
  acs712_2.maxADCVolt = 5.0;
  acs712_3.maxADCVolt = 5.0;
  acs712.ADCIn = A0;
  acs712_1.ADCIn = A1;
  acs712_2.ADCIn = A2;
  acs712_3.ADCIn = A3;
  acs712.Init();
  acs712_1.Init();
  acs712_2.Init();
  acs712_3.Init();
  lcd.begin(16, 4);
  lcd.setCursor(0, 0);
  lcd.print("Intelligent");
  lcd.setCursor(0, 1);
  lcd.print(" Power ");
  lcd.setCursor(0, 2);
  lcd.print("Distribution");
  lcd.setCursor(0, 3);
  lcd.print("System....");
  delay(4000);
  lcd.clear();
  delay(1000);

}

void loop()
{
  double AmpsRMS = acs712.Process();  //MACHINE
  double AmpsRMS_1 = acs712_1.Process(); // AC & HEATER
  double AmpsRMS_2 = acs712_2.Process(); //PLUG CIRCUIT
  double AmpsRMS_3 = acs712_3.Process(); // BUILDING LIGHTS
  lcd.setCursor(0, 0);
  lcd.print("MACHINE:");
  lcd.print(AmpsRMS);
  lcd.setCursor(0, 1); //(column, row)
  lcd.print("HEATER:");
  lcd.print(AmpsRMS_1);
  lcd.setCursor(0, 2);
  lcd.print("PLUGS:");
  lcd.print(AmpsRMS_2);
  lcd.setCursor(0, 3);
  lcd.print("LIGHTS:");
  lcd.print(AmpsRMS_3);
//***************************************
// CONTROL LOGIC (Highest proirity is Machine)
  if (AmpsRMS >= 10.5) {
  digitalWrite(relayPin[3], HIGH); //if Machine exceeds 10.5A then turn off lights.
  } 
  else {
  digitalWrite(relayPin[3], LOW);
  }

  if (AmpsRMS >= 13.4) {
  digitalWrite(relayPin[2], HIGH); //if Machine exceeds 13.4A then turn off Plugs.
  } 
  else {
  digitalWrite(relayPin[2], LOW);
  }

  if (AmpsRMS_1 > 10 && AmpsRMS > 12) {
    digitalWrite(relayPin[1], HIGH); // if HEATER exceeds 12A & Machine exceed 10A then turn Heater off
    }
    else if (AmpsRMS_1 < 5 && AmpsRMS < 5 ) { // else if both are below 5A then turn Heater on again.
    digitalWrite(relayPin[1], LOW);
    }

//****************************************** 

}
