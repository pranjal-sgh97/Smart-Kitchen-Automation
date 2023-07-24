#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#define DHTTYPE DHT11
// You should get Auth Token in the Blynk App
// Go to the Project Settings (nut icon)
char auth[] = "KvCOa8BMAOwR8tVkxAa_3cXZKtb1E20P";
// Your WiFi credentials
// Set password to "" for open networks
char ssid[] = "Wi-Fi";
char pass[] = "Password";
int buzzer = D2;
int smokeA0 = A0;
#include <Servo.h>
Servo servo;
// Your threshold value. You might need to change it.
int sensorThres = 350;
int pos=0;
int t=0;
int F=1;
#define relay_fan D5 // for exhosting fan
20
#define relay_frize D7 // for Refrigerator
#define ir_human D3 // for human presence
#define dht_pin D6 // for temperature 
int ir_status=0;
DHT dht(dht_pin, DHTTYPE);
void setup() {
pinMode(D1,OUTPUT); // led 
pinMode(relay_fan, OUTPUT);
pinMode(relay_frize, OUTPUT);
pinMode(ir_human,INPUT);
pinMode(buzzer, OUTPUT);
pinMode(smokeA0, INPUT);
servo.attach(2); // D4
servo.write(0);
Blynk.begin(auth, ssid, pass);
Serial.begin(9600);
dht.begin();
delay(2000);
}
void loop() {
Blynk.run();
int analogSensor = analogRead(smokeA0);
Serial.println("<------- updated ------->");
Serial.print("Gas Intensity: ");
Serial.println(analogSensor);
// Checks if it has reached the threshold value
float t=dht.readTemperature();
float h=dht.readHumidity();
Serial.print("Temperature: ");
21
Serial.print(t);
Serial.println("*C");
Serial.print("Humidity: ");
Serial.print(h);
Serial.println("%");
if(digitalRead(relay_fan)==1)
 Serial.println("Exhosting Fan: On");
else
 Serial.println("Exhosting Fan: Off");
if(digitalRead(relay_frize)==1)
 Serial.println("Refrigerator status: On");
else
 Serial.println("Refrigerator status: Off");
if (analogSensor > sensorThres)
{
 tone(buzzer, 1000, 200);
 if(t==0)
 {
 for(pos=0;pos<=180;pos+=2)
 {
 servo.write(pos);
 delay(15);
 F=1;
 }
}
t=1;
}
else
{
 noTone(buzzer);
 if(F==1)
22
 {
 for(pos=180;pos>=0;pos-=2)
 {
 servo.write(pos);
 delay(15);
 }
 F=0;
 t=0;
 }
}
ir_status=digitalRead(ir_human);
if(ir_status==1)
{
 Serial.println("Person Detected in Kitchen");
}
else
{
 Serial.println("Person Not Detected in Kitchen");
}
Blynk.virtualWrite(V1, analogSensor);
Blynk.virtualWrite(V2, ir_status);
Blynk.virtualWrite(V3, t);
Blynk.virtualWrite(V4,h);
Serial.println();
 
delay(2000);
}
