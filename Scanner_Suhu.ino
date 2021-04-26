#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>

/*
 * 2 - IR
 * 3 - RELAY 1
 * 4 - TX
 * 5 - RX
 * 6 - TRIG
 * 7 - ECHO
 */

 #define IR1 2
 #define RELAY 3
 #define DFTX 4
 #define RFTX 5
 #define TRIG 7
 #define ECHO 6

unsigned long millis_time = 0;
unsigned long period = 10000;

SoftwareSerial musicSerial(DFTX, RFTX);

// Initialize Music Device
void initializeMusic(){
  musicSerial.begin(9600);
  mp3_set_serial(musicSerial);
  delay(50);
  mp3_set_volume(30);
}

// Initialize Sensors
void initializeDevice(){
  Serial.begin(9600);
  Serial.println("Initializing Device...");
  pinMode(IR1, INPUT);
  pinMode(RELAY, OUTPUT);

  // HC-SR04 
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  
  initializeMusic();
  digitalWrite(RELAY, HIGH);
  delay(5000);
}

int getDistance(){
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  long duration = pulseIn(ECHO, HIGH);
  int distance = duration * 0.034 / 2;
  return distance;
}

/*
 * Detect if person is detected
 * (0) when detected, (1) when not detected
 */
bool personDetected(){
  if(digitalRead(IR1) == 0){
    return true;
  }
  return false;
}

/*
 * Dispensing Hand Sanitizer
 */
void dispenseHandSanitizer(int pre_made_delay=85){
  // Allow to dispense the hand sanitizer
  int counter = 0;
  delay(1000);
  mp3_play(2);
  delay(200);
  while(counter <= pre_made_delay){
    counter++;
    Serial.println("Dispensing");
    digitalWrite(RELAY, LOW);
    delay(50);
  }
  digitalWrite(RELAY, HIGH);
  delay(500);
}

void setup() {
  initializeDevice();
  delay(1000);
}

void loop() {

  // use millis
  if(getDistance() < 40){
    Serial.println(getDistance());
    if(millis() >= millis_time + period){
      mp3_play(1);
      millis_time = millis();
    }
  }
  
  // Detect Infrared
  if(personDetected()){
    dispenseHandSanitizer();
    delay(300);
  } 
  
}
