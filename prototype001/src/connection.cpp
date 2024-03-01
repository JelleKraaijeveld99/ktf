// #include <connection.h>


// //constructor
// Connection::Connection(){
    
// }

// //destructor
// Connection::~Connection(){

// }

// //function for setting up the WiFi
// void Connection::setupWifi(){
//   Serial.print("\nConnecting to ");
//   Serial.println(ssid); 
//   WiFi.begin(ssid, pass);

//   while(WiFi.status() != WL_CONNECTED){
//     delay(100);
//     Serial.print("-");
//   }
  
//   Serial.print("\nConnected to ");
//   Serial.println(ssid);
//   delay(100);
// }



// //function for setting up the Http connection
// void Connection::setupHttp(){
//   HTTPClient http;

//   String serverPath = "http://192.168.10.125:4000/anchors";
  
//   // Your Domain name with URL path or IP address with path
//   http.begin(serverPath.c_str());
  
//   // Send HTTP GET request
//   int httpResponseCode = http.GET();
  
//   if (httpResponseCode>0) {
//     Serial.print("HTTP Response code: ");
//     Serial.println(httpResponseCode);
//     String payload = http.getString();
//     Serial.println(payload);
//   }
//   else {
//     Serial.print("Error code: ");
//     Serial.println(httpResponseCode);
//   }

//   delay(1000);
//   http.end();
// }

// void Connection::httpGetReq(){

// }


