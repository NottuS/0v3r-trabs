//#include <Servo.h> 
//#include "Ultrasonic.h"


// Ultrasom
// --> front 
#define f_echoPin 3 //Pino 3 recebe o pulso do echo
#define f_trigPin 2 //Pino 2 envia o pulso para gerar o echo
//iniciando a função e passando os pinos
//Ultrasonic f_ultrasonic(f_trigPin,f_echoPin);

// --> left
#define l_echoPin 11 
#define l_trigPin 4 
//Ultrasonic l_ultrasonic(l_trigPin,l_echoPin);

// --> right
#define r_echoPin 12 
#define r_trigPin 13 
//Ultrasonic r_ultrasonic(r_trigPin,r_echoPin);

int l,r,f; // Left, Right e Forward -> Variáveis das distancias obtidas pelo Ultrasom.

// Motor
#define ENA 10 // Azul - Dir
#define IN1 9  // Verde
#define IN2 8  // Amarelo
#define ENB 5 // Marrom - Esq
#define IN3 7  // Laranja
#define IN4 6  // Vermelho
int delayMotor = 300; // Tempo em que o motor funciona

// Servo
/*#define SERV 4 
Servo myservo;  // Cria um servo object para controlar o servo (max 8)
int pos = 0;    // Variável para armazenar a posição do servo 
*/
int pinLDR = 5; //Porta analógica utilizada pelo LDR  
int state = 0;

char inData[20]; // Allocate some space for the string
char inChar=-1; // Where to store the character read
byte index = 0; // Index into array; where to store the character
 
 
int mapWork[5][5];
int x = 0;
int y = 0;
int mapPosition = 0; // 0 - N, 1 - D, 2 - E, 3 - S

void setup() 
{ 
  //myservo.attach(SERV);  // Vincula o servo no pino referente(SERV) ao servo object 
  Serial.begin(9600); //inicia a porta serial
  // Setando os pinos para o Ultrasom
  pinMode(f_echoPin, INPUT); // define o pino 3 como entrada (recebe)
  pinMode(f_trigPin, OUTPUT); // define o pino 2 como saida (envia)

  pinMode(l_echoPin, INPUT);
  pinMode(l_trigPin, OUTPUT);
  
  pinMode(r_echoPin, INPUT);
  pinMode(r_trigPin, OUTPUT);
  
  // Setando os pinos para o Motor
  pinMode (ENA, OUTPUT);
  pinMode (IN1, OUTPUT);
  pinMode (IN2, OUTPUT);
  pinMode (ENB, OUTPUT);
  pinMode (IN3, OUTPUT);
  pinMode (IN4, OUTPUT);
  pinMode (13, OUTPUT);  
  
  initMap();
} 
 
 
void loop() 
{
 
   //forward();
  
  btCommand();
  //btSimpleCommand();
  delay(2000);
  
  //nextPosition();
} 

void initMap(){
  int i,j;
  for(i = 0; i < 5; i++)
    for(j = 0; j < 5; j++){
      mapWork[i][j] = 0;
    }
}

int ultrSonic(int trigPin,int echoPin){
  
  long duration;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  duration = (((duration/2) / 2.91))/10;
  Serial.println((int)duration);
  return (int)duration;   

}
 
void stepForward(){
    forward(200,200);
    delay(750);
    stope();
    //Serial.println("SFW");
}

void forward(int r, int l)
{
    //Serial.println("FW");
    digitalWrite (IN1, HIGH);
    digitalWrite (IN2, LOW);
    digitalWrite (IN3, LOW);
    digitalWrite (IN4, HIGH);
    /*digitalWrite (ENA, HIGH);
    digitalWrite (ENB, HIGH);*/
    analogWrite (ENA, r);
    analogWrite (ENB, l);
    Serial.println("A\n");
}

void reverse()
{   
    digitalWrite (IN1, LOW);
    digitalWrite (IN2, HIGH);
    digitalWrite (IN3, HIGH);
    digitalWrite (IN4, LOW);
    digitalWrite (ENA, HIGH);
    digitalWrite (ENB, HIGH);
    //Serial.println("RV");

}

void rotate_right()
{
    digitalWrite (IN1, LOW);
    digitalWrite (IN2, HIGH);
    digitalWrite (IN3, LOW);
    digitalWrite (IN4, HIGH);
    /*analogWrite (ENA, 200);
    analogWrite (ENB, 200);*/
    digitalWrite (ENA, HIGH);
    digitalWrite (ENB, HIGH);
    //Serial.println("RR");
}

