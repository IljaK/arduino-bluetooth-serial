#if defined(ESP32)
#include <string.h>
#include "BLEServerHandler.h"


BLEServerHandler::BLEServerHandler()
{
}

BLEServerHandler::~BLEServerHandler()
{
}

void BLEServerHandler::Start(const char *deviceName)
{
    // Create the BLE Device
    BLEDevice::init(deviceName);
    //BLEDevice::setEncryptionLevel(ESP_BLE_SEC_ENCRYPT);
    BLEDevice::setSecurityCallbacks(this);
    if (pSecurity == NULL) {
        pSecurity = new BLESecurity();
    }
    uint32_t passkey = 0;
    pSecurity->setStaticPIN(passkey);

    // Create the BLE Server
    if (pServer == NULL) {
        pServer = BLEDevice::createServer();
    }

    pServer->setCallbacks(this);

    // Create the BLE Service
    pService = pServer->createService(SERVICE_UUID);

    // Create a UART Characteristic
    //uartRXCharacteristics = CreateCharacteristic(
    //    UART_RX_CHARACTERISTICS,
    //    BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_INDICATE
    //);
    //uartRXCharacteristics->setCallbacks(this);

    uartRXCharacteristics = pService->createCharacteristic(
        UART_RX_CHARACTERISTICS, 
        BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_INDICATE);

    BLE2902 * descriptor = new BLE2902();
    descriptor->setIndications(true);
    descriptor->setCallbacks(this);
    uartRXCharacteristics->addDescriptor(descriptor);
    uartRXCharacteristics->setCallbacks(this);

    uartTXCharacteristics = CreateCharacteristic(
        UART_TX_CHARACTERISTICS,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
    );

    // For making extra characteristics
    OnCreateCharacteristics(pService);

    // Start the service
    pService->start();

    // Start advertising
    pServer->getAdvertising()->start();

    //pSecurity = new BLESecurity();
    //pSecurity->setAuthenticationMode(ESP_LE_AUTH_REQ_SC_ONLY);
    //pSecurity->setCapability(ESP_IO_CAP_OUT);
    //pSecurity->setInitEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK);

    Serial.println("Waiting a client connection to notify...");
}

void BLEServerHandler::Stop()
{
    pServer->getAdvertising()->stop();
    BLEDevice::deinit();
}

void BLEServerHandler::OnCreateCharacteristics(BLEService *service)
{
    
}

BLECharacteristic * BLEServerHandler::CreateCharacteristic(const char * uuid, uint32_t properties)
{
    BLECharacteristic * characteristic = pService->createCharacteristic(uuid, properties);

    BLE2902 * descriptor = new BLE2902();
    descriptor->setNotifications(true);
    descriptor->setCallbacks(this);

    characteristic->addDescriptor(descriptor);
    characteristic->setCallbacks(this);

    return characteristic;
}

void BLEServerHandler::onConnect(BLEServer* pServer, esp_ble_gatts_cb_param_t* param)
{
    Serial.println("onConnect");

    //uartTXCharacteristics->setValue((uint8_t *)"test value", 11);
    //uartTXCharacteristics->indicate();
}

void BLEServerHandler::onDisconnect(BLEServer* pServer)
{
    Serial.println("onDisconnect");
    if (GetConnectedCount() <= 1) {
        Advertise();
    }
}

// BLECharacteristicCallbacks
void BLEServerHandler::onRead(BLECharacteristic* pCharacteristic)
{
    //Serial.print("onRead chrst: ");
    //Serial.println(pCharacteristic->getUUID().toString().data());
    //Serial.print(" core: ");
    //Serial.println(xPortGetCoreID());
}

void BLEServerHandler::onNotify(BLECharacteristic* pCharacteristic)
{
    //Serial.print("onNotify chrst: ");
    //Serial.println(pCharacteristic->getUUID().toString().data());

    //if (pCharacteristic == uartTXCharacteristics) {
    //Serial.print("onNotify chrst: ");
    //Serial.println(pCharacteristic->getUUID().toString().data());

    //Serial.print(" core: ");
    //Serial.println(xPortGetCoreID());
}

void BLEServerHandler::onStatus(BLECharacteristic* pCharacteristic, Status s, uint32_t code)
{

    //Serial.print("onStatus chrst: ");
    //Serial.println(pCharacteristic->getUUID().toString().data());

    //Serial.print("status: ");
    //Serial.print(s);
    //Serial.print(" code: ");
    //Serial.print(code);

    //Serial.print(" core: ");
    //Serial.println(xPortGetCoreID());
}

void BLEServerHandler::onWrite(BLECharacteristic* pCharacteristic) {

    Serial.print("onWrite chrst: ");
    Serial.print(pCharacteristic->getUUID().toString().data());

    Serial.print(" core: ");
    Serial.println(xPortGetCoreID());
    
}

void BLEServerHandler::onRead(BLEDescriptor* pDescriptor)
{
    Serial.print("onRead descriptor: ");
    Serial.println(pDescriptor->getUUID().toString().data());
}

void BLEServerHandler::onWrite(BLEDescriptor* pDescriptor)
{
    Serial.print("onWrite descriptor: ");
    Serial.println(pDescriptor->getUUID().toString().data());
}

uint32_t BLEServerHandler::onPassKeyRequest()
{
    return 123456;
}

void BLEServerHandler::onPassKeyNotify(uint32_t pass_key)
{
    Serial.printf("The passkey Notify number: %d\n", pass_key);
}

bool BLEServerHandler::onSecurityRequest()
{
    Serial.println("onSecurityRequest!");
    return true;
}

void BLEServerHandler::onAuthenticationComplete(esp_ble_auth_cmpl_t)
{
    Serial.println("onAuthenticationComplete!");
}

bool BLEServerHandler::onConfirmPIN(uint32_t pin)
{
    Serial.println("onConfirmPIN!");
    return true;
}

void BLEServerHandler::Advertise()
{
    if (pServer == NULL) return;
    if (pServer->getAdvertising() == NULL) return;
    pServer->getAdvertising()->start();
}

void BLEServerHandler::StopAdvertise()
{
    if (pServer == NULL) return;
    if (pServer->getAdvertising() == NULL) return;
    pServer->getAdvertising()->stop();
}

int BLEServerHandler::GetConnectedCount()
{
    if (pServer == NULL) return 0;
    return pServer->getConnectedCount();
}
/*
bool BLEServerHandler::IsValidCheckSum(BLEMessage * message)
{
    if (message->length == 0) {
        return false;
    }
    
    uint8_t calculatedSum = 0;
    uint8_t length = message->length;

    if (length > MAX_BLE_PART_SIZE) length = MAX_BLE_PART_SIZE;
    
    for (uint8_t i = 0; i < length; i++) {
        calculatedSum += message->data[i];
    }
    return calculatedSum == message->checkSum;
}
*/
#endif
