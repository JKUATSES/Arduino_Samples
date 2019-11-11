/*
Gets the distance in centimeters from an object, and prints it on the serial monitor

Hardware: HC-SR04 Ultrasonic module has been used. It contains four pins:
         - Ground,VCC, Echo, Trig.
        The trig and echo pins are to be connected to any I/O pins of arduino.
        In this example, the trigger pin isconnected to pin 9 and the echo to pin 10.
        The VCC shold be connected to the 5 V pin and the ground to the ground pin of the arduino

Written 11 November 2019
by Sackey Freshia
*/

const int trigPin = 9;
const int echoPin = 10;
long duration;
int distance;

void setup() {
//set trigger pin as output and echo pin as input
pinMode(trigPin, OUTPUT); 
pinMode(echoPin, INPUT);
Serial.begin(9600);

}
void loop() {

// Clears the trigPin
digitalWrite(trigPin, LOW);
delayMicroseconds(2);

//send trigger signal
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);

//read echo and calculate distance
duration = pulseIn(echoPin, HIGH);

distance= duration*0.034/2;

Serial.print("Distance: ");
Serial.println(distance);
}