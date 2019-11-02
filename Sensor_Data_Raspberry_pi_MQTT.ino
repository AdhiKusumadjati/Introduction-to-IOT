#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "My ASUS";
const char* password = "3215110322";
const char* mqtt_server = "192.168.43.217";

float sensorValue;
float V;
float tempC;

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
char msg1[150];

void setup_wifi() {
  Serial.begin(115200);
  delay(10);
  
  // We start by connecting to a WiFi network
  Serial.println();
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
}

void callback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  Serial.print("came into callback. The published message from Pi is ");
  Serial.println();
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] > '5') {
    digitalWrite(13, LOW); // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else if ((char)payload[0] <= '5') {
    digitalWrite(13, HIGH); // Turn the LED off by making the voltage HIGH
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(13,OUTPUT);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
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
  // put your main code here, to run repeatedly:
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  long now = millis();
  if ((now - lastMsg) > 2000){
    lastMsg = now;
    ++value;
  }
  sensorValue = analogRead(A0);
  V = (sensorValue/1024.0)*3.3;
  tempC = V*100.0;
    
  String tempV = String(tempC);
  tempV = "Room Temperature is: " + tempV;
  Serial.println(tempV);
  tempV.toCharArray(msg1, tempV.length()+1);
  snprintf (msg, 75, msg1, value);
  Serial.print("Publish message: ");
  Serial.println(msg);
  client.publish("outTopic", msg);
  delay(5000);
}

