#include <SoftwareSerial.h>
#include <SoftwareServo.h>

//SoftwareSerial rfSerial(3, 4);  // RX, TX
SoftwareSerial bluetoothSerial(2,5);  // RX, TX

SoftwareServo servo;

int servoPin = 4;  // Pin analógico para controlar el servo
int led1Pin = 0;   // Pin digital para LED 1
int led2Pin = 1;   // Pin digital para LED 2
int transistorPin = 3;  // Pin digital para el control del transistor

int gas = 0;
unsigned long servoRefresh;

void setup() {
  //rfSerial.begin(9600);
  bluetoothSerial.begin(9600);

  servo.attach(servoPin);
  
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(transistorPin, OUTPUT);
  pinMode(servoPin, OUTPUT);
  
  delay(100);
  servo.write(90);
}

void refreshServo()
{
  //servo refresh every 50ms
  if (millis() - servoRefresh > 50) {
    servoRefresh = millis();
    SoftwareServo::refresh();
  }
}

void loop() {

  //Bluetooth RC Car Controller Android App
  if (bluetoothSerial.available()) {

    char command = bluetoothSerial.read();
    bluetoothSerial.write(command);

    //forward
    if (command == 'F' || command == 'G' || command == 'I') { analogWrite(transistorPin, gas); }

    //servo direction
    if (command == 'F') { servo.write(90); }
    if (command == 'L') { servo.write(40); }
    if (command == 'R') { servo.write(140); }
        
    //lights
    if (command == 'W') { digitalWrite(led1Pin, HIGH); }
    if (command == 'w') { digitalWrite(led1Pin, LOW); }
    if (command == 'U') { digitalWrite(led2Pin, HIGH); }
    if (command == 'u') { digitalWrite(led2Pin, LOW); }

    //gas
    if (command == '0' || command == 'S') { gas = 0; }
    if (command == '1') { gas = 24; }
    if (command == '2') { gas = 48; }
    if (command == '3') { gas = 72; }
    if (command == '4') { gas = 96; }
    if (command == '5') { gas = 120; }
    if (command == '6') { gas = 144; }
    if (command == '7') { gas = 168; }
    if (command == '8') { gas = 192; }
    if (command == '9') { gas = 216; }
    if (command == 'q') { gas = 255; }
    
  }
/*
  if (rfSerial.available()) {
    char command = rfSerial.read();
    
    // Control del servo
    if (command == 'S') {
      int angle = rfSerial.parseInt();
      servo.write(angle);
    }
    
    // Control de los LEDs
    else if (command == 'L') {
      int ledState = rfSerial.parseInt();
      digitalWrite(led1Pin, ledState & 0x01);
      digitalWrite(led2Pin, (ledState >> 1) & 0x01);
    }
    
    // Control del transistor
    else if (command == 'T') {
      int transistorState = rfSerial.parseInt();
      digitalWrite(transistorPin, transistorState);
    }
  }
  */
  refreshServo();
}
