// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*
					.::. LCD Pin Function .::.
                    
                     |Display: LCD JHD 162A|
___________________________________________________________________
Pin 	- 		Function
____________________________________________________________________
1 		- 		GND
2 		- 		Vcc (+5V)
3 		- 		Vee (contrast voltage)
4 		- 		Rs (Register Select: 0=instruction register ; 1=data register)
5 		- 		R/W (read/write: 0=write mode; 1=read mode)
6 		- 		E (enable: 0=start to latch data to LCD character; 1=disable)
7 		- 		DB0 (databit 0; LSB)
8 		- 		DB1 (databit 1)
9 		- 		DB2 (databit 2)
10 		- 		DB3 (databit 3)
11 		- 		DB4 (databit 4)
12 		- 		DB5 (databit 5)
13 		- 		DB6 (databit 6)
14	 	- 		DB7 (databit 7; MSB)
15 		- 		BPL (backplane light: +5V or lower - optional)
16 		- 		GND (optional)
____________________________________________________________________
*/
//Begin.


#include "LiquidCrystal.h"


/*	
	Criando um objeto da classe LiquidCrystal e 
	 inicializando com os pinos da interface:	
*/
const int rs = 9, en = 13, d4 = 5, d5 = 12, d6 = 3, d7 = 6;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


const int sensorTmp = A0; //Pino analógico LM35.
const int BlueLed = 4; //Pino digital Led Azul.
const int YellowLed = 7; //Pino digital Led Amarelo.
const int RedLed = 8; //Pino digital Led Vermelho.
const int RestartSystem = 2; //Pino digital Botão.

//Variáveis//

int valueSensorTmp = 0;
float valueTmpCelsius = 0.00;

float glow=0;
int buttonState = 0;
int c=0; 

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void setup()
{

  Serial.begin(9600);
  
  //Inicializando o LCD:
  lcd.begin(16, 2); // informa o tamanho de 16 colunas e 2 linhas
  lcd.setCursor(0, 0);
  lcd.print("Unesp - SJBV");
  lcd.setCursor(0, 1);
  lcd.print("Joao R. Cioffi");
  delay(3000);
  
  pinMode(sensorTmp,INPUT);
  pinMode(BlueLed, OUTPUT);
  pinMode(YellowLed, OUTPUT);
  pinMode(RedLed, OUTPUT);
  pinMode(RestartSystem, INPUT);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void loop()
{
  
	valueSensorTmp = analogRead(sensorTmp); //leitura analógica do sensor de temp.  
	valueTmpCelsius = valueSensorTmp*0.488-49.76; //conversão dos valores de temperatura em °C.  
	buttonState = digitalRead(RestartSystem);
                        
  	////////////////////////////////////////////////////////////////
    if (valueTmpCelsius<=10 && c==0)
    {
    digitalWrite(BlueLed, HIGH);
    digitalWrite(YellowLed, LOW);
    digitalWrite(RedLed, LOW);  
    
    Serial.print("\t Temperatura (C): ");
  	Serial.println(valueTmpCelsius); 
    Serial.println("\t Status: OK!");
    delay(200);
      
      
    //Exibindo o Status
	lcd.clear();  //limpa o display do LCD.      
	lcd.print("Temp.(\xB0""C): ");  //imprime o valor temperatura no display.                 
    lcd.print(valueTmpCelsius);
    lcd.setCursor(0,1);  
    lcd.print("Status: OK!");
    delay(200);  
    }
    
  	////////////////////////////////////////////////////////////////
  	else if (10<valueTmpCelsius && valueTmpCelsius<=55 && c==0)
    {
    glow=(valueTmpCelsius-9)*5.5;
    digitalWrite(BlueLed, LOW);
    analogWrite(YellowLed, glow);
    digitalWrite(RedLed, LOW);
    
      
    Serial.print("\t Temperatura (C): ");
  	Serial.println(valueTmpCelsius); 
    Serial.println("\t Status: CAUTION!");
    delay(200);      
      

    //Exibindo o Status
	lcd.clear();  //limpa o display do LCD.      
	lcd.print("Temp.(\xB0""C): ");  //imprime o valor temperatura no display.                 
  	lcd.print(valueTmpCelsius);
    lcd.setCursor(0,1);  
    lcd.print("Status: CAUTION!");
    delay(200);        
    }
  
  
  	////////////////////////////////////////////////////////////////
    else if (55<valueTmpCelsius && valueTmpCelsius<=95 && c==0)
    {
    float frequency = 3.00; /*frequência do Led Vermelho: F=3Hz,
    					sendo F=1/T, com T=delay( )								
    */
    float ResponseTime = 1000.00/(2.00*frequency); /*tempo de resposta 
    								   do Led Vermelho 
                                    	[T ou delay()]
    */
    digitalWrite(BlueLed, LOW);
    digitalWrite(YellowLed, LOW);
    digitalWrite(RedLed, HIGH);
    delay(ResponseTime);
    digitalWrite(RedLed,LOW);
    delay(ResponseTime);
      
 
         
    Serial.print("\t Temperatura (C): ");
  	Serial.println(valueTmpCelsius); 
    Serial.println("\t OVERHEAT!");
    delay(200);      
      

    //Exibindo o Status
	lcd.clear();  //limpa o display do LCD.      
	lcd.print("Temp.(\xB0""C): ");  //imprime o valor temperatura no display.                 
  	lcd.print(valueTmpCelsius);
    lcd.setCursor(0,1);  
    lcd.print("OVERHEAT!");
    delay(200);             
	}  
  
    ////////////////////////////////////////////////////////////////
    else if (95<valueTmpCelsius && valueTmpCelsius<=125 && c==0)
    {  
    digitalWrite(BlueLed, HIGH);
    digitalWrite(YellowLed, HIGH);
    digitalWrite(RedLed, HIGH);
    c=1;
      
    Serial.print("\t Temperatura (C): ");
  	Serial.println(valueTmpCelsius); 
    Serial.println("\t SYSTEM FAILURE");
    Serial.println("\t RESET IF TEMP.<15");
    delay(200);      
      

    //Exibindo o Status
	lcd.clear();  //limpa o display do LCD.  
    lcd.setCursor(0, 0);
	lcd.print("SYSTEM FAILURE"); 
    lcd.setCursor(0, 1);
    lcd.print("RESET!");
    delay(200); 
    }
  
  	////////////////////////////////////////////////////////////////
   // Condição para reset do sistema:  
   else if (valueTmpCelsius<=15 && buttonState == HIGH && c==1)
   {
    c=0;
    lcd.clear();  //limpa o display do LCD.  
   }  
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//End.