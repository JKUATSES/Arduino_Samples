/*

Connects to wifi using esp8266 wifi module, initiates an mqtt connection with a broker,
then sends a birth message to a birth topic, to confirm the mqtt connection is up
Subscribes to a test topic and has a callback to print the messages received on this topic

Setup:
Download the ESP8266 arduino library (https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi) 
and pubsubclient library (https://www.arduinolibraries.info/libraries/pub-sub-client)

Written 12 November 2019
by Sackey Freshia

*/


#include <ESP8266WiFi.h>
#include <PubSubClient.h>
 
const char* ssid = "";
const char* wifi_password = "";
const char* mqtt_host = "broker.africastalking.com";
const int mqtt_host_port = 1883;
const char* mqtt_username = "";
const char* mqtt_user_password = "";

const char* birth_topic = "mytopic/birth";
const char* test_topic = "mytopic/test";

 
WiFiClient wifi_client;
PubSubClient client(wifi_client);
 
void setup() {
 
  Serial.begin(115200);
 
  WiFi.begin(ssid, wifi_password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  Serial.println(WiFi.localIP());
 
  client.setServer(mqtt_host, mqtt_host_port);
  client.setCallback(callback);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP8266Client", mqtt_username,mqtt_user_password)) {
 
      Serial.println("Connected to mqtt host");  
 
    } else {
 
      Serial.print("Failed with status ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
 
  client.publish(birth_topic, "This is the beginning of awesomeness");
  client.subscribe(test_topic);
 
}
 
void callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
 
  Serial.println();
}
 
void loop() {
  //allow client to process incoming messages and maintain connection to server
  client.loop();
}