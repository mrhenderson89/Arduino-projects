#include <Adafruit_HDC1000.h>

#include <ESP8266WiFi.h>
#include <Wire.h>
#include <PubSubClient.h>

#define wifi_ssid "SKY4F39D"
#define wifi_password "TFTSAUDY"

#define mqtt_server "192.168.0.150"
#define mqtt_user "pi"
#define mqtt_password "XXXXXXX"

#define humidity_topic "office/humidity"
#define temperature_topic "office/temperature"

WiFiClient espClient;
PubSubClient client(espClient);
Adafruit_HDC1000 hdc = Adafruit_HDC1000();

//Soil Moisture Sensors
int leftSoilSensorPin = 7;    // select the input pin for the potentiometer
int rightSoilSensorValue = 0;  // variable to store the value coming from the sensor

int rightSoilSensorPin = 5;    // select the input pin for the potentiometer
int rightSoilSensorValue = 0;  // variable to store the value coming from the sensor

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);

  // Set SDA and SDL ports
  Wire.begin(2, 14);

  // Start sensor
  if (!hdc.begin()) {
    Serial.println("Couldn't find sensor!");
    while (1);
  }}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    // If you do not want to use a username and password, change next line to
    // if (client.connect("ESP8266Client")) {
    if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
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

bool checkBound(float newValue, float prevValue, float maxDiff) {
  return newValue < prevValue - maxDiff || newValue > prevValue + maxDiff;
}

long lastMsg = 0;
float temp = 0.0;
float hum = 0.0;
float diff = 1.0;

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 1000) {
    lastMsg = now;

    float newTemp = hdc.readTemperature();
    float newHum = hdc.readHumidity();

      Serial.print("New temperature:");
      Serial.println(String(temp).c_str());
      //client.publish(temperature_topic, String(temp).c_str(), true);

      Serial.print("New humidity:");
      Serial.println(String(hum).c_str());
      //client.publish(humidity_topic, String(hum).c_str(), true);
  }
}
