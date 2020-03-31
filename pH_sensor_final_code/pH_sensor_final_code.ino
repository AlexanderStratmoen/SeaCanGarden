//THIS IS THE FINAL CONTROL CODE FOR THE ARDUINO THAT WILL BE RUNNING THE PH SENSOR. IT WILL THEN SEND DATA TO OVER SERIAL TO THE MAIN ARDUINO.


/*
 # Code is used to run the pH meter V1.0.
 #Edited from web, created by Alexander Stratmoen 2020
 # Ver : 1.0
 # Product: analog pH meter
 # SKU : SEN0161
  Receives from the hardware serial, sends to software serial.
 Receives from software serial, sends to hardware serial.

 The circuit:
 * RX is digital pin 2 (connect to TX of other device)
 * TX is digital pin 3 (connect to RX of other device)

 Note:
 Not all pins on the Mega and Mega 2560 support change interrupts,
 so only the following can be used for RX:
 10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69

 Not all pins on the Leonardo support change interrupts,
 so only the following can be used for RX:
 8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).
  //Needs to be calibrated see testing code audrino document for how to do this
  //SeaCan pH sensor sample Code. Contains code for 2 warning lights that flash when pH value is above/ below normal. Test with tap water and 3M KCL. If values or off, you need to recalibrate. 
  //Instructions on calibrating are at the DFRobot website. https://media.digikey.com/pdf/Data%20Sheets/DFRobot%20PDFs/SEN0161_SEN0169_Web.pdf
*/

#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX


#define SensorPin A0 //pH meter Analog output to Arduin
//o Analog Input 0
#define Offset 0.00 //deviation compensate
#define LED 13
#define samplingInterval 20
#define printInterval 200
#define ArrayLenth 40 //times of collection
int pHArray[ArrayLenth]; //Store the average value of the sensor
//feedback
int pHArrayIndex=0;

int ledPin = 12; //too acidic pH level
int ledPin2 = 13; //too basic pH level

int pump1 = 14; //too acidic pH level
int pump2 = 15; //too acidic pH level

void setup () {
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
  ;  // wait for serial port to connect. Needed for Native USB only

  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);

  pinMode (pump1, OUTPUT);
  pinMode (pump2, OUTPUT);
  
  
  pinMode (ledPin, OUTPUT);
  pinMode (ledPin2, OUTPUT);

 
  pinMode(LED,OUTPUT);
  } 
}

void loop () {

 
 static unsigned long samplingTime = millis();
 static unsigned long printTime = millis();
 static float pHValue,voltage;
 
  if(pHValue < 6.5)  //if pH value goes below 6.5, light1 should flash as a warning. When pH goes back to normal, light will stop flashing. 
  {
  
   digitalWrite(ledPin, HIGH);
   digitalWrite(pump1, HIGH);
   delay(1000);
   digitalWrite(ledPin, LOW);
   digitalWrite(pump1, LOW);
   delay(1000);
  }
  if(pHValue > 7.5) //if pH value goes above 7.5, light2 should flash as a warning. Light will stop when pH returns to normal. 
  
  {
   digitalWrite(ledPin2, HIGH);
   digitalWrite(pump2, HIGH);
   delay(1000);
   digitalWrite(ledPin2, LOW);
   digitalWrite(pump2, LOW);
   delay(1000);
  }
 

 if(millis()-samplingTime > samplingInterval)
 {
 pHArray[pHArrayIndex++]=analogRead(SensorPin);
 if(pHArrayIndex==ArrayLenth)pHArrayIndex=0;
 voltage = avergearray(pHArray, ArrayLenth)*5.0/1024;
 pHValue = 3.5*voltage+Offset;
 samplingTime=millis();
 }
 if(millis() - printTime > printInterval) //Every 800 milliseco
//nds, print a numerical, convert the state of the LED indicator
 {
 Serial.print("Voltage:");
 Serial.print(voltage,2);
 Serial.print(" pH value: ");
 Serial.println(pHValue);
 mySerial.write(pHValue);
 digitalWrite(LED,digitalRead(LED)^1);
 printTime=millis();
 }
}

double avergearray(int* arr, int number){
 int i;
 int max,min;
 double avg;
 long amount=0;
 if(number<=0){
 Serial.println("Error number for the array to avraging!/n");
 return 0;
 } 
 if(number<5){ //less than 5, calculated directly statistics
 for(i=0;i<number;i++)
 {
 amount+=arr[i];
 }
 avg = amount/number;
 return avg;
 }else{
 if(arr[0]<arr[1]){
 min = arr[0];max=arr[1];
 }
 else{
 min=arr[1];max=arr[0];
 }
 for(i=2;i<number;i++){
 if(arr[i]<min){
 amount+=min; //arr<min
 min=arr[i];
 }else {
 if(arr[i]>max){
 amount+=max; //arr>max
 max=arr[i];
 }else{
 amount+=arr[i]; //min<=arr<=max
 }
 }//if
 }//for
 avg = (double)amount/(number-2);
 }//if
 return avg;
} 
