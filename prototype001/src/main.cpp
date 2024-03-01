#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <point.h>

#define MESSAGE_SIZE 2048


//wifi 
const char* ssid = "KTFinternal5Ghz";
const char* pass = "k1kk3rw1f1";

const char* anchorpayload = "[{\"id\":\"3448\",\"fwVersion\":\"0x22\",\"hwVersion\":\"0x2\",\"x\":4096,\"y\":211,\"z\":1930,\"connected\":true,\"inUse\":true,\"lastSeen\":\"2023-11-08T14:51:06.409Z\",\"lastConnected\":\"2023-11-08T13:37:29.967Z\",\"neighbors\":[{\"anchorId\":\"26687\",\"range\":4209,\"range9010\":20},{\"anchorId\":\"26696\",\"range\":5192,\"range9010\":63},{\"anchorId\":\"26699\",\"range\":4254,\"range9010\":39}],\"labels\":[{\"text\":\"anchor2\",\"color\":\"#7E00FF\"}],\"presenceOnly\":false},{\"id\":\"26699\",\"fwVersion\":\"0x22\",\"hwVersion\":\"0x2\",\"x\":315,\"y\":510,\"z\":290,\"connected\":true,\"inUse\":true,\"lastSeen\":\"2023-11-08T14:51:06.409Z\",\"lastConnected\":\"2023-11-08T13:37:29.967Z\",\"neighbors\":[{\"anchorId\":\"3448\",\"range\":4254,\"range9010\":39},{\"anchorId\":\"26687\",\"range\":5117,\"range9010\":61},{\"anchorId\":\"26696\",\"range\":3479,\"range9010\":29}],\"labels\":[{\"text\":\"anchor1\",\"color\":\"#7E00FF\"}],\"presenceOnly\":false},{\"id\":\"26696\",\"fwVersion\":\"0x22\",\"hwVersion\":\"0x2\",\"x\":-88,\"y\":3507,\"z\":1780,\"connected\":true,\"inUse\":true,\"lastSeen\":\"2023-11-08T14:51:06.409Z\",\"lastConnected\":\"2023-11-08T13:37:29.967Z\",\"neighbors\":[{\"anchorId\":\"3448\",\"range\":5192,\"range9010\":63},{\"anchorId\":\"26687\",\"range\":4705,\"range9010\":59},{\"anchorId\":\"26699\",\"range\":3479,\"range9010\":29}],\"labels\":[{\"text\":\"anchor3\",\"color\":\"#7E00FF\"}],\"presenceOnly\":false},{\"id\":\"26687\",\"fwVersion\":\"0x22\",\"hwVersion\":\"0x2\",\"x\":4240,\"y\":3993,\"z\":290,\"connected\":true,\"inUse\":true,\"lastSeen\":\"2023-11-08T14:51:06.409Z\",\"lastConnected\":\"2023-11-08T14:09:34.696Z\",\"neighbors\":[{\"anchorId\":\"3448\",\"range\":4209,\"range9010\":20},{\"anchorId\":\"26696\",\"range\":4705,\"range9010\":59},{\"anchorId\":\"26699\",\"range\":5117,\"range9010\":61}],\"labels\":[{\"text\":\"anchor4\",\"color\":\"#7E00FF\"}],\"presenceOnly\":false}]";

DynamicJsonDocument doc(MESSAGE_SIZE);
DynamicJsonDocument anchordoc(MESSAGE_SIZE);

void setupWifi();
void httpProcess();

Point* Anchor1;
Point* Anchor2;
Point* Anchor3;
Point* Anchor4;

//mqtt
const char* mqtt_server = "192.168.10.125";
const int mqtt_port = 1883;
const char* mqtt_topic = "tags/+";

WiFiClient espClient;
HTTPClient httpClient;
PubSubClient client(espClient); 
HTTPClient http;

void callback(char *topic, byte *payload, unsigned int length);
void reconnect();

