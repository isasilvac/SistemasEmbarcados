const int buzzer = 11;
char option;

void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);
}

void loop()
{
  option = commandReader();
  if (option == '3')
  {
    music();
    Serial.println("3: musica");
  }
  else if (option == '6')
  {
    Serial.println("desligando buzzer");
    digitalWrite(buzzer, 0);
  }
  else if (option == '1' || option == '2' || option == '4' || option == '5')
  {
    Serial.print("enviando para pic o num: ");
    Serial.println(option);
    Serial1.write(option);
    if (Serial1.available())
    {                                  // Verificar se há caracteres disponíveis
      char caractere = Serial1.read(); // Armazena caracter lido.
      if (caractere == 'A')
      {
        Serial.println("Luz ligada");
      }
      else if (caractere == 'B')
      {
        Serial.println("Ventoinha ligada");
      }
      else if (caractere == 'C')
      {
        Serial.println("Luz desligada");
      }
      else if (caractere == 'D')
      {
        Serial.println("Ventoinha desligada");
      }
    }
  }
  option = 'a';
  delay(100);
}

char commandReader()
{ // le comando da serial ou teclado matricial
  char command;
  if (Serial.available())
  {
    command = Serial.read();
    Serial.println(command);
  }
  return command;
}

void music()
{ // tocar melodia no buzzer
  digitalWrite(buzzer, 1);
}