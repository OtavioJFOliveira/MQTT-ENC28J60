
#include <UIPEthernet.h>
#include "PubSubClient.h"

//--------------------------------------------------------------------------
#define CLIENT_ID       "Hal"
//#define TOPIC           "temp"
#define PUBLISH_DELAY   3000
#define PUB_TOPIC "my_username/f/temperature" //Adafruit dashboard
#define PUB_TOPIC_h "my_username/f/humidity" //Adafruit dashboard
#define DHTPIN          3
#define DHTTYPE         DHT11
#define ledPin 13
#define relayPin 8

#define mqttServer "broker.hivemq.com"
#define port "1883" 

String ip = "";

int lichtstatus;
uint8_t mac[6] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x06};
//------------------------------------------------------------------------

EthernetClient ethClient;
PubSubClient mqttClient;
//Dht DHT (DHTPIN, DHTTYPE);

long previousMillis;

void setup () {

  
  Serial.begin (9600);
  while (! Serial) {};
  Serial.println (F ("MQTT Arduino Demo"));
  Serial.println ();

  // configuração de comunicação ethernet usando DHCP
  if (Ethernet.begin (mac) == 0) {
    //Serial.println(F("Unable to configure Ethernet usando DHCP "));
    for (;;);
  }

  Serial.println (F ("Ethernet configurada via DHCP"));
  Serial.print ("endereço IP:");
  Serial.println (Ethernet.localIP ());
  Serial.println ();

  ip = String (Ethernet.localIP () [0]);
  ip = ip + ".";
  ip = ip + String (Ethernet.localIP () [1]);
  ip = ip + ".";
  ip = ip + String (Ethernet.localIP () [2]);
  ip = ip + ".";
  ip = ip + String (Ethernet.localIP () [3]);
  //Serial.println(ip);

  // setup mqtt client
  mqttClient.setClient (ethClient);
  mqttClient.setServer(mqttServer, port);

  //Serial.println(F("MQTT client configured ")));
  mqttClient.setCallback (callback);

  Serial.println ();
  Serial.println (F ("Pronto para enviar dados"));
  previousMillis = millis ();
  mqttClient.publish ("home / br / nb / ip", ip.c_str ()); // Revisar
}

void loop () {

  lichtstatus = analogRead (A0); // Lê um LDR


  // é hora de enviar novos dados?
  if (millis () - previousMillis> PUBLISH_DELAY) {
    sendData ();
    previousMillis = millis ();

  }

  mqttClient.loop ();
}

//--------------------------------------------------------------
void sendData () {


  char msgBuffer [20];
  float h = 3;
  float t = 88;

//  Serial.println ((relaystate == LOW)? "OPEN": "FECHADO");
  if (mqttClient.connect (CLIENT_ID)) 
  {
  //  mqttClient.publish ("home / pt / nb / temp", dtostrf (t, 6, 2, msgBuffer));
 //   mqttClient.publish ("home / br / nb / humid", dtostrf (h, 6, 2, msgBuffer));
 //   mqttClient.publish ("home / br / nb / deur", (statusBD == HIGH)? "ABERTO": "FECHADO");

    mqttClient.subscribe ("home / br / sb");

    int startsend=1; // apagar depois
    if (startsend) {
     // mqttClient.publish ("home / br / nb / relay", (relaystate == BAIXO)? "OPEN": "FECHADO");
      mqttClient.publish ("home / br / nb / ip", ip.c_str ());
      startsend = LOW;
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  char msgBuffer[20];
  // I am only using one ascii character as command, so do not need to take an entire word as payload
  // However, if you want to send full word commands, uncomment the next line and use for string comparison
   //payload[length] = '\0';            // terminate string with '0'
  //String strPayload = String((char*)payload);  // convert to string
  // Serial.println(strPayload); //can use this if using longer southbound topics
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");//MQTT_BROKER
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  Serial.println(payload[0]);

  // Examine only the first character of the message
  if (payload[0] == 49)             // Message "1" in ASCII (turn output ON)
  {
     // DEFINIR AÇÃO

  } else {
    Serial.println("Unknown value");
    mqttClient.publish("home/br/nb", "Syntax Error");
  }

}
