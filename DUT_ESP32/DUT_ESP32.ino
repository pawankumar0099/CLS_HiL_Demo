//PIR Emulation FW for ESP32
#include <ESP.h>

#include <WiFiClientSecure.h>
#include <PubSubClient.h> 
#include <ArduinoJson.h>

//#define DEBUG

//#ifdef DEBUG
//#define DEBUG Serial2
//#endif

const char* ssid = "pawan";
const char* password = "12457812";

const char* awsEndpoint = "a3a9zeyitx6zyi-ats.iot.us-east-1.amazonaws.com";   //Get your own AWS Endpoint from AWSIoT Settings
const char* ThingName = "ESP32_DUT_TEST";  //Replace this with your Thing Name created in AWSIoT
const char* SubTopic = "inTopic";  //Replace this Subscribe topic wiht your own Subscribe Topic.
const char* PubTopic = "outTopic"; //Replace this Publish topic with your own Publish topic. 

String LED_Status;
int LED_Intensity = 0;
// Replace these with your desired pins
const int ledPin1 = 2;
const int ledPin2 = 4;

// xxxxxxxxxx-certificate.pem.crt
const char* certificate_pem_crt = \
R"EOF(-----BEGIN CERTIFICATE-----
MIIDWjCCAkKgAwIBAgIVAI53LAa7nA6siqTu53kMRt+PWdoWMA0GCSqGSIb3DQEB
CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t
IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yNDAyMjIxMDM2
NDlaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh
dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDLNVQYhDAiNzt2hqfX
ikhjEvcJMNBrUSMMC6r8cLotPGPTPsoC6GTMiWQ8IwE8idxIeAttqKB0+RgvKwEe
6c9MMOcFZni76WzBx5S5z9jk+v1MeYd709bUCmfNjj5Z7CbweSWDMul2y3A5Nz1k
qGX8zRW/FgJDgb/A5I8D/cZ+UmYu1Q36UL/BG5sG52NoNgfBH7bVlX96cXk653ii
EHDoPWSFaH0pw3pDJ39g2j6dXpfiUPkA8BKHKFgad3rzpPNkuH6+ptcfH7hOk/G+
+y0UOrOlSq6dgnsXfii14OktQSiPNJDzj1xNiQVyd+LpnA4amXuisDZX3hnRq7bC
BjZBAgMBAAGjYDBeMB8GA1UdIwQYMBaAFD9VzrVI7R7an4vyR+xgF1P9TvnpMB0G
A1UdDgQWBBRkMzGVoBipQMxxLgZ/WE7mq4soxTAMBgNVHRMBAf8EAjAAMA4GA1Ud
DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAIriVNsiffEj6LkTRhIL/VKpd
YWlDExG0YTho8n85OKr8z4y0ox/1LTDJqgNBxRAvo6ak9hwFvSMSgJpaxgFldgcp
EzhJBrBM79m3vyIVKOU3YZ9TgVeuQrB8/KxnSkst0nvuPINQDrzFE6MWhpxxplyP
a9PgUrl1aq7/pFOfzbc5m6J2S64dznSC6Fk8YGJImCauXsRcPs3xk/FsKi10oDuX
2tcEUmLYhy6wr4cm7h4zPRxzPBSzQxjtORRitecewU3QEcsEweVcgigL3gqr0ARy
P1/suffjm71HQpwDqXXR9HSpukzOH/o96uVUYLHVDjMFO9dYCW5/1zeWHomPrQ==
-----END CERTIFICATE-----)EOF";

