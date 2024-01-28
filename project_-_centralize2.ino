#include "LiquidCrystal_I2C.h"
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 20, 4);

// INPUT - Keypad
int btnUp = 32;
int btnDown = 33;
int btnOK = 25;
int btnCancel = 26;
int btnAdmin = 27;

// INPUT - Coinslot
int coinslot = 13;

// OUTPUT - PC
int pcRelayPin[10] = {23, 1, 19, 18, 5, 17, 16, 4, 2, 15};

// 
int screen = 0;                 // 0 - idle
                                // 1 - selected pc
                                // 9 - setup mode

int convertTimerM = 5;           // Minutes
int tempConvertTimer = 0;
int selectedPC = 99;
unsigned long pcTimer[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int pcBalance[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
char key;

// Timers
unsigned long timerGlobal = 0;
unsigned long timerButton = 0;
unsigned long timerScreenIdle = 0;


// =================================
// START
// =================================
void setup() 
{
  //
  Serial.begin(9600);
  
  //
  lcd.init();
  lcd.backlight();

  //
  pinMode(btnUp, INPUT_PULLUP);
  pinMode(btnDown, INPUT_PULLUP);
  pinMode(btnOK, INPUT_PULLUP);
  pinMode(btnCancel, INPUT_PULLUP);
  pinMode(btnAdmin, INPUT_PULLUP);

  // 
  pinMode(coinslot, INPUT_PULLUP);

  //
  for (int x = 0; x < 10; x++)
  {
    pinMode(pcRelayPin[x], OUTPUT);
  }
}


// =================================
// LOOP
// =================================
void loop()
{
  //
  int btnUpVal = digitalRead(btnUp);
  int btnDownVal = digitalRead(btnDown);
  int btnOKVal = digitalRead(btnOK);
  int btnCancelVal = digitalRead(btnCancel);
  int btnAdminVal = digitalRead(btnAdmin);

  // coinslot
  int coinslotVal = digitalRead(coinslot);

  // Idle
  if (screen == 0)
  {
    // display
    lcd.setCursor(0, 0);
    lcd.print("=== PC  Coinslot ===");
    lcd.setCursor(0, 1);
    lcd.print("   Paano  gamitin   ");
    lcd.setCursor(0, 2);
    lcd.print("1.Piliin ang PC unit");
    lcd.setCursor(0, 3);
    lcd.print("2.Maghulog sa C.slot");
    
    // control
    {
      //
      if (!btnUpVal)
      { 
        if ((millis() - timerButton) > 200)
        {
          timerButton = millis();
          timerScreenIdle = millis();

          //
          selectedPC = 0;
          screen = 1;
        }
      }

      //
      if (!btnDownVal)
      { 
        if ((millis() - timerButton) > 200)
        {
          timerButton = millis();
          timerScreenIdle = millis();

          //
          selectedPC = 0;
          screen = 1;
        }
      }

      //
      if (!btnOKVal)
      { 
        
      }

      //
      if (!btnCancelVal)
      { 
        
      }
    }
  }

  // Selected
  if (screen == 1)
  {
    // display
    {
      lcd.setCursor(0, 0);
      lcd.print("PC Number:       " + ConvertNumber3(selectedPC + 1));
      lcd.setCursor(0, 1);
      lcd.print("Remaining:    " + ConvertTimer(pcTimer[selectedPC]));
      lcd.setCursor(0, 2);
      lcd.print("1 Peso:  " + ConvertNumber3(convertTimerM) + " Minutes");
      lcd.setCursor(0, 3);
      lcd.print("Balance:         " + ConvertNumber3(pcBalance[selectedPC]));
    }

    // control
    {
      //
      if (!btnUpVal)
      { 
        if ((millis() - timerButton) > 200)
        {
          timerButton = millis();
          timerScreenIdle = millis();

          //
          if (selectedPC >= 9)
          {
            selectedPC = 0;
            screen = 1;
          }
          else
          {
            selectedPC = selectedPC + 1;
          }
        }
      }

      //
      if (!btnDownVal)
      { 
        if ((millis() - timerButton) > 200)
        {
          timerButton = millis();
          timerScreenIdle = millis();

          //
          if (selectedPC <= 0)
          {
            selectedPC = 9;
            screen = 1;
          }
          else
          {
            selectedPC = selectedPC - 1;
          }
        }
      }

      //
      if (!btnOKVal)
      { 
        if ((millis() - timerButton) > 200)
        {
          timerButton = millis();
          timerScreenIdle = millis();

          //
          int newTime = pcBalance[selectedPC] * (convertTimerM * 60);
          pcBalance[selectedPC] = 0;
          pcTimer[selectedPC] = pcTimer[selectedPC] + newTime;
        }
      }

      //
      if (!btnCancelVal)
      { 
        if ((millis() - timerButton) > 200)
        {
          timerButton = millis();
          timerScreenIdle = millis();

          //
          selectedPC = 99;
          screen = 0;
        }
      }
    
      // coinslot
      if (coinslotVal)
      {
        timerScreenIdle = millis();
        pcBalance[selectedPC] = pcBalance[selectedPC] + 1;
        delay(20);
      }
    }
  }

  // Setup
  if (screen == 9)
  {
    // display
    lcd.setCursor(0, 0);
    lcd.print("= Adjust Time/Piso =");
    lcd.setCursor(0, 1);
    lcd.print("1 Peso:  " + ConvertNumber3(tempConvertTimer) + " Minutes");
    lcd.setCursor(0, 2);
    lcd.print(" [2]+1min  [8]-1min ");
    lcd.setCursor(0, 3);
    lcd.print(" [*] OK  [#] Cancel ");

    // control
    {
      //
      if (!btnUpVal)
      { 
        if ((millis() - timerButton) > 200)
        {
          timerButton = millis();
          timerScreenIdle = millis();

          //
          if (tempConvertTimer < 999)
          {
            tempConvertTimer = tempConvertTimer + 1;
          }
        }
      }

      //
      if (!btnDownVal)
      { 
        if ((millis() - timerButton) > 200)
        {
          timerButton = millis();
          timerScreenIdle = millis();

          //
          if (tempConvertTimer > 1)
          {
            tempConvertTimer = tempConvertTimer - 1;
          }
        }
      }

      //
      if (!btnOKVal)
      { 
        if ((millis() - timerButton) > 200)
        {
          timerButton = millis();
          timerScreenIdle = millis();

          //
          convertTimerM = tempConvertTimer;
          selectedPC = 99;
          screen = 0;
        }
      }

      //
      if (!btnCancelVal)
      { 
        if ((millis() - timerButton) > 200)
        {
          timerButton = millis();
          timerScreenIdle = millis();

          //
          selectedPC = 99;
          screen = 0;
        }
      }
    }
  }

  // Timer
  if ((millis() - timerGlobal) > 1000)
  {
    timerGlobal = millis();

    // pc timer
    for (int x = 0; x < 10; x++)
    {
      if (pcTimer[x] > 0)
      {
        pcTimer[x] = pcTimer[x] - 1;
        digitalWrite(pcRelayPin[x], HIGH);
      }
      else
      {
        pcTimer[x] = 0;
        digitalWrite(pcRelayPin[x], LOW);
      }
    }
  }

  // Display Idle
  if ((millis() - timerScreenIdle) > 10000)
  {
    //
    selectedPC = 99;
    screen = 0;
  }

  // 
  if (!btnAdminVal)
  { 
    timerScreenIdle = millis();
    tempConvertTimer = convertTimerM;
    screen = 9;
  }
}


// =================================
// FUNCTION
// =================================
String ConvertNumber3(int x) { // result will be 000

  String newx = String(x);

  if (x < 100)
  {
    newx = "0" + newx;
  }

  if (x < 10)
  {
    newx = "0" + newx;
  }

  if (x <= 0)
  {
    newx = "000";
  }

  return newx;
}

String ConvertTimer(unsigned long x) { // result will be 000:00

  unsigned long gmin = x / 60;
  unsigned long gsec = x % 60;
  String gmins = "";
  String gsecs = "";

  // min
  {
    if (gmin > 999)
    {
      gmins = "999";
    }
    
    if (gmin < 999)
    {
      gmins = String(gmin);
    }
    
    if (gmin < 100)
    {
      gmins = "0" + String(gmin);
    }

    if (gmin < 10)
    {
      gmins = "00" + String(gmin);
    }

    if (gmin <= 0)
    {
      gmins = "000";
    }
  } 

  // sec
  {
    gsecs = String(gsec);
    
    if (gsec < 10)
    {
      gsecs = "0" + String(gsec);
    }

    if (gsec <= 0)
    {
      gsecs = "00";
    }
  }
  
  String newx = gmins + ":" + gsecs;

  return newx;
}
