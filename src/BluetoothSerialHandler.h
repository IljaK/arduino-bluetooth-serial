#pragma once
#include <Arduino.h>
#include "common/Util.h"
#include "serial/SerialCharResponseHandler.h"


#define BLUETOOTH_SERIAL_BUFFER_SIZE 32
constexpr char BLUETOOTH_SERIAL_SEPARATOR[] = "\r\n";

class BluetoothSerialHandler : public SerialCharResponseHandler
{
private:
    StringCallback messageCallback = NULL;
public:
	BluetoothSerialHandler(Stream * serial, StringCallback messageCallback);
	~BluetoothSerialHandler();

	//void OnTimerComplete(TimerID timerId, uint8_t data) override;
	void OnResponseReceived(bool isTimeOut, bool isOverFlow = false) override;
    void OnTimerComplete(TimerID timerId, uint8_t data) override;
	bool IsBusy() override;
    void Loop() override;
    
    void SendTemp();
    void SendBat();
    void SendDev();

    size_t write(uint8_t);
    size_t write(const uint8_t *buffer, size_t size);
};
