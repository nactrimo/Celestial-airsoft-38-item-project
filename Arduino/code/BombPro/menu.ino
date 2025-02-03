//##################MENUS###############################

void menuPrincipal(){   //MAIN MENU

 digitalWrite(GREENLED, HIGH); 
 digitalWrite(REDLED, HIGH); 
 strip.fill(strip.Color(0, 0, 255), 0, 11);    // Bleu pour les 11 premières LEDs
 strip.fill(strip.Color(255, 255, 255), 11, 11); // Blanc pour les 10 suivantes
 strip.fill(strip.Color(255, 0, 0), 21, 11);    // Rouge pour les 10 dernières
 strip.show();
  sdStatus = false; //search and destroy game enable used in config
  saStatus = false; //same but SAbotaghe
  doStatus = false; //for DOmination
  
  //Draw menu
  cls();//clear lcd and set cursor to 0,0
  int i=0;
  char* menu1[]={
    "S&D CSGO","Sabotage","Domination", "Bomb Setup"        }; // HERE YOU CAN ADD MORE ITEMS ON THE MAIN MENU
  lcd.print(menu1[i]);
  lcd.setCursor(15,1);
  checkArrows(i,2);
  while(1){

    var = keypad.waitForKey();
    if(var == BT_UP && i>0){
      tone(tonepin,2400,100);
      i--;
      cls();
      lcd.print(menu1[i]);
      checkArrows(i,3);
      delay(100);
    }
    if(var == BT_DOWN && i<3){
      tone(tonepin,2400,100);
      i++;
      cls(); 
      lcd.print(menu1[i]);    
      checkArrows(i,3);
      delay(100);

    }

    if(var == BT_SEL){
      tone(tonepin,2400,100);
      cls();
      switch (i){

      case 0:
        sdStatus=true;
        configQuickGame();
        startGameCount();
        search();
        break;
      case 1: 
        saStatus=true;
        configQuickGame();
        startGameCount();
        sabotage();
        break;
      case 2:
        doStatus=true;
        configQuickGame();
        startGameCount();
        domination();
        break;
      case 3:
        config();
        break;
        
       
      }
    }
  }
}

void config(){
  //Draw menu
  lcd.clear();
  lcd.setCursor(0, 0);
  int i=0;
  char* menu2[]={
    "GAME CONFIG","MOSFET TEST", "PATCH LED","V BATT"}; // HERE YOU CAN ADD MORE ITEMS ON THE MENU
  delay(500);
  lcd.print(menu2[i]);
  checkArrows(i,3);

  while(1){
    var=keypad.waitForKey();
    if(var == BT_UP && i>0){
      tone(tonepin,2400,100);
      i--;
      lcd.clear();  
      lcd.print(menu2[i]);
      checkArrows(i,3);
      delay(50);

    }
    if(var == BT_DOWN && i<3){
      tone(tonepin,2400,100);
      i++;
      lcd.clear();  
      lcd.print(menu2[i]);
      checkArrows(i,3);
      delay(50);
    }
    if(var == BT_CANCEL){
      tone(tonepin,2400,100);
      menuPrincipal();
    }
    if(var == BT_SEL){
      tone(tonepin,2400,100);
      lcd.clear();
      switch (i){

      case 0:
        //gameConfigMenu
        lcd.print("menu non config !");
        delay(2000);
        config();
        break;

      case 1:
        //final time test menu
        lcd.print("MOSFET TEST");

        digitalWrite(mosfet, HIGH);
        delay(500);
        digitalWrite(mosfet, LOW);
        delay(1000);
        digitalWrite(mosfet, HIGH);
        delay(500);
        digitalWrite(mosfet, LOW);
        delay(100);
        config();
        break;

      case 2:
      //patch led
        cls();
        lcd.print("PATCH LED");
        setColor(strip.Color(0, 0, 0)); // off
        delay(2000);   // wait for 2 second
        cls();
        lcd.print("FULL white!");
        setColor(strip.Color(255, 255, 255)); // blanc
        delay(2000);   // wait for 2 second
        cls();
        lcd.print("magenta");
        setColor(strip.Color(127, 0, 127)); // Magenta
        delay(2000);
        strip.fill(strip.Color(0, 0, 255), 0, 10);    // Bleu pour les 10 premières LEDs
        strip.fill(strip.Color(255, 255, 255), 10, 10); // Blanc pour les 10 suivantes
        strip.fill(strip.Color(255, 0, 0), 20, 10);    // Rouge pour les 10 dernières
        strip.show();
        config();
        break;     

      case 3:
      //Test tension batterie
       cls();


        // Lire plusieurs fois et faire une moyenne
        float sommeADC = 0;
        int nbMesures = 60;  // Nombre de lectures pour la moyenne
        for (int i = 0; i < nbMesures; i++) {
        sommeADC += analogRead(pinBatterie);
        delay(15);  // Petite pause pour éviter le bruit
        }
        float valeurADC = sommeADC / nbMesures;

        // Conversion ADC → tension lue
        float tensionLue = valeurADC * (Vref / 4095.0);  

        // Calcul de la tension réelle de la batterie
        float tensionBatterie = tensionLue * (R1 + R2) / R2;  

        // Calcul du pourcentage de batterie
        int pourcentage = (tensionBatterie - Vmin) / (Vmax - Vmin) * 100;
        if (pourcentage > 100) pourcentage = 100;
        if (pourcentage < 0) pourcentage = 0;

        // Afficher la tension lue
        lcd.setCursor(0,0);
        lcd.print("Batterie:" + String(tensionBatterie) + "V");
        Serial.print("Tension Batterie: ");
        lcd.setCursor(0,1);
        lcd.print("Charge:" + String(pourcentage) + "%");

        delay(3000);
        config();
        break;    

      }
    }
  }
}




