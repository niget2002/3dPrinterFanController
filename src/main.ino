#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 10
#define RELAY_OUTPUT 9
#define TOGGLE 11

#define air 0

int number = 0;
int state = 0;

OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);
int numDevices=0;
float temps[2];
float tempAve[2];

int cold = 20;  // default fan on temperature
float tDelta = 0.5; // delta for turning heater/chiller off

int b=0;
unsigned long previousMillis = 0;        // will store last time values were updated
const long interval = 10000;           // how often to average values and udpate outputs

void findDevices(void);
void setOutputs(void);

void setup() {
  pinMode(13, OUTPUT);
  pinMode(RELAY_OUTPUT, OUTPUT);
  pinMode(TOGGLE, OUTPUT);


  Serial.begin(9600); // start serial for output
  Serial.println("Serial Up");

  findDevices();
  // Start up the library
  sensors.begin(); // IC Default 9 bit. If you have troubles consider upping it 12.
                  //Ups the delay giving the IC more time to process the temperature measurement

  Serial.println("Ready!");
}

void loop() {
  unsigned long currentMillis = millis();

  int i;
  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
  sensors.requestTemperatures(); // Send the command to get temperatures
  for(i=0; i < numDevices; i++){
    temps[i]=temps[i]+sensors.getTempCByIndex(i);
  }
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    for(i=0; i < numDevices; i++){
      tempAve[i]=temps[i]/b;
      temps[i]=0;
    }
    setOutputs(); // everytime we get new average temperatures, lets set outputs
    Serial.print("Air Temp=");
    Serial.println(tempAve[air]);
    Serial.print("Temp Set To=");
    Serial.println(cold);

    b=0;
  }
  else {
    b++;
  }
}


void setOutputs(void){
  if (digitalRead(TOGGLE) == 1){
    cold=40;  // ABS Enclosure Temperature
  }
  else {
    cold=20;  // PLA Enclosure Temperature
  }
  if ( cold < tempAve[air] ) {
    ledMatrix_ON(RELAY_OUTPUT);
  }
  else { //fan off
    if( cold+tDelta > tempAve[air]){
      ledMatrix_OFF(RELAY_OUTPUT);
    }
  }
}

void ledMatrix_OFF(int index) {
  // Relays connected directly to Arduino
  digitalWrite(index, LOW);

}

void ledMatrix_ON(int index) {
  // Relays connected directly to Arduino
  digitalWrite(index, HIGH);

}

void findDevices(void){
  byte i;
  byte present = 0;
  byte data[12];
  byte addr[8];

  Serial.print("Looking for 1-Wire devices...\n\r");// "\n\r" is NewLine
    while(oneWire.search(addr)) {
      Serial.print("\n\r\n\rFound \'1-Wire\' device with address:\n\r");
      for( i = 0; i < 8; i++) {
        Serial.print("0x");
        if (addr[i] < 16) {
          Serial.print('0');
        }
        Serial.print(addr[i], HEX);
        if (i < 7) {
          Serial.print(", ");
        }
      }
      if ( OneWire::crc8( addr, 7) != addr[7]) {
        Serial.print("CRC is not valid!\n\r");
        return;
      }
      numDevices++;
    }
    Serial.println();
}
