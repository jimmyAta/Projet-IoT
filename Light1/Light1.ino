#include <ESP8266WiFi.h>
#include <PubSubClient.h>

        const int buttonPin = 2;
        const int ledPin = 13;
        int currentButtonState = 0;
        int lastButtonState = 0;
        int ledState = 0;


const char* ssid = "YourNetworkName";
const char* password =  "YourNetworkPassword";
const char* mqttServer = "m23.cloudmqtt.com";
const int mqttPort = 18552;
const char* mqttUser = "iqrhwqjk";
const char* mqttPassword = "uBEotcdU9c2_";
 
WiFiClient espClient;
PubSubClient client(espClient);
 
void setup() {

        pinMode(ledPin, OUTPUT);
        pinMode(buttonPin, INPUT);
 
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
 
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
 
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
 
  client.publish("light1", "Hello from Arduino");
  client.subscribe("light1");
 
}
 
void callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  } 
  Serial.println(); 

if((char)payload[1]=='f'){
  digitalWrite(ledPin,LOW);
}else{
  digitalWrite(ledPin,HIGH);
}  
}
 
void loop() { 
    
            currentButtonState = digitalRead(buttonPin);
            ledState = digitalRead(ledPin); 
  
            if(currentButtonState != lastButtonState)
               {  lastButtonState = currentButtonState;
                  if(currentButtonState == HIGH)
                    {
                       if(ledState==HIGH)
                          {
                             digitalWrite(ledPin,LOW);
                             client.publish("light1", "off"); 
                                                                    
                          }
                       else if(ledState==LOW)
                          {                  
                             digitalWrite(ledPin,HIGH);
                             client.publish("light1", "on");  
                                                           
                          }                              
                     }  
                }
 client.loop();
 
}
