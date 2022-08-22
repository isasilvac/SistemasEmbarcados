// MASTER
#include <Wire.h>; //comunicacao I2C
#include <LiquidCrystal_I2C.h>
#define sizeInput 18
#define sizeMessage 30

String input;
char command[sizeInput];
char lcdMessage[sizeMessage];
int i = 0;
int flag = 0;

void setup()
{
  Wire.begin(); // inicializacao
  Serial.begin(9600);
}

void loop()
{
  if (Serial.available())
  {
    input = Serial.readStringUntil('\n');
    input.toCharArray(command, sizeInput);
    Wire.beginTransmission(2);
    Wire.write(command);
    Wire.endTransmission();
    if (input.compareTo("Display LCD") == 0)
    {
      Serial.println("Digite a mensagem desejada");
    }
  }
  Wire.requestFrom(2, 26);
  while (Wire.available())
  {
    char c = Wire.read();
    if (c == 'x')
      break;
    Serial.print(c);
  }
  delay(500);
}