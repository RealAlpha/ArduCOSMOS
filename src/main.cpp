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

struct __attribute__((packed)) command_t
{
	uint8_t id;

	/*
	 * Further parameters left up to children. NOTE: Children need to have the same order/datatypes & need to be packed
	 */
};

struct commandBinding_t
{
	commandBinding_t(command_t *_command, void (*command_binding)(command_t*))
	{
		command = _command;
		binding = command_binding;
	}

	command_t *command;

	// NOTE: Should be able to safely use the provided command as there is only one thread -> can't have a new instance while this one is still processing
	void (*binding)(command_t *command); 
};

// Child struct of command_t
struct LEDCommand_t : command_t
{
	LEDCommand_t()
	{
		command_t::id = 1;
	}

	uint16_t value;
};

std::vector<commandBinding_t> commands;

bool registered = false;

void LEDBinding(command_t *command)
{
	Serial.println("Received Command!");
}

void loop()
{
	if (!registered)
	{
		Serial.println(sizeof(LEDCommand_t));
		
		LEDCommand_t *LEDCommand = new LEDCommand_t;

		commandBinding_t binding((command_t*)LEDCommand, LEDBinding);

		commands.push_back(binding);
		registered = true;
	}
	int readValue = analogRead(0);
	
	// Update the telemetry state/packet's light value
	lightTelemetry.lightValue = readValue;
	
	// Is the serial pending data? (AKA: is there a command pending?)
	if (Serial.available())
	{
		// Command pending! TODO: Should we also be sending a length parameter so if there are 2 commands "queued" they aren't interpreted as one command?

		int commandLength = 0;

		size_t bufferSize = 1;
		uint8_t *commandBuffer = (uint8_t *)malloc(bufferSize);

		while (Serial.available())
		{	
			// Is the buffer still large enough?
			if (commandLength + 1 > bufferSize)
			{
				// Welp, it isn't! Reallocate it to twice the size (appears to be good memory vs performance)
				bufferSize *= 2;
				commandBuffer = (uint8_t*)realloc(commandBuffer, bufferSize);
				
				// Double-check that the command buffer is still valid (if it isn't then we may have ran out of memory, or hit another issue, thus exiting!!!!!!)
				if (!commandBuffer)
				{
					// TODO Remove this - serial print just in case we ever hit this
					Serial.println("Error reallocating command buffer!");
					return;
				}
			}
			
			// Now that we know there is a large enough buffer allocated, we can read the next byte and increase the buffer's size.
			commandBuffer[bufferSize] = Serial.read();
			commandLength++;

			// TODO Is there a better way to receive all of the command>? Without this it appears to be regesting every byte/keystoke as a new command, but with this it induces a delay into the equasion
			delay(2);
		}

		// If we got here that means that all of the command's bytes should have come in!
		Serial.println("Received Command!");
		Serial.println(commandLength);

	}

	// Tick the states!
	States::Tick();
	
	// Delay for a second so we aren't constantly reading the analog data
	delay(1);
}
