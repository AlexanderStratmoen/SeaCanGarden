//LATEST UPDATE OF MAIN CODE SORRY FOR NOT GREAT NOTATION.

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//This is the code to print data from all sensors to the LCD screen and to excel over serial
//Code was taken from demos on github as well as from the other parts of project
//To set up connect arduino to computer, load excel with data logger plug in and connect to serial port that arduino is on, then start data logging. 
//Note that sensor wires cannot cross or else the senor does not work and goes all wonkey printing high values
//Edited by: Alexander 2020 Pre-Eng Semester-2
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Include Libraries

#include <Wire.h>
#include "rgb_lcd.h"
#include <DHT.h>

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//Use this section to change data for program

//Set Co2 sensor input
int sensorIn = A0;

//Set Hum and Temp Sensor input
#define DHTPIN A1
// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

//Set the interval time (how often to sample in milliseconds)
int sensorsampletime = 1000;
//Set the screen hold time (how long the screen should hold on one display for in millseconds
//(due to rounding the screen may not switch exactly the inputed value of time))
int screenholdtime = 5000;
//MIN VALUES FOR BOTH ARE 500 anything lower will be changed to 500
// Set RGB LCD operating color (can change for other colors using 255 as max and 0 as min)
rgb_lcd lcd;

const int colorR = 255;
const int colorG = 0;
const int colorB = 255;
//This is the end of user changable code if you don't know what your doing DO NOT edit past this point,I warned you...
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


#define DHTTYPE DHT11 // DHT 11

//setup data
DHT dht(DHTPIN, DHTTYPE);

//declare global Verubles (For the Sensors)
float sensor1voltage;
float hums;
float temp;
float pHGlobal;
float v1;
float v2;
float v3;
float v4;
float v5;

//set time amount of times screen should loop based on inputed times
int screenholdloops = screenholdtime / sensorsampletime;
  
void setup(){
  
  //set the min time of
  if(sensorsampletime < 500)
  {
   sensorsampletime = 500;
  }

  if(screenholdtime < 500)
  {
    screenholdtime = 500;
  }
  Serial.begin(9600);
  // Set the default voltage of the reference voltage for serial communication 
  analogReference(DEFAULT);

  dht.begin();   //Begin DHT

  ph.begin();

   // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
//set LCD color
    lcd.setRGB(colorR, colorG, colorB);

    delay(1000);
}
// Loop for printing temp
void tempscreen(){
    lcd.setCursor(0, 1);
    if (isnan(temp))
    {
        lcd.print("error    ");
        lcd.setRGB(255, 0, 0);
    }
    else
    {
        lcd.print(temp);
        lcd.print(" *C  ");
        lcd.setRGB(colorR, colorG, colorB);
    }
}
// Loop for printing hum
void humscreen(){
    lcd.setCursor(0, 1);
    // check if returns are valid, if they are NaN (not a number) then something went wrong, then print out data over serial!
    if (isnan(hums))
    {
        lcd.print("error    ");
        lcd.setRGB(255, 0, 0);
    }
    else
    {
        lcd.print(hums);
        lcd.print(" %  ");
        lcd.setRGB(colorR, colorG, colorB);
    }
}
// Loop for printing pH
void phscreen(){
    lcd.setCursor(0, 1);
    lcd.print(pHGlobal);
}
// Loop for printing C02
void c02screen(){
  lcd.setCursor(0, 1);
    if(sensor1voltage == 0)
  {
    lcd.print("Error");
    lcd.setRGB(255, 0, 0);
  }
  //If voltage is lower than 400 print out a preheating message to LCD and set green color
  else if(sensor1voltage < 400)
  {
    lcd.print("preheating");
    lcd.setRGB(0, 255, 0);
  }
  //If voltage is in a normal range print out concentration on LCD, set color purple and prin out that value over serial
  else
  {
    int voltage_diference=sensor1voltage-400;
    float concentration=voltage_diference*50.0/16.0;
    //Print CO2 concentration
    lcd.print(concentration);
    lcd.print("ppm");
    lcd.setRGB(colorR, colorG, colorB);
  }
}


//Loop for reading all sensors and sending all data over serial

void readsensors (){
       //Read Co2 sensor
  int sensorValue = analogRead(sensorIn);
  // The analog signal is converted to a voltage
  sensor1voltage = sensorValue*(5000/1024.0);
      if(sensor1voltage > 400)
  {
    int voltage_diference=sensor1voltage-400;
    float concentration=voltage_diference*50.0/16.0;
    //we are setting veribles so we can print all at the end
    v1 = concentration;
  }
        //READ TEMP AND HUMS SENSOR
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to A0 seconds 'old' (its a very slow sensor)
      hums = dht.readHumidity();
      temp = dht.readTemperature();
 
    // check if returns are valid, if they are NaN (not a number) then something went wrong, then print out data over serial!
    if (!isnan(temp) || !isnan(hums))
    {
    v2 = hums;
    v3 = temp;
    }
}


void loop(){
  //Print the label to screen
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("C02 Level:");
  //Declare the count
  int count = screenholdloops;
  while(count > 0 )                                   // repeat until count is no longer greater than zero
  {
  readsensors ();
  c02screen ();
  delay(sensorsampletime);
  count = count -1;                                  // decrement count
  }


  lcd.clear();  
  lcd.setCursor(0, 0);
  lcd.print("Temp:");
  count = screenholdloops;
  while(count > 0 )                                   // repeat until count is no longer greater than zero
  {
  readsensors ();
  tempscreen ();
  delay(sensorsampletime);
  count = count -1;                                  // decrement count
  }


  lcd.clear();  
  lcd.setCursor(0, 0);
  lcd.print("Humidity:");
  count = screenholdloops;
  while(count > 0 )                                   // repeat until count is no longer greater than zero
  {
  readsensors ();
  humscreen ();
  delay(sensorsampletime);
  count = count -1;                                  // decrement count
  }


  lcd.clear();  
  lcd.setCursor(0, 0);
  lcd.print("pH Level:");
  count = screenholdloops;
  while(count > 0 )                                   // repeat until count is no longer greater than zero
  {
  readsensors ();
  phscreen ();
  delay(sensorsampletime);
  count = count -1;                                  // decrement count
  }
}
