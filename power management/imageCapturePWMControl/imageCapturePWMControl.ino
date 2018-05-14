#include "LowPower.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  //pinMode(5V, OUTPUT);
  //digitalWrite(5V, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Into sleep....");
  delay(1000);
  for(int i = 0; i < 1; i = i+1){
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  }
  Serial.println("Out of sleep.");
  digitalWrite(13, HIGH);
  //digitalWrite(5V, HIGH);
  Serial.println("HIGH");
  
  delay(80000);
  Serial.println("LOW");
  digitalWrite(13, LOW);
  //digitalWrite(5V, LOW);
  Serial.println(" ");
}