// xxxxxxxxxx-private.pem.key
const char* private_pem_key = \
R"EOF(-----BEGIN RSA PRIVATE KEY-----
MIIEpAIBAAKCAQEAyzVUGIQwIjc7doan14pIYxL3CTDQa1EjDAuq/HC6LTxj0z7K
AuhkzIlkPCMBPIncSHgLbaigdPkYLysBHunPTDDnBWZ4u+lswceUuc/Y5Pr9THmH
e9PW1ApnzY4+Wewm8HklgzLpdstwOTc9ZKhl/M0VvxYCQ4G/wOSPA/3GflJmLtUN
+lC/wRubBudjaDYHwR+21ZV/enF5Oud4ohBw6D1khWh9KcN6Qyd/YNo+nV6X4lD5
APAShyhYGnd686TzZLh+vqbXHx+4TpPxvvstFDqzpUqunYJ7F34oteDpLUEojzSQ
849cTYkFcnfi6ZwOGpl7orA2V94Z0au2wgY2QQIDAQABAoIBAQCXGs+u71TPjJyl
9VmHmGddbLPP3MGOm6XhBigeUUDP9HESm0UiRmb2BVBIu4rm1mi8Zq+w2kx+zt+h
62uI7jncoRyY0msC6H+b1OoOBbV2CAEkxdXuSUcdI6OuoZBR053dAWjYl5otYPfR
Q/deCy7Tfjkmf5otUqUhLZP/Vs0sDov8g4TQJvBWC2d9k+b9oeuSnnuM+I9ECoA+
0RcBrCYktfvi6Fexl7velQ2yVILaqWtJzR23Z+4ZSK8yGv1yWcPXqI2SV7QwVwso
+JFUeF5gDmhn0ApQSczIqwv4HoebEWiHuid6JC+lZ8gXnkdDlNu5FZZcFVgcU2BH
aR3uJfpxAoGBAPEcETOo/v/ns6eKjMFsAcEK6jVx5G52JHP8kBlFO7p7fxVX7WDf
Chavgxk51EBy+yeHGioyOj6Xl6AujbP8KHqpHkMdfF+shxHNDha58SiHOKiXVRVR
Bi/94RBnY7zuJpexir8EafnTih1W3fZAXJnWYh6sBuWIVG+lCKiIvD+lAoGBANfC
CwUwpsodKX55BYDIXfAFpBvdiCS6hFAxetu0f9LAFAAM8nlPInGNuOqOHrPX9zQ5
BlL6cxWb7n6YbKqI/GDwclxPShZpcbOB4FK2MQSIZvKkY/Sgr+TyDwAH9UFVqRsh
4yANOiVmlqaYGR5koYyZZNTuPKjD66nlpkd4iBltAoGBANlmCUyfUG3FqYOjtY6h
qGM7SDqwWtwOwoeibvuaJWTb/ZZtFaiG8z3JO1f/gJYsFrFpa/ooF8RgQ2M3FqX2
8zjGTHcPmxlZIuC2F8H+HqgAZOH+scTqrb2QGOMFljTANyQH/aaGgFe2mY4iyThE
iDpdx1MIoZ9M8XVJRSlRQQvRAoGAJjjP7vlxcri70xXpbakRSyYUroMOTUNlkTdS
xn0xb8prQJ8KjYcBWRB8Q8EtmzqVJoE7p5OKs7dOBstF5OgjnaC7AbNiMpo3dE97
x+tSv7TSWAXbV0joICzpj13Zitt7+7yDGfh95oZIBfdH6+Rz4g6CmF4kDNm4c/6k
+GBX6/kCgYAOIqfXvQYD19Ogp9qxQoqZX0eNvAvcArHHBhi322eXlC6K4zroXwLM
EUnqsPnynYc2/4+8FKKHWPcmjbWFpBPWx9SZ7lejlE4Cr8iuaA9opNGm1+Oh03/V
ZJf3eG536tXAZZg8e9UrpHutAwmu/NmLqgTi+pOGY/Dxk8DZVHeB4g==
-----END RSA PRIVATE KEY-----)EOF";

/* root CA can be downloaded in:
  https://www.symantec.com/content/en/us/enterprise/verisign/roots/VeriSign-Class%203-Public-Primary-Certification-Authority-G5.pem
*/
const char* rootCA = \
R"EOF(-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----)EOF";

WiFiClientSecure wiFiClient;
void msgReceived(char* topic, byte* payload, unsigned int len);
PubSubClient pubSubClient(awsEndpoint, 8883, msgReceived, wiFiClient); //PORT 8883/443 Only 


void setup() {
  // Set baud rate for both ESP32 serial and software serial
  Serial.begin(9600);
  
//  #ifdef DEBUG
//  DEBUG.begin(9600);
//  #endif
  
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);

  Serial.println("ESP32 AWS IoT Example");
  Serial.printf("SDK version: %s\n", ESP.getSdkVersion());

  Serial.print("Connecting to "); Serial.print(ssid);
  WiFi.begin(ssid, password);
  WiFi.waitForConnectResult();
  Serial.print(", WiFi connected, IP address: "); Serial.println(WiFi.localIP());

  wiFiClient.setCACert(rootCA);
  wiFiClient.setCertificate(certificate_pem_crt);
  wiFiClient.setPrivateKey(private_pem_key);
}

