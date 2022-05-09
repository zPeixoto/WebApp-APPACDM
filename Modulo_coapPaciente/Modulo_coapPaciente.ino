/*
  ESP-COAP Client
*/
#include <ESP8266WiFi.h>
#include "coap_client.h"
#include <Wire.h>

#define CMPS12_ADDRESS 0x60 

//instance for coapclient
coapClient coap;

//WiFi connection info
const char* ssid = "AREA51-WIFI";
const char* password = "area51-pass";

char angles[256];
double Ang_AcX, Ang_AcZ, Ang_Rot;
unsigned char high_byte, low_byte;
int8_t regis_8b[3];
int16_t regis_16b[6];
int count;

//ip address and default port of coap server in which your interested in
IPAddress ip(192, 168, 1, 51);//take ETH Zurich or coap.me server to run and check client
int port = 5683;

// coap client response callback
void callback_response(coapPacket &packet, IPAddress ip, int port);

// coap client response callback
void callback_response(coapPacket &packet, IPAddress ip, int port) {
  char p[packet.payloadlen + 1];
  memcpy(p, packet.payload, packet.payloadlen);
  p[packet.payloadlen] = NULL;

  //response from coap server
  if (packet.type == 3 && packet.code == 0) {
    Serial.println("ping ok");
  }

  Serial.println(p);
}

void setup() {

  Serial.begin(115200);  // Start serial port
  Wire.begin();

  WiFi.begin(ssid, password);
  Serial.println(" ");
  \
  // Connection info to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    //delay(500);
    yield();
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  // Print the IP address of client
  Serial.println(WiFi.localIP());

  // client response callback.
  // this endpoint is single callback.
  coap.response(callback_response);

  // start coap client
  coap.start();
}
//int i=0;

void loop() {
  bool state;

  Wire.beginTransmission(CMPS12_ADDRESS);  //starts communication with CMPS12
  Wire.write(0);                     //Sends the register we wish to start reading from
  Wire.endTransmission();

  // Request 31 bytes from the CMPS12
  Wire.requestFrom(CMPS12_ADDRESS, 18);
  while (Wire.available() < 18);       // Wait for all bytes to come back

  regis_8b[0] = Wire.read(); // 0x00 (COMMAND RESITER)
  regis_8b[1] = Wire.read(); // 0x01 (COMPASS BEARING 8 Bit)
  regis_16b[0] = Wire.read() << 8 | Wire.read(); // 0x02 (COMPASS BEARING 8 BITS HIGH) & 0x03 (COMPASS BEARING 8 BITS LOW)
  regis_8b[2] = Wire.read(); // 0x04 (PITCH ANGLE +/-90º)
  regis_8b[3] = Wire.read(); // 0x05 (ROLL ANGLE +/-90º)
  regis_16b[1] = Wire.read() << 8 | Wire.read(); // 0x06 (MAGNETOMETER X AXIS RAW - 8 BITS HIGH) & 0x07 (MAGNETOMETER X AXIS RAW - 8 BITS LOW)
  regis_16b[2] = Wire.read() << 8 | Wire.read(); // 0x08 (MAGNETOMETER Y AXIS RAW - 8 BITS HIGH) & 0x09 (MAGNETOMETER Y AXIS RAW - 8 BITS LOW)
  regis_16b[3] = Wire.read() << 8 | Wire.read(); // 0x0A (MAGNETOMETER Z AXIS RAW - 8 BITS HIGH) & 0x0B (MAGNETOMETER Z AXIS RAW - 8 BITS LOW)
  regis_16b[4] = Wire.read() << 8 | Wire.read(); // 0x0C (ACCELEROMETER X AXIS RAW - 8 BITS HIGH) & 0x0D (ACCELEROMETER X AXIS RAW - 8 BITS LOW)
  regis_16b[5] = Wire.read() << 8 | Wire.read(); // 0x0E (ACCELEROMETER Y AXIS RAW - 8 BITS HIGH) & 0x0F (ACCELEROMETER Y AXIS RAW - 8 BITS LOW)
  regis_16b[6] = Wire.read() << 8 | Wire.read(); // 0x10 (ACCELEROMETER Z AXIS RAW - 8 BITS HIGH) & 0x11 (ACCELEROMETER Z AXIS RAW - 8 BITS LOW)

  getAngles();
  sprintf(angles, "{'device':'DEVPACIENTE','data':[{'name':'Costas_Frente_Tras', 'data':'%f'},{'name':'Costas_Esq_Dir', 'data':'%f'},{'name':'Rotacao', 'data':'%d' }], 'count':'%d'}", Ang_AcX, Ang_AcZ, regis_16b[0] / 10, count);

  int msgid = coap.put(ip, port, "deviceman", angles, strlen(angles));
  
  state = coap.loop();
  
  count++;
  delay(100);
}

void getAngles() {
  Ang_AcX = (180 / PI) * atan(regis_16b[6] / sqrt(pow(regis_16b[5], 2) + pow(regis_16b[4], 2)));
  Ang_AcZ = (180 / PI) * atan(regis_16b[5] / sqrt(pow(regis_16b[6], 2) + pow(regis_16b[4], 2)));
}
