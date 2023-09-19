#include<String.h>
#include<cstring>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <WebServer.h>
#include "html.h"
#include "html_web.h"
#include "EEPROM.h"
#include <Ticker.h>
#include <time.h>
#include <FirebaseESP32.h>


#define LED 2
#define BUTTON 0
#define LENGTH(x) (strlen(x) + 1)
#define EEPROM_SIZE 300 // 300bytes eeprom
#define FIREBASE_HOST "https://node-red-2fe69-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "PpFJ4Ni3kznPRO047QPMqefv1vfKS8Cd0kIuvlLm"

WebServer server(80);

Ticker ticker;
FirebaseData fbdb;
String id_sta,pass_sta;
bool done_bit = 0; // 0 cho not done , 1 cho done
String ledstate = "OFF";
String JsonString ="";
int a[8] ;

//watch dog  timer --------------------------
// #define WDT_TIMEOUT  60 
// hw_timer_t * WDT = NULL;
// unsigned long RUN_WDT = 0;
// void IRAM_ATTR WDT_interrupt()
// {
//   Serial.println("Reset ESP");
//   delay(300);
//   ESP.restart();
// }


void SendWebPage_config();
void SendWebPage_web();
void GetWifi();
void return_wifi();
void write_eeprom(const char* toStore, int startAddr);
String read_eeprom(int startAddr);

void readFBString();
bool change_mode();
void ConfigMode();
void ButtonClicked();
void UpdateState();
void OnOff();
void tick();
void x2b() ;
int x2i() ;
void GetData();
void setup(){
  EEPROM.begin(EEPROM_SIZE);
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT);
  Serial.begin(115200);
  Serial.println(read_eeprom(200));

  // WDT = timerBegin(1,80,true);
  // timerAttachInterrupt(WDT,&WDT_interrupt,true);
  // timerAlarmWrite(WDT,WDT_TIMEOUT*1000000,false);
  // timerAlarmEnable(WDT);
  
  if (read_eeprom(200)=="0") // CONFIG MODE
  {
    
    Serial.println(" CONFIG MODE");
    ticker.attach(0.15,tick);
    if (!WiFi.softAP("ESP32", "12345678")) {
    log_e("Soft AP creation failed.");
    while(1);
    }
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);

    server.on("/",SendWebPage_config);
    server.on("/Input",GetWifi);
    server.begin();
  }

  else if (read_eeprom(200)=="1") // WEBSERVER MODE
  {
    Serial.println(" WEB Server MODE");
    WiFi.disconnect();
    delay(1000);
    ticker.attach(0.75,tick);
    //Read SSID stored at address 0
    id_sta = read_eeprom(0);
    Serial.print("SSID = ");
    Serial.println(id_sta);
    // Read Password stored at address 40
    pass_sta = read_eeprom(70);
    Serial.print("PASS = ");
    Serial.println(pass_sta);
    WiFi.begin(id_sta.c_str(), pass_sta.c_str());
    delay(300);
    int i=0;
    // x2b("00FF");
    while (i<=41)
    {
      if (WiFi.status() != WL_CONNECTED){
      delay(750);
      Serial.print(".");
      i++;
      }
      if (WiFi.status() == WL_CONNECTED){
        Serial.println("");
        Serial.println("WiFi connected!");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        //firebase connect
        Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
        Firebase.reconnectWiFi(true);
        break;
      }
      if(WiFi.status() != WL_CONNECTED && i==41){
        Serial.println("Connection Failed !");
      }
    }

    
    delay(100);

    server.on("/",SendWebPage_web); //khi nhận request có đường dẫn trên thì esp sẽ thực hiện hàm tương ứng
    server.on("/Update",[]{UpdateState();});
    server.on("/Button",ButtonClicked);
    server.on("/Input",GetData);
    server.begin();

  }

}


void loop()
{
  server.handleClient();
  if(change_mode())
  {
    ConfigMode();
  }

  // if(millis()-RUN_WDT>1000)
  // {
  //   RUN_WDT = millis();
  //   timerWrite(WDT,0);
  // }

}

void readFBString(){
  if(Firebase.getString(fbdb,"state/led")) {
    Serial.print("String data: ");
    Serial.println(fbdb.stringData());
  }
}

void x2b() 
{
  int x = 0;
  char arr[5];
  if(Firebase.getString(fbdb,"state/led"))  {
    fbdb.stringData().toCharArray(arr,5);
  }
  for(int i=0;i<5;i++) {
    char c = arr[i];
    if (c >= '0' && c <= '9') {
      x *= 16;
      x += c - '0'; 
    }
    else if (c >= 'A' && c <= 'F') {
      x *= 16;
      x += (c - 'A') + 10; 
    }
    else break;
  }

	for (int i=0;i<8;i++)
	{
		a[i] = x%2;
		x=x/2;
	}


}

