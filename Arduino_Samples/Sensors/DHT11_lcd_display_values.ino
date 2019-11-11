/*
Gets the temperature and the humidity using DHT11 sensor, and displays the values on an LCD

Setup:
First, download the DHT11 library from here: http://playground.arduino.cc/Main/DHTLib

Hardware connections: DHT11 positive pin to arduino 5V
                      DHT11 negative pin to ground
                      DHT11 S pin to Arduino Analog A0 (Can be connected to any analog pin)

Written 11 November 2019
by Sackey Freshia
*/


#include <dht.h>
#include <LiquidCrystal.h>


LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define dht_apin A0 // Analog Pin sensor is connected to
 
dht dht_sensor;
 
void setup(){

  lcd.begin(16,2); //16 by 2 character display
  Serial.begin(9600);
  delay(1000);
 
}
 
void loop(){
    dht_sensor.read11(dht_apin);
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Humidity = ");
    lcd.print(dht_sensor.humidity);
    lcd.setCursor(0,1);
    lcd.print("Temp = ");
    lcd.print(dht_sensor.temperature);
    
    delay(5000);
 
}