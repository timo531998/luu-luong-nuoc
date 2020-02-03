#include<SPI.h>            // thư viện để kết nối shield
#include<Ethernet.h>       // thư viện cho dự án
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };   // địa chỉ MAC( địa chỉ phần cứng của shield ethernet), cứ để nguyên
IPAddress ip(192, 168, 1, 9);                         // địa chỉ IP của shield, có thể thay đổi tùy ý.

EthernetServer server = EthernetServer(80);           // giá trị port là 80
String readString;                   

byte statusLed    = 13;

byte sensorInterrupt1 = 0;
byte sensorPin1       = 2;

byte sensorInterrupt2 = 1;
byte sensorPin2       = 3;


float calibrationFactor = 7.5;

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

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
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

  Ethernet.begin(mac, ip);                            // viết ít thôi nhé, cái này cober đọc sẽ tự hiểu
  server.begin();
  Serial.print("Dia chi server la: ");
  Serial.println(Ethernet.localIP());
  Serial.println("Ban da ket noi thanh cong mang ethernet...");
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
 
    attachInterrupt(sensorInterrupt1, pulseCount1, FALLING);
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

    attachInterrupt(sensorInterrupt2, pulseCount2, FALLING);
  }

   EthernetClient client = server.available();           
  delay(2000);
  
  if (client = true) { //if client:

    while (client.connected()) { 
      if (client.available()) {
        char c = client.read();               //Đọc char bởi yêu cầu HTTP
        
        if (readString.length() < 100) {     //Lưu trữ các ký tự vào chuỗi
          
          readString += c;
        }
        
        if (c == '\n') {
          Serial.println(readString);                //In ra serial monitor
          client.println("HTTP/1.1 200 OK");        //Tạo 1 trang mới
          client.println("Content-Type: text/html");
          client.println("Refresh: 5");             // lệnh để tự động tải lại trang, cứ 5 giây 1 lần tải lại.
          // vì giá trị analog thay đổi liên tục nên luôn phải tải lại, nếu kho có dòng này thì 
          // phải tải lại trang bằng cách kích chuột để đọc giá trị analog mới => rất mệt.
          client.println();
          client.println("<HTML>");             // câu lệnh của web để trình duyệt hiểu mà hiển thị các phần đầu, thân, cuối,...mình chưa học lập trình web nên chưa hiểu lắm
          client.println("<HEAD>");
          client.println("<TITLE>He Thong Do Luu Luong Nuoc Arduino Uno - Ethernet Shield Arduino</TITLE>");
          // tên tiêu đề ở mác của tab trình duyệt đó, cái này ít quan trọng. có thể thay đổi tùy ý
          client.println("</HEAD>");
          client.println("<BODY>");
          client.println("<CENTER>");
          client.println("<h1>He Thong Do Luu Luong Nuoc</h1>");
          client.println("<h3>************************************************************</h3>");
          
          client.println("<br/>");
          client.println("<br/>");
          client.print(" Gia tri cam bien 1 la:    ");                // in ra màn hình trình duyệt thông báo " Gia tri cam bien 1 la:    "
          client.println("<br/>");
          client.print(flowRate1);
          client.println("<br/>");
          client.print(totalMilliLitres1);                                            // in ra giá trị analog 
          client.println("<br/>");                                       
          client.println("<br/>");                                       // xuống 2 dòng
          client.print(" Gia tri cong analog 2 la:    ");                // tương tự như trên
          client.println("<br/>");
          client.print(flowRate2);
          client.println("<br/>");
          client.print( totalMilliLitres2);
          client.println("<br/>");
          client.println("</BODY>");
          client.println("</HTML>");
          delay(1);
          //Stop Client:
          client.stop();
          Serial.println("client disconnected");
        
          readString = "";                       // đặt lại chuỗi thành không có giá trị nào
        }
        
      } 
    } 
  }  
  
  }
 void pulseCounter1()
{
  pulseCount1++;
}
void pulseCounter2()
{
  pulseCount2++;
}
