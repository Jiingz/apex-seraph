#include <core\hardware\serial.h>
#include <core/libs/ArduSerial.h>
#include <core/libs/xor.h>

#include <iostream>

using namespace hardware;

void SerialProvider::Setup()
{
	Serial3.begin(345600);

	//yellow
	OutputDebugString(L"Connecting to Device...");

	while (!Serial3);
	Sleep(1);

	while (Serial3.available())
		Serial3.read();
	OutputDebugString(L"Connected Successfully.");

}