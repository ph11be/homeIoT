/*
 * PIR sensor tester
 */

#include <Wire.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>


int ledPin = 13;                // choose the pin for the LED
int inputPin = 5;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status

const char* ssid = "<Wifi network name>";
const char* password = "<Wifi password>";

const char* mqtt_server = "<MQTT server IP address>";
const char* mqtt_username = "<MQTT username>";
const char* mqtt_password = "<MQTT password>";
const char* mqtt_topic = "<MQTT topic>";

String sensorString;
char sensorChar[30];
unsigned long start, finished, elapsed;

WiFiClient espClient;
PubSubClient client(espClient);

 
void setup() {
  pinMode(ledPin, OUTPUT);      // declare LED as output
  pinMode(inputPin, INPUT);     // declare sensor as input
 
  Serial.begin(9600);

  // setup WiFi
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

}

void setup_wifi() {
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

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
//    if (client.connect("ESP8266Client", mqtt_username, mqtt_password)) {
    if (client.connect("ESP8266Client")) {      
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
 
void loop(){
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  val = digitalRead(inputPin);  // read input value
  
  if (val == HIGH) {            // check if the input is HIGH
   // digitalWrite(ledPin, HIGH);  // turn LED ON
    if (pirState == LOW) {
      start=millis();
      // we have just turned on
      Serial.println("Motion detected!");
      sensorString="1, 0";
      sensorString.toCharArray(sensorChar,30);
      client.publish(mqtt_topic, sensorChar);
      
      // We only want to print on the output change, not state
      pirState = HIGH;
    }
  } else {
    //digitalWrite(ledPin, LOW); // turn LED OFF
    if (pirState == HIGH){
      finished=millis();
      elapsed=finished-start;
      // we have just turned of
      Serial.println("Motion ended!");
      sensorString="0, ";
      sensorString.concat(elapsed/1000);
      sensorString.toCharArray(sensorChar,30);
      client.publish(mqtt_topic, sensorChar);
      // We only want to print on the output change, not state
      pirState = LOW;
    }
  }
}
