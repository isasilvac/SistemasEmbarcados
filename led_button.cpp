// Atividade prática 4

// --- Ligações entre PIC e LCD ---
sbit LCD_RS at RE2_bit; // PINO 2 DO PORTD LIGADO AO RS DO DISPLAY
sbit LCD_EN at RE1_bit; // PINO 3 DO PORTD LIGADO AO EN DO DISPLAY
sbit LCD_D7 at RD7_bit; // PINO 7 DO PORTD LIGADO AO D7 DO DISPLAY
sbit LCD_D6 at RD6_bit; // PINO 6 DO PORTD LIGADO AO D6 DO DISPLAY
sbit LCD_D5 at RD5_bit; // PINO 5 DO PORTD LIGADO AO D5 DO DISPLAY
sbit LCD_D4 at RD4_bit; // PINO 4 DO PORTD LIGADO AO D4 DO DISPLAY

// Selecionando direção de fluxo de dados dos pinos utilizados para a comunicação com display LCD
sbit LCD_RS_Direction at TRISE2_bit; // SETA DIREÇÃO DO FLUXO DE DADOS DO PINO 2 DO PORTD
sbit LCD_EN_Direction at TRISE1_bit; // SETA DIREÇÃO DO FLUXO DE DADOS DO PINO 3 DO PORTD
sbit LCD_D7_Direction at TRISD7_bit; // SETA DIREÇÃO DO FLUXO DE DADOS DO PINO 7 DO PORTD
sbit LCD_D6_Direction at TRISD6_bit; // SETA DIREÇÃO DO FLUXO DE DADOS DO PINO 6 DO PORTD
sbit LCD_D5_Direction at TRISD5_bit; // SETA DIREÇÃO DO FLUXO DE DADOS DO PINO 5 DO PORTD
sbit LCD_D4_Direction at TRISD4_bit; // SETA DIREÇÃO DO FLUXO DE DADOS DO PINO 4 DO PORTD

char contador_string[7] = {';', ';', ';', ';', ';', ';', ';'}; // STRING PARA RECEBER O VALOR CONVERTIDO DA LEITURA DO POTENCIÔMETRO 1

void main()
{

  unsigned int uiContador; // Variavel de armazenamento do contador.
  unsigned int uiValor;    // Variavel auxiliar para exibi??o do contador.
  unsigned int uiValor2;
  unsigned char ucStatus_inc; // Variavel de travamento do incremento.
  int contador = 0;

  ADCON1 = 0x0f; // Configura todos canais como Digital.

  Lcd_Init();               // Inicializa módulo LCD
  Lcd_Cmd(_LCD_CURSOR_OFF); // Apaga cursor
  Lcd_Cmd(_LCD_CLEAR);      // Limpa display

  TRISB.RB0 = 1; // Define o pino RB2 do PORTB como entrada.

  TRISB.RB7 = 0; // Define o pino RB7 do PORTB como saida
  TRISC.RC1 = 0;

  uiContador = 0; // Inicializa a variavel com o valor 0.
  uiValor = 0;
  uiValor2 = 0;

  while (1)
  {
    if ((PORTB.RB0 == 0) && (ucStatus_inc == 0))
    { // Incrementa somente uma vez quando a tecla for pressionada.
      uiContador++;
      ucStatus_inc = 1;
    }
    if ((PORTB.RB0 == 1) && (ucStatus_inc == 1))
    { // Volta a disponibilizar a opcao de incremento quando a tecla for solta.
      contador++;
      ucStatus_inc = 0;
    }
    delay_ms(100);
    IntToStr(contador, contador_string); // necessario passar para string para impressao no LCD
    lcd_out(1, 4, "Pressionou ");
    lcd_out(2, 3, contador_string);
    lcd_chr_cp('x');
    uiValor = uiContador % 2;
    uiValor2 = uiContador % 3;
    if (uiContador > 0 && uiValor2 == 0) // checando se é multiplo de 3, e maior que o ultimo
    {                                    // numero multiplo de 3 checado
      PORTC.RC1 = ~PORTC.RC1;            // acionamento do buzzer
      delay_ms(1000);
      PORTC.RC1 = ~PORTC.RC1;
      delay_ms(300);
    }
    if (uiValor == 0)
    {
      PORTB.RB7 = 1; // led liga se for par
    }
    else
    {
      PORTB.RB7 = 0; // led desliga se for impar
    }
  }
}