void configQuickGame(){

  cls();
  //GAME TIME
  if(sdStatus || doStatus || saStatus){
    lcd.print("Game Time:");
    lcd.setCursor(0,1);
    checkArrows(1,2);
    delay(400);

    while(1){


      lcd.setCursor(0,1);  
      lcd.print(GAMEMINUTES);  
      lcd.print("   Minutes");

      var = keypad.waitForKey();

      if(var == 'a' && GAMEMINUTES<180){
        tone(tonepin,2400,100);
        GAMEMINUTES++;
        delay(50);
      }
      if(var == 'b' && GAMEMINUTES>1){
        tone(tonepin,2400,100);
        GAMEMINUTES--;
        delay(50);
      }
      if(var == 'c') // Cancel or Back Button :')
      {
        tone(tonepin,2400,100);
        sdStatus = false; //search and destroy game enable used in config
        saStatus = false; //same but SAbotaghe
        doStatus = false; //for DEmolition
        menuPrincipal();
      } 
      if(var == 'd') // Cancel or Back Button :')
      {
        tone(tonepin,2400,100);
        break;
      }        
    }
    tone(tonepin,2400,100);
    cls();
  }
  //BOMB TIME
  if(sdStatus || saStatus){
    lcd.print("Bomb Time:");
    lcd.setCursor(0,1);
    checkArrows(1,2);
    delay(400);

    while(1){
      lcd.setCursor(0,1);  
      lcd.print(BOMBMINUTES);  
      lcd.print(" Minutes");
      var = keypad.waitForKey();


      if(var == 'b' && BOMBMINUTES>1){
        tone(tonepin,2400,100);
        BOMBMINUTES--;
        delay(50);
      }
      if(var == 'a' && BOMBMINUTES<20){
        tone(tonepin,2400,100);
        BOMBMINUTES++;
        delay(50);
      }
      if(var == 'c') // Cancel or Back Button :')
      {
        tone(tonepin,2400,100);
        sdStatus = false; //search and destroy game enable used in config
        saStatus = false; //same but SAbotaghe
        doStatus = false; //for DEmolition
        menuPrincipal();
      } 
      if(var == 'd') // Cancel or Back Button :')
      {
        tone(tonepin,2400,100);
        break;
      }          
    }
    tone(tonepin,2400,100);
  }
  cls();
  //ARMING TIME
  if(sdStatus || doStatus || saStatus){
    lcd.print("Arm time:");
    lcd.setCursor(0,1);
    checkArrows(1,2);
    delay(400);

    while(1){
      lcd.setCursor(0,1);  
      lcd.print(ACTIVATESECONDS);  
      lcd.print(" secondes");
      var = keypad.waitForKey();
      
      if(var == 'b' && ACTIVATESECONDS>5){
        tone(tonepin,2400,100);
        ACTIVATESECONDS--;
        delay(50);
      }   
      if(var == 'a' && ACTIVATESECONDS<30){
        ACTIVATESECONDS++;
        tone(tonepin,2400,100);
        delay(50);
      }
      if(var == 'c') // Cancel or Back Button :')
      {
        tone(tonepin,2400,100);
        sdStatus = false; //search and destroy game enable used in config
        saStatus = false; //same but SAbotaghe
        doStatus = false; //for DEmolition
        menuPrincipal();
      } 
      if(var == 'd') // Cancel or Back Button :')
      {
        tone(tonepin,2400,100);
        break;
      }     
    }
    tone(tonepin,2400,100);
    ACTIVATESECONDS-=1; // Just a fix

  }
  //Want sound??
  if(sdStatus || saStatus || doStatus){
    cls();
  lcd.print("Active Buzzer?");
  lcd.setCursor(0,1);
  lcd.print("A:  Oui  B: Non");


    while(1)
    {
      var = keypad.waitForKey();
      if(var == 'a' ){
        soundEnable=true;
        tone(tonepin,2400,100);
        break;
      }  

      if(var == 'b' ){
        soundEnable=false;
        tone(tonepin,2400,100);
        break;
      }  
    }
  } 
  //Activate Mosfet at Terrorist game ends??? Boom!

  if(sdStatus || saStatus){
    cls();
    lcd.print("Active relais?");
    lcd.setCursor(0,1);
    lcd.print("A:  Oui  B: Non");
    while(1)
    {
      var = keypad.waitForKey();
      if(var == 'a' ){
        mosfetEnable=true;
        tone(tonepin,2400,100);
        break;
      }  
      if(var == 'b' ){
        mosfetEnable=false;
        tone(tonepin,2400,100);
        break;
      }  
    } 
  }
  //You Want a password enable-disable game?
  if(sdStatus || saStatus){
    cls();
    lcd.print("Definir un code?");
    lcd.setCursor(0,1);
    lcd.print("A:  Oui  B: Non");

    while(1)
    {
      var = keypad.waitForKey();
      if(var == 'a' ){
        tone(tonepin,2400,100);
        setNewPass();
        passwordEnable = true;
        break;
      }  
      if(var == 'b' ){
        tone(tonepin,2400,100);
        passwordEnable = false;
        break;
      }  
    } 
    tone(tonepin,2400,100);
  }  

  //Multiple site
  if(saStatus){
    cls();
    lcd.print("NB de ZONE:");
    lcd.setCursor(0,1);
    checkArrows(1,2);
    delay(400);

    while(1){
      lcd.setCursor(0,1);  
      lcd.print(NBSITE);  
      lcd.print(" SITE(S)");
      var = keypad.waitForKey();


      if(var == 'b' && NBSITE>1){
        tone(tonepin,2400,100);
        NBSITE--;
        delay(50);
      }
      if(var == 'a' && NBSITE<10){
        tone(tonepin,2400,100);
        NBSITE++;
        delay(50);
      }
      if(var == 'c') // Cancel or Back Button :')
      {
        tone(tonepin,2400,100);
        sdStatus = false; //search and destroy game enable used in config
        saStatus = false; //same but SAbotaghe
        doStatus = false; //for DEmolition
        menuPrincipal();
      } 
     if( var == 'd') // Cancel or Back Button :')
      {
        tone(tonepin,2400,100);
        break;
      }          
    }
    tone(tonepin,2400,100);
  } 
  //Continue the game :D
}




