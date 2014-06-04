//for use bluetoth, plug the module bluetoth on sensor shield, and pair with android smatphone
//use application terminal bluetoth for controll the robot
//deve estar tudo escrito errado ai em cima

#include <LiquidCrystal.h>

#define ENA 5 // brown
#define IN1 6  // red
#define IN2 7  // orange

#define ENB 11 // blue
#define IN3 9  // yellow
#define IN4 10  // green
LiquidCrystal LCD(12, 11, 4, 3, 2, 13);

void setup()
{
  pinMode (ENA, OUTPUT);
  pinMode (IN1, OUTPUT);
  pinMode (IN2, OUTPUT);
  pinMode (ENB, OUTPUT);
  pinMode (IN3, OUTPUT);
  pinMode (IN4, OUTPUT);
  pinMode (13, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  char c;
  if(Serial.available())
    c = Serial.read();
  else
    //digitalWrite(13, HIGH);
  if (c=='S') {stope(); digitalWrite(13, LOW);}
  if (c=='F') forward();
  if (c=='B') reverse();  
  if (c=='L') rotate_left();    
  if (c=='R') rotate_right();  
  if (c=='5') message();
  if (c=='A') {Serial.write('A'); digitalWrite(13, HIGH);}
  c = '0';
  
}

void forward()
{
    digitalWrite (IN1, LOW);
    digitalWrite (IN2, HIGH);
    digitalWrite (IN3, HIGH);
    digitalWrite (IN4, LOW);
    digitalWrite (ENA, HIGH);
    digitalWrite (ENB, HIGH);
}

void reverse()
{
    digitalWrite (IN1, HIGH);
    digitalWrite (IN2, LOW);
    digitalWrite (IN3, LOW);
    digitalWrite (IN4, HIGH);
    digitalWrite (ENA, HIGH);
    digitalWrite (ENB, HIGH);

}

void rotate_right()
{
    digitalWrite (IN1, HIGH);
    digitalWrite (IN2, LOW);
    digitalWrite (IN3, HIGH);
    digitalWrite (IN4, LOW);
    digitalWrite (ENA, HIGH);
    digitalWrite (ENB, HIGH);

}

void rotate_left()
{
    digitalWrite (IN1, LOW);
    digitalWrite (IN2, HIGH);
    digitalWrite (IN3, LOW);
    digitalWrite (IN4, HIGH);
    digitalWrite (ENA, HIGH);
    digitalWrite (ENB, HIGH);

}

void stope()

{
   digitalWrite (IN1, LOW);
   digitalWrite (IN2, LOW);
   digitalWrite (IN3, LOW);
   digitalWrite (IN4, LOW);
   digitalWrite (ENA, LOW);
   digitalWrite (ENA, LOW);

    delay(100);

}

void message ()
{
  LCD.begin(16,2);              // columns, rows.  use 16,2 for a 16x2 LCD, etc.
  LCD.clear();                  // start with a blank screen
  LCD.setCursor(0,0);           // set cursor to column 0, row 0 (the first row)
  LCD.print("text");    // change this text to whatever you like. keep it clean.
  LCD.setCursor(0,1);           // set cursor to column 0, row 1
  LCD.print("text2");
}


