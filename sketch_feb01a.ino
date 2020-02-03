#include<SPI.h>            // thư viện để kết nối shield
#include<Ethernet.h>       // thư viện cho dự án

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };   // địa chỉ MAC( địa chỉ phần cứng của shield ethernet), cứ để nguyên
IPAddress ip(192, 168, 1, 9);                         // địa chỉ IP của shield, có thể thay đổi tùy ý.

EthernetServer server = EthernetServer(80);           // giá trị port là 80
String readString;                                    // khai báo chuỗi có tên readString
int AN1, AN2, AN3, AN4, AN5;                          // khai báo biến để đọc giá trị analog, đọc xuống dưới sẽ hiểu
void setup() {

  Serial.begin(9600);                     
  while (!Serial) {
    ;
  }
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);

  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);

  Ethernet.begin(mac, ip);                            // viết ít thôi nhé, cái này cober đọc sẽ tự hiểu
  server.begin();
  Serial.print("Dia chi server la: ");
  Serial.println(Ethernet.localIP());
  Serial.println("Ban da ket noi thanh cong mang ethernet...");

}

void loop() {
  
  AN1 = analogRead(A1);                             
  AN2 = analogRead(A2);
  AN3 = analogRead(A3);
  AN4 = analogRead(A4);
  AN5 = analogRead(A5);
  
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
          client.println("<TITLE>He Thong Dieu Khien Thiet Bi Bang Arduino Uno - Ethernet Shield Arduino</TITLE>");
          // tên tiêu đề ở mác của tab trình duyệt đó, cái này ít quan trọng. có thể thay đổi tùy ý
          client.println("</HEAD>");
          client.println("<BODY>");
          client.println("<CENTER>");
          client.println("<h1>HE THONG DIEU KHIEN THIET BI QUA INTERNET BANG ARDUINO UNO VA MODULE ETHERNET SHIELD</h1>");
          client.println("<h3>************************************************************</h3>");
          client.println("<h3>DIEU KHIEN BANG NUT LENH</h3>");
          //===========================================================================
          client.println("<br/>");                                        // lệnh để cách xuống dòng   
          client.println("<a href='button1on'>MO THIET BI 1</a>");        // tạo một nút (button) trên web( nút bật thiết bị 1), khi bấm nút tức là ta đã gửi 1 lệnh về shield( arduino sẽ sử lí tín hiệu này)
          client.println("<br/>");
          client.println("<a href='button1off'>TAT THIET BI 1</a>");      // nút dùng để tắt thiết bị 1
          client.println("<br/>");

          client.println("<br/>");
          client.println("<a href='button2on'>MO THIET BI 2</a>");       // tương tự nút bật trên nhưng là tb 2
          client.println("<br/>");
          client.println("<a href='button2off'>TAT THIET BI 2</a>");     // tương tự nút tắt trên nhưng là thiết bị 2
          client.println("<br/>");

          client.println("<br/>");
          client.println("<a href='button3on'>MO THIET BI 3</a>");       // tương tự nút bật trên nhưng là tb 3
          client.println("<br/>");
          client.println("<a href='button3off'>TAT THIET BI 3</a>");      // tương tự nút tắt trên nhưng là thiết bị 3
          client.println("<br/>");

          client.println("<h3>***********************************************************</h3>");   // dấu sao để ngăn cách
          client.println("<h3>**********************************************</h3>");
          client.println("<h3>******************************</h3>");

          client.println("<br/>");
          client.print(" Gia tri cong analog 1 la:    ");                // in ra màn hình trình duyệt thông báo " Gia tri cong analog 1 la:    "
          client.print( AN1);                                            // in ra giá trị analog 
          client.println("<br/>");                                       
          client.println("<br/>");                                       // xuống 2 dòng
          client.print(" Gia tri cong analog 2 la:    ");                // tương tự như trên
          client.print( AN2);
          client.println("<br/>");
          client.println("<br/>");
          client.print(" Gia tri cong analog 3 la:    ");
          client.print( AN3);
          client.println("<br/>");
          client.println("<br/>");
          client.print(" Gia tri cong analog 4 la:    ");
          client.print( AN4);
          client.println("<br/>");
          client.println("<br/>");
          client.print(" Gia tri cong analog 5 la:    ");
          client.print( AN5);
          
          client.println("<br/>");
          client.println("</BODY>");
          client.println("</HTML>");
          delay(1);
          //Stop Client:
          client.stop();
          Serial.println("client disconnected");
           
          if (readString.indexOf("button1on") > 0) {     // nút bấm thiết bị 1( button1) được bấm thì
            digitalWrite(9, HIGH);                       // xuất ra tín hiệu điện dương ra cổng 9
          }                                              // các phần dưới hiểu tương tự
     
          if (readString.indexOf("button1off") > 0) {
            digitalWrite(9 , LOW);
          }
          
          if (readString.indexOf("button2on") > 0) {
            digitalWrite(8, HIGH);
          }
          
          if (readString.indexOf("button2off") > 0) {
            digitalWrite(8 , LOW);
          }
         
          if (readString.indexOf("button3on") > 0) {
            digitalWrite(7, HIGH);
          }
          
          if (readString.indexOf("button3off") > 0) {
            digitalWrite(7 , LOW);
          }
          
          readString = "";                       // đặt lại chuỗi thành không có giá trị nào
        }
        
      } 
    } 
  }  
  

}
