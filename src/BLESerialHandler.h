#pragma once
#if defined(ESP32)
#include <Arduino.h>
#include "BLEServerHandler.h"
#include "array/ByteStackArray.h"
#include "array/BinaryMessageStack.h"

#define MAX_TX_STACK_SIZE 20u
#define MAX_RX_STACK_SIZE 3u

#define PACKAGE_EXTRA_DATA_SIZE 1
constexpr uint8_t MAX_BLE_PART_SIZE = MAX_BLE_MESSAGE_SIZE - PACKAGE_EXTRA_DATA_SIZE;

struct BLEMessage {
    uint8_t length;
    uint8_t data[];
};

class BLESerialHandler: public Print, public BLEServerHandler
{
private:
    // Serial TX
    ByteStackArray serialTXBuffer;
    volatile bool isTransferrig = false;
    SemaphoreHandle_t xTXSemaphore = xSemaphoreCreateRecursiveMutex();

    // Serial RX
    BinaryMessageStack rxMessageStack;
    SemaphoreHandle_t xRXSemaphore = xSemaphoreCreateMutex();

    void SendSerialMessage();
    void ReadRxData(uint8_t * data);
    void FlushRxData();

protected:
    BLESecurity* pSecurity = NULL;

    void onDisconnect(BLEServer* pServer) override;

    // BLECharacteristicCallbacks
    //void onRead(BLECharacteristic* pCharacteristic) override;
	void onWrite(BLECharacteristic* pCharacteristic) override;
	//void onNotify(BLECharacteristic* pCharacteristic) override;
	void onStatus(BLECharacteristic* pCharacteristic, Status s, uint32_t code) override;

    void AppenSerialStack(uint8_t * data, uint8_t length);

    int AvailableMessages();
    BinaryMessage * GetMessage();

public:
    BLESerialHandler();
    virtual ~BLESerialHandler();

    virtual void Loop();

    // Compatibility with serial->write
    size_t write(uint8_t) override;
    size_t write(const uint8_t *buffer, size_t size) override;
};
#endif