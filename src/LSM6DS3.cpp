
#include "LSM6DS3.h"

LSM6DS3::LSM6DS3() {}

bool LSM6DS3::begin() {
  Wire.begin(); // Initialisation de la communication I2C
  delay(15); // Pause pour permettre au capteur de s'initialiser
  return readRegister(LSM6DS3_WHO_AM_I) == LSM6DS3_ID; // Vérification de l'identifiant du capteur
}

bool LSM6DS3::end() {
  Wire.end(); // Fin de la communication I2C
}

void LSM6DS3::toggleAccel(bool enable) {
  if(enable){
    writeRegister(LSM6DS3_CTRL1_XL,0x60); // Activation de l'accéléromètre avec une sensibilité de ±2g
  }else{
    writeRegister(LSM6DS3_CTRL1_XL,0x00); // Désactivation de l'accéléromètre
  }
}

void LSM6DS3::toggleGyro(bool enable) {
  if(enable){
    writeRegister(LSM6DS3_CTRL2_G,0x60); // Activation du gyroscope avec une sensibilité de ±245dps
  }else{
    writeRegister(LSM6DS3_CTRL2_G,0x00); // Désactivation du gyroscope
  }
} 

void LSM6DS3::readAccel(float *x, float *y, float *z) {
  int16_t rawX, rawY, rawZ;
  uint8_t data[6];
  uint8_t range = readRegister(LSM6DS3_CTRL1_XL); // Lecture de la configuration de l'accéléromètre
  double factor=0; // Facteur de conversion des données brutes en unités physiques (g)
  if((range | 0xF3) == 0xF3){
    factor = 0.061; // Facteur pour la plage ±2g
  }else if((range | 0xF3) == 0xFB){
    factor = 0.122; // Facteur pour la plage ±4g
  }else if((range | 0xF3) == 0xFF){
    factor = 0.244;  // Facteur pour la plage ±8g
  }else{
    factor = 0.488; // Facteur pour la plage ±16g
  }
  readRegisters(LSM6DS3_OUTX_L_XL, data, 6); // Lecture des données brutes de l'accéléromètre
  rawX = (data[1] << 8) | data[0]; // Conversion des données brutes en entiers signés
  rawY = (data[3] << 8) | data[2];
  rawZ = (data[5] << 8) | data[4];

  *x = rawX * factor; // Conversion des données brutes en unités physiques (g)
  *y = rawY * factor;
  *z = rawZ * factor;
}

void LSM6DS3::readGyro(float *x, float *y, float *z) {
  int16_t rawX, rawY, rawZ;
  uint8_t data[6];
  uint8_t range = readRegister(LSM6DS3_CTRL2_G); // Lecture de la configuration du gyroscope
  double factor=0; // Facteur de conversion des données brutes en unités physiques (dps)
  if((range | 0xFD) == 0xFF){
    factor = 4.375; // Facteur pour la plage ±245dps
  }else if((range | 0xF3) == 0xF3){
    factor = 8.75; // Facteur pour la plage ±500dps
  }else if((range | 0xF3) == 0xF7){
    factor = 17.5; // Facteur pour la plage ±1000dps
  }else if((range | 0xF3) == 0xFB){
    factor = 35; // Facteur pour la plage ±2000dps
  }else{
    factor=70; // Facteur pour la plage ±4000dps
  }
  readRegisters(LSM6DS3_OUTX_L_G, data, 6); // Lecture des données brutes du gyroscope
  rawX = (data[1] << 8) | data[0]; // Conversion des données brutes en entiers signés
  rawY = (data[3] << 8) | data[2];
  rawZ = (data[5] << 8) | data[4];

  *x = rawX * factor; // Conversion des données brutes en unités physiques (dps)
  *y = rawY * factor;
  *z = rawZ * factor;
}

float LSM6DS3::readTemperature() {
  int16_t rawTemp;
  uint8_t data[2];
  readRegisters(LSM6DS3_OUT_TEMP_L, data, 2); // Lecture des données de température depuis le capteur
  rawTemp = (data[1] << 8) | data[0]; // Conversion des données brutes en entiers signés
  return rawTemp / 256.0 + 25.0; // Formule de conversion en dégres Celsius (C°)
}

