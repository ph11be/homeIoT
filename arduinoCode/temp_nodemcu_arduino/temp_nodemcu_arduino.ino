

/*
 Sketch which publishes temperature data from a DS1820 sensor to a MQTT topic.

 This sketch goes in deep sleep mode once the temperature has been sent to the MQTT
 topic and wakes up periodically (configure SLEEP_DELAY_IN_SECONDS accordingly).

 Hookup guide:
 - connect D0 pin to RST pin in order to enable the ESP8266 to wake up periodically
 - DS18B20:
     + connect VCC (3.3V) to the appropriate DS18B20 pin (VDD)
     + connect GND to the appopriate DS18B20 pin (GND)
     + connect D4 to the DS18B20 data pin (DQ)
     + connect a 4.7K resistor between DQ and VCC.
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

#define SLEEP_DELAY_IN_SECONDS  10

// DHT 11or22 SENSOR
#define DHTPIN 5
#define DHTTYPE DHT11  // THIS CAN BE CHANGED TO DHT22

// DHT sensor
DHT dht(DHTPIN, DHTTYPE, 15);

const char* ssid = "<Wifi network name>";
const char* password = "<Wifi password>";

const char* mqtt_server = "<IP address of MQTT broker>";
const char* mqtt_username = "<MQTT username>";
const char* mqtt_password = "<MQTT password>";
const char* mqtt_topic = "<MQTT feed topic>";

WiFiClient espClient;
PubSubClient client(espClient);


String temperatureString;
String humidityString;
String sensorString;
char sensorChar[20];

void setup() {
  // setup serial port
  Serial.begin(115200);

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

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  sensorString = "";

  int humidity_data = (int)dht.readHumidity();
  Serial.println("Humidity" + String(humidity_data));
  float temperature = 1.0*(int)dht.readTemperature();
  Serial.println("Temperature" + String(temperature));

  sensorString.concat(temperature);
  sensorString.concat(", ");
  sensorString.concat(humidity_data);
  
  sensorString.toCharArray(sensorChar,20);
  Serial.println(sensorString);
  // send temperature and humidity to the MQTT topic
  client.publish(mqtt_topic, sensorChar);

//  Serial << "Closing MQTT connection..." << endl;
//  client.disconnect();

  delay(60 * 1000);
}
