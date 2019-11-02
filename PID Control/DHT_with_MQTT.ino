#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include "max6675.h"

const char* ssid = "miRed";   
const char* password = "12345678";
const char* mqtt_server = "192.168.43.217";  // You can chose any mqtt sever web.

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50],msgt[50];
int value = 0;

int ktcSO = 14;
int ktcCS = 12;
int ktcCLK = 13;
int relay = 0;

MAX6675 ktc(ktcCLK, ktcCS, ktcSO);

void setup() {
  Serial.begin(115200);
  delay(10);
  
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  //WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  randomSeed(micros());
  
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  pinMode(relay, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }  
}


void reconnect() {
  // Loop until weâ€™re reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  long now = millis();
  if ((now - lastMsg) > 2000){
    lastMsg = now;
    ++value;
  }
  
  // Wait a few seconds between measurements.
  //delay(2000);
  float t = ktc.readCelsius();
  //sprintf(msgt, "%d.%02d", (int)t, (int)(t*100)%100);  //OK
  //dtostrf(t, 2, 2, msgt); //OK   Arduino didn't support the conversion from float to char.
  //client.publish("outTopic", msgt);

  //max6675 termocouple
  Serial.print("Deg C = "); 
  Serial.print(ktc.readCelsius());
  
  String temp = String(t); //input to string
  temp.toCharArray(msgt, temp.length()+1); //string to array char
  client.publish("outTopic", msgt); //array char message MQTT
  delay(5000);
}
