#ifndef GSM_H
#define GSM_H

#include <Arduino.h>
#include <HardwareSerial.h>

// Must be defined BEFORE including TinyGsmClient.h
#define TINY_GSM_MODEM_SIM800
#include <TinyGsmClient.h>

// Configuration
#define GSM_RX_PIN 16
#define GSM_TX_PIN 17
#define GSM_RST_PIN 4
#define GSM_BAUDRATE 9600
#define NETWORK_TIMEOUT_MS 180000L

class GSM
{
public:
    // Constructor with serial initialization
    GSM(HardwareSerial &serial = Serial2) : gsmSerial(serial), modem(gsmSerial) {}

    void init();
    void run();

    // Network Functions
    bool isNetworkConnected();
    bool setAutomaticNetwork();
    bool setManualNetwork(const String &carrierName, const String &carrierCode);
    String scanNetworks();

    // SIM & Carrier Functions
    bool unlockSIM(const String &pin = "");
    String getCarrier();
    String getSignalStrength();
    String checkBalance(const String &ussdCode);

    // Call Functions
    bool makeCall(const String &number);
    bool endCall();
    bool answerCall();

    // SMS Functions
    bool sendSMS(const String &number, const String &message);
    String readSMS(uint8_t index);
    bool deleteSMS(uint8_t index);

private:
    HardwareSerial &gsmSerial;
    TinyGsm modem;
    bool checkNetworkRegistration();
};

void initGSMTask(void *parameters);
void runGSMTask(void *parameters);

extern GSM gsm;

#endif