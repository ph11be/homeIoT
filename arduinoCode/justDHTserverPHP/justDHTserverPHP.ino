/* DHTServer - ESP8266 Webserver with a DHT sensor as an input

   Based on ESP8266Webserver, DHTexample, and BlinkWithoutDelay (thank you)

   Version 1.0  5/3/2014  Version 1.0   Mike Barela for Adafruit Industries
*/
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <pgmspace.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>
#include <DHT.h>

#define DHTTYPE DHT22
#define DHTPIN 2

const char* ssid     = "<Wifi network name>";
const char* password = "<Wifi password>";

//ESP8266WebServer server(80);

  WiFiClient client;
  char server[] = "<IP address of PHP server>"; // IP Address (or name) of server to dump data to 

// Initialize DHT sensor 
DHT dht(DHTPIN, DHTTYPE);
 
float humidity, temp_c;  // Values read from sensor
String webString="";     // String to display

// Generally, you should use "unsigned long" for variables that hold time
unsigned long previousMillis = 0;        // will store last temp was read
const long interval = 2000;              // interval at which to read sensor

 
void setup(void)
{
  // You can open the Arduino IDE Serial Monitor window to see what the code is doing
  Serial.begin(115200);  // Serial connection from ESP-01 via 3.3v console cable
  
  // Connect to WiFi network
  WiFi.begin(ssid, password);
  Serial.print("\n\r \n\rWorking to connect");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("DHT Weather Reading Server");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("HTTP server started");
}
 
void loop(void)
{
  
  gettemperature(); 
     
  if (client.connect(server, 80)) {     
    Serial.println("-> Connected");     // Make a HTTP request:     
    client.print( "GET /store.php?");     
    client.print("temperature=");     
    client.print(String((float)temp_c));
    client.print("&&");
    client.print("humidity=");
    client.print(String((float)humidity));
    client.println( " HTTP/1.1");     
    client.print( "Host: " );     
    client.println(server);     
    client.println( "Connection: close" );     
    client.println();     
    client.println();     
    client.stop();   }   
  else {     // you didn't get a connection to the server:     
    Serial.println("--> connection failed/n");   }
    delay(60000);
} 

void gettemperature() {
  // Wait at least 2 seconds seconds between measurements.
  // if the difference between the current time and last time you read
  // the sensor is bigger than the interval you set, read the sensor
  // Works better than delay for things happening elsewhere also
  unsigned long currentMillis = millis();
 
  if(currentMillis - previousMillis >= interval) {
    // save the last time you read the sensor 
    previousMillis = currentMillis;   
    
//    dht.read11(DHTPIN);
    humidity = dht.readHumidity();
    temp_c = dht.readTemperature();
    Serial.println(humidity);
    Serial.println(temp_c);
    
    // Check if any reads failed and exit early (to try again).
    if (isnan(humidity) || isnan(temp_c)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
    
  }
}
