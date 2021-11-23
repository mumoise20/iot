#include <dht.h>      // Importing the humidity and temperature sensor libary
#include <TimeLib.h> // Importing the time libary
#include <Servo.h>   // Importing the servo motor library
dht DHT;

Servo myServo;  //declare the servo object

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

// Declarations

int redPin = 8; // set red light to pin 8
int greenPin = 7; // set red light to pin 7
int bluePin = 6; // set blue light to pin 6
int smallBuzzerPin = 13; // set the buzzeru to pin 13
int fan = 22; // set the fan to pin 22
int heater = 26; // set the heater to pin 26
int count = 0;
int days = 0;
int pos = 0;
int hoursCounter = 21600; // initialising 6 hours time in seconds
int servoTarget =0; // value used to keeos the rotation angle of the servo motot
int lastTarget = 0;
int counter2 = 0;
int counter3 =0;

int DHT11_temp_sensor = A2; // Assign dht sensor as analog input to A2
int DHT11_temp_sensor_value = 0;

int temperature = 0; // value to hld the humidity
int humidity = 0; // value to hold the temperature
const int trigPin = 9; // connect trigger pin of ultrasonodinc sensot to pin 9
const int echoPin = 10; // connect trigger pin of ultrasonodinc sensot to pin 10

int distanceCm;
long waveDuration; 

time_t seconds = 0;// set the initial time