void LSM6DS3::readAccelRaw(float *rawX, float *rawY, float *rawZ) {
  uint8_t data[6];
  readRegisters(LSM6DS3_OUTX_L_XL, data, 6); // Lecture des données brutes de l'accéléromètre
  *rawX = (data[1] << 8) | data[0]; // Conversion des données brutes en entiers signés
  *rawY = (data[3] << 8) | data[2];
  *rawZ = (data[5] << 8) | data[4];
}

void LSM6DS3::readGyroRaw(float *rawX, float *rawY, float *rawZ) {
  uint8_t data[6];
  readRegisters(LSM6DS3_OUTX_L_G, data, 6); // Lecture des données brutes du gyroscope
  *rawX = (data[1] << 8) | data[0]; // Conversion des données brutes en entiers signés
  *rawY = (data[3] << 8) | data[2];
  *rawZ = (data[5] << 8) | data[4];
}

float LSM6DS3::readTemperatureRaw() {
  int16_t rawTemp;
  uint8_t data[2];
  readRegisters(LSM6DS3_OUT_TEMP_L, data, 2); // Lecture des données brutes de température depuis le capteur
  rawTemp = (data[1] << 8) | data[0]; // Conversion des données brutes en entiers signés
  return rawTemp;
}

void LSM6DS3::setAccelScale(uint8_t scale = 0x00) {
  uint8_t previousValue = readRegister(LSM6DS3_CTRL1_XL); // Lecture valeur précédente du registre
  writeRegister(LSM6DS3_CTRL1_XL, (previousValue & ~0x0C) | (scale & 0x0C)); // Mise à jour du registre de contrôle de l'accéléromètre avec la nouvelle échelle
}

void LSM6DS3::setGyroScale(uint8_t scale = 0x00) {
  uint8_t previousValue = readRegister(LSM6DS3_CTRL2_G); // Lecture valeur précédente du registre
  writeRegister(LSM6DS3_CTRL2_G, (previousValue & ~0x0E) | (scale & 0x0E)); // Mise à jour du registre de contrôle du gyroscope avec la nouvelle échelle
}

void LSM6DS3::setAccelDataRate(uint8_t rate = 0x60){
  uint8_t previousValue = readRegister(LSM6DS3_CTRL1_XL); // Lecture valeur précédente du registre
  uint8_t trueRate = rate; 
  if(rate > 0x0A){ // Vérification de la plage valide pour le taux de données de l'accéléromètre
    trueRate = 0x0A;
  }
  writeRegister(LSM6DS3_CTRL1_XL,(previousValue & 0x0F) | (trueRate << 4)); // Mise à jour du registre de contrôle de l'accéléromètre avec le nouveau taux de données
}

void LSM6DS3::setGyroDataRate(uint8_t rate = 0x60){
  uint8_t previousValue = readRegister(LSM6DS3_CTRL2_G); // Lecture valeur précédente du registre
  uint8_t trueRate = rate;
  if(rate > 0x0A){ // Vérification de la plage valide pour le taux de données de l'accéléromètre
    trueRate = 0x0A;
  }
  writeRegister(LSM6DS3_CTRL2_G,(previousValue & 0x0F) | (trueRate << 4)); // Mise à jour du registre de contrôle du gyroscope avec le nouveau taux de données
}

void LSM6DS3::toggleTapDetection(bool enable) {
  uint8_t previousState=readRegister(LSM6DS3_MD1_CFG); // Lecture valeur précédente du registre
  if(enable){
    writeRegister(LSM6DS3_MD1_CFG,previousState | 0x40); // Activation de la détection de tap
    previousState=readRegister(LSM6DS3_TAP_CFG);
    writeRegister(LSM6DS3_TAP_CFG,previousState | 0x8E); // Configuration du registre de configuration de tap
    setTapSensibility(0x07); // Définition de la sensibilité de tap
  }else{ 
    writeRegister(LSM6DS3_MD1_CFG,previousState & ~0x40); // Désactivation de la détection de tap
    previousState=readRegister(LSM6DS3_TAP_CFG);
    writeRegister(LSM6DS3_TAP_CFG,previousState & ~0x0E); // Réinitialisation du registre de configuration de tap
  }
}

