//Used to get keys, here you can configure how works the input without modify the other code





boolean isPressed(char key) {
  char pressedKey = keypad.getKey(); // Obtenir la touche actuellement pressée
  byte state = keypad.getState(); // Obtenir l'état actuel du clavier

  // Imprimer les informations de débogage
  Serial.print("verification= ");
  Serial.print(key);
  Serial.print(" etat = ");
  Serial.print(state);
  Serial.print(" etat = ");
  Serial.print(pressedKey);

  // Vérifier si la touche actuellement pressée correspond à la touche recherchée
  if (pressedKey == key) {
    if (state == PRESSED) { // Si la touche est pressée
      Serial.println(" PRESSED");
      return true;
    } else if (state == HOLD) { // Si la touche est maintenue enfoncée
      Serial.println(" HOLD");
      return true;
    }
  }

  // Si aucune condition n'est remplie, retourner false
  Serial.println(" false");
  return false;
}



//This fuction compare enteredText[8] and password[8] variables
boolean comparePassword(){

  for(int i=0;i<4;i++){
    if(enteredText[i]!=password[i])return false;
  }
  return true;

}

//Set the password variable
void setCode(){

  lcd.setCursor(0, 1);
  for(int i=0;i<4;i++){
    while(1){
      var= getNumber();
      if(var !='x'){
        enteredText[i] = var;

        if (i != 0){
          lcd.setCursor(i-1,1);
          lcd.print("*");
          lcd.print(var);
        }
        else
        {
          lcd.print(var);
        }
        tone(tonepin,2400,30);
        break;
      }
    }
  }
}
void setPass(){
  lcd.setCursor(0, 1);

  for(int i=0;i<4;i++){ 
    while(1){
      var= getNumber();
      if(var !='x'){
        password[i] =  var;
        if (i != 0){
          lcd.setCursor(i-1,1);
          lcd.print("*");
          lcd.print(var);
        }
        else
        {
          lcd.print(var);
        }
        tone(tonepin,2400,30);
        break;
      }
    }  
  }
}


void setNewPass(){

  while(1){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter New Pass 4");
    setPass();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Retype Pass");

    setCode();

    if(comparePassword()){

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Password Set OK!");
      delay(2000); 
      break; 
    }
    else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("ERROR Dont Match!");
      if(soundEnable)tone(tonepin,errorTone,200);
      delay(2000); 

    }
  }

}
//Whait until a button is pressed is is a number return the number 'char' if not return x
char getNumber(){

  var = keypad.waitForKey();

  switch (var) {
  case 'a': 
    return 'x';
    break;
  case 'b': 
    return 'x';
    break;

  case 'c': 
    return 'x';
    break;
  case 'd': 
    return 'x';
    break;
  case '*': 
    menuPrincipal();
    break;
  case '#': 
    return 'x';
    break;
  default:
    return var;
    break;
  }

}






