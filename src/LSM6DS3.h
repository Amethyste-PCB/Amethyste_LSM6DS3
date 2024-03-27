#ifndef LSM6DS3_h
#define LSM6DS3_h
#include <Arduino.h>
#include <Wire.h>
// Registre d'adresse du capteur
#define LSM6DS3_ID 0x6A 

// Définition des registres du LSM6DS3
#define LSM6DS3_FUNC_CFG_ACCESS   0x01 // Contrôles FIFO
#define LSM6DS3_FIFO_CTRL2        0x07 // Contrôles FIFO
#define LSM6DS3_DRDY_PULSE_CFG_G  0x0B // Interruption inclinaison du poignet 
#define LSM6DS3_INT1_CTRL         0x0D // Contrôles INT1
#define LSM6DS3_WHO_AM_I          0x0F // Adresse du capteur
#define LSM6DS3_CONFIG_PEDO_THS_MIN 0x0F // Seuil du podomètre
#define LSM6DS3_CTRL1_XL          0x10 // Contrôles de l'accéléromètre
#define LSM6DS3_CTRL2_G           0x11 // Contrôles du gyroscope
#define LSM6DS3_CTRL3_C           0x12 // Contrôles de réinitialisation, logique et connexion
#define LSM6DS3_CTRL4_C           0x13 // Place les interruptions de INT2 sur INT1
#define LSM6DS3_CTRL6_C           0x15 // Contrôles supplémentaires de l'accéléromètre et du gyroscope
#define LSM6DS3_CTRL10_C          0x19 // Activation du basculement, du podomètre et de la temporisation
#define LSM6DS3_WAKE_UP_SRC       0x1B // Mesures de l'interruption de réveil
#define LSM6DS3_TAP_SRC           0x1C // Mesures de détection de tap
#define LSM6DS3_OUT_TEMP_L        0x20 // Mesures de température
#define LSM6DS3_OUTX_L_G          0x22 // Mesures du gyroscope
#define LSM6DS3_OUTX_L_XL         0x28 // Mesures de l'accéléromètre
#define LSM6DS3_A_WRIST_TILT_LAT  0x50 // Latence de l'inclinaison du poignet
#define LSM6DS3_A_WRIST_TILT_THS  0x54 // Seuil de l'inclinaison du poignet
#define LSM6DS3_A_WRIST_TILT_Mask 0x59 // Masque de l'inclinaison du poignet
#define LSM6DS3_FUNC_SRC1         0x53 // Mesures du podomètre, de l'inclinaison
#define LSM6DS3_FUNC_SRC2         0x54 // Mesures de l'inclinaison du poignet
#define LSM6DS3_TAP_CFG           0x58 // Contrôles d'interruption, d'inactivité, de filtrage et de tap
#define LSM6DS3_TAP_THS_6D        0x59 // Seuil pour Tap et 6D
#define LSM6DS3_INT_DUR2          0x5A // Paramètres de Tap
#define LSM6DS3_WAKE_UP_THS       0x5B // Seuil de double tap et de réveil
#define LSM6DS3_WAKE_UP_DUR       0x5C // Durée de la chute libre, du réveil, de la temporisation et du sommeil
#define LSM6DS3_FREE_FALL         0x5D // Paramètres de la chute libre
#define LSM6DS3_MD1_CFG           0x5E // Routage d'interruption INT1

