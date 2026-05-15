// 27cm/s rýchlosť robota
// zistiť dialku aku dojazdi po vypnuti motora (cm)
// musime spraviť aby sa otáčal okolo stredu kryžovatky

float block = 32; //cm
int ir = 650; //0 - 1000
int delAfterKrizovatkaTurn = 250;
int turnCorrection = 130;
int fireAfterDetection = 100; // after detection how long spin motor to land in the center of the intersection
int turnIRCD = 250; // (turn infrared cooldown) cooldown in ms that says how long to wait before measuring ir after started to turn
bool button = false; // if should start started
bool leftTurnFirst = true;

// no touch pls
int tas=0;
bool slepaUlicka = false;
bool lastTurnLeft = true;
int fwCounter = 0; // how many times it went foward after last turn


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
      if(button){vpred(-1);}
   }

   if(button){
      if(tas >= 3 && !slepaUlicka){
         vpred(fireAfterDetection);// vzdialenosť senzora od stredu motora (8cm) / rychlost robota (160)
         delay(500);
         if(ultraZvuk(3)<block){//1l
            kTurn(leftTurnFirst, turnIRCD, turnCorrection, ir);
            delay(delAfterKrizovatkaTurn);

            if(ultraZvuk(3)<block){//2r
              kTurn(!leftTurnFirst, turnIRCD, turnCorrection, ir);
              delay(delAfterKrizovatkaTurn);

              kTurn(!leftTurnFirst, turnIRCD, turnCorrection, ir);
              delay(delAfterKrizovatkaTurn);

               if(ultraZvuk(3)<block){//1r
                kTurn(!leftTurnFirst, turnIRCD, turnCorrection, ir);
                delay(delAfterKrizovatkaTurn);

                vpred(-1);
                slepaUlicka = true;

               }else{vpred(-1); slepaUlicka = false; lastTurnLeft = false; fwCounter = 0;}
            }else{vpred(-1); slepaUlicka = false; lastTurnLeft = true; fwCounter = 0;}
         }else{vpred(-1); slepaUlicka = false; fwCounter++;}


      }else if(tas >= 3 && slepaUlicka){
         vpred(fireAfterDetection); // vzdialenosť senzora od stredu motora (8cm) / rychlost robota
         delay(500);

         kTurn(leftTurnFirst, turnIRCD, turnCorrection, ir);
         delay(delAfterKrizovatkaTurn);

         if(ultraZvuk(3)>block && (fwCounter != 0 || lastTurnLeft == true)){
            vpred(-1); fwCounter = 0; slepaUlicka = false;
         }else{

            kTurn(!leftTurnFirst, turnIRCD, turnCorrection, ir);
            delay(delAfterKrizovatkaTurn);

            kTurn(!leftTurnFirst, turnIRCD, turnCorrection, ir);
            delay(delAfterKrizovatkaTurn);

            if(ultraZvuk(3)>block && (fwCounter != 0 || lastTurnLeft == false)){
               vpred(-1); fwCounter = 0; slepaUlicka = false;
            }else{
               kTurn(!leftTurnFirst, turnIRCD, turnCorrection, ir);
               delay(delAfterKrizovatkaTurn);
               vpred(-1); fwCounter--;
            }
         }

      }else if(analogRead(A0) < ir || analogRead(A1) < ir){
         turn(true, 0);
      }else if(analogRead(A4) < ir || analogRead(A3) < ir){
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
      analogWrite(7, 250);   // rzchlosť

      analogWrite(2, 0);  // rzchlosť
      digitalWrite(3, LOW);  // dopredu
      digitalWrite(4, LOW);   // dozadu
   }else{
      digitalWrite(5, LOW);  // dozadu
      digitalWrite(6, LOW);  // dopredu
      analogWrite(7, 0);   // rzchlosť

      analogWrite(2, 250);  // rzchlosť
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

int kTurn(bool left, int delIR, int turnCorrection, int ir) {

   if(left){
      digitalWrite(5, LOW);  // dozadu
      digitalWrite(6, HIGH);  // dopredu
      analogWrite(7, 250);   // rzchlosť

      analogWrite(2, 250);  // rzchlosť
      digitalWrite(3, LOW);  // dopredu
      digitalWrite(4, HIGH);   // dozadu
   }else{
      digitalWrite(5, HIGH);  // dozadu
      digitalWrite(6, LOW);  // dopredu
      analogWrite(7, 250);   // rzchlosť

      analogWrite(2, 250);  // rzchlosť
      digitalWrite(3, HIGH);  // dopredu
      digitalWrite(4, LOW);   // dozadu
   }

   delay(delIR);
   while(analogRead(A2)>ir){}

   if(!left){
      digitalWrite(5, LOW);  // dozadu
      digitalWrite(6, HIGH);  // dopredu
      analogWrite(7, 250);   // rzchlosť

      analogWrite(2, 250);  // rzchlosť
      digitalWrite(3, LOW);  // dopredu
      digitalWrite(4, HIGH);   // dozadu
   }else{
      digitalWrite(5, HIGH);  // dozadu
      digitalWrite(6, LOW);  // dopredu
      analogWrite(7, 250);   // rzchlosť

      analogWrite(2, 250);  // rzchlosť
      digitalWrite(3, HIGH);  // dopredu
      digitalWrite(4, LOW);   // dozadu
   }

   delay(turnCorrection);

   digitalWrite(5, LOW);  // dozadu
   digitalWrite(6, LOW);  // dopredu
   analogWrite(7, 0);   // rzchlosť

   analogWrite(2, 0);  // rzchlosť
   digitalWrite(3, LOW);  // dopredu
   digitalWrite(4, LOW);   // dozadu

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
