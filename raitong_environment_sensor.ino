/*******************************************************
    Author: Lim Ren Yi
    Date: Jul 2018
    Title: Raitong Environment Sensor
    Description: Environment Sensor for Black Soldier Fly and Earthworm monitoring
                 Includes a DHT22 (temp and humidity), capacitive soil moisture 
                 sensor and light sensor.
   
    Connections:
    

    Note:
   
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

int moistureSensor = 0;

#define DHTPIN D5  // what digital pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);

#define OLED_RESET 0
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

Button myButton = Button(D4, PULLUP); 

void setup()   {
  Wire.begin();
  Serial.begin(57600);
  dht.begin();
  delay(1000); // delay 1 sec

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(32, 32);
  display.println("Raitong\nEnvironment\nSensor");
  display.display();
  delay(2000);
}


void loop() {

  float temp = dht.readTemperature();

  float h = dht.readHumidity();

  int sensorValue = analogRead(moistureSensor);
  sensorValue = map(sensorValue, 375, 740, 100, 0);
  // print out the value you read:
  // float voltage = sensorValue * (1024 / 100);

  int i;
  uint16_t val = 0;
  BH1750_Init(BH1750address);
  delay(200);

  if (2 == BH1750_Read(BH1750address))
  {
    val = ((buff[0] << 8) | buff[1]) / 1.2;
  }
  delay(150);

  if (myButton.uniquePress())
  {
    Serial.println("Button pressed");

    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print("Temp: ");
    display.print(temp);
    display.println("C");
    display.println("");

    display.setTextSize(1);
    display.print("Humidity: ");
    display.print(h);
    display.println("%");
    display.println("");

    display.print("Moist: ");
    display.print(sensorValue);
    display.println("%");
    display.println("");

    display.setTextSize(1);
    display.print("Light");
    display.print(" ");
    display.print(val, DEC);
    display.println("[lx]");

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