class LSM6DS3 {
  public:
    LSM6DS3();
    // Adresse du capteur LSM6DS3 (0x6A sur les cartes AméthystePCB)
    uint8_t LSM6DS3_address = 0x6A;
    // Initialise la communication et vérifie l'adresse du capteur
    bool begin();
    // Termine la communication avec le capteur
    bool end();
    // Active ou désactive l'accéléromètre à 2g
    void toggleAccel(bool enable);
    // Active ou désactive le gyrocope à 245dps
    void toggleGyro(bool enable);
    // Lit les mesures de l'accéléromètre sur les trois axes en g
    void readAccel(float *x, float *y, float *z);
    // Lit les mesures du gyroscope sur les trois axes en dps
    void readGyro(float *x, float *y, float *z);
    // Lit la température du capteur en C°
    float readTemperature();
    // Lit les mesures de l'accéléromètre sur les trois axes sans conversion d'unités
    void readAccelRaw(float *x, float *y, float *z);
    // Lit les mesures du gyroscope sur les trois axes sans conversion d'unités
    void readGyroRaw(float *x, float *y, float *z);
    // Lit la température du capteur sans conversion d'unités
    float readTemperatureRaw();
    /* Paramètre l'échelle de précision de l'accéléromètre

              0x00 -> 2g

              0x08 -> 4g

              0x0C -> 8g

              0x04 -> 16g

    */
    void setAccelScale(uint8_t scale = 0x00);
    /* Paramètre l'échelle de précision du gyroscope

              0x02 -> 125dps

              0x00 -> 245dps

              0x04 -> 500dps

              0x08 -> 1000dps

              0x0C -> 2000dps

    */
    void setGyroScale(uint8_t scale = 0x00);
    /* Paramètre le débit de données de l'accéléromètre

              0x00 -> Désactive accéléromètre

              0x10 -> 12.5Hz

              0x20 -> 26Hz

              0x30 -> 52Hz

              0x40 -> 104Hz

              0x50 -> 208Hz

              0x60 -> 416Hz

              0x70 -> 833Hz

              0x80 -> 1.66kHz

              0x90 -> 3.33kHz

              0xA0 -> 6.66kHz

    */
    void setAccelDataRate(uint8_t rate = 0x60);
    /* Paramètre le débit de données du gyroscope

              0x00 -> Désactive accéléromètre

              0x10 -> 12.5Hz

              0x20 -> 26Hz

              0x30 -> 52Hz

              0x40 -> 104Hz

              0x50 -> 208Hz

              0x60 -> 416Hz

              0x70 -> 833Hz

              0x80 -> 1.66kHz

              0x90 -> 3.33kHz

              0xA0 -> 6.66kHz

    */
    void setGyroDataRate(uint8_t rate = 0x60);
    // Active ou désactive la détection de tapotement
    void toggleTapDetection(bool enable);
    // Observe si un tapotement a été détecté
    bool readTapDetection();
    /* Paramètre la sensibilité de détection de tapotement
    
        0x00 -> le plus sensible

        0x08 -> moyennement sensible

        0x10 -> le moins sensible
    */ 
    void setTapSensibility(uint8_t sensibility = 0x00);
    // Active ou désactive la détection de chute libre
    void toggleFreeFallDetection(bool enable);
    // Observe si une chute libre a été détectée
    bool readFreeFallDetection();
    /* Paramètre la durée minimum d'une chute libre detectable et son seuil

    minDuration :

        0x00 - 0x07 -> durée minimum détectable
        
        0x40 -> durée moyenne détectable

        0xF8 -> durée maximum détectable

    threshold (seuil) :

        0x00 -> le plus sensible

        0x04 -> moyennement sensible

        0x07 -> le moins sensible
    

    */
    void setFreeFallParameters(uint8_t minDuration = 0x00, uint8_t threshold = 0x00);
    // Active ou désactive le podomètre
    void togglePedometer(bool enable);
    // Renvoie true si un pas a été détecté, sinon false
    bool readPedometer();
    /* Paramètre la sensibilité de détection d'un pas

    threshold (seuil) :

        0x00 -> le plus sensible

        0x10 -> moyennement sensible

        0x1F -> le moins sensible
    

    */
    void setPedometerParameters(uint8_t threshold = 0x10);
    // Active ou désactive la détection de l'inclinaison du poignet
    void toggleWristTilt(bool enable);
    // Renvoie vrai si l'angle d'inclinaison du poignet est supérieure au seuil sur l'axe choisi 
    bool readWristTilt();
    /* Paramètre la latence, l'angle minimum et les axes détectés pour l'inclinaison du poignet
        
        latency :

        0x00 -> durée minimum détectable
        
        0xF0 -> durée moyenne détectable

        0xFF -> durée maximum détectable

    threshold (seuil) :

        0x00 -> le plus sensible

        0xF0 -> moyennement sensible

        0xFF -> le moins sensible

    mask (choix des axes) :

        0xC0 -> inclinaison selon l'axe des X

        0x30 -> inclinaison selon l'axe des Y

        0x0C -> inclinaison selon l'axe des Z

    */
    void setWristTiltParameters(uint8_t latency = 0x0F, uint8_t threshold = 0x20, uint8_t mask = 0xC0);
    // Choisis si les interruptions se font à état haut ou a état bas
    void toggleInterruptLogicLevel(bool isActiveHigh);
    // Lit la valeur du registre d'adresse reg
    uint8_t readRegister(uint8_t reg);
    // Ecrit dans le registre d'adresse reg la valeur value
    void writeRegister(uint8_t reg, uint8_t value);
    // Lit le nombre len de registres depuis l'adresse reg avec le résultat dans data
    void readRegisters(uint8_t reg, uint8_t *data, uint8_t len);
};
#endif