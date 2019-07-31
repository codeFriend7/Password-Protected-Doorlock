#include <EEPROM.h>
#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Servo.h>

Servo myservo;

LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

#define Password_Lenght 5 
int pos = 0;   
int led_green=12;
int led_red=11;
int buzz=10;

int count=0;
int i=1;  

char Data[Password_Lenght]; 
char Master[Password_Lenght]; 
byte data_count = 0, master_count = 0;
//bool Pass_is_good;
char customKey;

const byte ROWS = 4; 
const byte COLS = 4; 
char keys[ROWS][COLS] = {
  
{'1','2','3','A'},
{'4','5','6','B'},
{'7','8','9','C'},
{'*','0','#','D'}
};
byte rowPins[ROWS] = {2,3,4,5}; 
byte colPins[COLS] = {6,7,8,9};  
bool door = true;


Keypad customKeypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS); 
void setup()
{
  
  pinMode(led_green,OUTPUT);
  pinMode(led_red,OUTPUT);
  pinMode(buzz,OUTPUT);
  myservo.attach(13);
  ServoClose();
  lcd.begin(16, 2);
  lcd.print(" Arduino Door");
  lcd.setCursor(0, 1);
  lcd.print("--Lock project--");
  delay(3000);
  lcd.clear();
   for(int j=0;j<4;j++)
    EEPROM.write(j, j+49);
  for(int j=0;j<4;j++)
    Master[j]=EEPROM.read(j);

 
}

void loop()
{
  if (door == 0)
  {
    customKey = customKeypad.getKey();

    if (customKey == '#')

    {
      lcd.clear();
      ServoClose();
      lcd.print("  Door is close");
      delay(3000);
      door = 1;
    }
  }

  else Open();
  if(customKey=='C')
  {
    change();
  }
  
}
void clearData()
{
  while (data_count != 0)
  { 
    Data[data_count--] = 0; 
  }
  return;
}

void ServoOpen()
{
  for (pos = 180; pos >= 90; pos -= 5) { 
    
    myservo.write(pos);              
    delay(15);                       
  }
}

void ServoClose()
{
  for (pos = 90; pos <= 180; pos += 5) { 
    myservo.write(pos);              
    delay(15);                       
  }
}

void Open()
{

  lcd.setCursor(0, 0);
  lcd.print(" Enter Password");
  
  customKey = customKeypad.getKey();
  if (customKey) 
  {
   Data[data_count] = customKey; 
    lcd.setCursor(data_count, 1); 
    lcd.print(Data[data_count]); 
    data_count++; 
      beep();
  }
 

  if (data_count == Password_Lenght - 1) 
  {
    
   if (!strcmp(Data, Master))
    {
      lcd.clear();
      ServoOpen();
      lcd.print("  Door is Open");
       digitalWrite(led_green,HIGH);
  delay(1000);
   digitalWrite(led_green,LOW);
      door = 0;
    }
    else                                       
    {
      lcd.clear();
      lcd.print("  Wrong Password");
     count++;
      delay(1000);
         digitalWrite(led_red,HIGH);
        delay(1000);
       digitalWrite(led_red,LOW);
      door = 1;
      if(count>3*i)
      {
    
     lcd.clear();
        lcd.print("Max limit over");
        digitalWrite(led_red,HIGH);
        delay(1000);
        digitalWrite(led_red,LOW);
        digitalWrite(buzz,HIGH);
        delay(4000);
        digitalWrite(buzz,LOW);
        ++i;

      }
      
      
    }
    clearData();
  }
}
void change()
{
  int j=0;
  lcd.clear();
  lcd.print("UR Current Pass");
 
  lcd.setCursor(0,1);
  while(j<4)
  {
    char key=customKeypad.getKey();
    if(key)
    {
      Data[j++]=key;
      lcd.print(key);
      beep();
    }
    key=0;
  }
  delay(500);

  if((strncmp(Data, Master, 4)))
  {
    lcd.clear();
    lcd.print("Wrong Pass...");
    lcd.setCursor(0,1);
    lcd.print("Better Luck Again");
    Serial.println("Better Luck Again");
    delay(1000);
  }
  else
  {
    j=0;
    lcd.clear();
    lcd.print("Enter New Pass:");
   
    lcd.setCursor(0,1);
    while(j<4)
    {
      char key=customKeypad.getKey();
      if(key)
      {
        Master[j]=key;
        lcd.print(key);
        EEPROM.write(j,key);
        j++;
        beep();
      }
    }
    lcd.print(" Done......");
    Serial.println(" Done......");
    delay(1000);
  }
  lcd.clear();


   lcd.setCursor(0,1);
  customKey=0;
}
void beep()
{
  digitalWrite(buzz, HIGH);
  delay(20);
  digitalWrite(buzz, LOW);
}
