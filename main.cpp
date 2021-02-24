#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define FIREBASE_HOST "" //--> URL address of your Firebase Realtime Database.
#define FIREBASE_AUTH "" //--> Your firebase database secret code.

#define ON_Board_LED 2  //--> Defining an On Board LED, used for indicators when the process of connecting to a wifi router

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//----------------------------------------SSID and Password of your WiFi router.
const char* ssid = ""; //--> Your wifi name or SSID.
const char* password = ""; //--> Your wifi password.
//----------------------------------------

String p = "Parafusadeira";

void setup() {
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 7);
  // Display static text
  display.println("Hello, world!");
  display.display(); 
  
  delay(500);
  
  WiFi.begin(ssid, password); //--> Connect to your WiFi router
  Serial.println("");
    
  pinMode(ON_Board_LED,OUTPUT); //--> On Board LED port Direction output
  digitalWrite(ON_Board_LED, HIGH); //--> Turn off Led On Board

  //----------------------------------------Wait for connection
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    //----------------------------------------Make the On Board Flashing LED on the process of connecting to the wifi router.
    digitalWrite(ON_Board_LED, LOW);
    delay(250);
    digitalWrite(ON_Board_LED, HIGH);
    delay(250);
    //----------------------------------------
  }
  //----------------------------------------
  digitalWrite(ON_Board_LED, HIGH); //--> Turn off the On Board LED when it is connected to the wifi router.
  //----------------------------------------If successfully connected to the wifi router, the IP Address that will be visited is displayed in the serial monitor.
  Serial.println("");
  Serial.print("Successfully connected to : ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  //----------------------------------------

  //----------------------------------------Firebase Realtime Database Configuration.
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  //----------------------------------------
}

void loop() {
  delay(5000);
  display.setCursor(0, 8);
  display.clearDisplay();
  display.println("Aguardando leitura...");
  display.display();

  delay(2000);
  display.clearDisplay();
  delay(500);
  display.setCursor(0, 8);
  display.println("Leitura Feita");
  display.display();
  delay(500);
  display.clearDisplay();
  delay(500);
  
  display.println("Informacoes do produto: ");
  display.println("DATA: " + Firebase.getString("produtos/"+p+"/Data"));
  display.println("NOME: " + Firebase.getString("produtos/"+p+"/NomeProduto"));
  display.println("PRATELEIRA: " + Firebase.getString("produtos/"+p+"/Prateleira"));
  display.println("SETOR: " + Firebase.getString("produtos/"+p+"/Setor"));//--> Command or code to get data (String data type) from Firebase Realtime Database.

  display.display();
  
  // Conditions for handling errors.
  if (Firebase.failed()) { 
      Serial.print("Getting /Value failed :");
      Serial.println(Firebase.error()); 
      delay(500); 
      return;
  }
  
  //----------------------------------------

  delay(5000);
}
