float duration, distance;

void setup() {
  // put your setup code here, to run once:
  pinMode(30, INPUT);
  pinMode(8, OUTPUT);
  pinMode(9, INPUT);

  Serial.begin(9600);
}

void loop() {
 if(digitalRead(30)){
  Serial.println(ultraZvuk(10));
  }
 
}

float ultraZvuk(int repeat){
  distance = 0;
  for (int i = 0; i < repeat; i++){
    digitalWrite(8, LOW);
    delayMicroseconds(2);
    digitalWrite(8, HIGH);
    delayMicroseconds(10);
    digitalWrite(8, LOW);

    duration = pulseIn(9, HIGH);
    distance += (duration*.0343)/2;
    delay(10);
  }
  distance /= repeat;
  return distance;
}