bool LSM6DS3::readTapDetection() {
  uint8_t tapState=readRegister(LSM6DS3_TAP_SRC); // Lecture de l'état actuel du registre
  return tapState != 0; // Retourne true si un tap est détecté, sinon false
}
void LSM6DS3::setTapSensibility(uint8_t sensibility = 0x00) {
  uint8_t previousState=readRegister(LSM6DS3_TAP_THS_6D); // Lecture valeur précédente du registre
  writeRegister(LSM6DS3_TAP_THS_6D,(sensibility & 0x0F) | (previousState & 0xF0)); // Mise à jour du registre de sensibilité de tap avec la nouvelle valeur
}
void LSM6DS3::toggleFreeFallDetection(bool enable) {
  uint8_t previousState=readRegister(LSM6DS3_MD1_CFG); // Lecture valeur précédente du registre
  if(enable){
    writeRegister(LSM6DS3_MD1_CFG,previousState | 0x10); // Activation de la détection de chute libre
    previousState=readRegister(LSM6DS3_TAP_CFG); // Lecture valeur précédente du registre
    writeRegister(LSM6DS3_TAP_CFG,previousState | 0x80); // Configuration du registre de configuration de chute libre
    setFreeFallParameters(0x00,0x0A); // Définition des paramètres de chute libre
  }else{ 
    writeRegister(LSM6DS3_MD1_CFG,previousState & ~0x10); // Désactivation de la détection de chute libre
  } 
}

bool LSM6DS3::readFreeFallDetection() {
  uint8_t fallState=readRegister(LSM6DS3_WAKE_UP_SRC); // Lecture de l'état actuel du registre de détection de chute libre
  return (fallState & 0x20) != 0; // Retourne true si une chute libre est détectée, sinon false
}

void LSM6DS3::setFreeFallParameters(uint8_t minDuration = 0x00, uint8_t threshold = 0x00){
  
  writeRegister(LSM6DS3_FREE_FALL,(minDuration & 0xF8) | (threshold & 0x07)); // Configuration des paramètres de chute libre avec la durée minimale et le seuil spécifié
}

void LSM6DS3::togglePedometer(bool enable) {
  uint8_t previousState=readRegister(LSM6DS3_INT1_CTRL); // Lecture valeur précédente du registre
  if(enable){
    writeRegister(LSM6DS3_INT1_CTRL,previousState | 0x80); // Routage de l'événement du podomètre vers l'interruption INT1
    previousState=readRegister(LSM6DS3_CTRL10_C); // Lecture valeur précédente du registre
    writeRegister(LSM6DS3_CTRL10_C,previousState | 0x14);  // Activation des fonctions embarquées et du podomètre
  }else{   
    writeRegister(LSM6DS3_INT1_CTRL,previousState & ~0x80); // Désactivation du podomètre sur l'interruption INT1
    previousState=readRegister(LSM6DS3_CTRL10_C); // Lecture valeur précédente du registre
    writeRegister(LSM6DS3_CTRL10_C,previousState & ~0x14); // Désactivation des fonctions embarquées et du podomètre
  }
}

bool LSM6DS3::readPedometer() {
  uint8_t tapState=readRegister(LSM6DS3_FUNC_SRC1); // Lecture de l'état actuel du registre de fonction SRC1 pour la détection du podomètre
  return (tapState & 0x10) != 0; // Retourne vrai si le podomètre détecte un pas, sinon faux
}

void LSM6DS3::setPedometerParameters(uint8_t threshold = 0x10){
  uint8_t accPrevState = readRegister(LSM6DS3_CTRL1_XL); // Lecture valeurs précédentes des registres
  uint8_t gyroPrevState = readRegister(LSM6DS3_CTRL2_G);
  setAccelDataRate(0);// Désactivation de l'accéléromètre et du gyroscope pour modifier les paramètres embarqués
  setGyroDataRate(0);
  
  writeRegister(LSM6DS3_FUNC_CFG_ACCESS,0x80); // Activation de la modification des paramètres des fonctions embarquées
  writeRegister(LSM6DS3_CONFIG_PEDO_THS_MIN, threshold & 0x1F);  // Modification de la valeur du seuil du podomètre
  writeRegister(LSM6DS3_FUNC_CFG_ACCESS, 0x00); // Désactivation de la modification des paramètres des fonctions embarquées 
  writeRegister(LSM6DS3_CTRL1_XL, accPrevState);// Réactivation de l'accéléromètre et du gyroscope
  writeRegister(LSM6DS3_CTRL2_G, gyroPrevState);
}

