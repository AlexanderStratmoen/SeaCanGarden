//THIS CODE WAS A TEST FOR A FRONT FACING UI OVER SERIAL USED TO EDIT THE CONFIG CODE.
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

char input;
 if(Serial.read() == ' '){
  Serial.println("Keyboard interupt detected");
  Serial.println("set sample rate");
  delay(2000);
   input = Serial.read();
   Serial.print("You typed: " );
   Serial.println(input);

 }else{
  Serial.println("Data");
  delay(1000);
 }

}