void Test_Case()
{
  if (Serial.available() > 0)
  {
    char data = Serial.read();

//#ifdef DEBUG
//    DEBUG.println("Data:");
//    DEBUG.print(data);
//#endif

    switch (data)
    {
      //Entering in Zone 1
      case 0x10:
        analogWrite(ledPin1, 255);
         LED_Status = "LED 1 ON";
         LED_Intensity = 255;
        break;

      //Leaving Zone 1: Means we first have to dim the ledPin1 to desending brigthness for 5 sec and then turn off
      case 0x00:
        analogWrite(ledPin1, 127);
        LED_Status = "LED 1 ON";
        LED_Intensity = 127;
        delay(2500);
        analogWrite(ledPin1, 63);
        LED_Status = "LED 1 ON";
        LED_Intensity = 63;
        delay(2500);
        analogWrite(ledPin1, 0);
        LED_Status = "LED 1 OFF";
        LED_Intensity = 0;
        break;

      //Entering in Zone 2
      case 0x01:
        analogWrite(ledPin2, 255);
        LED_Status = "LED 2 ON";
        LED_Intensity = 255;
        break;

      //Leaving Zone 2: Means we first have to dim the ledPin2 to desending brigthness for 5 sec and then turn off
      case 0x11:
        //Serial.println(data);
        analogWrite(ledPin2, 127);
        LED_Status = "LED 2 ON";
        LED_Intensity = 127;
        delay(2500);
        analogWrite(ledPin2, 63);
        LED_Status = "LED 2 ON";
        LED_Intensity = 63;
        delay(2500);
        analogWrite(ledPin2, 0);
        LED_Status = "LED 2 OFF";
        LED_Intensity = 0;
        break;

      case 0x19:  //25 degree temperature case
          analogWrite(ledPin1,255);
          analogWrite(ledPin2,255);          
        break;

      case 0x28: //40 degree
          analogWrite(ledPin1,173);
          analogWrite(ledPin2,173);
        break;

      case 0x3C:  //60 degree
          analogWrite(ledPin1,10);
          analogWrite(ledPin2,10);
        break;

      case 0x14: //-20 degree
          analogWrite(ledPin1,173);
          analogWrite(ledPin2,173);
        break;
        
      case 0x1E: //-30 degree
          analogWrite(ledPin1,10);
          analogWrite(ledPin2,10);
        break;

      //Reset case
      case 0x34:
        ESP.restart();
        break;
    
      default:
      
        break;
    }
  }
}

unsigned long lastPublish;
int msgCount;

void loop() {
//  #ifdef DEBUG
//  DEBUG.println("Inside the Loop");
//  #endif
  Test_Case();
  delay(200);
  pubSubCheckConnect();
  if (millis() - lastPublish > 1000) {  //Generating delay, One can minimise the delay by Decreasing the value (10000).

//------------------------------------------DATA----------------------------------------    

    float temperatureC = float(random(10,40));
    char buffer[5];
    String s = dtostrf(temperatureC, 1, 2, buffer);
    String temp= s;

    float humidp = float(random(20,90));
    String r = dtostrf(humidp, 1,2, buffer);
    String humid = r;

    String Device_ID = "ESP32_TS";

    //String Device_ID = String(random(0xffff),HEX);
    //String current = String(random(0,20));
    //String F_ID = String(random(0xffff),HEX);
    //String PostData="{\"f_id\":\"" + F_ID + "\",\"t\":\"" + temp + "\",\"i\":\"" + current + "\",\"c_id\":\"" + C_ID + "\"}"; //JSON Format

    //String PostData="{\"Device_ID\":\"" + Device_ID + "\",\"time\":\"" + time + "\",\"temp\":\"" + temp + "\",\"humid\":\"" + humid + "\"}"; //JSON Format

    StaticJsonDocument<200> doc;
    doc["Device_ID"] = Device_ID;
    doc["time"] = millis();
    doc["temp"] = temp;
    doc["LED Status"] = LED_Status; 
    doc["LED Intensity"] = LED_Intensity;
    char jsonBuffer[512];
    serializeJson(doc, jsonBuffer); // print to client

//-------------------------------------------DATA-----------------------------------------
  
    //String msg = String("Hello from ESP32: ") + ++msgCount;
    boolean rc = pubSubClient.publish(PubTopic, jsonBuffer); 
    Serial.print("Published, rc="); Serial.print( (rc ? "OK: " : "FAILED: ") );
    Serial.println(jsonBuffer);
    lastPublish = millis();
  }
}

void msgReceived(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received on "); Serial.print(topic); Serial.print(": ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void pubSubCheckConnect() {
  if ( ! pubSubClient.connected()) {
    Serial.print("PubSubClient connecting to: "); Serial.print(awsEndpoint);
    while ( ! pubSubClient.connected()) {
      Serial.print(".");
      pubSubClient.connect(ThingName);  //Replace with Your Thing name
      delay(1000);
    }
    Serial.println(" connected");
    pubSubClient.subscribe(SubTopic);
  }
  pubSubClient.loop();
}
