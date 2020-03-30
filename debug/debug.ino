//THIS IS A DEBUG CODE THAT WAS USED WHEN I WAS TRYING TO FIND OUT WHAT WAS WRONG WITH SENSORS FOR THE LED DISPLAY
//Set sensor input

int sensorIn = A0;

// Include Libraries

#include <Wire.h>
#include "rgb_lcd.h"

// Set RGB LCD operating color (can change for other colors)
rgb_lcd lcd;

float voltage;
const int colorR = 255;
const int colorG = 0;
const int colorB = 255;

void setup(){
  Serial.begin(9600);
  // Set the default voltage of the reference voltage for serial communication 
  analogReference(DEFAULT);

   // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
//set LCD color
    lcd.setRGB(colorR, colorG, colorB);

    // Print a message to the LCD.
    lcd.print("C02 Level:");

    delay(1000);
}

void c02screen()
{
  lcd.setCursor(0, 1);
    if(voltage == 0)
  {
    lcd.print("Error    ");
    lcd.setRGB(255, 0, 0);
  }
  //If voltage is lower than 400 print out a preheating message to LCD and set green color
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
}

void loop(){
  //Read voltage of Co2 sensor
  int sensorValue = analogRead(sensorIn);
  // The analog signal is converted to a voltage
  voltage = sensorValue*(5000/1024.0);
  c02screen ();
  delay(100);
}
