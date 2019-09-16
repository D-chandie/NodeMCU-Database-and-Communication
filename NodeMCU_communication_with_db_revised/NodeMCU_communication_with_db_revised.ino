#include <ESP8266WiFi.h>

const char* ssid = "B315_F9519";                  // Your wifi Name       
const char* password = "1GGRTA85GD4"; // Your wifi Password
const char * host2 = "technofarmers.000webhostapp.com"; 
const char * host = "192.168.254.122";        // IP Server

const int httpPort = 80;

const char* Commands;                       // The command variable that is sent to the server                     
const char* Commands1;
int button = 5;                             // push button is connected
bool btn_press = true;                      // The variable to detect the button has been pressed
int con = 0;                                // Variables for mode
int sensorPin = A0;    
int ledPin = 5;      
int sensorValue = 0; 

void setup() {
  // put your setup code here, to run once:
  pinMode(button, INPUT_PULLUP);            // initialize the pushbutton pin as an input:
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);                     // initialize serial:
  Serial.println("");
  Serial.println("Client-------------------------------");
  Serial.print("Connecting to Network");
  WiFi.mode(WIFI_STA);                      // Mode Station
  WiFi.begin(ssid, password);               // Matching the SSID and Password
  delay(1000);

  // Waiting to connect to wifi
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("Successfully Connecting");  
  Serial.println("Status : Connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("-------------------------------------");
  Serial.println("");
}

void loop() {
  // put your main code here, to run repeatedly:
    sensorValue = analogRead(sensorPin); 
    if(sensorValue <= 615)
    {
      Commands1 = "0mm";
      Commands = "LED_On";
      Serial.println(Commands);
      Serial.println(analogRead(sensorPin));
       send_commands();
    }
    else if (sensorValue>615 && sensorValue<=680)
     {
      Commands1 = "1mm";
      Commands = "LED_On";
      Serial.println(Commands);
      Serial.println(analogRead(sensorPin));
      send_commands();
     }
    else if (sensorValue>680 && sensorValue<=700)
     {
      Commands1 = "2mm";
      Commands = "LED_On";
      Serial.println(Commands);
      Serial.println(analogRead(sensorPin));
      send_commands();
     } 
    else if (sensorValue>700)
     {
      Commands1 = "3mm";
      Commands = "LED_Off";
      Serial.println(Commands);
      Serial.println(analogRead(sensorPin));
      send_commands();
     }
  delay(100);

  Serial.print("connecting to ");
  Serial.println(host2);
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host2, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  String url = "/api/insert.php?pinlevel=" + String(sensorValue)+ "&waterlevel="+ String(Commands1);
  Serial.print("Requesting URL: ");
  Serial.println(url);
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host2 + "\r\n" +
               "Connection: close\r\n\r\n");
  delay(500);
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  Serial.println();
  Serial.println("closing connection");
  delay(3000);
}

void send_commands(){
  Serial.println("Sending command...");
  Serial.println("Don't press the button for now...");
  Serial.println("");
  Serial.print("Connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(host, httpPort)) {
    Serial.println("Connection failed");
    return;
  }

  // We now create a URI for the request  
  Serial.print("Requesting URL : ");
  Serial.println(Commands);

  // This will send the request to the server
  client.print(String("GET ") + Commands + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: Close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {      
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  Serial.print("Server Reply = "); 
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  Serial.println("Now you can press the button ...");
  Serial.println("-------------------------------------");
  Serial.println("");
}
