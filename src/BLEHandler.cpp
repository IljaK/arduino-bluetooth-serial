#if ESP32
#include <string.h>
#include "BLEHandler.h"

BLEHandler::BLEHandler(StringCallback rxCallback)
{
    this->rxCallback = rxCallback;
}

BLEHandler::~BLEHandler()
{
}

void BLEHandler::Loop()
{
    BLESerialHandler::Loop();
    if (AvailableMessages() > 0) {
        BinaryMessage * message = GetMessage();
        if (message != NULL) {
            if (rxCallback != NULL) {
                rxCallback((char *)message->data, (size_t)message->length);
            }
            if (message->data != NULL) {
                free(message->data);
            }
            free(message);
        }
    }
}

void BLEHandler::onConnect(BLEServer* pServer, esp_ble_gatts_cb_param_t* param)
{
    // May launch from different core!
    BLESerialHandler::onConnect(pServer, param);
    isClientConnected = true;
}

void BLEHandler::onDisconnect(BLEServer* pServer)
{
    // May launch from different core!
    BLESerialHandler::onDisconnect(pServer);
    isClientConnected = false;
}

#endif
