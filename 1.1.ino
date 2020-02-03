#include <SPI.h>
#include <Ethernet.h>
#include <Wire.h>
String data;
String tstr;

byte statusLed    = 13;

byte sensorInterrupt1 = 0;
byte sensorPin1       = 2;

byte sensorInterrupt2 = 1;
byte sensorPin2       = 3;


float calibrationFactor = 4.5;

volatile byte pulseCount1;  
float flowRate1;
unsigned int flowMilliLitres1;
unsigned long totalMilliLitres1;
unsigned long oldTime1;

volatile byte pulseCount2; 
float flowRate2;
unsigned int flowMilliLitres2;
unsigned long totalMilliLitres2;
unsigned long oldTime2;


byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x01 };   //physical mac address
byte ip[] = { 192, 168, 1, 13};                      // ip in lan (that's what you need to use in your browser. ("192.168.1.178")
byte gateway[] = { 192, 168, 1, 5};                   // internet access via router=la ip cua PC ban
byte subnet[] = { 255, 255, 255, 0 };
EthernetServer server(80);                    // Cong truy cap internet cua XAMP(mac dinh la 80 ban co the doi)
EthernetClient client;                       //server port
String readString;

void setup() {
  Serial.begin(9600);
   
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  
  pinMode(sensorPin1, INPUT);
  digitalWrite(sensorPin1, HIGH);

  pulseCount1        = 0;
  flowRate1          = 0.0;
  flowMilliLitres1   = 0;
  totalMilliLitres1  = 0;
  oldTime1           = 0;
  attachInterrupt(sensorInterrupt1, pulseCount1, FALLING);

  pinMode(sensorPin2, INPUT);
  digitalWrite(sensorPin2, HIGH);
  
  pulseCount2        = 0;
  flowRate2          = 0.0;
  flowMilliLitres2   = 0;
  totalMilliLitres2  = 0;
  oldTime2           = 0;

  attachInterrupt(sensorInterrupt2, pulseCount2, FALLING);
}
void loop() {
  delay(2000);
 
   if((millis() - oldTime1) > 1000)    
  { 
    detachInterrupt(sensorInterrupt1);

    flowRate1 = ((1000.0 / (millis() - oldTime1)) * pulseCount1 ) / calibrationFactor;
    oldTime1 = millis();
    flowMilliLitres1 = (flowRate1 / 60) * 1000;
    totalMilliLitres1 += flowMilliLitres1;
      
    unsigned int frac;
   
    Serial.print("Flow rate1: ");
    Serial.print(int(flowRate1)); 
    Serial.print("L/min");
    Serial.print("\n");     
    Serial.print("Output Liquid Quantity1: ");        
    Serial.print(totalMilliLitres1);
    Serial.println("mL"); 
    Serial.print("\t");      
    Serial.print(totalMilliLitres1/1000);
    Serial.print("L");
    Serial.print("\n");
    Serial.print("\n");
   
  
    pulseCount1 = 0;
 
    attachInterrupt(sensorInterrupt1, pulseCounter1, FALLING);

  }
  
  delay (1000);
  
  if((millis() - oldTime2) > 1000)   
  { 
    detachInterrupt(sensorInterrupt2);
   
    flowRate2 = ((1000.0 / (millis() - oldTime2)) * pulseCount2) / calibrationFactor;
    oldTime2 = millis();
    flowMilliLitres2 = (flowRate2 / 60) * 1000;
    totalMilliLitres2 += flowMilliLitres2;
      
    unsigned int frac;

    Serial.print("Flow rate2: ");
    Serial.print(int(flowRate2));  
    Serial.print("L/min");
    Serial.print("\n");     
    Serial.print("Output Liquid Quantity2: ");        
    Serial.print(totalMilliLitres2);
    Serial.println("mL"); 
    Serial.print("\n");      
    Serial.print(totalMilliLitres2/1000);
    Serial.print("L");
    Serial.print("\n");
  
    pulseCount2 = 0;

    attachInterrupt(sensorInterrupt2, pulseCounter2, FALLING);
    delay(1000);

  }
}
 
void pulseCounter1()
{
  pulseCount1++;
  delay(1000);
}

 
void pulseCounter2()
{
  pulseCount2++;
  delay (1000);
}


 sendTotalMilliLitres1(totalMilliLitres1);
 sendTotalMilliLitres2(totalMilliLitres2);
  
  
  // khi client da ket noi thanh cong va san sang thi thuc hien doc tin hieu tu server gui ve
  //va in ra kien tra thanh cong- sau do thuc hien ham bat/tat thiet bi

  
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        tstr += c;
        if (c == '\n') {
          break;
        }
        Serial.println("kiem tra thanh cong");
      }
    }
    parseGet(tstr);
    tstr = "";

    delay(1);
  }
  client.stop();

//Ham gui luu luong vao bang "mililit1" trong Databse
void sendTotalMilliLitres1(float totalMilliLitres1) {
  if (client.connect("192, 168, 1, 5", 80)) { // REPLACE WITH YOUR SERVER ADDRESS
    String request = "GET /arduino/mo/includes/write_mililit1.php?mililit1=";
    request += String(totalMilliLitres1);
    request += " HTTP/1.0";
    client.println(request);
    Serial.println("Gui luu luong 1 thanh cong");
    client.println("Host: 192, 168, 1, 5"); // SERVER ADDRESS HERE TOO
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.println(data.length());
    client.println();
    client.print(data);
  }

  if (client.connected()) {
    client.stop();  // DISCONNECT FROM THE SERVER
  }
}
//Ham gui luu luong vao bang "mililit2" trong Databse

void sendTotalMilliLitres2(float totalMilliLitres2) {
  if (client.connect("192, 168, 1, 5", 80)) { // REPLACE WITH YOUR SERVER ADDRESS
    String request = "GET /arduino/hum/includes/write_mililit2.php?mililit2=";
    request += String(totalMilliLitres2);
    request += " HTTP/1.0";
    client.println(request);
    Serial.println("Gui luu luong 2 thanh cong");
    // Serial.println(request);
    client.println("Host: 192, 168, 1, 4"); // SERVER ADDRESS HERE TOO
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.println(data.length());
    client.println();
    client.print(data);
  }

  if (client.connected()) {
    client.stop();  // DISCONNECT FROM THE SERVER
  }
}
