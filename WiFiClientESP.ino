#include <ESP8266WiFi.h>


// Сюда название вашей Wi-FI сети и пароль!!!
const char* ssid = "Keenetic-4272";
const char* password = "jPH34VN5";

// Поднимаем веб сервер
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);

  // подготавливаем пин для реле GPIO2
  pinMode(4, OUTPUT);
  digitalWrite(4, 1);
  pinMode(5, OUTPUT);
  digitalWrite(5, 1);
  pinMode(14, OUTPUT);
  digitalWrite(14, 1);
  pinMode(12, OUTPUT);
  digitalWrite(12, 1);
 
  // подклюсаемя к сети WI-FI
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");

  // печатаем в монитор порта полученный IP адрес
  Serial.println(WiFi.localIP());
}

void loop() {
  // Смотрим, подключен ли к нам клиент
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  // если клиент подключился печатаем
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
 
  // читаем строку ссылки
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
 
  // присваиваем значение переменной val в зависимости от ссылки по которой перешли
  int val;
  int gpio;
  if (req.indexOf("/gpio4/0") != -1)
    {val = 0;
    gpio = 4;}
  else if (req.indexOf("/gpio4/1") != -1)
   { val = 1;
    gpio = 4;}
  else if (req.indexOf("/gpio5/0") != -1)
   { val = 0;
    gpio = 5;}
  else if (req.indexOf("/gpio5/1") != -1)
    {val = 1;
    gpio = 5;}
  else if (req.indexOf("/gpio12/0") != -1)
    {val = 0;
    gpio =12;}
  else if (req.indexOf("/gpio12/1") != -1)
    {val = 1;
    gpio =12;}
   else if (req.indexOf("/gpio14/0") != -1)
    {val = 0;
    gpio =14;}
  else if (req.indexOf("/gpio14/1") != -1)
    {val = 1;
    gpio =14;}
  else {
    Serial.println("invalid request");
    client.stop();
    return;
  }
  // работаем с GPIO4 и переменной val
  digitalWrite(gpio, val);
 
  client.flush();

  // выводим html код
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nGPIO is now ";
  s += (val)?"OFF":"ON";
  s += "</html>\n";

  // что там с клиентом..
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");

}
  