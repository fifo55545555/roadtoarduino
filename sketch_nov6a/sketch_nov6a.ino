// 27cm/s rýchlosť robota
// zistiť dialku aku dojazdi po vypnuti motora (cm)
// musime spraviť aby sa otáčal okolo stredu kryžovatky

float block = 30; //cm
int ir = 650; //0 - 1000
int delAfterKrizovatkaTurn = 500;
int turnIRCD = 250; // (turn infrared cooldown) cooldown in ms that says how long to wait before measuring ir after started to turn
bool button = false; // if should start started

// no touch pls
int tas=0;
bool slepaUlicka = false;
bool lastTurnLeft = true;
bool fwCounter = 0; // how many times it went foward after last turn

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
      if(tas >= 3 && !slepaUlicka){
         vpred(296); // vzdialenosť senzora od stredu motora (8cm) / rychlost robota
         //vpred(0); // ked dojazdi + vpred(int del) tak aby stred robota bol v strede krizovatky
         if(ultrazvuk(3)<block){
            turn(true, turnIRCD);
            stop(); delay(delAfterKrizovatkaTurn);

            if(ultrazvuk(3)<block){
               turn(false, turnIRCD);
               stop(); delay(delAfterKrizovatkaTurn);
               turn(false, turnIRCD);
               stop(); delay(delAfterKrizovatkaTurn);

               if(ultrazvuk(3)<block){
                  turn(false, turnIRCD);
                  vpred(-1);
                  stop(); delay(delAfterKrizovatkaTurn);
                  slepaUlicka = true;

               }else{vpred(-1); slepaUlicka = false; lastTurnLeft = false; fwCounter = 0;}
            }else{vpred(-1); slepaUlicka = false; lastTurnLeft = true; fwCounter = 0;}
         }else{vpred(-1); slepaUlicka = false; fwCounter++;}


      }else if(tas >= 3 && slepaUlicka){
         vpred(296); // vzdialenosť senzora od stredu motora (8cm) / rychlost robota

         turn(true, turnIRCD);
         stop(); delay(delAfterKrizovatkaTurn);

         if(ultrazvuk(3)>block && (fwCounter != 0 || lastTurnLeft == true)){
            vpred(-1); fwCounter = 0; slepaUlicka = false;
         }else{
            turn(false, turnIRCD);
            stop(); delay(delAfterKrizovatkaTurn);
            turn(false, turnIRCD);
            stop(); delay(delAfterKrizovatkaTurn);

            if(ultrazvuk(3)>block && (fwCounter != 0 || lastTurnLeft == false)){
               vpred(-1); fwCounter = 0; slepaUlicka = false;
            }else{
               turn(true, turnIRCD);
               stop(); delay(delAfterKrizovatkaTurn);
               vpred(-1); fwCounter--;
            }
         }


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

