#include <WiFi.h>
#include <PubSubClient.h>
#include"SPIFFS.h"


// Update these with values suitable for your network.

const char* ssid = "F A I S A L";
const char* password = "pakistan123";
const char* mqtt_server = "broker.hivemq.com";

int iter = 1; // iterate through the number of chunks
int i=0; // location in the big pointer array

String topic_chunks = "faisal";
String topic_array_size = "array-size";

//unsigned char *gImage_4in01f PROGMEM = NULL;
//esp_bt_controller_mem_release(ESP_BT_MODE_BTDM);
unsigned char gImage_4in01f[134400]={};//86144

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
 if (strcmp(topic,topic_array_size.c_str()) == 0)
 {
   payload[length] = '\0'; // Make payload a string by NULL terminating it.
   int sizeArray = atoi((char *)payload);
   Serial.print("The size of array to be declared is: ");
   Serial.println(sizeArray);
    
//   File logFile = SPIFFS.open("/test.txt", "r");
//   Serial.println("File opened for reading.");
//    unsigned char gImage_4in01f[sizeArray];
//   gImage_4in01f = alloc(sizeArray);
//   if (!gImage_4in01f) Serial.println("The memory was not allocated.");
//   else Serial.println("The memory was allocated successfully.");

//   Serial.print("The memory was allocated successfully.");
    
//   Serial.println("Starting to read from the file...");
//   logFile.read((uint8_t*) gImage_4in01f, logFile.size());
//   gImage_4in01f[size] = '\0';
//   Serial.print("Data: ");
//   for (int i = 0; i < size; i++)
//     {Serial.print(gImage_4in01f[i],HEX);
//     Serial.print(',');
//     }
//   Serial.println("");
//   Serial.println();
//   Serial.print("The size of final array is: ");
//   Serial.println(sizeof(gImage_4in01f));
//   logFile.close(); 
   return;
 }

  if (strcmp(topic,topic_chunks.c_str()) == 0)
  {
//  File logFile = SPIFFS.open("/test.txt", "a");
  Serial.print("Received Chunk ");Serial.print(iter);Serial.print(" : ");
  String chunk = "";
  for (int inside = 0; inside < length;)
  {
     chunk.concat((char)payload[inside]);
     inside++;
//     if (i==0)
//     {
//       if((char)payload[i] == '0' && (char)payload[i+1]== 'x')
//       {
//         logFile.print('\\');
//         Serial.print('\\');
//         i=i+1;
//       }
//     }
//    if (i==length-1)
//    {
//      if((char)payload[i] == ',')
//      {
//        break;
//      }
//    }
//    if ((char)payload[i] == ',' && (char)payload[i+1]== '0')
//    {
//      logFile.print('\\');
//      Serial.print('\\');
//      i=i+2;
//    }
//    else
//    {
//      logFile.print((char)payload[i]);
//      Serial.print((char)payload[i]);
//      i++;
//    }
  }
  Serial.println(chunk);
  unsigned int str_len = chunk.length()+1;
  Serial.print("The length of recieved chunk is: "); Serial.println(str_len);
  char charArray[str_len];
  chunk.toCharArray(charArray, str_len);
  chunk="";
  Serial.print("The chunk converted to array is: "); Serial.println(charArray);
  Serial.println();
  
  // split charArray at each ',' and convert to uint8_t
   char *p = strtok(charArray, ",");
   while(p != NULL) {
//    Serial.printf("%x \n", p);
    byte f = strtoul(p, NULL, 16);
//    Serial.println(f);
   gImage_4in01f[i++] = f;
   Serial.println(f);
   p = strtok(NULL, ",");
   }
//  Serial.print("File size: ");
//  Serial.println(logFile.size());
//  logFile.close();
  Serial.println("Values were stored in the big pointer array.");
  iter++;
  return;
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
//      client.publish(topic_chunks.c_str(), "hello world");
      // ... and resubscribe
      client.subscribe(topic_chunks.c_str());
      client.subscribe(topic_array_size.c_str());
      client.subscribe("finished");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  while(!Serial);
//  if(!SPIFFS.begin(true)){
//        Serial.println("An Error has occurred while mounting SPIFFS");
//        return;
//   }
    
//  bool formatted = SPIFFS.format();
//
//  if(formatted){
//    Serial.println("\n\nSuccess formatting");
//  }else{
//    Serial.println("\n\nError formatting");
//  }

  setup_wifi();
//  SPIFFS.remove("/test.txt");
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

unsigned char * alloc(int size) {
    unsigned char *data = (unsigned char *) malloc( size );

    if ( data ) memset( data, 0, size );

    return data;
}
