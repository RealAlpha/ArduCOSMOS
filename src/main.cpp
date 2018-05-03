#include "Arduino.h"

#include "state.h"
#include "telemetrystate.h"
#include "commandstate.h"

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

// Child struct of command_t
struct __attribute__((packed)) LEDCommand_t : command_t
{
	LEDCommand_t()
	{
		command_t::id = 1;
	}

	uint16_t value;
};

void LEDBinding(LEDCommand_t *command)
{
	analogWrite(9, map(command->value, 0, 1023, 0, 255));
}

void setup()
{

	// Testing functionality - set up pin 13 (internal LED) so it can be turned on by a COSMOS command
	pinMode(13, OUTPUT);
	digitalWrite(13, LOW);


	// Start a serial connection @ 9600hz
	Serial.begin(9600);

	// Register the telemetry module/state
	TelemetryState *telemetryState = new TelemetryState();
	States::RegisterState(telemetryState);
	
	// Log the light telemetry at a rate of 10hz (100 ms delay between packets; 100 is a multiple of 5 so it's valid)
	// Register the light telemetry we defined earlier
	telemetryState->RegisterTelemetry(&lightTelemetry, 100);

	// Register the command module/state
	CommandState *commandState = new CommandState();
	States::RegisterState(commandState);
	
	command_t command = LEDCommand_t();
	commandState->RegisterCommand(command, (commandBinding)LEDBinding);

	// Set a low timeout so the receiving of commands can be fast (readBytes works, while adding a delay/etc. causes issues)
	Serial.setTimeout(10);
}

void loop()
{
	int readValue = analogRead(0);
	
	// Update the telemetry state/packet's light value
	lightTelemetry.lightValue = readValue;

	// Tick the states!
	States::Tick();
	
	// Delay for a second so we aren't constantly reading the analog data
	delay(5);
}
