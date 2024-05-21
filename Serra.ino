#include <DHT.h>

#include <Arduino_HTS221.h>
#include <Arduino_LPS22HB.h>
#include <Arduino_APDS9960.h>

#include "thingProperties.h"


int sensorPin = A2;     
bool water = false;

#define RELAY_PIN 1 
int StatoPompa = 0;    

#define DHTPIN 2       
#define DHTTYPE DHT22  

#define POWER_PIN_WT 7    
#define SIGNAL_PIN_WT A5  
#define SENSOR_MIN_WT 0
#define SENSOR_MAX_WT 521
int value = 0;  



DHT dht(DHTPIN, DHTTYPE);

int MoistureMin = 500;
int MoistureTarget = 600;
int WaterMin = -1;


void setup() {
  delay(1000);
  Serial.begin(9600);
  dht.begin();
  delay(1500); 
  
  pinMode(POWER_PIN_WT, OUTPUT);    
  digitalWrite(POWER_PIN_WT, LOW); 
  
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  pompa = false;

  initProperties();

  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

void loop() {
  ArduinoCloud.update();
 
  
  digitalWrite(POWER_PIN_WT, HIGH);   
  delay(10);                          
  value = analogRead(SIGNAL_PIN_WT);  
  digitalWrite(POWER_PIN_WT, LOW);   

  WaterLevel = map(value, SENSOR_MIN_WT, SENSOR_MAX_WT, 0, 4); 
 
  
  
  humidity = dht.readHumidity();         
  temperature = dht.readTemperature();
  
  int h = dht.readHumidity();        
  int t = dht.readTemperature();     

   if (isnan(humidity) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
      //Serial.println(temperature);                        
      //Serial.print(humidity);                           

    return;
  }


  
  
  moistureValue = analogRead(sensorPin);      
  delay(1000);                                
  
  
  Serial.println("******************");
  Serial.print("Temperature = ");
  Serial.println(t);                        
  Serial.print("Humidity = ");
  Serial.print(h);                           
  Serial.println("%");
  Serial.print("Moisture = ");                
  Serial.println(moistureValue);
  Serial.print("Water level: ");
  Serial.println(WaterLevel);
  Serial.println("******************");
  
  
  if (moistureValue < MoistureMin) {
    if(WaterLevel > WaterMin) {
      while(moistureValue < MoistureTarget) {
        digitalWrite(RELAY_PIN, HIGH);  
        pompa = true;
        moistureValue = analogRead(sensorPin);
        
        Serial.println("------------------");
        Serial.println("Pompa ON");
        Serial.print("Moisture = ");                
        Serial.println(moistureValue);
        Serial.println("------------------");
        
        delay(500);
      }
      
      digitalWrite(RELAY_PIN, LOW);
      pompa = false;
      
    } else {
      Serial.println("");
      Serial.println("\\\\\\\\\\\\\\\\\\");
      Serial.println("Poca acqua");
      Serial.println("//////////////////");
      Serial.println("");
      
      delay(10000);
    }
  }
  

  
}





void onMoistureValueChange()  {
}


void onPompaChange()  {
}

 void onHumidityChange() {
 }

void onTemperatureChange() {
}







/*
  Since WaterLevel is READ_WRITE variable, onWaterLevelChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onWaterLevelChange()  {
  // Add your code here to act upon WaterLevel change
}