#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
int fan = 16;

void setup() {
    // put your setup code here, to run once:
    
    Serial.begin(115200);
    //pinMode(fan, OUTPUT); 

    //WiFiManager
    //Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;
    //reset saved settings
    //wifiManager.resetSettings();
    
    //set custom ip for portal
    //wifiManager.setAPStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

    //fetches ssid and pass from eeprom and tries to connect
    //if it does not connect it starts an access point with the specified name
    //here  "AutoConnectAP"
    //and goes into a blocking loop awaiting configuration
    //wifiManager.autoConnect("AutoConnectAP");
    //or use this for auto generated name ESP + ChipID
    wifiManager.autoConnect();

    pinMode(fan, OUTPUT); 
    
    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
}

void loop() {
  Serial.println("connected...yeey :)");
  digitalWrite(fan, LOW);   // Turn the LED on (Note that LOW is the voltage level
                                    // but actually the LED is on; this is because 
                                    // it is active low on the ESP-01)
  delay(500);                      // Wait for a second
  digitalWrite(fan, HIGH);  // Turn the LED off by making the voltage HIGH
  delay(500);        

}
