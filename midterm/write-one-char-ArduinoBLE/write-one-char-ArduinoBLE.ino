/*
  LED
  This example creates a BLE peripheral with service that contains a
  characteristic to control an LED.
  The circuit:
  - Arduino MKR WiFi 1010 or Arduino Uno WiFi Rev2 board
  This example code is in the public domain.
*/

#include <ArduinoBLE.h>
#include <Servo.h>
Servo myservo1;
Servo myservo2; 
int pos = 0;    // variable to store the servo position

BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214"); // BLE LED Service

// BLE LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLEByteCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

// const int ledPin = 4; // pin to use for the LED

void setup() {
  Serial.begin(9600);
  while (!Serial);
  myservo1.attach(7);
  myservo2.attach(9);
  // set LED pin to output mode
  //pinMode(ledPin, OUTPUT);

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }

  // set advertised local name and service UUID:
  BLE.setLocalName("Henry");
  BLE.setAdvertisedService(ledService);

  // add the characteristic to the service
  ledService.addCharacteristic(switchCharacteristic);

  // add service
  BLE.addService(ledService);

  // set the initial value for the characeristic:
  switchCharacteristic.writeValue(0);

  // start advertising
  BLE.advertise();

  Serial.println("BLE LED Peripheral");
}

void loop() {
  // listen for BLE peripherals to connect:
  BLEDevice central = BLE.central();

  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());

    // while the central is still connected to peripheral:
    while (central.connected()) {
      // if the remote device wrote to the characteristic,
      // use the value to control the LED:
      if (switchCharacteristic.written()) {
        char value = char(switchCharacteristic.value());
        Serial.println(value);
        if (value == 'H') { // If the value is 'H'
          Serial.println("LED on");
           for (pos = 0; pos <= 180; pos += 1) { 
            myservo1.write(pos);
           // myservo2.write(pos);  
             delay(15);  
          }
          //digitalWrite(ledPin, HIGH);         // will turn the LED on
        } else { // If the value is anything else
          for (pos = 0; pos <= 180; pos += 1) { 
            //myservo1.write(pos);
            myservo2.write(pos);  
             delay(15);  
          }
          Serial.println(F("LED off"));
         // digitalWrite(ledPin, LOW);          // will turn the LED off
        }
      }
    }

    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}
