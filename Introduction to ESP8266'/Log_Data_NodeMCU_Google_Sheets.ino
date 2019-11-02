#include <ESP8266WiFi.h>
 
const char* ssid     = "Apa Ya";   //replace with your own SSID
const char* password = "Nenek4558";    //replace with your own password
const char* host = "api.pushingbox.com";  
 
int button = 4;
double data;
float V;
float tempC;
 
void setup() {
  Serial.begin(115200);
  delay(10);
  
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
 
  pinMode(A0, INPUT);
  pinMode(button, INPUT);
}
 
void loop() {
 if(digitalRead(button) == 0){
    delay(1000);
    data = analogRead(A0);
    V = (data/1024.0)*3.3;
    tempC = V*100.0;
    Serial.print("connecting to ");
    Serial.println(host);
  
    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
      return;
    }
 
    // We now create a URI for the request
    String url = "/pushingbox?";
    url += "devid=";
    url += "vA6A43E8859642AC";
    url += "&data="+String(tempC);
  
    Serial.print("Requesting URL: ");
    Serial.println(url);
  
    // This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
    Serial.print("Data Sent!");
  }
  
  Serial.println();
  Serial.println("closing connection");
  }
}

