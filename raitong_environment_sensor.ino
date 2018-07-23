/*******************************************************
    Author: Lim Ren Yi
    Date: Jul 2018
    Title: Raitong Environment Sensor
    Description: Environment Sensor for Black Soldier Fly and Earthworm monitoring
                 Includes a DHT22 (temp and humidity), capacitive soil moisture 
                 sensor and light sensor.
   
    Connections: WeMos D1 mini
        D1 (SCL) - OLED D0 & BH1750 SCL
        D2 (SDA) - OLED D1 & BH1750 SDA
        A0 - moisture sensor A
        D5 - DHT22
        D4 - button

    Power for components:
        BH1750 - 5V
        OLED - 3.3V
        Moisture - 5V
        DHT22 - 5V

    Notes:
    OLED: Need to remove R3 resistor for oled and short R1, R4, R5, R6
    In SSD1306.h:  #define SSD1306_128_64
*******************************************************/

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <Wire.h> //BH1750 IIC Mode
#include <Math.h>
#include <Button.h>

int BH1750address = 0x23; //setting i2c address
byte buff[2];

#define moistureSensorPin A0

#define DHTPIN D5  // what digital pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);

#define OLED_RESET 0 //not connected actually
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

Button myButton = Button(D4, PULLUP); 

void setup()   {
  Wire.begin();
  Serial.begin(9600);
  dht.begin();
  delay(1000); // delay 1 sec

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(32, 32);
  display.println("Raitong\nEnvironment\nSensor");
  display.display();

  Serial.println("Raitong Environment Sensor");
  delay(2000);
}


void loop() {

  //get all the sensor values
  float temp = dht.readTemperature();
  float h = dht.readHumidity();

  int moistValue = analogRead(moistureSensorPin);
  moistValue = map(moistValue, 375, 740, 100, 0);
  // float voltage = sensorValue * (1024 / 100);

  uint16_t lightVal = 0;
  BH1750_Init(BH1750address);
  delay(200);
  
  if (2 == BH1750_Read(BH1750address))
  {
    lightVal = ((buff[0] << 8) | buff[1]) / 1.2;
  }
  delay(150);

  if (myButton.uniquePress()) {
  //display all the sensor values
    Serial.println("Button pressed");

    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print("Temp: ");
    display.print(temp);
    display.println("C");
    display.println("");

    Serial.print("Temp: ");
    Serial.print(temp);
    Serial.println("C");
    Serial.println("");

    display.setTextSize(1);
    display.print("Humidity: ");
    display.print(h);
    display.println("%");
    display.println("");

    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.println("%");
    Serial.println("");

    display.print("Moist: ");
    display.print(moistValue);
    display.println("%");
    display.println("");

    Serial.print("Moist: ");
    Serial.print(moistValue);
    Serial.println("%");
    Serial.println("");

    display.setTextSize(1);
    display.print("Light");
    display.print(" ");
    display.print(lightVal, DEC);
    display.println("[lx]");

    Serial.print("Light");
    Serial.print(" ");
    Serial.print(lightVal, DEC);
    Serial.println("[lx]");

    display.display();
    delay(2000);
    display.clearDisplay();
  }

}

int BH1750_Read(int address) //
{
  int i = 0;
  Wire.beginTransmission(address);
  Wire.requestFrom(address, 2);
  while (Wire.available()) //
  {
    buff[i] = Wire.read();  // receive one byte
    i++;
  }
  Wire.endTransmission();
  return i;
}

void BH1750_Init(int address)
{
  Wire.beginTransmission(address);
  Wire.write(0x10);//1lx reolution 120ms
  Wire.endTransmission();
}




