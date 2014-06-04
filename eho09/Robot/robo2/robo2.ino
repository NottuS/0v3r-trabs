#include <LiquidCrystal.h>
#include <SoftwareSerial.h> //Software Serial Port

#define uint8 unsigned char 
#define uint16 unsigned int
#define uint32 unsigned long int

#define RxD 6 // This is the pin that the Bluetooth (BT_TX) will transmit to the Arduino (RxD)
#define TxD 7 // This is the pin that the Bluetooth (BT_RX) will receive from the Arduino (TxD)
 
#define DEBUG_ENABLED 1


int Clkpin = 9; //RGB LED Clock Pin (Digital 9)
int Datapin = 8; //RGB LED Data Pin (Digital 8)
//int ENA = ; //PWM
//int IN1 = ;
//int IN2 = ;
//
//int ENB = ; //PWM
//int IN3 = ;
//int IN4 = ;

#define ENA 5 // brown
#define IN1 6  // red
#define IN2 7  // orange

#define ENB 11 // blue
#define IN3 9  // yellow
#define IN4 10  // green

//LiquidCrystal LCD (2, 8, 4, 5, 6, 7);
SoftwareSerial blueToothSerial(RxD,TxD);

void setup ()
{
 Serial.begin(9600); // Allow Serial communication via USB cable to computer (if required)
 pinMode(RxD, INPUT); // Setup the Arduino to receive INPUT from the bluetooth shield on Digital Pin 6
 pinMode(TxD, OUTPUT); // Setup the Arduino to send data (OUTPUT) to the bluetooth shield on Digital Pin 7
 pinMode(13,OUTPUT); // Use onboard LED if required.
 setupBlueToothConnection(); //Used to initialise the Bluetooth shield
 
 pinMode(Datapin, OUTPUT); // Setup the RGB LED Data Pin
 pinMode(Clkpin, OUTPUT); // Setup the RGB LED Clock pin
  
  pinMode (ENA, OUTPUT);
  pinMode (IN1, OUTPUT);
  pinMode (IN2, OUTPUT);
  pinMode (ENB, OUTPUT);
  pinMode (IN3, OUTPUT);
  pinMode (IN4, OUTPUT);
  Serial.begin (9600);

}

void test_motor_1 ()
{
  //LCD.print ("Motor 1 Speed:");
  for (int i = 0; i < 256; i++)
  {
    digitalWrite (IN1, HIGH);
    digitalWrite (IN2, LOW);
    analogWrite (ENA, i);
    delay (50);
    Serial.println(i);
  }

  delay (3000);
  digitalWrite (IN1, LOW);
  
  for (int i = 0; i < 256; i++)
  {
    digitalWrite (IN1, LOW);
    digitalWrite (IN2, HIGH);
    analogWrite (ENA, i);
    delay (50);
    Serial.println(i);
  }

  delay (3000);
  digitalWrite (IN2, LOW);
}

void test_motor_2 ()
{
  //LCD.print ("Motor 2 Speed");
  for (int i = 0; i < 256; i++)
  {
    digitalWrite (IN3, HIGH);
    digitalWrite (IN4, LOW);
    analogWrite (ENB, i);
    delay (50);
    Serial.println(i);
  }

  delay (3000);
  digitalWrite (IN3, LOW);
  
  for (int i = 0; i < 256; i++)
  {
    digitalWrite (IN3, LOW);
    digitalWrite (IN4, HIGH);
    analogWrite (ENB, i);
    delay (50);
    Serial.println(i);
  }

  delay (3000);
  digitalWrite (IN4, LOW);
}

void test(){
  digitalWrite(13,LOW); //Turn off the onboard Arduino LED
 char recvChar;
 while(1){
 if(blueToothSerial.available()){//check if there's any data sent from the remote bluetooth shield
 recvChar = blueToothSerial.read();
 Serial.print(recvChar); // Print the character received to the Serial Monitor (if required)
 
 //If the character received = 'r' , then change the RGB led to display a RED colour
 if(recvChar=='r'){
 Send32Zero(); // begin
 DataDealWithAndSend(255, 0, 0); // first node data
 Send32Zero(); // send to update data 
 }
 
 //If the character received = 'g' , then change the RGB led to display a GREEN colour
 if(recvChar=='g'){
 Send32Zero(); // begin
 DataDealWithAndSend(0, 255, 0); // first node data
 Send32Zero(); // send to update data 
 }
 
 //If the character received = 'b' , then change the RGB led to display a BLUE colour
 if(recvChar=='b'){
 Send32Zero(); // begin
 DataDealWithAndSend(0, 0, 255); // first node data
 Send32Zero(); // send to update data 
 }
 }
 
 //You can use the following code to deal with any information coming from the Computer (serial monitor)
 if(Serial.available()){
 recvChar = Serial.read();
 
 //This will send value obtained (recvChar) to the phone. The value will be displayed on the phone.
 blueToothSerial.print(recvChar);
 }
 }
}

void loop()
{
  test_motor_1();
  test_motor_2();
}


//The following code is necessary to setup the bluetooth shield ------copy and paste----------------
void setupBlueToothConnection()
{
 blueToothSerial.begin(38400); //Set BluetoothBee BaudRate to default baud rate 38400
 blueToothSerial.print("\r\n+STWMOD=0\r\n"); //set the bluetooth work in slave mode
 blueToothSerial.print("\r\n+STNA=SeeedBTSlave\r\n"); //set the bluetooth name as "SeeedBTSlave"
 blueToothSerial.print("\r\n+STOAUT=1\r\n"); // Permit Paired device to connect me
 blueToothSerial.print("\r\n+STAUTO=0\r\n"); // Auto-connection should be forbidden here
 delay(2000); // This delay is required.
 blueToothSerial.print("\r\n+INQ=1\r\n"); //make the slave bluetooth inquirable 
 Serial.println("The slave bluetooth is inquirable!");
 delay(2000); // This delay is required.
 blueToothSerial.flush();
}


//The following code snippets are used update the colour of the RGB LED-----copy and paste------------
void ClkProduce(void){
 digitalWrite(Clkpin, LOW);
 delayMicroseconds(20); 
 digitalWrite(Clkpin, HIGH);
 delayMicroseconds(20); 
}
 
void Send32Zero(void){
 unsigned char i;
 for (i=0; i<32; i++){
 digitalWrite(Datapin, LOW);
 ClkProduce();
 }
}
 
uint8 TakeAntiCode(uint8 dat){
 uint8 tmp = 0;
 if ((dat & 0x80) == 0){
 tmp |= 0x02; 
 }
 
 if ((dat & 0x40) == 0){
 tmp |= 0x01; 
 }
 
 return tmp;
}
 
// gray data
void DatSend(uint32 dx){
 uint8 i;
 for (i=0; i<32; i++){
 if ((dx & 0x80000000) != 0){
 digitalWrite(Datapin, HIGH);
 } else {
 digitalWrite(Datapin, LOW);
 }
 
 dx <<= 1;
 ClkProduce();
 }
}
 
// data processing
void DataDealWithAndSend(uint8 r, uint8 g, uint8 b){
 uint32 dx = 0;
 
 dx |= (uint32)0x03 << 30; // highest two bits 1，flag bits
 dx |= (uint32)TakeAntiCode(b) << 28;
 dx |= (uint32)TakeAntiCode(g) << 26; 
 dx |= (uint32)TakeAntiCode(r) << 24;
 
 dx |= (uint32)b << 16;
 dx |= (uint32)g << 8;
 dx |= r;
 
 DatSend(dx);
}
