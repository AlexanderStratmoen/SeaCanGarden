//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//This is the code to print data from a c02 sensor to the LCD screen and to excel over serial
//Code was taken from demos on github as well as from the other parts of project
//To set up connect arduino to computer, load excel with data logger plug in and connect to serial port that arduino is on, then start data logging. 
//Edited by: Alexander 2020 Pre-Eng Semester-2
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

//Set sensor input

int sensorIn = A0;

// Include Libraries

#include <Wire.h>
#include "rgb_lcd.h"

// Set RGB LCD operating color (can chage for other colors)
rgb_lcd lcd;

const int colorR = 255;
const int colorG = 0;
const int colorB = 255;

void setup(){
  Serial.begin(9600);
  // Set the default voltage of the reference voltage for serial comunication 
  analogReference(DEFAULT);

   // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
//set LCD color
    lcd.setRGB(colorR, colorG, colorB);

    // Print a message to the LCD.
    lcd.print("C02 Level:");

    delay(1000);
}

void loop(){
  lcd.setCursor(0, 1);
  //Read voltage of Co2 sensor
  int sensorValue = analogRead(sensorIn);

  // The analog signal is converted to a voltage
  float voltage = sensorValue*(5000/1024.0);
  //If the voltage is 0 print out an error message to LCD and set red color
  if(voltage == 0)
  {
    lcd.print("Error    ");
    lcd.setRGB(255, 0, 0);
  }
  //If volatge is lowver than 400 print out a preheating message to LCD and set green color
  else if(voltage < 400)
  {
    lcd.print("preheating");
    lcd.setRGB(0, 255, 0);
  }
  //If voltage is in a normal range print out concentration on LCD, set color purple and prin tout that value over serial
  else
  {
    int voltage_diference=voltage-400;
    float concentration=voltage_diference*50.0/16.0;
    //Print CO2 concentration
    lcd.print(concentration);
    lcd.print("ppm    ");
    lcd.setRGB(colorR, colorG, colorB);
    //We have to add another line to serial for excel to notice data so we add a 0 to the end of the value
    Serial.print(concentration);
    Serial.println("0");
  }
  delay(100);
}
