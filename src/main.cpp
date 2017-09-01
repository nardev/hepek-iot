#include <Arduino.h>

// ESP8266 RELATED INCLUDES
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <../lib/WiFiManagerCustom/WiFiManager.h>
#include <FS.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

// DEVICES INCLUDES
#include <../lib/IRremoteESP8266/src/IRremoteESP8266.h>
#include <../lib/IRremoteESP8266/src/IRsend.h>
#include <../lib/rc-switch/RCSwitch.h>
#include <Adafruit_NeoPixel.h>
#include <DHT.h>


// LOAD SETTINGS
// =============================================================

#define  DEVICE_NAME  = "edin"

struct JsonBundle {
  public:
    void parse(const char* json) {
      _jsonBuffer = new DynamicJsonBuffer();
      _jsonVariant = _jsonBuffer->parseObject(json);
    }

    void parse(Stream& json) {
      _jsonBuffer = new DynamicJsonBuffer();
      _jsonVariant = _jsonBuffer->parseObject(json);
    }

    const JsonObject& root() const {
      return _jsonVariant;
    }

    const JsonObject& getNode(String key) const {
      JsonObject& obj = _jsonVariant.as<JsonObject>().get<JsonVariant>(key);
      return obj;
    }

  private:
    DynamicJsonBuffer* _jsonBuffer;
    JsonVariant _jsonVariant;
};

JsonBundle settings;

void getSettings() {
  DynamicJsonBuffer configFileJB;
  File configFile = SPIFFS.open("/settings.json", "r");

  if (!configFile) {
     Serial.println("Failed to open config file");
  }

  settings.parse(configFile);
}


// EVERYTHING RELATED TO NEOPIXEL
// =============================================================

#define NEOPIXELPIN 13
#define NUMPIXELS 12
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NEOPIXELPIN, NEO_GRB + NEO_KHZ800);

void setNeopixel() {
  pixels.begin();
  pixels.setBrightness(255);
  pixels.show();
  pinMode(NEOPIXELPIN, OUTPUT);
  digitalWrite(NEOPIXELPIN, LOW);
}

void turnNeopixelOn(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<pixels.numPixels(); i++) {
    pixels.setPixelColor(i, c);
    pixels.show();
    delay(wait);
  }
}

// EVERYTHING RELATED TO DHT22 SENSOR, GETTING TEMP AND HUM DATA
// =============================================================
#define DHTPIN 5
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

float getTemperature() {
  return dht.readTemperature();
}

float getHumidity() {
  return dht.readHumidity();
}
// =============================================================

// EVERYTHING IR SIGNALS
// =============================================================
#define IRPIN 11

void sendIRcode(int code) {
  Serial.println(code);
}

int findAndSendCode() {
  return 1;
}

// EVERYTHING RC SIGNALS
// =============================================================
#define RCPIN 12

void sendRCcode(int code) {
  Serial.println(code);
}

// =============================================================

// DEVICE - NETWORK RELATED
// =============================================================
String ipToString(IPAddress ip){
  String s="";
  for (int i=0; i<4; i++)
    s += i  ? "." + String(ip[i]) : String(ip[i]);
  return s;
}

String ifconfig() {
  // vea add - this will have to be published to mqtt broker
  // get all possible settings like gateway,
  // IPAddress getLocalIpAddress()
  // IPAddress WiFi.localIP();
  // String WiFi.macAddress();
  // IPAddress WiFi.subnetMask();
  // IPAddress WiFi.gatewayIP();
  // IPAddress WiFi.dnsIP();
  // wl_status_t WiFi.status();
  // String WiFi.psk();
  return ipToString(WiFi.localIP());
}

String getMacAddress() {
  return WiFi.macAddress();
}


// HEPEK FUNCTIONS
// =============================================================

void startConfigPortal() {
  WiFiManager wifiManager;
  if (!wifiManager.startConfigPortal("HEPEK","sarajevo")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    ESP.reset();
    delay(5000);
  }
}

void startWiFiManager() {
  WiFiManager wifiManager;
  if (!wifiManager.autoConnect("HEPEK","sarajevo")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    ESP.reset();
    delay(5000);
  }
}

void activateOTA() {

  t_httpUpdate_return ret = ESPhttpUpdate.update("");
  switch(ret) {
    case HTTP_UPDATE_FAILED:
        Serial.println("[update] Update failed.");
        Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());

        break;
    case HTTP_UPDATE_NO_UPDATES:
        Serial.println("[update] Update no Update.");
        break;
    case HTTP_UPDATE_OK:
        Serial.println("[update] Update ok.");
        break;
  }
}


// =============================================================

// MQTT RELATED FUNCTIONS
// =============================================================

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print(topic);

  if (strcmp(topic,"edin/getSensor/temperature")==0) {
    client.publish("edin/temperature",String(getTemperature()).c_str());
  } else if (strcmp(topic,"edin/getSensor/humidity")==0) {
    client.publish("edin/humidity",String(getHumidity()).c_str());
  }

  if ((strcmp(topic,"edin/light") == 0) && (char)message[0] == '1') {
    turnNeopixelOn(pixels.Color(0, 0, 64), 100);
  } else if ((strcmp(topic,"edin/light") == 0) && (char)message[0] == '0') {
    turnNeopixelOn(pixels.Color(0,0,0), 25);
  }

  if ((strcmp(topic,"edin/config") == 0) && (char)message[0] == '1') {
    startConfigPortal();
  }

  if ((strcmp(topic,"edin/ping") == 0)) {
    client.publish("edin/ping","echo...");
    delay(2000);
    activateOTA();
  }

  if ((strcmp(topic,"edin/reset")==0) && (char)message[0] == '1') {
    client.publish("edin/reset","idemo reset...");
    delay(5000);
    ESP.reset();
    delay(2000);
  }

  if ((strcmp(topic,"edin/ota") == 0) && (char)message[0] == '9') {
    client.publish("edin","idemo OTA...");
    delay(2000);
    activateOTA();
  }

}


// =============================================================

void setMQTT() {
  const char* mqttDeviceId = settings.getNode("mqtt")["device_id"];
  const char* mqttServer = settings.getNode("mqtt")["hostname"];
  int mqttPort = settings.getNode("mqtt")["port"];
  const char* mqttUser = settings.getNode("mqtt")["username"];
  const char* mqttPassword = settings.getNode("mqtt")["password"];

  client.setServer(mqttServer, mqttPort);
  while (!client.connected()) {
      Serial.println("Connecting to MQTT...");

      if (client.connect(mqttDeviceId, mqttUser, mqttPassword)) {
        Serial.println("connected");
        client.publish("outTopic","resetovo se...");
        client.subscribe("edin/#");
        client.setCallback(callback);
      } else {
        Serial.print("failed with state ");
        Serial.print(client.state());
        delay(2000);
      }
  }
}

// FILE SYSTEM
// =============================================================

void setFileSystem() {
  SPIFFS.begin();
  FSInfo fs_info;
  SPIFFS.info(fs_info);
}

void setup() {
  Serial.begin(115200);
  Serial.println(WiFi.status());

  setFileSystem();
  getSettings();
  setNeopixel();
  startWiFiManager();
  setMQTT();

}

void loop() {
  client.loop();
  yield();

  settings.getNode("mqtt").printTo(Serial);



}
