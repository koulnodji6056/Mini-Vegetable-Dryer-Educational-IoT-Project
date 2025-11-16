#include "BluetoothSerial.h"
#include "DHT.h" 

BluetoothSerial SerialBT;

#define DHTPIN 4        // GPIO where DHT21 is connected
#define DHTTYPE DHT21   // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

const int heaterPin1 = 18;   // Heater pin1 
const int heaterPin2 = 19;   // Heater pin2 
const int fanPin1 = 26;   // Fan pin1 
const int fanPin2 = 27;   // Fan pin2 

unsigned long lastSend = 0;   // for timing data sending
const unsigned long interval = 10000; // send every 10s

String device_name = "ESP32";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  SerialBT.begin(device_name);
  dht.begin();
  pinMode(heaterPin1, OUTPUT);
  pinMode(fanPin1, OUTPUT);
  pinMode(heaterPin2, OUTPUT);
  pinMode(fanPin2, OUTPUT);
  // Initialize actuator at low meaning off
  digitalWrite(heaterPin1,LOW);
  digitalWrite(heaterPin2,LOW);
  digitalWrite(fanPin1, LOW);
  digitalWrite(fanPin2, LOW);
 Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str());
}

void loop() {

  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

 
  if (SerialBT.available()) {
    char c = SerialBT.read();
    if (c == '0'){

      digitalWrite(heaterPin1,HIGH);
      digitalWrite(heaterPin2,HIGH);

      Serial.print("Heater is On ");

    }else if (c == '1'){

      digitalWrite(heaterPin1,LOW);
      digitalWrite(heaterPin2,LOW);

      Serial.print("Heater is Off ");

    }else if (c == '2') { 

      digitalWrite(fanPin1, HIGH);
      digitalWrite(fanPin2, HIGH);

      Serial.print("Fan is On ");

    } else if (c == '3') {
      
      digitalWrite(fanPin1, LOW);
      digitalWrite(fanPin2, LOW);

      Serial.print("Fan is Off ");
    } else if (c == '4') {
      digitalWrite(heaterPin1,HIGH);
      digitalWrite(heaterPin2,HIGH);
      digitalWrite(fanPin1, HIGH);
      digitalWrite(fanPin2, HIGH);

      Serial.print("Dryer ON ");
    } else if (c == '5') {
      digitalWrite(heaterPin1,LOW);
      digitalWrite(heaterPin2,LOW);
      digitalWrite(fanPin1, LOW);
      digitalWrite(fanPin2, LOW);

      Serial.print("Dryer OFF ");
    } else if (temp >= 70){  // Turn OFF heater to avoir over heating
      digitalWrite(heaterPin1,LOW);
      digitalWrite(heaterPin2,LOW);
    }
    Serial.print(" ==========> ");
    Serial.println(c);
  }
 
  // Send DHT21 readings periodically
  if (millis() - lastSend >= interval) {
    lastSend = millis();
    if (!isnan(temp) && !isnan(hum)) {
      SerialBT.print(temp, 1);
      SerialBT.print(",");
      SerialBT.println(hum, 1);
      // Debug to Serial Monitor
      Serial.print("Temp: ");
      Serial.print(temp, 1);
      Serial.print("  Hum: ");
      Serial.println(hum, 1);

    }else{
      Serial.println("Failed to read from DHT sensor!");
    }
  }


}