int x2i() 
{
  int x = 0;
  char arr[5];
  if(Firebase.getString(fbdb,"/data"))  {
    fbdb.stringData().toCharArray(arr,5);
  }
  for(int i=0;i<5;i++) {
    char c = arr[i];
    if (c >= '0' && c <= '9') {
      x *= 16;
      x += c - '0'; 
    }
    else if (c >= 'A' && c <= 'F') {
      x *= 16;
      x += (c - 'A') + 10; 
    }
    else break;
  }
  return x;
}




void GetData()
{
  String data =  server.arg("Input"); // lấy giá trị của thuộc tính được đường dẫn đuôi Input gửi đến esp
  Firebase.setString(fbdb,"/data_received",data);
  server.send(200,"text/html",html);
}


void ButtonClicked()
{
  ticker.detach();
  String button =  server.arg("Button"); // lấy giá trị của thuộc tính được đường dẫn đuôi Button gửi đến esp
  OnOff();
  server.send(200,"text/html",html_web);
  
}

void OnOff()
{
  //Y0
  if (server.arg("Button").indexOf("A0B")>=0) // indexof  trả về vị trí của phân tử trong chuỗi "A1B". Nếu "A1B" có trong chuỗi 
  {                                           // thì giá trị trả về sẽ >=0 , nếu không có se trả về giá trị -1
    Firebase.setString(fbdb,"/led","ON Y00");
    digitalWrite(LED,LOW);
    
    
  }
  else if (server.arg("Button").indexOf("C0D")>=0)
  {
    Firebase.setString(fbdb,"/led","OFF Y00");
    digitalWrite(LED,HIGH);
  }
  
  //Y1
  if (server.arg("Button").indexOf("A1B")>=0) // indexof  trả về vị trí của phân tử trong chuỗi "A1B". Nếu "A1B" có trong chuỗi 
  {                                           // thì giá trị trả về sẽ >=0 , nếu không có se trả về giá trị -1
    Firebase.setString(fbdb,"/led","ON Y01");
    digitalWrite(LED,LOW);
    
    
  }
  else if (server.arg("Button").indexOf("C1D")>=0)
  {
    Firebase.setString(fbdb,"/led","OFF Y01");
    digitalWrite(LED,HIGH);
  }
  
  //Y2
  if (server.arg("Button").indexOf("A2B")>=0) // indexof  trả về vị trí của phân tử trong chuỗi "A1B". Nếu "A1B" có trong chuỗi 
  {                                           // thì giá trị trả về sẽ >=0 , nếu không có se trả về giá trị -1
    Firebase.setString(fbdb,"/led","ON Y02");
    digitalWrite(LED,LOW);
    
    
  }
  else if (server.arg("Button").indexOf("C2D")>=0)
  {
    Firebase.setString(fbdb,"/led","OFF Y02");
    digitalWrite(LED,HIGH);
  }
  
  //Y3
  if (server.arg("Button").indexOf("A3B")>=0) // indexof  trả về vị trí của phân tử trong chuỗi "A1B". Nếu "A1B" có trong chuỗi 
  {                                           // thì giá trị trả về sẽ >=0 , nếu không có se trả về giá trị -1
    Firebase.setString(fbdb,"/led","ON Y03");
    digitalWrite(LED,LOW);
    
    
  }
  else if (server.arg("Button").indexOf("C3D")>=0)
  {
    Firebase.setString(fbdb,"/led","OFF Y03");
    digitalWrite(LED,HIGH);
  }
  
  //Y4
  if (server.arg("Button").indexOf("A4B")>=0) // indexof  trả về vị trí của phân tử trong chuỗi "A1B". Nếu "A1B" có trong chuỗi 
  {                                           // thì giá trị trả về sẽ >=0 , nếu không có se trả về giá trị -1
    Firebase.setString(fbdb,"/led","ON Y04");
    digitalWrite(LED,LOW);
    
    
  }
  else if (server.arg("Button").indexOf("C4D")>=0)
  {
    Firebase.setString(fbdb,"/led","OFF Y04");
    digitalWrite(LED,HIGH);
  }
  
  //Y5
  if (server.arg("Button").indexOf("A5B")>=0) // indexof  trả về vị trí của phân tử trong chuỗi "A1B". Nếu "A1B" có trong chuỗi 
  {                                           // thì giá trị trả về sẽ >=0 , nếu không có se trả về giá trị -1
    Firebase.setString(fbdb,"/led","ON Y05");
    digitalWrite(LED,LOW);
    
    
  }
  else if (server.arg("Button").indexOf("C5D")>=0)
  {
    Firebase.setString(fbdb,"/led","OFF Y05");
    digitalWrite(LED,HIGH);
  }
  
  //Y6
  if (server.arg("Button").indexOf("A6B")>=0) // indexof  trả về vị trí của phân tử trong chuỗi "A1B". Nếu "A1B" có trong chuỗi 
  {                                           // thì giá trị trả về sẽ >=0 , nếu không có se trả về giá trị -1
    Firebase.setString(fbdb,"/led","ON Y06");
    digitalWrite(LED,LOW);
    
    
  }
  else if (server.arg("Button").indexOf("C6D")>=0)
  {
    Firebase.setString(fbdb,"/led","OFF Y06");
    digitalWrite(LED,HIGH);
  }
  
  //Y7
  if (server.arg("Button").indexOf("A7B")>=0) // indexof  trả về vị trí của phân tử trong chuỗi "A1B". Nếu "A1B" có trong chuỗi 
  {                                           // thì giá trị trả về sẽ >=0 , nếu không có se trả về giá trị -1
    Firebase.setString(fbdb,"/led","ON Y07");
    digitalWrite(LED,LOW);
    
    
  }
  else if (server.arg("Button").indexOf("C7D")>=0)
  {
    Firebase.setString(fbdb,"/led","OFF Y07");
    digitalWrite(LED,HIGH);
  }
  
}

