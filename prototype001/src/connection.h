// #ifndef CONNECTION_H
// #define CONNECTION_H

// #include <Arduino.h>
// #include <WiFi.h>
// #include <PubSubClient.h>
// #include <ArduinoJson.h>
// #include <HTTPClient.h>

// #define MESSAGE_SIZE 1024

// DynamicJsonDocument doc(MESSAGE_SIZE);
// WiFiClient espClient;
// HTTPClient httpClient;
// PubSubClient client(espClient); 

// class Connection {
//     //constructor and destructor
    
//     public:
//     Connection();
//     ~Connection();

//     //setup functions
//     void setupWifi();
//     void setupHttp();

//     //retrieve information functions
//     void httpGetReq();
//     void reconnect();

//     private:
//     //wifi 
//     const char* ssid = "KTFinternal5Ghz";
//     const char* pass = "k1kk3rw1f1";
//     //http
//     String serverPath = "http://192.168.10.125:4000/anchors";

// };

// #endif //CONNECTION_H