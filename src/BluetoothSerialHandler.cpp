#include "BluetoothSerialHandler.h"

extern uint8_t btPinState;
extern unsigned long onlineDuration;

BluetoothSerialHandler::BluetoothSerialHandler(Stream * serial, StringCallback messageCallback):
    SerialCharResponseHandler(BLUETOOTH_SERIAL_BUFFER_SIZE, BLUETOOTH_SERIAL_SEPARATOR, serial)
{
    this->messageCallback = messageCallback;
}

BluetoothSerialHandler::~BluetoothSerialHandler()
{
}

void BluetoothSerialHandler::OnTimerComplete(TimerID timerId, uint8_t data)
{
	SerialCharResponseHandler::OnTimerComplete(timerId, data);
}

void BluetoothSerialHandler::Loop() {
    SerialCharResponseHandler::Loop();
}

void BluetoothSerialHandler::OnResponseReceived(bool isTimeOut, bool isOverFlow)
{
    SerialCharResponseHandler::OnResponseReceived(isTimeOut, isOverFlow);

    //size_t size = strlen(buffer);
    if (messageCallback != NULL) {
        messageCallback(buffer, strlen(buffer));
    }
}

bool BluetoothSerialHandler::IsBusy()
{
    return SerialCharResponseHandler::IsBusy();
}

// Compatibility with serial->write
size_t BluetoothSerialHandler::write(uint8_t data)
{
    return serial->write(data);
}
size_t BluetoothSerialHandler::write(const uint8_t *buffer, size_t size)
{
    return serial->write(buffer, size);
}

