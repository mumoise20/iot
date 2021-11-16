#include <dht.h>
#include <TimeLib.h>
dht DHT;

//#define DHT11_PIN A2

/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 modified 7 Nov 2016
 by Arturo Guadalupi

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystalHelloWorld

*/

// include the library code:
#include <LiquidCrystal.h>


// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int redPin = 8;
int greenPin = 7;
int bluePin = 6;
int smallBuzzerPin = 13;
int fan = 22;
int count = 0;
int days = 0;

int DHT11_temp_sensor = A2;
int DHT11_temp_sensor_value = 0;

int temperature = 0;
int humidity = 0;
const int trigPin = 9;
const int echoPin = 10;

int distanceCm;
long waveDuration; 

time_t seconds = 0;



void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.setCursor(0, 0);
  lcd.print(" Welcome to the ");
  lcd.setCursor(0, 1);
  lcd.print("Smart Incubator");

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT); 
  setTime(seconds);
   
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(smallBuzzerPin, OUTPUT);
  pinMode(fan, OUTPUT);

  Serial.begin(9600);  // baud rate of 9600
  Serial.println("System is ON");
  digitalWrite(bluePin, HIGH);
  delay(1500);
  digitalWrite(bluePin, LOW);
  lcd.clear();
}

//lcd.clear();
void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  //lcd.setCursor(0, 1);
  //Reading sensor inputs
  if (count ==0)
  {
  DHT11_temp_sensor_value = DHT.read11(DHT11_temp_sensor);

  temperature = DHT.temperature;
  humidity = DHT.humidity;
  }
  count+=1;

  if (temperature <= 28)
  {
    digitalWrite(fan, LOW);
    lcd.clear();
    delay(500);
  }

  if (temperature > 28)
  {
    digitalWrite(greenPin, LOW);
    digitalWrite(redPin, HIGH);
    digitalWrite(smallBuzzerPin, HIGH);
    delay(200);
    digitalWrite(smallBuzzerPin, LOW);
    //if (fan != HIGH)
    //{
      digitalWrite(fan, HIGH);
    //}

    Serial.println("---------------------");
    Serial.print("Temperature: ");
    Serial.println(temperature);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Fan ON");
    lcd.setCursor(0, 1);
    lcd.print("Temperature: ");
    lcd.print(temperature);

    delay(3000);
    digitalWrite(redPin, LOW);
    
   }

   else if (temperature < 19)
  {
    digitalWrite(greenPin, LOW);
    digitalWrite(redPin, HIGH);
    digitalWrite(smallBuzzerPin, HIGH);
    delay(300);
    digitalWrite(smallBuzzerPin, LOW);

    Serial.println("---------------------");
    Serial.print("Temperature: ");
    Serial.println(temperature);

    lcd.clear(); 
    lcd.setCursor(0, 0);
    lcd.print("Heater ON");
    lcd.setCursor(0, 1);
    lcd.print("Temperature: ");
    lcd.print(temperature);
    delay(3000);
    
    
    }

    else
    {
      Serial.println("---------------------");
      Serial.print("Temperature: ");
      Serial.println(temperature);
      Serial.print("Humidity: ");
      Serial.println(humidity);
      digitalWrite(redPin, LOW);  
      digitalWrite(greenPin, HIGH); 

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Temperature: ");
      lcd.print(temperature);
      lcd.setCursor(0, 1);
      lcd.print("Humidity: ");
      lcd.print(humidity);
      delay(1000);
      digitalWrite(greenPin, LOW);
     }
    
  
    seconds = now();
    days = int(seconds/60);
   Serial.println("---------------------");
  Serial.print("Seconds: ");
  Serial.println(seconds);
  lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" Incubator OK   ");
    lcd.setCursor(1, 1);
    lcd.print("   Days:   ");
    lcd.print(days);
    digitalWrite(redPin, LOW);  
    digitalWrite(greenPin, HIGH); 

    if (days>=3 && days<30)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("  Hatch period  ");
      lcd.setCursor(0, 1);
      lcd.print("   Days: ");
      lcd.print(days);
      lcd.print("     ");
      }

      if (days>=5 )
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Please reset the");
      lcd.setCursor(1, 1);
      lcd.print("incubator ");
      lcd.print(days);
      lcd.print("days");
      }
      

      Serial.println("---------------------");
      Serial.println("Systems are normal");
      Serial.print("Temperature: ");
      Serial.println(temperature);
      Serial.print("Humidity: ");
      Serial.println(humidity);

    delay(3000);
    //lcd.clear();

      //Proximity sensor values
  digitalWrite(trigPin, LOW);
  delay(2);
  digitalWrite(trigPin, HIGH);
  delay(10);
  digitalWrite(trigPin, LOW);
  waveDuration = pulseIn(echoPin, HIGH);
  distanceCm = (waveDuration*0.034)/2.0;
  Serial.println("---------------------");
  Serial.print("Distace in cm: ");
  Serial.println(distanceCm);
  
  while (distanceCm<=10 || distanceCm<0)
  {
    seconds = now();
    Serial.println("---------------------");
    Serial.print("Seconds: ");
    Serial.println(seconds);
    digitalWrite(greenPin, LOW);
    digitalWrite(redPin, HIGH);
    digitalWrite(smallBuzzerPin, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Object detected ");
    lcd.setCursor(0, 1);
    lcd.print("    at ");
    lcd.print(distanceCm);
    lcd.print("m");
    delay(1000);
    digitalWrite(smallBuzzerPin, LOW);
    digitalWrite(redPin, LOW);
    Serial.println("---------------------");
    Serial.print("Distace in cm: ");
    Serial.println(distanceCm);
    distanceCm=0;
    waveDuration=0;
    digitalWrite(echoPin, LOW);

    digitalWrite(trigPin, LOW);
    delay(2);
    digitalWrite(trigPin, HIGH);
    delay(10);
    digitalWrite(trigPin, LOW);
    waveDuration = pulseIn(echoPin, HIGH);
    distanceCm = (waveDuration*0.034)/2.0; 

   }
    //End Proximity sensor
    DHT11_temp_sensor_value = DHT.read11(DHT11_temp_sensor);

  temperature = DHT.temperature;
  humidity = DHT.humidity;
  
}