void setup() {
  Serial.begin(115200);
  delay(5000); //delay so the terminal doesnt print weird characters
  
  //wifi
  setupWifi();
  delay(1000);
  
  //http
  httpProcess(); //get the anchors with an http request
  delay(1000000);
  // Free resources
  http.end();

  //mqtt
  Serial.print("Now setting up the mqtt server");
  client.setBufferSize(MESSAGE_SIZE);
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  if(client.connect("espClient-1-2-34", NULL, NULL)){
    Serial.println("\nConnected to MQTT broker ");
  } else {
    Serial.println("\nConnection to MQTT broker has failed");
    Serial.println(client.state());
    delay(200);
  }

  Serial.println(client.subscribe(mqtt_topic, 0));
  client.publish("outTopic", "hello world");
  
}

void loop() {

  if (!client.connected()) Serial.println(":-(");
  
  client.loop();
}

void setupWifi(){ //implementation of setupWIFI

  Serial.print("\nConnecting to ");
  Serial.println(ssid); 
  WiFi.begin(ssid, pass);

  while(WiFi.status() != WL_CONNECTED){
    delay(100);
    Serial.print("-");
  }
  Serial.print("\nConnected to ");
  Serial.println(ssid);
  delay(100);
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.println(topic);

  deserializeJson(doc, payload);

  const char* tagId = doc[0]["tagId"];
  Serial.println(tagId);

  int tagxPos = doc[0]["data"]["coordinates"]["x"];
  Serial.println("X-coordinate: " + String(tagxPos));

  int tagyPos = doc[0]["data"]["coordinates"]["y"];
  Serial.println("Y-coordinate: " + String(tagyPos));

  int tagzPos = doc[0]["data"]["coordinates"]["z"];
  Serial.println("Z-coordinate: " + String(tagzPos));

}

void httpProcess(){
  String serverPath = "http://192.168.10.125:4000/anchors";
  
  // Your Domain name with URL path or IP address with path
  http.begin(serverPath.c_str());
  
  // Send HTTP GET request
  int httpResponseCode = http.GET();
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String anchorpayload = http.getString();
    Serial.println(anchorpayload);  
    deserializeJson(anchordoc, anchorpayload);

    int anchor1xPos = anchordoc[0]["x"];
    int anchor1yPos = anchordoc[0]["y"];
    int anchor1zPos = anchordoc[0]["z"];
    Serial.println("X-coordinate 1: " + String(anchor1xPos));
    Serial.println("Y-coordinate 1: " + String(anchor1yPos));
    Serial.println("Z-coordinate 1: " + String(anchor1zPos));

    int anchor2xPos = anchordoc[1]["x"];
    int anchor2yPos = anchordoc[1]["y"];
    int anchor2zPos = anchordoc[1]["z"];
    Serial.println("X-coordinate 2: " + String(anchor2xPos));
    Serial.println("Y-coordinate 2: " + String(anchor2yPos));
    Serial.println("Z-coordinate 2: " + String(anchor2zPos));

    int anchor3xPos = anchordoc[2]["x"];
    int anchor3yPos = anchordoc[2]["y"];
    int anchor3zPos = anchordoc[2]["z"];
    Serial.println("X-coordinate 3: " + String(anchor3xPos));
    Serial.println("Y-coordinate 3: " + String(anchor3yPos));
    Serial.println("Z-coordinate 3: " + String(anchor3zPos));

    int anchor4xPos = anchordoc[3]["x"];
    int anchor4yPos = anchordoc[3]["y"];
    int anchor4zPos = anchordoc[3]["z"];
    Serial.println("X-coordinate 4: " + String(anchor4xPos));
    Serial.println("Y-coordinate 4: " + String(anchor4yPos));
    Serial.println("Z-coordinate 4: " + String(anchor4zPos));

    Anchor1 = new Point(anchor1xPos, anchor1yPos, anchor1zPos);
    Anchor2 = new Point(anchor2xPos, anchor2yPos, anchor2zPos);
    Anchor3 = new Point(anchor3xPos, anchor3yPos, anchor3zPos);
    Anchor4 = new Point(anchor4xPos, anchor4yPos, anchor4zPos);

  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
}
