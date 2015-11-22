
// Plot DTH11 data on thingspeak.com 

 
 
#include <DHT.h>

#include <SPI.h>
#include <Ethernet.h>

const int fan =  7;      // the number of the LED pin
 byte mac[] = { 0xD4, 0x28, 0xB2, 0xFF, 0xA0, 0xA1 }; // Must be unique on local network
 
// replace with your channel's thingspeak API key, 
char thingSpeakAddress[] = "api.thingspeak.com";
String writeAPIKey = "CMKRR51G4VAYJOEP";
#define DHTPIN 2 // what pin we're connected to
 
DHT dht(DHTPIN, DHT22,15);
EthernetClient client;
   
 
void setup() {                
  Serial.begin(115200);
  delay(10);
  dht.begin();
  
 
pinMode(fan, OUTPUT);

  // Start Ethernet on Arduino
  startEthernet();
}
 
 
void loop() {
   
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

///////////////FAN/////////////////////////////////////////
  
  if (t >25) {
    //digitalWrite(7,HIGH);
    Serial.println("7ar fash5"); //comment this line , debuging only
  } else {
    //digitalWrite(7,LOW);
    Serial.println("kolo tamam"); // comment this line, debuggigg only
  }
  delay(500);
  ////////////////////////////////////////////////////////////////////////
  
  if (client.connect(thingSpeakAddress,80)) {  //   "184.106.153.149" or api.thingspeak.com
    String postStr = writeAPIKey;
           postStr +="&field1=";
           postStr += String(t);
           postStr +="&field2=";
           postStr += String(h);
           postStr += "\r\n\r\n";
 
     client.print("POST /update HTTP/1.1\n"); 
     client.print("Host: api.thingspeak.com\n"); 
     client.print("Connection: close\n"); 
     client.print("X-THINGSPEAKAPIKEY: "+writeAPIKey+"\n"); 
     client.print("Content-Type: application/x-www-form-urlencoded\n"); 
     client.print("Content-Length: "); 
     client.print(postStr.length()); 
     client.print("\n\n"); 
     client.print(postStr);
           
 
     Serial.print("Temperature: ");
     Serial.print(t);
     Serial.print(" degrees Celcius Humidity: "); 
     Serial.print(h);
     Serial.println("% send to Thingspeak");    
  }
  client.stop();
   
  Serial.println("Waiting...");    
  // thingspeak needs minimum 15 sec delay between updates
  delay(10000);  
}

void startEthernet()
{
  
  client.stop();

  Serial.println("Connecting Arduino to network...");
  Serial.println();  

  delay(1000);
  
  // Connect to network amd obtain an IP address using DHCP
  if (Ethernet.begin(mac) == 0)
  {
    Serial.println("DHCP Failed, reset Arduino to try again");
    Serial.println();
  }
  else
  {
    Serial.println("Arduino connected to network using DHCP");
    Serial.println();
  }
  
  delay(1000);
}

