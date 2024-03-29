#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
 
// Replace with your network credentials
const char* ssid = "Apa Ya";
const char* password = "Nenek4558";
 
ESP8266WebServer server(80);   //instantiate server at port 80 (http port)
 
String page = "";
double data;
float V;
float tempC;

void setup(void){
 
  pinMode(A0, INPUT);
  
  delay(1000);
  Serial.begin(115200);
  WiFi.begin(ssid, password); //begin WiFi connection
  Serial.println("");
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  server.on("/", [](){
    page = "<h1>Sensor LM35 to Node MCU Web Server</h1> <h2>Analog Output:</h2> <h3>"+String(data)+"</h3> <h2>Voltage:</h2> <h3>"+String(V)+"</h3> <h2>Temperature</h2> <h3>"+String(tempC)+" Degree Celcius</h3>";
    server.send(200, "text/html", page);
  });
  
  server.begin();
  Serial.println("Web server started!");
}
 
void loop(void){
  data = analogRead(A0);
  V =(data/1024.0)*3.3; 
  tempC= V*100.0; 
  delay(1000);
  server.handleClient();
}
