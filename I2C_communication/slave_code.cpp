// SLAVE
#include <Wire.h> //comunicacao I2C
#include <LiquidCrystal_I2C.h>

#define commandSize 18
char message[commandSize];
int i = 0;
String command, lastReceive, lcdCommand;
const int ledPin = 8;
const int buzzerPin = 7;
LiquidCrystal_I2C lcd(32, 16, 2);
int incomingByte, x, y;
bool led = 0, buzzer = 0, lcdMessage = 0;
const int button1 = 6;
const int button2 = 5;
int buttonPressed = 0;

// debouncing botoes
// botao 1
int buttonState;
int lastButtonState = LOW;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 100;

// botao 2
int buttonState2;
int lastButtonState2 = LOW;
unsigned long lastDebounceTime2 = 0;
unsigned long debounceDelay2 = 100;

void setup()
{
  lcd.init();
  lcd.begin(16, 2);
  Wire.begin(2); // inicializacao
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  Serial.begin(9600);
  memset(message, '\0', commandSize);
  pinMode(ledPin, OUTPUT);
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(buzzerPin, OUTPUT);
}

void loop()
{
  if (led == 1)
  {
    if (digitalRead(ledPin == LOW))
    {
      digitalWrite(ledPin, HIGH);
    }
  }
  if (led == 0)
  {
    if (digitalRead(ledPin) == 1)
    {
      digitalWrite(ledPin, LOW);
    }
  }
  if (buzzer == 1)
  {
    if (digitalRead(buzzerPin) == LOW)
    {
      digitalWrite(buzzerPin, HIGH);
    }
  }
  if (buzzer == 0)
  {
    if (digitalRead(buzzerPin) == HIGH)
    {
      digitalWrite(buzzerPin, LOW);
    }
  }
  if (lcdMessage == 1)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(lcdCommand);
    lcdMessage = 0;
    buttonPressed = 3;
  }

  // debouncing dos botoes
  debouncing(button1);
  debouncing(button2);
}

void debouncing(const int button)
{
  if (button == 6)
  {
    int reading = digitalRead(button1); // botao 1
    if (reading != lastButtonState)
    {
      lastDebounceTime = millis();
    }
    if ((millis() - lastDebounceTime) > debounceDelay)
    {
      if (reading != buttonState)
      {
        buttonState = reading;
        if (buttonState == HIGH)
        {
          buttonPressed = 1;
          Serial.println("Botao 1 on");
        }
        else
        {
          Serial.println("Botao 1 off");
          buttonPressed = 4;
        }
      }
    }
    lastButtonState = reading;
  }
  else
  {
    int reading2 = digitalRead(button2); // botao 2
    if (reading2 != lastButtonState2)
    {
      lastDebounceTime2 = millis();
    }
    if ((millis() - lastDebounceTime2) > debounceDelay2)
    {
      if (reading2 != buttonState2)
      {
        buttonState2 = reading2;
        if (buttonState2 == HIGH)
        {
          buttonPressed = 2;
          Serial.println("Botao 2 on");
        }
        else
        {
          Serial.println("Botao 2 off");
          buttonPressed = 4;
        }
      }
    }
    lastButtonState2 = reading2;
  }
}

void receiveEvent(int howMany)
{
  while (Wire.available() > 0)
  {
    char c = Wire.read();
    message[i] = c;
    i++;
  }
  i = 0;
  if (lastReceive.compareTo("Display LCD") != 0)
  {
    command = String(message);
  }
  else
  {
    lcdCommand = String(message);
  }
  memset(message, '\0', commandSize);

  if (command.compareTo("LigarLed") == 0)
  {
    led = 1;
    Serial.println("ledzin");
  }
  else if (command.compareTo("DesligarLed") == 0)
  {
    led = 0;
    Serial.println("ledzinOff");
  }
  else if (command.compareTo("LigarBuzzer") == 0)
  {
    buzzer = 1;
    Serial.println("buzzzzz");
  }
  else if (command.compareTo("DesligarBuzzer") == 0)
  {
    buzzer = 0;
    Serial.println("buzzoff");
  }
  else if (command.compareTo("Display LCD") == 0)
  {
    if (lastReceive == command)
    {
      lcdMessage = 1;
      lastReceive = ' ';
    }
    else
    {
      lastReceive = command;
      Serial.println("comando eh display");
    }
  }
  else
  {
    Serial.println(command);

    Serial.println("Comando invalido");
  }
}

void requestEvent()
{
  if (buttonPressed == 0)
  {
    Wire.write("x");
  }
  if (buttonPressed == 1)
  {
    Wire.write("CHAVE 1 ON \nCHAVE 2 OFF \n\n");
  }
  if (buttonPressed == 2)
  {
    Wire.write("CHAVE 2 ON \nCHAVE 1 OFF \n\n");
  }
  if (buttonPressed == 3)
  {
    Wire.write("Mensagem alterada.      \n\n");
  }
  if (buttonPressed == 4)
  {
    Wire.write("CHAVE 1 OFF \nCHAVE 2 OFF\n\n");
  }
  buttonPressed = 0;
}
