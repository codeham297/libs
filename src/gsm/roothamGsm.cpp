#include "roothamGsm.h"
#include <Arduino.h>

// Initialize with Serial2
GSM gsm(Serial2);

void GSM::init()
{
    // Initialize hardware serial for SIM800L
    gsmSerial.begin(GSM_BAUDRATE, SERIAL_8N1, GSM_RX_PIN, GSM_TX_PIN);

    // Modem reset sequence
    pinMode(GSM_RST_PIN, OUTPUT);
    digitalWrite(GSM_RST_PIN, LOW);
    delay(1000);
    digitalWrite(GSM_RST_PIN, HIGH);
    delay(2000);

    // Initialize modem
    modem.restart();
    modem.init();
    Serial.println("[GSM] Modem initialized");
}

void initGSMTask(void *parameters)
{
    delay(10000);
    gsm.init();
    Serial.println("[GSM] GSM task initialized");
    vTaskDelete(NULL); // Delete task after initialization
}

void runGSMTask(void *parameters)
{
    delay(10000);
    while (true)
    {
        gsm.run();
        vTaskDelay(1000 / portTICK_PERIOD_MS); // Run GSM task every second
    }
}

void GSM::run()
{
    static uint32_t lastCheck = 0;
    if (millis() - lastCheck > 30000)
    { // Check every 30 seconds
        lastCheck = millis();
        if (!isNetworkConnected())
        {
            Serial.println("[GSM] Network lost! Reconnecting...");
            setAutomaticNetwork();
        }
    }
}

bool GSM::isNetworkConnected()
{
    return modem.isNetworkConnected();
}

bool GSM::setAutomaticNetwork()
{
    modem.sendAT("+COPS=0");
    return modem.waitResponse(15000L) == 1;
}

bool GSM::setManualNetwork(const String &carrierName, const String &carrierCode)
{
    String cmd = "+COPS=1,2,\"" + carrierCode + "\"";
    modem.sendAT(cmd);
    if (modem.waitResponse(30000L) != 1)
        return false;
    return checkNetworkRegistration();
}

String GSM::scanNetworks()
{
    modem.sendAT("+COPS=?");
    if (modem.waitResponse(60000L) != 1)
        return "Scan failed";
    return modem.stream.readString();
}

bool GSM::unlockSIM(const String &pin)
{
    if (modem.getSimStatus() == 3)
        return true; // 3 = SIM_READY
    return modem.simUnlock(pin.c_str());
}

String GSM::getCarrier()
{
    modem.sendAT("+COPS?");
    if (modem.waitResponse(10000L) == 1)
    {
        return modem.stream.readString();
    }
    return "Unknown";
}

String GSM::getSignalStrength()
{
    return String(modem.getSignalQuality());
}

String GSM::checkBalance(const String &ussdCode)
{
    modem.sendAT("+CUSD=1,\"" + ussdCode + "\"");
    if (modem.waitResponse(10000L) == 1)
    {
        return modem.stream.readString();
    }
    return "USSD failed";
}

bool GSM::makeCall(const String &number)
{
    return modem.callNumber(number.c_str());
}

bool GSM::endCall()
{
    modem.sendAT("+CHUP");
    return modem.waitResponse(10000L) == 1;
}

bool GSM::answerCall()
{
    modem.sendAT("ATA");
    return modem.waitResponse(10000L) == 1;
}

bool GSM::sendSMS(const String &number, const String &message)
{
    return modem.sendSMS(number.c_str(), message.c_str());
}

String GSM::readSMS(uint8_t index)
{
    // Alternative SMS reading implementation for SIM800
    modem.sendAT("+CMGR=" + String(index));
    if (modem.waitResponse(10000L) == 1)
    {
        return modem.stream.readString();
    }
    return "";
}

bool GSM::deleteSMS(uint8_t index)
{
    modem.sendAT("+CMGD=" + String(index));
    return modem.waitResponse(10000L) == 1;
}

bool GSM::checkNetworkRegistration()
{
    return modem.waitForNetwork(NETWORK_TIMEOUT_MS);
}