void LSM6DS3::toggleWristTilt(bool enable) {
  uint8_t previousState=readRegister(LSM6DS3_CTRL10_C); // Lecture valeur précédente du registre
  if(enable){
    writeRegister(LSM6DS3_CTRL10_C,previousState | 0x84); // Activation fonctions embarquées et détection d'inclinaison du poignet
    previousState = readRegister(LSM6DS3_DRDY_PULSE_CFG_G);
    writeRegister(LSM6DS3_DRDY_PULSE_CFG_G, previousState | 0x01); // Activation interruption d'inclinaison du poignet sur INT2
    previousState = readRegister(LSM6DS3_CTRL4_C);
    writeRegister(LSM6DS3_CTRL4_C,previousState | 0x20); // Mise des interrruptions de INT2 sur INT1

  }else{   
    writeRegister(LSM6DS3_CTRL10_C,previousState & ~0x84); // Désactivation détection d'inclinaison du poignet
    previousState = readRegister(LSM6DS3_DRDY_PULSE_CFG_G);
    writeRegister(LSM6DS3_DRDY_PULSE_CFG_G, previousState & ~0x01); // Désactivation interruption d'inclinaison du poignet sur INT2
  }
}

bool LSM6DS3::readWristTilt() {
  uint8_t wristTiltState=readRegister(LSM6DS3_FUNC_SRC2);// Lecture de l'état actuel du registre de fonction SRC2 pour la détection d'inclinaison du poignet
  return (wristTiltState & 0x01) != 0; // Retourne vrai si une inclinaison du poignet est détectée, sinon faux
}

void LSM6DS3::setWristTiltParameters(uint8_t latency = 0x0F, uint8_t threshold = 0x20, uint8_t mask = 0xC0){
  uint8_t accPrevState = readRegister(LSM6DS3_CTRL1_XL); // Lecture valeur précédente du registre
  if(accPrevState & 0xF0 == 0){ // Annulation changement si accéléromètre désactivé
    return;
  }
  uint8_t ctrl10PrevState = readRegister(LSM6DS3_CTRL10_C); // Lecture valeur précédente du registre
  writeRegister(LSM6DS3_CTRL10_C,0x04); // Activation des fonctions embarquées
  delay(50);
  writeRegister(LSM6DS3_CTRL10_C,0x00 ); // Désactivation des fonctions embarquées
  writeRegister(LSM6DS3_FUNC_CFG_ACCESS,0xA0);// Active la modification des fonctions embarquées

  writeRegister(LSM6DS3_A_WRIST_TILT_LAT, latency);// Modification des paramètres de latence, seuil et des axes détectés
  writeRegister(LSM6DS3_A_WRIST_TILT_THS, threshold);
  writeRegister(LSM6DS3_A_WRIST_TILT_Mask, mask & 0xFC);

  writeRegister(LSM6DS3_FUNC_CFG_ACCESS,0x00);// Désactive la modification des fonctions embarquées
  writeRegister(LSM6DS3_CTRL10_C, ctrl10PrevState); // Retourne le registre à son état de départ
}

void LSM6DS3::toggleInterruptLogicLevel(bool isActiveHigh) {
  uint8_t previousState = readRegister(LSM6DS3_CTRL3_C); // Lecture valeur précédente du registre
  if(isActiveHigh){
    writeRegister(LSM6DS3_CTRL3_C, previousState & ~0x20); // Front montant pour les interruptions
  }else{
    writeRegister(LSM6DS3_CTRL3_C,previousState | 0x20); // Front descendant pour les interruptions
  }
}
uint8_t LSM6DS3::readRegister(uint8_t reg) {
  Wire.beginTransmission(LSM6DS3_address);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.requestFrom((uint8_t)LSM6DS3_address,(uint8_t) 1);
  return Wire.read();
}

void LSM6DS3::writeRegister(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(LSM6DS3_address);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

void LSM6DS3::readRegisters(uint8_t reg, uint8_t *data, uint8_t len) {
  Wire.beginTransmission(LSM6DS3_address);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.requestFrom((uint8_t)LSM6DS3_address, len);
  for (uint8_t i = 0; i < len; i++) {
    data[i] = Wire.read();
  }
}