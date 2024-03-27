//Inclut la librairie Améthyste_LSM6DS3
#include <Amethyste_LSM6DS3.h>

// Instancie un capteur LSM6DS3
LSM6DS3 sensor;

//Variables contenant les mesures de l'accéléromètre et du gyroscope
float accX,accY,accZ=0;

void setup(){

  // Active la liaison série pour pouvoir afficher les données dans le moniteur série
  Serial.begin(115200);

  // vérifie si le capteur LSM6DS3 est détecté avec la bonne adresse
  while(!sensor.begin()){
    // Si le capteur n'est pas détecté, revérifier le câblage (inversion SCL, SDA possible)
    Serial.println("Capteur non detecté");
    delay(500);
  }

  // Active l'accéléromètre
  sensor.toggleAccel(true);
}

void loop(){
  
  // Lit les valeurs de l'accéléromètre
  sensor.readAccel(&accX,&accY,&accZ);

  // Affiche les mesures de l'accéléromètre
  Serial.print("\nAccel :");
  Serial.print("  X = ");
  Serial.print(accX);
  Serial.print("  Y = ");
  Serial.print(accY);
  Serial.print("  Z = ");
  Serial.print(accZ);
  delay(500);
}
