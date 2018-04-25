#include "Arduino.h"

#include "state.h"
#include "telemetrystate.h"

struct lightTelemetry_t : telemetryPacket_t
{
	lightTelemetry_t()
{
	// Set the telemetry packet's ID to 1
	/*uint8_t */telemetryPacket_t::id = 1;
	
	/*uint8_t */telemetryPacket_t::length = sizeof(lightTelemetry_t);
}
	// Analog read gives us 10 bits of data, so we need to use a 16 bit (unsigned) integer to send this to cosmos 
	// Current value that we're receiving from analog reading the photoresistor
	uint16_t lightValue;
};

lightTelemetry_t lightTelemetry;

void setup()
{
	//((telemetryPacket_t)lightTelemetry).size = sizeof(lightTelemetry_t);
//	lightTelemetry.length = sizeof(lightTelemetry_t);
	Serial.begin(9600);
	TelemetryState *telemetryState = new TelemetryState();
	States::RegisterState(telemetryState);
	
	// Log the light telemetry at a rate of 10hz (100 ms delay between packets; 100 is a multiple of 5 so it's valid)
	telemetryState->RegisterTelemetry(&lightTelemetry, 100);
}

void loop()
{
	int readValue = analogRead(0);
	
	// Update the telemetry state/packet's light value
	lightTelemetry.lightValue = readValue;

//	Serial.println(analogRead(0));
//	delay(500);
	// Tick the states!
	States::Tick();
}
