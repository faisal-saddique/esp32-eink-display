#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "F A I S A L";
const char* password = "pakistan123";
const char* mqtt_server = "broker.hivemq.com";

int iter = 1;

String topic_chunks = "faisal";
String topic_array_size = "array-size";
String topic_finished = "finished";

//unsigned char *gImage_4in01f = NULL;  

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
//  int size;
//  if (strncmp(topic,topic_array_size.c_str(),topic_array_size.length())==0)
//  {
//    payload[length] = '\0'; // Make payload a string by NULL terminating it.
//    size = atoi((char *)payload);
//    Serial.print("The size of array to be declared is: ");
//    Serial.println(size);
//    gImage_4in01f = alloc(size); 
//  }
//  if (strncmp(topic,topic_finished.c_str(),topic_finished.length()) == 0)
//  {
//    Serial.println("Finishing up...");
//    gImage_4in01f[size] = '\0';
//    Serial.print("Data: ");
//    for (int i = 0; i < size; i++)
//    {
//      Serial.print(gImage_4in01f[i],HEX);
//      Serial.print(',');
//    }
//    Serial.println("");
//    Serial.print("The size of final array is: ");
//    Serial.println(sizeof(gImage_4in01f));
//  }
//  if (strncmp(topic,topic_chunks.c_str(),topic_chunks.length())==0)
//  {
    Serial.print("Received Chunk ");Serial.print(iter);Serial.print(" : ");
    for (int i = 0; i < length;i++)
    {
      Serial.print((char)payload[i]);
    }
    iter++;
//  }
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
      client.subscribe(topic_chunks.c_str());
//      client.subscribe(topic_array_size.c_str());
//      client.subscribe(topic_finished.c_str());
    } 
    else 
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  while(!Serial);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

//unsigned char * alloc(int size) {
//    unsigned char *data = (unsigned char *) malloc( size );
//    if ( data ) memset( data, 0, size );
//    return data;
//}
