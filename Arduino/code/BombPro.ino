#include <Wire.h> 
#include <Keypad.h>
//#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal.h>
#include <Adafruit_NeoPixel.h>



// Configuration de la LED WS2812B
#define LED_PIN 33
#define NUM_LEDS 32
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

LiquidCrystal lcd(13, 12, 14, 27, 26, 25);
const byte ROWS = 4;
const byte COLS = 4; 
char keys[ROWS][COLS] = {
  {'1','2','3','a'},
  {'4','5','6','b'},
  {'7','8','9','c'},
  {'*','0','#','d'}
};
byte rowPins[ROWS] = {15, 4, 16, 17}; 
byte colPins[COLS] = {5, 18, 19, 21};

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


char enteredText[8];
char password[4];
int key=-1;
char lastKey;
char var;
boolean passwordEnable=false;

//adminpass+
//const char adminpassword[1412];


char BT_RIGHT = '4';
char BT_UP = 'a';
char BT_DOWN = 'b';
char BT_LEFT = '6';
char BT_SEL = 'd';   
char BT_CANCEL = 'c';
char BT_DEFUSER = 'x';   // not implemented

//leds

const int REDLED = 0;
const int GREENLED = 0;

boolean mosfetEnable = false;
const int mosfet = 22;
//IS VERY IMPORTANT THAT YOU TEST THIS TIME. BY DEFAULT IS IN 1 SEC. THAT IS NOT TOO MUCH. SO TEST IT!
const int MOSFET_TIME = 15000;

//TIME INTS
int GAMEHOURS = 0;
int GAMEMINUTES = 1;
int BOMBMINUTES = 5;
int ACTIVATESECONDS = 5;

int NBSITE = 1;
int SiteExplose = 0;

boolean endGame = false;

boolean sdStatus = false; //search and destroy game enable used in config
boolean saStatus = false; //same but SAbotaghe
boolean doStatus = false; //for DEmolition
boolean start = true;
boolean defuseando;
boolean cancelando;
// SOUND TONES
boolean soundEnable = true;
int tonepin = 32; 
int tonoPitido = 3000;
int tonoAlarma1 = 700;
int tonoAlarma2 = 2600;
int tonoActivada = 1330;
int errorTone = 100;

unsigned long iTime;
unsigned long timeCalcVar;
unsigned long redTime;
unsigned long blueTime;
unsigned long iZoneTime;
byte team=0;

void setup(){
  strip.begin(); // Initialise le strip
  setColor(strip.Color(0, 0, 0)); // Éteint toutes les LEDs
  strip.show();  // Met à jour le strip (pour s'assurer qu'il est éteint au départ)
  lcd.begin(16, 2);
  Serial.begin(115200);
  //  lcd.init();                      
  //  lcd.backlight();
  lcd.setCursor(3,0);
  lcd.print("CELESTIAL");
  lcd.setCursor(4,1);
  lcd.print("AIRSOFT");
  keypad.setHoldTime(50);
  keypad.setDebounceTime(50);
  keypad.addEventListener(keypadEvent);
  delay(3000); 
  pinMode(GREENLED, OUTPUT);     
  pinMode(REDLED, OUTPUT); 
  pinMode(mosfet, OUTPUT);  
  // 
  byte bar1[8] = {
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
  };
  byte bar2[8] = {
    B11000,
    B11000,
    B11000,
    B11000,
    B11000,
    B11000,
    B11000,
    B11000,
  };
  byte bar3[8] = {
    B11100,
    B11100,
    B11100,
    B11100,
    B11100,
    B11100,
    B11100,
    B11100,
  };
  byte bar4[8] = {
    B11110,
    B11110,
    B11110,
    B11110,
    B11110,
    B11110,
    B11110,
    B11110,
  };
  byte bar5[8] = {
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
  };
  byte up[8] = {
    B00000,
    B00100,
    B01110,
    B11111,
    B11111,
    B00000,
    B00000,
  };

  byte down[8] = {
    B00000,
    B00000,
    B11111,
    B11111,
    B01110,
    B00100,
    B00000,
  };
  lcd.createChar(0,bar1);
  lcd.createChar(1,bar2);
  lcd.createChar(2,bar3);
  lcd.createChar(3,bar4);
  lcd.createChar(4,bar5);
  lcd.createChar(5,up);
  lcd.createChar(6,down);
}

void loop(){
  menuPrincipal();
}
void keypadEvent(KeypadEvent key){
  switch (keypad.getState()){
    case PRESSED:
      switch (key){

      }
    break;
    case RELEASED:
      switch (key){
         case 'd': defuseando= false;
         Serial.println("d Releases");
         break;
         case 'c': cancelando=false;
         Serial.println("c Releases");
         break;
      }
    break;
    case HOLD:
      switch (key){
        case 'd': defuseando= true;
        Serial.println("d hold");
        break;
        case 'c': cancelando=true;
        Serial.println("c hold");
        break;
      }
    break;
  }
}

void disarmedSplash(){
  endGame = false;
  digitalWrite(REDLED, LOW); 
  digitalWrite(GREENLED, LOW);
  if(sdStatus || saStatus){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("BOMBE DESARME");
    lcd.setCursor(0,1);
    lcd.print(" FELICITATION");
    digitalWrite(GREENLED, LOW);  
    setColor(strip.Color(0, 255, 0)); 
    delay(5000);
    digitalWrite(GREENLED, HIGH); 


  }
  //end code
  lcd.clear();
  lcd.print("Rejouer?");
  lcd.setCursor(0,1);
  lcd.print("A: Oui    B: Non");
  digitalWrite(REDLED, HIGH);  
  digitalWrite(GREENLED, HIGH); 


  while(1)
  {
    var = keypad.waitForKey();
    if(var == 'a' ){
      tone(tonepin,2400,30);
      //
      if(sdStatus){
        startGameCount();
        search();
      }
      if(saStatus){
        saStatus=true;
        startGameCount();
        start=true; //
        sabotage();
      }
    }  
    if(var == 'b' ){
      tone(tonepin,2400,30);
      menuPrincipal();
      break;
    }  
  } 
}

void explodeSplash(){
  digitalWrite(REDLED, HIGH);  
  digitalWrite(GREENLED, HIGH); 
  setColor(strip.Color(255, 0, 0));


  cls();
  delay(100);
  endGame = false;
  lcd.setCursor(1,0);
  lcd.print("TERRORISE WIN");
  lcd.setCursor(0,1);
  lcd.print("FIN DE PARTIE");
  for(int i = 200; i>0; i--)// 
  {
    tone(tonepin,i);
    delay(20);
  }
  noTone(tonepin);
  if(mosfetEnable){
    activateMosfet(); 
  }
  delay(5000);
  cls();

  //end code
  lcd.print("Rejouer?");
  lcd.setCursor(0,1);
  lcd.print("A: Oui    B: Non");
  while(1)
  {
    var = keypad.waitForKey();
    if(var == 'a' ){
      tone(tonepin,2400,30);
      //
      if(sdStatus){
        startGameCount();
        search();
      }
      if(saStatus){
        saStatus=true;
        startGameCount();
        start=true; //
        sabotage();
      }
    }  
    if(var == 'b' ){
      tone(tonepin,2400,30);
      menuPrincipal();

      break;
    }  
  } 
}

void setColor(uint32_t color) {
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}
