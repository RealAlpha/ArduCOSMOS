#include "Arduino.h"

#include "state.h"
#include "telemetrystate.h"

struct lightTelemetry_t : telemetryPacket_t
{
	lightTelemetry_t()
	{
		// Set the telemetry packet's ID to 1
		telemetryPacket_t::id = 1;
	
		// Set the packet's length (sizeof is evaluated at compiletime and this avoids issues related to it not getting the instance's sizeof later in the code
		telemetryPacket_t::length = sizeof(lightTelemetry_t);
	}
	
	// Analog read gives us 10 bits of data, so we need to use a 16 bit (unsigned) integer to send this to cosmos 
	// Current value that we're receiving from analog reading the photoresistor
	uint16_t lightValue;
};

// Create an instance of the lgiht telemetry struct so we can log LDR data to COSMOS
lightTelemetry_t lightTelemetry;

void setup()
{
	// Start a serial connection @ 9600hz
	Serial.begin(9600);

	// Register the telemetry module/state
	TelemetryState *telemetryState = new TelemetryState();
	States::RegisterState(telemetryState);
	
	// Log the light telemetry at a rate of 10hz (100 ms delay between packets; 100 is a multiple of 5 so it's valid)
	// Register the light telemetry we defined earlier
	telemetryState->RegisterTelemetry(&lightTelemetry, 100);
}

void loop()
{
	int readValue = analogRead(0);
	
	// Update the telemetry state/packet's light value
	lightTelemetry.lightValue = readValue;

	// Tick the states!
	States::Tick();
	
	// Delay for a second so we aren't constantly reading the analog data
	delay(1);
}
