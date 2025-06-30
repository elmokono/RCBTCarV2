#include <SoftwareSerial.h>
#include <SoftwareServo.h>

// at tiny pins ///////////////////////////////
#define servoPin PB3       // A3
#define RxD PB2            // A1
#define TxD PB1            // PWM
#define transistorPin PB0  // PWM
#define led1Pin PB4        // A2
#define led2Pin PB5

#define NPN
//#define PNP
//////////////////////////////////////////////

SoftwareSerial bluetoothSerial(RxD, TxD);  // RX, TX
SoftwareServo servo;
int gas = 0;
unsigned long servoRefresh;

void setup() {
  bluetoothSerial.begin(19200);

  servo.attach(servoPin);

  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(transistorPin, OUTPUT);
  pinMode(servoPin, OUTPUT);

  delay(100);
  servo.write(90);
  stopGas();
}

void refreshServo() {
  // servo refresh every 50ms
  if (millis() - servoRefresh > 50) {
    servoRefresh = millis();
    SoftwareServo::refresh();
  }
}

void throttleGas() {
#ifdef NPN
  digitalWrite(transistorPin, LOW);
#else
  digitalWrite(transistorPin, HIGH);
#endif
}

void stopGas() {
#ifdef NPN
  digitalWrite(transistorPin, HIGH);
#else
  digitalWrite(transistorPin, LOW);
#endif
}

void loop() {

  // Bluetooth RC Car Controller Android App
  if (bluetoothSerial.available()) {

    char command = bluetoothSerial.read();
    bluetoothSerial.write(command);

    // forward
    if (command == 'F' || command == 'G' || command == 'I') {
      throttleGas();
    }

    // servo direction
    if (command == 'F') {
      servo.write(90);
    }
    if (command == 'L') {
      servo.write(40);
    }
    if (command == 'R') {
      servo.write(140);
    }

    // lights
    if (command == 'W') {
      digitalWrite(led1Pin, HIGH);
    }
    if (command == 'w') {
      digitalWrite(led1Pin, LOW);
    }
    if (command == 'U') {
      digitalWrite(led2Pin, HIGH);
    }
    if (command == 'u') {
      digitalWrite(led2Pin, LOW);
    }

    // gas
    if (command == '0' || command == 'S') {
      gas = 0;
      stopGas();
    }
    if (command == '1') {
      gas = 24;
    }
    if (command == '2') {
      gas = 48;
    }
    if (command == '3') {
      gas = 72;
    }
    if (command == '4') {
      gas = 96;
    }
    if (command == '5') {
      gas = 120;
    }
    if (command == '6') {
      gas = 144;
    }
    if (command == '7') {
      gas = 168;
    }
    if (command == '8') {
      gas = 192;
    }
    if (command == '9') {
      gas = 216;
    }
    if (command == 'q') {
      gas = 255;
    }
  }

  refreshServo();
}