void DataJson(String Led_State, int y0, int y1, int y2, int y3, int y4, int y5, int y6, int y7){
  JsonString = "{\"led_stt\":\"" + String(Led_State) + "\"," +
               "\"y0\":\"" + String(y0) + "\"," +                  
               "\"y1\":\"" + String(y1) + "\"," +                  
               "\"y2\":\"" + String(y2) + "\"," +                  
               "\"y3\":\"" + String(y3) + "\"," +                  
               "\"y4\":\"" + String(y4) + "\"," +                  
               "\"y5\":\"" + String(y5) + "\"," +                  
               "\"y6\":\"" + String(y6) + "\"," +                  
               "\"y7\":\"" + String(y7) + "\""  + "}";
}

void UpdateState(){

  // readFBString();
  x2b();
  
  //send du lieu dang json
  DataJson(String(x2i()),a[0],a[1],a[2],a[3],a[4],a[5],a[6],a[7]);
  server.send(200,"text/html",String(JsonString));//gui response lai cho webserver
  
}

void ConfigMode(){
  Serial.println("CONFIG MODE!!");
  if(read_eeprom(200) == "1")
  {
    write_eeprom("0",200);
    Serial.println("Going to CONFIG MODE");
    delay(1000);
    ESP.restart();
  }
  else if(read_eeprom(200) == "0" && done_bit ==1)
  {
    write_eeprom("1",200);
    Serial.println("Going to WEB MODE");
    delay(1000);
    ESP.restart();
  }

}

bool change_mode(){
  static int lastpress = 0;
  if(millis()- lastpress >=3000 && digitalRead(BUTTON)==0)
  {return true;}
  else if (digitalRead(BUTTON)==1)
  {lastpress = millis();}
  return false;
}

void GetWifi(){
  String wf =  server.arg("Input"); // lấy giá trị của thuộc tính được đường dẫn đuôi Input gửi đến esp
  return_wifi(wf);
  Serial.println(wf);

  server.send(200,"text/html",html);
}

void SendWebPage_config(){
  server.send(200,"text/html",html);
}

void SendWebPage_web(){
  server.send(200,"text/html",html_web);
}
void return_wifi(String str) {
  String id_sta,pass_sta;
  for (int i=0;i<str.length();i++)
  {
    if(str[i]=='-')
    {
      for (int j=0;j<i;j++) id_sta += str[j];
      for (int k=i+1;k<str.length();k++) pass_sta += str[k] ;
    }
  }
  write_eeprom(id_sta.c_str(),0);
  Serial.print("SSID: ");
  Serial.println(read_eeprom(0));
  write_eeprom(pass_sta.c_str(),70);
  Serial.print("PASS: ");
  Serial.println(read_eeprom(70));

  //done bit 
  done_bit = 1;
}
//tick--------------------------------------
void tick(){

  int state = digitalRead(LED);
  digitalWrite(LED,!state);
}
//EEPROM------------------------------------
void write_eeprom(const char* toStore, int startAddr) {
  int i = 0;
  for (; i < LENGTH(toStore); i++) {
    EEPROM.write(startAddr + i, toStore[i]);
  }
  EEPROM.write(startAddr + i, '\0');
  EEPROM.commit();
}

String read_eeprom(int startAddr) {
  char in[60]; // char array of size 60 for reading the stored data 
  int i = 0;
  for (; i < 60; i++) {
    in[i] = EEPROM.read(startAddr + i);
  }
  return String(in);
}

