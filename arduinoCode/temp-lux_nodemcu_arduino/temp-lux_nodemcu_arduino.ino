// Your sketch must #include this library, and the Wire library
// (Wire is a standard library included with Arduino):

#include <SparkFunTSL2561.h>
#include <Wire.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <DHT.h>

// Create an SFE_TSL2561 object, here called "light":

SFE_TSL2561 light;

// DHT 11or22 SENSOR
#define DHTPIN 0
#define DHTTYPE DHT22  // THIS CAN BE CHANGED TO DHT22

// DHT sensor
DHT dht(DHTPIN, DHTTYPE, 15);

// Global variables:

boolean gain;     // Gain setting, 0 = X1, 1 = X16;
unsigned int ms;  // Integration ("shutter") time in milliseconds

const char* ssid = "<Wifi network name>";
const char* password = "<Wifi password>";

const char* mqtt_server = "<MQTT server address>";
const char* mqtt_username = "<MQTT username>";
const char* mqtt_password = "<MQTT password>";
const char* mqtt_topic = "<MQTT topic>";

String sensorString;
char sensorChar[30];
String temperatureString;
String humidityString;


WiFiClient espClient;
PubSubClient client(espClient);


void setup()
{
  // Initialize the Serial port:
  
  Serial.begin(9600);
  Serial.println("TSL2561 example sketch");

  // setup WiFi
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  // Initialize the SFE_TSL2561 library

  // You can pass nothing to light.begin() for the default I2C address (0x39),
  // or use one of the following presets if you have changed
  // the ADDR jumper on the board:
  
  // TSL2561_ADDR_0 address with '0' shorted on board (0x29)
  // TSL2561_ADDR   default address (0x39)
  // TSL2561_ADDR_1 address with '1' shorted on board (0x49)

  // For more information see the hookup guide at: https://learn.sparkfun.com/tutorials/getting-started-with-the-tsl2561-luminosity-sensor

  light.begin();

  // Get factory ID from sensor:
  // (Just for fun, you don't need to do this to operate the sensor)

  unsigned char ID;
  
  if (light.getID(ID))
  {
    Serial.print("Got factory ID: 0X");
    Serial.print(ID,HEX);
    Serial.println(", should be 0X5X");
  }
  // Most library commands will return true if communications was successful,
  // and false if there was a problem. You can ignore this returned value,
  // or check whether a command worked correctly and retrieve an error code:
  else
  {
    byte error = light.getError();
    printError(error);
  }

  // The light sensor has a default integration time of 402ms,
  // and a default gain of low (1X).
  
  // If you would like to change either of these, you can
  // do so using the setTiming() command.
  
  // If gain = false (0), device is set to low gain (1X)
  // If gain = high (1), device is set to high gain (16X)

  gain = 0;

  // If time = 0, integration will be 13.7ms
  // If time = 1, integration will be 101ms
  // If time = 2, integration will be 402ms
  // If time = 3, use manual start / stop to perform your own integration

  unsigned char time = 2;

  // setTiming() will set the third parameter (ms) to the
  // requested integration time in ms (this will be useful later):
  
  Serial.println("Set timing...");
  light.setTiming(gain,time,ms);

  // To start taking measurements, power up the sensor:
  
  Serial.println("Powerup...");
  light.setPowerUp();
  
  // The sensor will now gather light during the integration time.
  // After the specified time, you can retrieve the result from the sensor.
  // Once a measurement occurs, another integration period will start.
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



void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  unsigned int data0, data1;

  sensorString = "";

//---------------------------------------------------------------------------------------------------
// Getting the light data ---------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------  
  if (light.getData(data0,data1))
  {
    // getData() returned true, communication was successful
    
    Serial.print("data0: ");
    Serial.print(data0);
    Serial.print(" data1: ");
    Serial.print(data1);
  
    // To calculate lux, pass all your settings and readings
    // to the getLux() function.
    
    // The getLux() function will return 1 if the calculation
    // was successful, or 0 if one or both of the sensors was
    // saturated (too much light). If this happens, you can
    // reduce the integration time and/or gain.
    // For more information see the hookup guide at: https://learn.sparkfun.com/tutorials/getting-started-with-the-tsl2561-luminosity-sensor
  
    double lux;    // Resulting lux value
    boolean good;  // True if neither sensor is saturated
    
    // Perform lux calculation:

    good = light.getLux(gain,ms,data0,data1,lux);
    
    // Print out the results:
	
    Serial.print(" lux: ");
    Serial.print(lux);

     sensorString.concat(data0);
     sensorString.concat(", ");
     sensorString.concat(data1);
     sensorString.concat(", ");
     sensorString.concat(lux);

     float humidity_data = dht.readHumidity();
     Serial.println("Humidity" + String(humidity_data));
     float temperature = dht.readTemperature();
     Serial.println("Temperature" + String(temperature));

     sensorString.concat(", ");
     sensorString.concat(temperature);
     sensorString.concat(", ");
     sensorString.concat(humidity_data);
  
     sensorString.toCharArray(sensorChar,30);
     Serial.println(sensorString);
      
    if (good) Serial.println(" (good)"); else Serial.println(" (BAD)");
    if (good) client.publish(mqtt_topic, sensorChar);
  }
  else
  {
    // getData() returned false because of an I2C error, inform the user.

    byte error = light.getError();
    printError(error);

//---------------------------------------------------------------------------------------------------
// Getting the temperature/humidity data ------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
  }

  delay (60 * 1000);
}

void printError(byte error)
  // If there's an I2C error, this function will
  // print out an explanation.
{
  Serial.print("I2C error: ");
  Serial.print(error,DEC);
  Serial.print(", ");
  
  switch(error)
  {
    case 0:
      Serial.println("success");
      break;
    case 1:
      Serial.println("data too long for transmit buffer");
      break;
    case 2:
      Serial.println("received NACK on address (disconnected?)");
      break;
    case 3:
      Serial.println("received NACK on data");
      break;
    case 4:
      Serial.println("other error");
      break;
    default:
      Serial.println("unknown error");
  }
}

