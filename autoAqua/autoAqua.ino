#include <Wire.h>
#include "DS3231.h"

#define DS3231_ADDRESSE 0x68

DS3231 Clock;

RTClib RTC;

int moistureLevel = 0; //value for storing moisture value 
unsigned long previousMillis = 0;
bool open = false;
int measureCount = 0;
unsigned long pumpTime = 0;

const int soilPin = A0;//Declare a variable for the soil moisture sensor 
const int soilPower = 3;//Variable for Soil moisture Power
const int relaisPin = 4;
const int led = A1;
const unsigned long timeThreshold = 3000; // 43200000 // 12 hours
const unsigned long delayTime = 1000;
const unsigned long maxPumpTime = 600000; // 10 min

void setup() 
{
  Serial.begin(115200);   // open serial over USB

  pinMode(soilPower, OUTPUT);//Set D7 as an OUTPUT
  digitalWrite(soilPower, LOW);//Set to LOW so no power is flowing through the sensor

  //pinMode(led, OUTPUT);  
  pinMode(relaisPin, OUTPUT);
  //setClock();

  digitalWrite(led, LOW); // make sure relais is closed

  initClock();
}

void loop() 
{  
  int soilMoisture = readSoil();
  
  Serial.print("soil moisture ");
  Serial.println(soilMoisture);
      
  setPumpRelais(soilMoisture);

  //showTimeAndTemp();
  
  delay(delayTime);
}

//This is a function used to get the soil moisture content
int readSoil()
{
  digitalWrite(soilPower, HIGH);
  delay(10);//wait 10 milliseconds 
  moistureLevel = analogRead(soilPin);//Read the SIG value form sensor 
  digitalWrite(soilPower, LOW);
  
  return moistureLevel;
}

void setPumpRelais(int soilMoisture) 
{
  int servoVal = map(moistureLevel, 0, 1023, 1, 180);
  if(moistureLevel >= 100 && !open) 
  {
    measureCount++;

    if(measureCount >= 5)
    {
      //myservo.write(servoVal); 
      digitalWrite(relaisPin, HIGH);
      open = true;
      digitalWrite(led, HIGH);
    
      Serial.println("servo open");
    }
  }
  else if(moistureLevel < 100 && open) 
  {
    //myservo.write(servoVal); 
    digitalWrite(relaisPin, LOW);
    open = false;
    digitalWrite(led, LOW);
    measureCount = 0;
    
    Serial.println("servo close");
  }
}

void setClock() 
{
    Clock.setClockMode(false);  // set to 24h
    //setClockMode(true); // set to 12h

    Clock.setYear(20);
    Clock.setMonth(12);
    Clock.setDate(03);
    Clock.setDoW(02);
    Clock.setHour(20);
    Clock.setMinute(33);
    Clock.setSecond(40);
}
