/*
  ESP-COAP Client
*/

#include <ESP8266WiFi.h>
#include "coap_client.h"
#include <SoftwareSerial.h>


coapClient coap;
SoftwareSerial CMPS12(2, 3); // RX TX

const char* ssid = "AREA51-WIFI";
const char* password = "area51-pass";

char angles[256];
double Ang_AcX, Ang_AcZ, Ang_Rot;
unsigned char high_byte, low_byte;
int16_t regis_16b[6];
int count;

IPAddress ip(192, 168, 1, 51);
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

  Serial.begin(9600);  // Start serial port
  CMPS12.begin(9600);

  WiFi.begin(ssid, password);
  Serial.println(" ");

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

void loop() {
  bool state;
  CMPS12.write(0x13);  // Request and read 16 bit angle
  while (CMPS12.available() < 2);
  high_byte = CMPS12.read();
  low_byte = CMPS12.read();
  regis_16b[0] = high_byte;                // Calculate 16 bit angle
  regis_16b[0] <<= 8;
  regis_16b[0] += low_byte;

/*
  CMPS12.write(0x19);  // Request and read 16 bit angle
  while (CMPS12.available() < 6);
  high_byte = CMPS12.read();
  low_byte = CMPS12.read();
  regis_16b[0] = high_byte;                // Calculate 16 bit angle
  regis_16b[0] <<= 8;
  regis_16b[0] += low_byte;

  high_byte = CMPS12.read();
  low_byte = CMPS12.read();
  regis_16b[1] = high_byte;      // Y ACELL
  regis_16b[1] <<= 8;
  regis_16b[1] += low_byte;

  high_byte = CMPS12.read();
  low_byte = CMPS12.read();
  regis_16b[2] = high_byte;      // Z ACELL
  regis_16b[2] <<= 8;
  regis_16b[2] += low_byte;
*/
  CMPS12.write(0x20);  // Request and read 16 bit angle
  while (CMPS12.available() < 6);
  high_byte = CMPS12.read();
  low_byte = CMPS12.read();
  regis_16b[3] = high_byte;      // X ACELL
  regis_16b[3] <<= 8;
  regis_16b[3] += low_byte;

  high_byte = CMPS12.read();
  low_byte = CMPS12.read();
  regis_16b[4] = high_byte;      // Y ACELL
  regis_16b[4] <<= 8;
  regis_16b[4] += low_byte;

  high_byte = CMPS12.read();
  low_byte = CMPS12.read();
  regis_16b[5] = high_byte;      // Z ACELL
  regis_16b[5] <<= 8;
  regis_16b[5] += low_byte;

  getAngles();

  sprintf(angles, "{'device':'DEVCAVALO','data':[{'name':'Costas_Frente_Tras', 'data':'%f'},{'name':'Costas_Esq_Dir', 'data':'%f'},{'name':'Rotacao', 'data':'%d' }], 'count':'%d'}", Ang_AcX, Ang_AcZ, regis_16b[0] / 10, count);

  //put request
  //arguments server ip address, default port, resource name, payload, payloadlength
  int msgid = coap.put(ip, port, "devicepet", angles, strlen(angles));

  state = coap.loop();

  count++;
  delay(100);
}

void getAngles() {
  Ang_AcX = (180 / PI) * atan(regis_16b[3] / sqrt(pow(regis_16b[5], 2) + pow(regis_16b[4], 2)));
  Ang_AcZ = (180 / PI) * atan(regis_16b[4] / sqrt(pow(regis_16b[5], 2) + pow(regis_16b[3], 2)));
  //Ang_Rot = 180 - atan2(regis_16b[0], regis_16b[1]) * 180 / PI;
}
