#pragma once
#if defined(ESP32)
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "common/Timer.h"
#include "common/Util.h"
#include "array/ByteStackArray.h"
#include "BLESerialHandler.h"

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define STATS_REFRESH_RATE 1000000u

class BLEHandler : public BLESerialHandler
{
private:
    StringCallback rxCallback = NULL;
    bool isClientConnected = false;

protected:

    // BLE Server callbacks
	void onConnect(BLEServer* pServer, esp_ble_gatts_cb_param_t *param) override;
    void onDisconnect(BLEServer* pServer) override;

public:
    BLEHandler(StringCallback rxCallback);
    virtual ~BLEHandler();

    void Loop() override;

    bool IsClientConnected();
};
#endif