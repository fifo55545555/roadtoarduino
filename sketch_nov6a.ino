// 27cm/s rýchlosť robota
// zistiť dialku aku dojazdi po vypnuti motora (cm)
// musime spraviť aby sa otáčal okolo stredu kryžovatky
int tas=0;

float block = 30; //cm
int ir = 650; //0 - 1000
bool button = false;
int delAfterKrizovatkaTurn = 500;

void setup() {

 pinMode(LED_BUILTIN, OUTPUT);
 pinMode(30, INPUT); //button

 pinMode(5,OUTPUT);
 pinMode(6,OUTPUT);
 pinMode(7,OUTPUT);

 pinMode(2,OUTPUT);
 pinMode(3,OUTPUT);
 pinMode(4,OUTPUT);

 pinMode(A0,INPUT);
 pinMode(A1,INPUT);
 pinMode(A2,INPUT);
 pinMode(A3,INPUT);
 pinMode(A4,INPUT);

 pinMode(8, OUTPUT);
 pinMode(9, INPUT);

 Serial.begin(9600);

}

void loop() {

   tas=0;

   if(analogRead(A0) < ir ){tas++;}
   if(analogRead(A1) < ir ){tas++;}
   if(analogRead(A2) < ir ){tas++;}
   if(analogRead(A3) < ir ){tas++;}
   if(analogRead(A4) < ir ){tas++;}

   if(digitalRead(30) == true){
      button = !button;
      delay(250);
   }

   if(button){
      if(tas >= 3){
         vpred(296); // vzdialenosť senzora od stredu motora (8cm) / rychlost robota
         //vpred(0); // ked dojazdi + vpred(int del) tak aby stred robota bol v strede krizovatky
         if(ultraZvuk(3)<block){
            turn(true, 250);
            stop(); delay(delAfterKrizovatkaTurn);

            if(ultraZvuk(3)<block){
               turn(false, 250);
               stop(); delay(delAfterKrizovatkaTurn);
               turn(false, 250);
               stop(); delay(delAfterKrizovatkaTurn);

               if(ultraZvuk(3)<block){
                  turn(false, 250);
                  vpred(-1);
                  stop(); delay(delAfterKrizovatkaTurn);
               }else{vpred(-1);}
            }else{vpred(-1);}
         }else{vpred(-1);}


      }else if(analogRead(A0) < ir){
         turn(true, 0);
      }else if(analogRead(A4) < ir){
         turn(false, 0);
      }else if(analogRead(A2) < ir){
         vpred(-1);
         }
   }else{
      stop();
   }
  //if(digitalRead(30)){vpred(3000);}


}

int vpred(int del){

   digitalWrite(5, LOW);  // dozadu
   digitalWrite(6, HIGH);  // dopredu
   analogWrite(7, 250);   // rzchlosť

   analogWrite(2, 1);  // rzchlosť
   digitalWrite(3, HIGH);  // dopredu
   digitalWrite(4, LOW);   // dozadu

   if(del == -1){
      return 0;
   }
   delay(del);

   digitalWrite(5, LOW);  // dozadu
   digitalWrite(6, LOW);  // dopredu
   analogWrite(7, 0);   // rzchlosť

   analogWrite(2, 0);  // rzchlosť
   digitalWrite(3, LOW);  // dopredu
   digitalWrite(4, LOW);   // dozadu
   return 1;
}

void stop(){
   digitalWrite(5, LOW);  // dozadu
   digitalWrite(6, LOW);  // dopredu
   analogWrite(7, 0);   // rzchlosť

   analogWrite(2, 0);  // rzchlosť
   digitalWrite(3, LOW);  // dopredu
   digitalWrite(4, LOW);   // dozadu
}

int turn(bool left, int del){
   if(left){
      digitalWrite(5, LOW);  // dozadu
      digitalWrite(6, HIGH);  // dopredu
      analogWrite(7, 5);   // rzchlosť

      analogWrite(2, 0);  // rzchlosť
      digitalWrite(3, LOW);  // dopredu
      digitalWrite(4, HIGH);   // dozadu
   }else{
      digitalWrite(5, HIGH);  // dozadu
      digitalWrite(6, LOW);  // dopredu
      analogWrite(7, 5);   // rzchlosť

      analogWrite(2, 0);  // rzchlosť
      digitalWrite(3, HIGH);  // dopredu
      digitalWrite(4, LOW);   // dozadu
   }

   if(del == -1){
      return 0;
   }
   delay(del);

   while(true){
      if(analogRead(A2) < 650){break;}
   }

   return 1;
}

float ultraZvuk(int repeat){
  float distance = 0;
  for (int i = 0; i < repeat; i++){
    digitalWrite(8, LOW);
    delayMicroseconds(2);
    digitalWrite(8, HIGH);
    delayMicroseconds(10);
    digitalWrite(8, LOW);

    float duration = pulseIn(9, HIGH);
    distance += (duration*.0343)/2;
    delay(10);
  }
  distance /= repeat;
  if(distance > 100){
   return 100;
  }
  return distance;
}

//void right(){
//   digitalWrite(5, HIGH);  // dozadu
//   digitalWrite(6, LOW);  // dopredu
//   analogWrite(7, 5);   // rzchlosť
//
//   analogWrite(2, 0);  // rzchlosť
//   digitalWrite(3, HIGH);  // dopredu
//   digitalWrite(4, LOW);   // dozadu
//
//   while(true){
//      if(analogRead(A2) < 650){break;}
//   }}

