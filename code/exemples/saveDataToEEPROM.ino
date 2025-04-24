#include <Preferences.h>

Preferences preferences;

void setup() {
  Serial.begin(115200);
  preferences.begin("EEPROMData", false);
}

void loop() {
  int positionMoteur = retrieveData();
  Serial.print(positionMoteur);
  positionMoteur++;
  saveData(positionMoteur);
  delay(2000);
}

int retrieveData(){
  int EEPROMDataMoteur = preferences.getInt("positionMoteur", 0); // Provide a default value of 0
  return(EEPROMDataMoteur);
}

void saveData(int i){
  preferences.putInt("positionMoteur", i);    // Store an integer value
  Serial.println("Data sauvegardé.");
}