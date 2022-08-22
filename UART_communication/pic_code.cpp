/* ============================================================================================================

     PROJETO FINAL DE SISTEMAS EMBARCADOS
     Comunicação entre Arduino e PIC - UART
     Feito por Isabela Cristina
     11/08/2022

 ============================================================================================================ */
// --- Ligações entre PIC e LCD ---
sbit LCD_RS at RE2_bit; // PINO 2 DO PORTD INTERLIGADO AO RS DO DISPLAY
sbit LCD_EN at RE1_bit; // PINO 3 DO PORTD INTERLIGADO AO EN DO DISPLAY
sbit LCD_D7 at RD7_bit; // PINO 7 DO PORTD INTERLIGADO AO D7 DO DISPLAY
sbit LCD_D6 at RD6_bit; // PINO 6 DO PORTD INTERLIGADO AO D6 DO DISPLAY
sbit LCD_D5 at RD5_bit; // PINO 5 DO PORTD INTERLIGADO AO D5 DO DISPLAY
sbit LCD_D4 at RD4_bit; // PINO 4 DO PORTD INTERLIGADO AO D4 DO DISPLAY

// Selecionando direção de fluxo de dados dos pinos utilizados para a comunicação com display LCD
sbit LCD_RS_Direction at TRISE2_bit; // SETA DIREÇÃO DO FLUXO DE DADOS DO PINO 2 DO PORTD
sbit LCD_EN_Direction at TRISE1_bit; // SETA DIREÇÃO DO FLUXO DE DADOS DO PINO 3 DO PORTD
sbit LCD_D7_Direction at TRISD7_bit; // SETA DIREÇÃO DO FLUXO DE DADOS DO PINO 7 DO PORTD
sbit LCD_D6_Direction at TRISD6_bit; // SETA DIREÇÃO DO FLUXO DE DADOS DO PINO 6 DO PORTD
sbit LCD_D5_Direction at TRISD5_bit; // SETA DIREÇÃO DO FLUXO DE DADOS DO PINO 5 DO PORTD
sbit LCD_D4_Direction at TRISD4_bit; // SETA DIREÇÃO DO FLUXO DE DADOS DO PINO 4 DO PORTD

// ============================================================================================================
// --- Variáveis globais ---
char pot1_string[8];                                                                              // String para receber o valor lido do LM35
float pot1 = 0;                                                                                   // Variável para armazenar o valor lido pelo LM35
float temp = 0;                                                                                   // Variável para armazenar a temperatura correspondente
char apagar[] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}; // STRING PARA APAGAR O CONTEÚDO DAS RESPECTIVAS LINHAS
int ucRead;                                                                                       // Variavel para armazenar o dado recebido do arduino
int control = 0;

// ============================================================================================================
// --- Funções Auxiliares ---
void lightControl(int status)
{
  if (status == 1)
  {
    PORTB = 255;
  }
  else
  {
    PORTB = 0;
  }
} // end of lightControl

float temperature()
{
  pot1 = ADC_Get_Sample(2); // RECEBE O VALOR DE TENSÃO DO POTENCIOMETRO NA PORTA E
  pot1 = pot1 * 0.4887585532;
  floatToStr(pot1, pot1_string); // CONVERTE O VALOR DO TIPO INTEIRO OBTIDO PELO CONVERSOR AD NA PORTA AN0 PARA UMA STRING
  Ltrim(pot1_string);            // REMOVE TODOS OS ESPAÇOS EM BRANCO LOCALIZADOS A ESQUERDA DO PRIMEIRO CHAR DA STRING COM O VALOR DE AN0
  /*    Lcd_Out(1, 1, apagar);
      Lcd_Out(2, 1, apagar);
      Lcd_Out(1, 1, pot1_string);//ESCREVE O VALOR OBTIDO DO CONVERSOR A/D NA POSIÇÃO EM QUE SE ENCONTRA O CURSOR DO DISPLAY
      Lcd_Out(1, 6, apagar);
      Lcd_Out(2, 1, apagar);  */
  return pot1;
} // end of temperature

void ventoinha(int intensity)
{
  PWM1_Set_Duty(intensity);
  delay_10ms;
} // end of ventoinha

// ============================================================================================================
// --- Função Main ---
void main()
{
  // ventoinha
  PWM1_Init(5000); // inicializa modulo PWM com 5Khz
  PWM1_Start();

  // LM35
  TRISA2_bit = 1;      // CONFIGURA O PINO EM QUE SE ENCONTRA A PORTA ANALÓGICA COMO ENTRADA
  ADCON1 = 0b00001100; // 0b00001100;//SELECIONA OS PINOS 1, 2 DO PORTA COMO ENTRADAS ANALÓGICAS (AN0, AN1)
  ADC_Init();          // INICIALIZA MÓDULO ANALGÓGICO
  CMCON = 0b00000111;  // DESABILITA COMPARADORES INTERNOS

  // leds
  TRISB = 0; // Define todos os pinos do PORTB como saída.
  PORTB = 0; // Colocar todos os pinos em nível baixo.

  // LCD
  Lcd_Init();               // Inicializa módulo LCD
  Lcd_Cmd(_LCD_CURSOR_OFF); // Apaga cursor
  Lcd_Cmd(_LCD_CLEAR);      // Limpa display

  UART1_Init(9600); // Utiliza bibliotecas do compilador para configuração o Baud rate.

  while (1)
  {
    if (UART1_Data_Ready())
    {                        // Verifica se um dado foi recebido no buffer
      ucRead = UART1_Read(); // Lê o dado recebido do buffer.
      Delay_ms(50);          // Pausa de 50ms.
      if (ucRead == '1')     // ligar luz
      {
        Lcd_Out(1, 1, apagar);
        lcd_out(1, 2, "Ligar Luzes");
        lightControl(1); // 1 para ligar

        UART1_Write('A');
        Delay_ms(50);
      }
      else if (ucRead == '2')
      { // ligar ventoinha
        Lcd_Out(1, 1, apagar);
        lcd_out(1, 1, "Ligar Ventilador");
        ventoinha(255);

        UART1_Write('B');
        Delay_ms(50);
      }
      else if (ucRead == '4')
      { // desligar luz
        Lcd_Out(1, 1, apagar);
        lcd_out(1, 2, "Desligar Luzes");
        lcd_chr_cp(ucRead);
        lightControl(0); // 0 para desligar

        UART1_Write('C');
        Delay_ms(50);
      }
      else if (ucRead == '5')
      { // desligar ventoinha
        Lcd_Out(1, 1, apagar);
        lcd_out(1, 2, "Desligar Vento");
        control = 1;
        ventoinha(0);

        UART1_Write('D');
        Delay_ms(50);
      } // end of else if (ucRead == '5')
    }   // end of UART communication
    temp = temperature();
    if (temp > 30.00)
    {
      if (control != 1)
      {
        ventoinha(255);
      }
    }
    delay_ms(300);
    control = 0;
  } // end of while
} // end of main
