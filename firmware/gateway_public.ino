
/**
   ESPNOW - Basic communication - Slave
   Date: 26th September 2017
   Author: Arvind Ravulavaru <https://github.com/arvindr21>
*/

#include <esp_now.h>
#include <WiFi.h>
#include <ArduinoOTA.h>
#include <esp_wifi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
//#include <MQTTClient.h>
#include <ESP32Ping.h>

// Structure example to send data
// Must match the receiver structure
#define CHANNEL 6
typedef struct struct_message {
  char a[32];
  int b;
  float c;
  bool e;
} struct_message;

// Create a struct_message called myData
struct_message myData;

//Sensitive data here

const char* wifi_ssid = "SSID";
const char* wifi_password = "xxxxxxxxxxxxxx";

String API_endpoint = "https://makeathon.ddnss.org/talkwithyourifxsensor/api/";

WiFiClient mqttReceiver;
int recvMsgNum;

String hostname = "ESP32Gateway";




// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  String JSON_message;
  DynamicJsonDocument doc(1024);

  doc["Char"] = myData.a;
  doc["Int"]   = myData.b;
  doc["Float"] = myData.c;
  doc["Bool"] = myData.e;

  
  serializeJson(doc, JSON_message);
  Serial.println(JSON_message);
 
  
}
// Init ESP Now with fallback
void InitESPNow() {
  if (esp_now_init() == ESP_OK) {
    Serial.println("ESPNow Init Success");
  }
  else {
    Serial.println("ESPNow Init Failed");
    ESP.restart();
  }
}

// config AP SSID
void configDeviceAP() {
  String Prefix = "Slave:";
  String Mac = WiFi.macAddress();
  String SSID = Prefix + Mac;
  String Password = "123456789";
  bool result = WiFi.softAP(SSID.c_str(), Password.c_str(), CHANNEL, 0);
  if (!result) {
    Serial.println("AP Config failed.");
  } else {
    Serial.println("AP Config Success. Broadcasting with AP: " + String(SSID));
    Serial.println("Radio channel: " + String(CHANNEL));
  }
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.mode(WIFI_AP_STA);

  esp_wifi_set_ps(WIFI_PS_NONE);

  WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
  WiFi.setHostname(hostname.c_str());

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("WiFi channel");
  Serial.println(WiFi.channel());
}

int login (String username, String password) {

  String login_JSON;
  DynamicJsonDocument login_data(1024);
  String API_login = API_endpoint + "login.php";

  login_data["name"] = username;
  login_data["password"] = password;
  
  serializeJson(login_data, login_JSON);
  Serial.println(login_JSON);

   HTTPClient http;
   http.begin(API_login);
   http.addHeader("Content-Type", "application/json");
   int httpResponse = http.POST(login_JSON);
  
   Serial.print(httpResponse);

    return httpResponse;

 
  
 
 
  
}



void ping_google () {
  const char* remote_host = "www.google.com";
  Serial.print("Pinging host ");
  Serial.println(remote_host);

  if(Ping.ping(remote_host)) {
    Serial.println("Success!!");
  } else {
    Serial.println("Error :(");
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("TalkWithYourInfineonSensors Gateway Showcase");

  setup_wifi();
  
  configDeviceAP();
  // This is the mac address of the Slave in AP Mode
  Serial.print("AP MAC: "); Serial.println(WiFi.softAPmacAddress());
  Serial.println(WiFi.macAddress());

 

  InitESPNow();
  
  //Testing login to API server
  int login_response = login("erich", "erich001");
  Serial.print(login_response);
  
  esp_now_register_recv_cb(OnDataRecv);
}


void loop() {

  delay(5000);
  
}
