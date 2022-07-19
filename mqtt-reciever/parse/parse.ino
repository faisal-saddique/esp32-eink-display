#include "SPIFFS.h"


void setup()
{
    Serial.begin(115200);
    String s = "0x84,0x43,0xFF,0x67,0xAB,0x22,0x38,0x58,0x64";

    // Convert String object to char array
    unsigned int str_len = s.length()+1;
    char charArray[str_len];
    s.toCharArray(charArray, str_len);

    byte byteArray[9]={};
    int i=0;
    
    // split charArray at each ',' and convert to uint8_t
    char *p = strtok(charArray, ",");
    while(p != NULL) {
    byteArray[i++] = strtoul(p, NULL, 16);
    p = strtok(NULL, ",");
    }
    
    
    // Print the byteArray
    for (i=0; i<sizeof(byteArray)/sizeof(unsigned char); i++) {
    Serial.print(byteArray[i]);
    Serial.println(", ");
    }
    Serial.println();
}

void loop()
{

}
