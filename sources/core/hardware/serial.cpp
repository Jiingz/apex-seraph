#include <core\hardware\serial.h>
#include <core/libs/ArduSerial.h>
#include <core/libs/xor.h>

#include <iostream>

using namespace hardware;

void SerialProvider::Setup()
{
	Serial5.begin(345600);

	//yellow
	system(xor ("Color e"));
	std::cout << xor ("[+] Connecting to device...") << std::endl;

	while (!Serial5);
	system(xor ("cls")); 

	while (Serial5.available())
		Serial5.read();
	system(xor ("Color a"));
	std::cout << xor ("[+] Connected") << std::endl;

}