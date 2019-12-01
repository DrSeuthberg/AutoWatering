#include <Servo.h>

Servo myservo;  // create servo object to control a servo

int moistureLevel = 0; //value for storing moisture value 
unsigned long previousMillis = 0;
bool open = false;
int measureCount = 0;
unsigned long pumpTime = 0;

const int soilPin = A0;//Declare a variable for the soil moisture sensor 
const int soilPower = 3;//Variable for Soil moisture Power
const int servoPin = 4;
const int led = A1;
const unsigned long timeThreshold = 3600000; // 43200000 // 12 hours
const unsigned long delayTime = 900000;
const unsigned long maxPumpTime = 600000; // 10 min

void setup() 
{
  Serial.begin(115200);   // open serial over USB

  pinMode(soilPower, OUTPUT);//Set D7 as an OUTPUT
  digitalWrite(soilPower, LOW);//Set to LOW so no power is flowing through the sensor

  myservo.attach(servoPin);  // attaches the servo on pin 5 to the servo object
  myservo.write(1); // ensure pump is closed
  pinMode(led, OUTPUT);  
}

void loop() 
{  
  unsigned long currentMillis = millis();

  Serial.print("time: ");
  Serial.print(currentMillis / 1000);
  Serial.println("s");

  bool thresholdBroken = (unsigned long)(currentMillis - previousMillis) >= timeThreshold;

  // measure soil moisture in a certain interval or when servo is open
  if (thresholdBroken || open) 
  {
      measureCount = 0;
      for(int i = 0; i < 7; i++) 
      {
        delay(1000);
          
        int soilMoisture = readSoil();
  
        Serial.print("soil moisture ");
        Serial.println(soilMoisture);
      
        setServo(soilMoisture);
      }      

      // don't reset when run due to pump open - otherwise pump open time will delay interval
      if(thresholdBroken)
        previousMillis = currentMillis;
  }  

  if(thresholdBroken || open)
    delay(delayTime - 7000);
  else
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

void setServo(int soilMoisture) 
{
  int servoVal = map(moistureLevel, 0, 1023, 1, 180);
  if(moistureLevel >= 100 && !open) 
  {
    measureCount++;

    if(measureCount >= 5)
    {
      myservo.write(servoVal); 
      open = true;
      digitalWrite(led, HIGH);
    
      Serial.println("servo open");
    }
  }
  else if(moistureLevel < 100 && open) 
  {
    myservo.write(servoVal); 
    open = false;
    digitalWrite(led, LOW);
    measureCount = 0;
    
    Serial.println("servo close");
  }
}
