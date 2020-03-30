//THIS IS THE ORIGINAL CODE FROM THE PREVIOUS STUDENTS FOR THE PH SENSOR
//Needs to be calibrated see testing code audrino document for how to do this
//SeaCan pH sensor sample Code. Contains code for 2 warning lights that flash when pH value is above/ below normal. Test with tap water and 3M KCL. If values or off, you need to recalibrate. 
//Instructions on calibrating are at the DFRobot website. https://media.digikey.com/pdf/Data%20Sheets/DFRobot%20PDFs/SEN0161_SEN0169_Web.pdf
/*
 # This sample code is used to test the pH meter V1.0.
 # Editor : YouYou
 # Ver : 1.0
 # Product: analog pH meter
 # SKU : SEN0161
*/
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

int ledPin = 11; //too acidic pH level
int ledPin2 = 13; //too basic pH level

void setup(void)
{
 pinMode (ledPin, OUTPUT);
 pinMode (ledPin2, OUTPUT);

 
 pinMode(LED,OUTPUT);
 Serial.begin(9600);
 Serial.println("pH meter experiment!"); //Test the serial monitor
} 
void loop(void)
{


 
 static unsigned long samplingTime = millis();
 static unsigned long printTime = millis();
 static float pHValue,voltage;
if(pHValue < 6.5)  //if pH value goes below 6.5, light1 should flash as a warning. When pH goes back to normal, light will stop flashing. 
{
  
  digitalWrite(ledPin, HIGH);
  delay(1000);
  digitalWrite(ledPin, LOW);
  delay(1000);
}
if(pHValue > 7.5) //if pH value goes above 7.5, light2 should flash as a warning. Light will stop when pH returns to normal. 
{
  digitalWrite(ledPin2, HIGH);
  delay(1000);
  digitalWrite(ledPin2, LOW);
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
 Serial.println(pHValue,2);
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