void rotate_left()
{
    digitalWrite (IN1, HIGH);
    digitalWrite (IN2, LOW);
    digitalWrite (IN3, HIGH);
    digitalWrite (IN4, LOW);
    /*analogWrite (ENA, 200);
    analogWrite (ENB, 200);*/
    digitalWrite (ENA, HIGH);
    digitalWrite (ENB, HIGH);
    //Serial.println("RL");
}

void left()
{
    digitalWrite (IN1, HIGH);
    digitalWrite (IN2, LOW);
    digitalWrite (IN3, LOW);
    digitalWrite (IN4, HIGH);
    analogWrite (ENA, 255);
    analogWrite (ENB, 150);
    //Serial.println("L");
}

void right()
{
    digitalWrite (IN1, HIGH);
    digitalWrite (IN2, LOW);
    digitalWrite (IN3, LOW);
    digitalWrite (IN4, HIGH);
    analogWrite (ENA, 150);
    analogWrite (ENB, 255);
    //Serial.println("R");
}

void stope()
{
    //digitalWrite (BEEP, LOW);
/*  digitalWrite (IN1, LOW);
    digitalWrite (IN2, LOW);
    digitalWrite (IN3, LOW);
    digitalWrite (IN4, LOW);*/
    digitalWrite (ENA, LOW);
    digitalWrite (ENB, LOW);
    //Serial.println("S");
}

void backward()
{
     rotate_left();
     delay(200);
     rotate_left();
     delay(200);
     forward(200,200);
}

void btSimpleCommand() {
  int c;
  
  if (Serial.available() > 0){
    c = Serial.read();
    //Serial.print(c);
    /*Serial.print("BT: ");
    Serial.print(c);
    Serial.println(" :");*/
  
    if (c=='S') stope();
    if (c=='F') forward(200,200);
    if (c=='B') backward();
    if (c=='L') left();
    if (c=='R') right();
    if (c==69) rotate_left();    
    if (c==70) rotate_right();  
    /*if (c==67) beep();
    //stope();
    */
   }
}

void btCommand() {
  char c;
  int i;

    while (Serial.available() > 0) // S efetua a leitura
                                   // quando houver dados
    {
        if(index < 19) // Le ate 20 caracteres
        {
            inChar = Serial.read(); // Le um caracter
            inData[index] = inChar; // aramazena-o
            index++;
            inData[index] = '\0';
        }
    }
     c = inChar;
     
     for(i = 0; i < index;i++) 
     {
       //Serial.println(inData[i]);
       switch(inData[i]) 
       {
         case 4:
                   stope();
                   break;
         case 1:
                   if( r < 10 )
                     forward(200,150);
                   else if( l < 10 )
                     forward(150,250);
                   else
                     forward(200,200);  
                   delay(500); 
                   break;
         case 3:
                   backward();
                   delay(500);
                   break;
         case 0:
                   rotate_left();
                   delay(200);
                   forward(150,250);
                   delay(200);
                   break;
         case 2:
                   rotate_right();
                   delay(200);
                   forward(150,250);
                   delay(200);
                   break;
         case 'S':
                  l = ultrSonic(l_trigPin, l_echoPin); 
                  f = ultrSonic(f_trigPin, f_echoPin);
                  r = ultrSonic(r_trigPin, r_echoPin);
                  state = analogRead(pinLDR);  
                  Serial.println(state);
                  delay(200);
                  break;
       }
       
     }
     
  inChar=-1;
  index = 0;
  stope();
  //delay(2000);
  
}


/*void correctPosition(int l,int r,int f){
  while(r > 16) {
      rotate_right();
      delay(200);
      stope();
      r = ultrSonic(r_trigPin, r_echoPin);
  }  
  while(l > 16) {
    rotate_left();
    delay(200);
    stope();
    l = ultrSonic(l_trigPin, l_echoPin);
  }
}*/
 void nextPosition() {
   /*correctPosition(l,r,f);
   if(f >= 20){
     forward(200,200);
     delay(750);
   } else if(r >= 20) {
      rotate_right();
      delay(delayMotor*2);
      forward(200,200);
      delay(delayMotor);
   } else if(l >= 20) {
      rotate_left();
      delay(delayMotor*2);
      forward(200,200);
      delay(delayMotor);
   } else {
     rotate_left();
     delay(delayMotor*2);
     rotate_left();
     delay(delayMotor*2);
     forward(200,200);
     delay(delayMotor);
   }
   delay(1000);
   stope();*/
   if(f > 20) {
     
   if( r < 10 )
     forward(200,110);
   else if( l < 10 )
     forward(110,200);
   else 
     forward(200,200);  
   }
   delay(250);
   stope();
 }
 

