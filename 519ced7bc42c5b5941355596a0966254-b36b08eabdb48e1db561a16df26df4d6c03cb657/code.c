// #include <ESP8266WiFi.h> // use this library when burning the code to esp8266
#include <WiFi.h> // use this library when burning the code to esp32
#include <PubSubClient.h>
#include <LinkedList.h> // used for creating the list
#include <ArduinoJson.h> // used for dumping to json


#include "DEV_Config.h"
#include "EPD.h"
#include "GUI_Paint.h"
#include <stdlib.h>



/* Defining a new class */
class Topic {
  public:
    char *name;
    int qos;

    Topic(char *namex, int qosx)
    {
        name = namex;
        qos = qosx; // Quality of service, can be 0 or 1 only
    }
};

/* Update these with values suitable for your network. */
const char* ssid = "Aaron’s iPhone";
const char* wifi_password = "password";

/* Update these with your broker & client credentials */
const char* mqtt_server = "18.144.37.188";
const int port = 1883;
const char *username = "altrMqttUser";
const char *password = "zmCBgsFcKG";

/* Creating a WiFi client and passing on this client to the MQTT client */
WiFiClient espClient;
PubSubClient client(espClient);

/* This is the device_id which will show up in the portal. */
char client_id[] = "access-card-pi-client-3";

/* Defining data storing elements */
LinkedList<Topic*> topics = LinkedList<Topic*>();
String jsonStr;
int msg_count = 0;
UBYTE *BlackImage;

void setup_wifi() {

  delay(10);


  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  Paint_Clear(EPD_4IN01F_WHITE);
  char buffer[40];
  sprintf(buffer, "ssid: %s", ssid);
  Paint_DrawString_EN(10, 0, buffer, &Font16, BLACK, WHITE);
  EPD_4IN01F_Display(BlackImage);


  WiFi.mode(WIFI_STA);

int n = WiFi.scanNetworks();
for (int i = 0; i < n; i++)
{
  Serial.println(WiFi.SSID(i));
}
  
  WiFi.begin(ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  sprintf(buffer, "wifi-connected!!!: %s", WiFi.localIP());
  Paint_DrawString_EN(10, 0, buffer, &Font16, BLACK, WHITE);
  EPD_4IN01F_Display(BlackImage);


  
}

void callback(char* topic, byte* payload, unsigned int length) 
{
  if(topic=="update_badge")
  {
    Serial.println("Update badge request...");
  //  json_message = json.loads(msg.payload.decode())

    /* Please handle this part. The data from the topic has been recieved in the 'payload_string', and it is in string form.
       You'll need to convert it into json and perform the further functionalities. */

    /* 
        # write image to make easy to debug
        image_file = 'writeimage.jpg'
        with open(image_file, "wb") as f:
            try:
                f.write(
                    base64.b64decode(json_message['badge'])
                )
            except KeyError:
                print("Missing image key");
                return
            write_ink(image_file) 
    */
  }
  String payload_string = "";
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    payload_string = payload_string + (char)payload[i];
  }

  Serial.print("Received ");
  Serial.print(payload_string);
  Serial.print(" from ");
  Serial.print(topic);
  Serial.print(" topic");
}

void reconnect() {
  // Loop until we're reconnected
  DynamicJsonDocument doc(1024);

  while (!client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(client_id, username, password)) 
    {
      Serial.println("Connected to MQTT Broker!");
      
      doc["client_id"] = client_id;
      serializeJson(doc, jsonStr);
      // send clientConnect message with client_id
      char Buf[jsonStr.length()+1];
      jsonStr.toCharArray(Buf, jsonStr.length()+1);
      client.publish("clientConnect", Buf);

      Topic *topic;
      for(int i = 0; i < topics.size(); i++)
      {
        topic = topics.get(i);
        client.subscribe(topic->name,topic->qos);
      }
    } 
    else 
    {
      Serial.print("Failed to connect, return code = ");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void publish()
{
  DynamicJsonDocument doc(1024);
    delay(1000);
    String msg = "messages: " + String(msg_count);
    // result = client.publish(topic, msg)
    doc["client_id"] = client_id;
    serializeJson(doc, jsonStr);
    // send clientConnect message with client_id
    char Buf[jsonStr.length()+1];
    Serial.print("jsonSTR \n");

    jsonStr.toCharArray(Buf, jsonStr.length()+1);
    char bufferJson[1024];
    
    // Fixme there is bug here fscreate04
    sprintf(bufferJson, "{'client_id': '%s'}", client_id);
    char bufferJson2[1024];

    sprintf(bufferJson2, "'%s'", bufferJson2);

    bool result = client.publish("clientConnect", bufferJson);
    
    if (result)
    {
       Serial.print("Sent clientConnect to topic \n");
       Serial.println(bufferJson);
    }
    else
    {
      Serial.print("Failed to send message to topic ");
      Serial.println(Buf);
    }
    msg_count += 1;
}

void setup() 
{
  //Serial.begin(9600);
  
  // Creating the topics and passing on the parameters. The second parameter is the Quality of Servie (Qos). It can be 1 or 0 only
  Topic *one = new Topic("hello",1);
  Topic *two = new Topic(client_id,1);
  Topic *three = new Topic("bhaijanan",1);
  
  // Add topics to list
  topics.add(one);
  topics.add(two);
  topics.add(three);
  
  Serial.print("initing something Demo\r\n");
  DEV_Module_Init();

  Serial.print("e-Paper Init and Clear...\r\n");
  EPD_4IN01F_Init();
  EPD_4IN01F_Clear(EPD_4IN01F_WHITE);
  DEV_Delay_ms(100);


  UDOUBLE Imagesize = ((EPD_4IN01F_WIDTH % 2 == 0)? (EPD_4IN01F_WIDTH / 2 ): (EPD_4IN01F_WIDTH / 2 + 1)) * EPD_4IN01F_HEIGHT;
  Imagesize = Imagesize*3/4 + 1000;
  printf("Imagesize %d\r\n",Imagesize);
  if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
      printf("Failed to apply for black memory...\r\n");
      while(1);
  }
  Paint_NewImage(BlackImage, EPD_4IN01F_WIDTH, EPD_4IN01F_HEIGHT, 0, EPD_4IN01F_WHITE);
  Paint_SetScale(7);

  setup_wifi();

  client.setServer(mqtt_server, port);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
    publish();
  }
  client.loop();
}