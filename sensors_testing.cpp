const int LDR = 0;
// const int LM35= 1;
const int Buzzer = 2;
const int led[] = {
    5, 6, 7, 8, 9, 10, 11}; // 8, 9, 10, 11 LDR, alto-brilho
// int ValorLDR = 0;
// int ADClido = 0;
// float temperatura = 0;
float temperatura;
int ADClido;
int ValorLDR;
int LM35 = 600;
unsigned long buzzerTimerLM35;
unsigned long buzzerTimerLDR;
bool buzzerMoodLM35 = false; // false para HIGH
bool buzzerMoodLDR = false;  // false para HIGH
int pwm = 0;
unsigned long printTimer = 0;
float temperaturaInicial;
int pwmLDR;
unsigned long testeLDR = 0;
unsigned long testeLM35 = 0;

void setup()
{
  for (int x = 0; x < 7; x++)
  {
    pinMode(led[x], OUTPUT);
    if (x < 3)
    {
      digitalWrite(led[x], HIGH);
    }
    else
    {
      digitalWrite(led[x], LOW);
    }
  }
  pinMode(Buzzer, OUTPUT);
  Serial.begin(9600);
  temperaturaInicial = analogRead(LM35);
  temperatura = 24;
  ValorLDR = 350;
}

void loop()
{
  // ValorLDR = analogRead(LDR);
  // ADClido = analogRead(LM35);
  // temperatura = ADClido * 0.4887585532;
  // LM35
  if (millis() - printTimer > 1000)
  {
    Serial.println(temperatura);
    printTimer = millis();
  }

  temperaturaInicial = 20;
  if (millis() - testeLM35 > 2000 && temperatura < 40)
  {
    temperatura++;
  }

  // PARTE 1 - LM35
  if (temperatura - temperaturaInicial > 2)
  {
    if (digitalRead(led[2]) == HIGH)
    {
      for (pwm = 255; pwm >= 0; pwm--)
      {
        analogWrite(led[2], pwm);
        delay(10);
      }
    }
  }
  else
  {
    if (digitalRead(led[1]) == HIGH && digitalRead(led[2]) == LOW)
    {
      for (pwm = 0; pwm <= 255; pwm++)
      {
        analogWrite(led[2], pwm);
        delay(10);
      }
    }
  }
  if (temperatura - temperaturaInicial > 4)
  {
    if (digitalRead(led[2]) == LOW && digitalRead(led[1]) == HIGH)
    {
      for (pwm = 255; pwm >= 0; pwm--)
      {
        analogWrite(led[1], pwm);
        delay(10);
      }
    }
  }
  else
  {
    if (digitalRead(led[0]) == HIGH && digitalRead(led[1]) == LOW)
    {
      for (pwm = 0; pwm <= 255; pwm++)
      {
        analogWrite(led[1], pwm);
        delay(10);
      }
    }
  }
  if (temperatura - temperaturaInicial > 6)
  {
    if (digitalRead(led[0]) == HIGH && digitalRead(led[1]) == LOW)
    {
      for (pwm = 255; pwm >= 0; pwm--)
      {
        analogWrite(led[0], pwm);
        delay(10);
      }
    }
  }
  else
  {
    if (digitalRead(led[0]) == LOW)
    {
      for (pwm = 0; pwm <= 255; pwm++)
      {
        analogWrite(led[0], pwm);
        delay(10);
      }
    }
  }

  // PARTE 2 - LDR
  //  LDR maior a luz, menor a resistencia
  //  LDR calibrado com intervalo aprox de 30 a 350
  /*if (ValorLDR > 340)
  {
    if (digitalRead(led[4] == LOW) && digitalRead(led[3] == HIGH))
    {
      for (pwmLDR = 255; pwmLDR >= 0; pwmLDR--)
      {
        analogWrite(led[3], pwmLDR);
        delay(10);
      }
    }
  }
  else
  {
    for (pwmLDR = 0; pwmLDR <= 255; pwmLDR++)
    {
      analogWrite(led[3], pwmLDR);
      delay(10);
    }
  }
  if (ValorLDR > 160)
  { // media do intervalo
    if (digitalRead(led[3] == LOW) && digitalRead(led[4] == HIGH))
    {
      for (pwmLDR = 255; pwmLDR >= 0; pwmLDR--)
      {
        analogWrite(led[4], pwmLDR);
        delay(10);
      }
    }
  }
  else
  {
    if (digitalRead(led[3] == HIGH) && digitalRead(led[4] == LOW))
    {
      for (pwmLDR = 0; pwmLDR <= 255; pwmLDR++)
      {
        analogWrite(led[4], pwmLDR);
        delay(10);
      }
    }
  }
  if (ValorLDR > 40)
  {
    for (pwmLDR = 255; pwmLDR >= 0; pwmLDR--)
    {
      analogWrite(led[6], pwmLDR);
      delay(10);
    };
    if (digitalRead(led[6] == LOW) && digitalRead(led[5] == HIGH))
    {
      for (pwmLDR = 255; pwmLDR >= 0; pwmLDR--)
      {
        analogWrite(led[5], pwmLDR);
        delay(10);
      }
    }
  }
  else
  {
    if (digitalRead(led[4] == HIGH) && digitalRead(led[5] == LOW))
    {
      for (pwmLDR = 0; pwmLDR <= 255; pwmLDR++)
      {
        analogWrite(led[5], pwmLDR);
        delay(10);
      }
    }
    if (digitalRead(led[5] == HIGH) && digitalRead(led[6] == LOW))
    {
      for (pwmLDR = 0; pwmLDR <= 255; pwmLDR++)
      {
        analogWrite(led[5], pwmLDR);
        delay(10);
      }
    }
  }
*/
  // BUZZER LM35
  if (digitalRead(led[0]) == LOW)
  {
    if (buzzerMoodLM35 == false)
    {
      digitalWrite(Buzzer, HIGH);
    }
    else
    {
      digitalWrite(Buzzer, LOW);
    }
    if (millis() - buzzerTimerLM35 > 2000)
    {
      buzzerMoodLM35 = !buzzerMoodLM35;
      buzzerTimerLM35 = millis();
    }
  }
  else
  {
    if (digitalRead(led[6]) == LOW && led[0] == HIGH)
    { // se LDR nao utiliza o buzzer
      digitalWrite(Buzzer, LOW);
    }
  }
  // BUZZER LDR
  if (digitalRead(led[6]) == HIGH)
  {
    if (buzzerMoodLDR == false)
    {
      digitalWrite(Buzzer, HIGH);
      buzzerTimerLDR = millis();
      if (millis() - buzzerTimerLDR > 4000)
      {
        buzzerMoodLDR = true;
      }
    }
    else
    {
      digitalWrite(Buzzer, LOW);
      buzzerMoodLDR = false;
    }
  }
  else
  {
    if (digitalRead(led[0]) == HIGH)
    { // se LM35 nao utiliza o buzzer
      digitalWrite(Buzzer, LOW);
    }
  }
}