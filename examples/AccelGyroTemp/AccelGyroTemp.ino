//Inclut la librairie Améthyste_LSM6DS3
#include <Amethyste_LSM6DS3.h>

// Instancie un capteur LSM6DS3
LSM6DS3 sensor;

//Variables contenant les mesures de l'accéléromètre et du gyroscope
float accX,accY,accZ=0;
float gyroX,gyroY,gyroZ=0;

void setup(){

  // Active la liaison série pour pouvoir afficher les données dans le moniteur série
  Serial.begin(115200);

  // vérifie si le capteur LSM6DS3 est détecté avec la bonne adresse
  while(!sensor.begin()){
    // Si le capteur n'est pas détecté, revérifier le câblage (inversion SCL, SDA possible)
    Serial.println("Capteur non detecté");
    delay(500);
  }

  // Met l'accéléromètre et le gyroscope en précision maximum (16g et 2000dps)
  sensor.setAccelScale(0x04);
  sensor.setGyroScale(0x0C);

  // Met l'accéléromètre et le gyroscope en débit maximum (6.66 kHz)
  // Active aussi l'accéléromètre et le gyroscope (un débit de 0 les désactive)
  sensor.setAccelDataRate(0xA0);
  sensor.setGyroDataRate(0xA0);
}

void loop(){
  
  // Lit les valeurs de l'accéléromètre et du gyroscope
  sensor.readAccel(&accX,&accY,&accZ);
  sensor.readGyro(&gyroX,&gyroY,&gyroZ);

  // Affiche les mesures de l'accéléromètre
  Serial.print("Accel :");
  Serial.print("  X = ");
  Serial.print(accX);
  Serial.print("  Y = ");
  Serial.print(accY);
  Serial.print("  Z = ");
  Serial.print(accZ);

  // Affiche les mesures du gyroscope
  Serial.print("  Gyro :");
  Serial.print("  X = ");
  Serial.print(gyroX);
  Serial.print("  Y = ");
  Serial.print(gyroY);
  Serial.print("  Z = ");
  Serial.print(gyroZ);

  // Lit la température et l'affiche
  Serial.print("  Temp = ");
  Serial.print(sensor.readTemperature());
  Serial.print(" C°\n");
  delay(500);
}
