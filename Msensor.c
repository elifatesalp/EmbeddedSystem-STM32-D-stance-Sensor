sbit LCD_RS at GPIOE_ODR.B8;
sbit LCD_EN at GPIOE_ODR.B10;
sbit LCD_D4 at GPIOE_ODR.B4;
sbit LCD_D5 at GPIOE_ODR.B5;
sbit LCD_D6 at GPIOE_ODR.B6;
sbit LCD_D7 at GPIOE_ODR.B7;

int cti(char a)
{
    return (int)a - 48;
}

void setup()
{
    GPIO_Digital_Output(&GPIOE_ODR, _GPIO_PINMASK_ALL);
    LCD_Init();
    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Cmd(_LCD_CURSOR_OFF);
    Lcd_Out(1, 1, ">");
}
void InitTimer7()
{
    RCC_APB1ENR.TIM7EN = 1;
    TIM7_CR1.CEN = 0;
    TIM7_PSC = 0;
    TIM7_CNT = 0;
}
unsigned int say = 0, mesafe = 0, sayac = 0;
char h[7];
void main()
{
    InitTimer7();
    GPIO_Digital_Output(&GPIOB_BASE, _GPIO_PINMASK_0);
    GPIO_Digital_Output(&GPIOD_ODR, _GPIO_PINMASK_12);
    GPIO_Digital_Input(&GPIOB_BASE, _GPIO_PINMASK_1);
    setup();
    while (1)
    {
        TIM7_CR1.CEN = 0; // timer durduruluyor.
        TIM7_CNT = 0;
        GPIOB_ODR.B0 = 1; // tetikleme veriliyor.
        delay_us(10);
        GPIOB_ODR.B0 = 0; // tetikleme kesiliyor.
        sayac = 0;
        while (!GPIOB_IDR.B1)
        { // Echo pininden 1 gelmesi bekleniyor.
            sayac++;
            delay_us(10);
            if (sayac > 10000)
                continue;
        }
        TIM7_CR1.CEN = 1;
        while (GPIOB_IDR.B1)
        { // Timer baÅŸlatÄ±lÄ±yor.;// Echo pininden 0 gelmesi bekleniyor.
            sayac++;
            delay_us(10);
            if (sayac > 10000)
                continue;
        }
        TIM7_CR1.CEN = 0; // timer durduruluyor.
        say = TIM7_CNT;
        TIM7_CNT = 0;
        say = say - 200;
        mesafe = (say / 834);
        InttoStr(mesafe, h);
        Lcd_Out(1, 2, h);
        Lcd_Out(1, 12, "cm");
        InttoStr(say, h);
        Lcd_Out(2, 3, h);
        Lcd_Out(2, 10, "us");
        GPIOD_ODR.B12 = 1;
        Delay_Ms(100);
        GPIOD_ODR.B12 = 0;
        delay_ms(100);

        Delay_ms(500);
    }
}