void setup() // setup function is used for initialisation
{
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.setCursor(0, 0);
  lcd.print(" Welcome to the ");
  lcd.setCursor(0, 1);
  lcd.print("Smart Incubator");
// setting up pins as INPUTS OUTPUTS
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT); 
  setTime(seconds);
  myServo.attach(24);
   
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(smallBuzzerPin, OUTPUT);
  pinMode(fan, OUTPUT);
  pinMode(heater, OUTPUT);

  Serial.begin(9600);  // baud rate of 9600
  Serial.println("System is ON");
  digitalWrite(bluePin, HIGH);
  delay(1500);
  digitalWrite(bluePin, LOW);
  lcd.clear(); // reseting the LCD
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  //lcd.setCursor(0, 1);
  //Reading sensor inputs
  if (count ==0)
  {
  DHT11_temp_sensor_value = DHT.read11(DHT11_temp_sensor); // read the dht11 value

  temperature = DHT.temperature; // retrieve the temperature value
  humidity = DHT.humidity; // retrieve the humidity value
  }
  count+=1; // increase counter by 1
  myServo.write(0); // set the servo motor to 0 degrees

  while(temperature > 38) // loop to be executed while the temperature is beyond 38 degrees
  {
    digitalWrite(greenPin, LOW); // green light off
    digitalWrite(redPin, HIGH); // red light on
    digitalWrite(smallBuzzerPin, HIGH); // buzzer on
    delay(200); // buzzer dely time before it switches off
    digitalWrite(smallBuzzerPin, LOW); // buzzer off
    digitalWrite(fan, HIGH); // fan on
    digitalWrite(heater, LOW); // heater off

    Serial.println("---------------------");
    Serial.print("Temperature: ");
    Serial.println(temperature);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("    Fan ON");
    lcd.setCursor(0, 1);
    lcd.print("Temperature: ");
    lcd.print(temperature);

    delay(1200);
    digitalWrite(redPin, LOW);

    distanceInit(); // Call the distanceInit() function
    computeDistance(); // Call the computeDistance() function
    computeDays(); // Call the computeDays() function
    delay(1200);
    
    DHT11_temp_sensor_value = DHT.read11(DHT11_temp_sensor); // read the dht11 value
    temperature = DHT.temperature; // retrieve the temperature value
    humidity = DHT.humidity; // retrieve the humidity value
    
   }

   while (temperature < 37) // loop to be executed while the temperature is below 37 degrees
  {
    digitalWrite(fan, HIGH);
    digitalWrite(heater, HIGH);
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
    lcd.print("   Heater ON");
    lcd.setCursor(0, 1);
    lcd.print("Temp ");
    lcd.print(temperature);
    lcd.print(" Hum: ");
    lcd.print(humidity);
    delay(1200);
    distanceInit(); // Call the distanceInit() function
    computeDistance(); // Call the computeDistance() function
    computeDays(); // Call the computeDays() function
    delay(1200);
    
    DHT11_temp_sensor_value = DHT.read11(DHT11_temp_sensor);
    temperature = DHT.temperature;
    humidity = DHT.humidity;
   
    }
    
    while (temperature >= 37 && temperature <= 38) // loop to be executed while the temperature is between 37 and 38 degrees
    {
      Serial.println("---------------------");
      Serial.print("Temperature: ");
      Serial.println(temperature);
      Serial.print("Humidity: ");
      Serial.println(humidity);
      digitalWrite(fan, LOW);
      digitalWrite(heater, LOW);
      digitalWrite(redPin, LOW);  
      digitalWrite(greenPin, HIGH); 

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(" Incubator OK   ");
      lcd.setCursor(0, 1);
      lcd.print("Temp ");
      lcd.print(temperature);
      lcd.print(" Hum: ");
      lcd.print(humidity);
      delay(1200);
      digitalWrite(greenPin, LOW);
      distanceInit(); // Call the distanceInit() function
      computeDistance(); // Call the computeDistance() function
      computeDays(); // Call the computeDays() function
      delay(1200);

      while (humidity < 60) // loop to be executed while the temperature is beyond 38 degrees
    {
    digitalWrite(fan, HIGH);
    digitalWrite(heater, HIGH);
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
    lcd.print("   Moist Low   ");
    lcd.setCursor(0, 1);
    lcd.print("Temp ");
    lcd.print(temperature);
    lcd.print(" Hum: ");
    lcd.print(humidity);
    delay(1200);
    distanceInit(); // Call the distanceInit() function
    computeDistance(); // Call the computeDistance() function
    computeDays(); // Call the computeDays() function 
    delay(1200);
    
    DHT11_temp_sensor_value = DHT.read11(DHT11_temp_sensor);
    temperature = DHT.temperature;
    humidity = DHT.humidity;
   
    }

    while (humidity > 80)
    {
    digitalWrite(fan, HIGH);
    digitalWrite(heater, LOW);
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
    lcd.print("   Moist High   ");
    lcd.setCursor(0, 1);
    lcd.print("Temp ");
    lcd.print(temperature);
    lcd.print(" Hum: ");
    lcd.print(humidity);
    delay(1200);
    distanceInit(); // Call the distanceInit() function
    computeDistance(); // Call the computeDistance() function
    computeDays(); // Call the computeDays() function
    delay(1200);
    
    DHT11_temp_sensor_value = DHT.read11(DHT11_temp_sensor);
    temperature = DHT.temperature;
    humidity = DHT.humidity;
   
    }
    
      DHT11_temp_sensor_value = DHT.read11(DHT11_temp_sensor);
      temperature = DHT.temperature;
      humidity = DHT.humidity;
     }
    
}
void computeDistance() //function to compute the distance
{
   //Proximity sensor values
  digitalWrite(trigPin, LOW);
  delay(2);
  digitalWrite(trigPin, HIGH);
  delay(10);
  digitalWrite(trigPin, LOW);
  waveDuration = pulseIn(echoPin, HIGH); // retrieve the wave duration interval
  distanceCm = (waveDuration*0.034)/2.0; // computer the distance in cm
  Serial.println("---------------------");
  Serial.print("Distace in cm: ");
  Serial.println(distanceCm);
  
  while (distanceCm<=10 || distanceCm<0) // loop to be excecuted if the distance is less than 1m
  {
    digitalWrite(greenPin, LOW);
    digitalWrite(redPin, HIGH);
    digitalWrite(smallBuzzerPin, HIGH);
    delay(200);
    digitalWrite(smallBuzzerPin, LOW);
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
    distanceInit();//Call the distanceInit() function to sent pulses to the ultrasonic sensor

   }
    //End Proximity sensor
  
  }

  void distanceInit() //function to send pulse on the Ultrasonic sensor
  {
    distanceCm=0; // reseting the distance to zero
    waveDuration=0; // reseting the wave duration period to zero
    digitalWrite(echoPin, LOW);

    digitalWrite(trigPin, LOW);
    delay(2);
    digitalWrite(trigPin, HIGH);
    delay(10);
    digitalWrite(trigPin, LOW);
    waveDuration = pulseIn(echoPin, HIGH); // retrieve the wave duration 
    distanceCm = (waveDuration*0.034)/2.0; // computer the distance in cm
    }
  void computeDays() //function to compute the number of days
  {
    seconds = now(); // Retrieve the numver of elapsed seconds
    days = int(seconds/86400); // convert seconds in days
    Serial.println("---------------------");
    Serial.print("Seconds: ");
    Serial.println(seconds);
    //print to the LCD
    lcd.setCursor(0, 1);     
    lcd.print("   Days:   ");
    lcd.print(days);
    lcd.print("      ");
    digitalWrite(redPin, LOW);

    Serial.println("---------------------");
    Serial.print("Servo angle: ");
    Serial.println(myServo.read()); // print the curret servo motor degree to the serial monitor

    // Conditions to change the servo motor angle to 180 degrees
    counter2+=1;
    if (counter2%30) 
    {
      counter3 +=1;
      }

    if(counter3%2)
    {
      servoTarget=180;
      if(seconds>hoursCounter)
      {
        hoursCounter += 30;
        if (servoTarget != lastTarget)
        {
          myServo.write(servoTarget);
          lastTarget = servoTarget;
          Serial.println("---------------------");
          Serial.print("Servo angle: ");
          Serial.println(myServo.read());
      }}}
      // Conditions to to change the servo motor angle to 0 degree
     if(!(counter3%2))
     {
      servoTarget=0;
      if(seconds>hoursCounter)
      {
        hoursCounter += 30;
        if (servoTarget != lastTarget)
         {
            myServo.write(servoTarget);
            lastTarget = servoTarget;
            Serial.println("---------------------");
            Serial.print("Servo angle: ");
            Serial.println(myServo.read());
      }}}

    // Check if the hatching period has reached
    if (days>=20 && days<30)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("  Hatch period  ");
      lcd.setCursor(0, 1);
      lcd.print("   Days: ");
      lcd.print(days);
      lcd.print("     ");
      }
    // Check if the hatching period has passed
      if (days>=30 )
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Please reset the");
      lcd.setCursor(1, 1);
      lcd.print("incubator ");
      lcd.print(days);
      lcd.print("days");
      }
      
    }
