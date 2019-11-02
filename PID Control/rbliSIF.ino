#include <ESP8266WiFi.h> //TCP
#include <PubSubClient.h> //MQTT
#include <DNSServer.h>
#include <ESP8266WebServer.h>  
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include "max6675.h"

const char* ssid = "My ASUS";
const char* password = "3215110322";
const char* mqtt_server = "192.168.43.217";

int ktcSO = 14;
int ktcCS = 12;
int ktcCLK = 13;

MAX6675 ktc(ktcCLK, ktcCS, ktcSO);

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50]; //Array char
int value = 0;
char msg1[150];

void setup_wifi() {
  Serial.begin(115200);
  //From WIFI Manager
  //WiFiManager wifiManager;
  //wifiManager.autoConnect("AutoConnectAP");
  //wifiManager.autoConnect();

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
   
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  Serial.print("came into callback. The published message from Pi is: ");
  Serial.println();

  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  char d = (char)payload[0];
/*
  if(d == '1'){
    Serial.println("YES");
    digitalWrite(fan, HIGH);
  } else {
    Serial.println("NO");
    digitalWrite(fan, LOW);
  }
  */
}

void setup() {
  setup_wifi();
  //pinMode(fan, OUTPUT);
  //digitalWrite(fan, LOW);
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void reconnect() {
  // Loop until reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println(" connected!");
      // Once connected, publish an announcement...
      client.publish("outTopic", "Welcome....");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print(" Failed, rc = ");
      Serial.print(client.state());
      Serial.println(" Try again in 5 seconds");
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
  
  float c = ktc.readCelsius();
  Serial.print("Deg C = "); 
  Serial.println(c);
  //Serial.print("\t Deg F = ");
  //Serial.println(ktc.readFahrenheit());

  if (isnan(c)){
      Serial.println("Failed to read from sensor!");
      return;
    }
  String tempV = (String)c; //input to string
  tempV = tempV;
  tempV.toCharArray(msg1, tempV.length()+1); //string to array char
  snprintf (msg, 75, msg1, value);
  Serial.print("Publish message: ");
  Serial.println(msg);
  client.publish("outTopic", msg); //array char message MQTT
  delay(5000);
}
