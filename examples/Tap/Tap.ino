//Inclut la librairie Améthyste_LSM6DS3
#include <Amethyste_LSM6DS3.h>

//Interruption sur pin 2 de l'arduino uno à changer selon la carte :
//(https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/)
const byte interruptPin = 2;

// Instancie un capteur LSM6DS3
LSM6DS3 sensor;

//Fonction affichant un texte lors d'une interrruption
void printInterrupt(){
    Serial.print("\nTap en interruption");
}

void setup(){

  // Active la liaison série pour pouvoir afficher les données dans le moniteur série
  Serial.begin(115200);

  // vérifie si le capteur LSM6DS3 est détecté avec la bonne adresse
  while(!sensor.begin()){
    // Si le capteur n'est pas détecté, revérifier le câblage (inversion SCL, SDA possible)
    Serial.println("Capteur non detecté");
    delay(500);
  }

  // Mets la pin d'interruption en Input pour pouvoir observer le front montant
  pinMode(2,INPUT_PULLUP);

  // Lance la fonction printInterrupt lorsque le capteur emet une interruption (front montant)
  attachInterrupt(digitalPinToInterrupt(2),printInterrupt,RISING);

  // Active l'accéléromètre, nécessaire pour la fonctionnalité embarquée
  sensor.toggleAccel(true);
  
  // Active la détection de tapotement son interruption
  sensor.toggleTapDetection(true);

  // Paramètre la sensibilité de la détection de tapotement
  sensor.setTapSensibility(0x02);
}

void loop(){

    //Lit en bloquant si un tapotement est détecté
    if(sensor.readTapDetection()){
        Serial.print("\nTap en bloquant");
        delay(10);
    }  
}
