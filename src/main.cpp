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

struct __attribute__((packed)) command_t
{
	uint8_t id;

	/*
	 * Further parameters left up to children. NOTE: Children need to have the same order/datatypes & need to be packed
	 */
};

// Typedef the "standard" binding function so it can be typecasted/used in variables more easily + create a more uniform experience within the library
typedef void (*commandBinding)(command_t *command);

struct commandBinding_t
{
	commandBinding_t(command_t *_command, void (*command_binding)(command_t*))
	{
		command = _command;
		binding = command_binding;
	}

	command_t *command;

	// NOTE: Should be able to safely use the provided command as there is only one thread -> can't have a new instance while this one is still processing
	commandBinding binding;
};

// Child struct of command_t
struct __attribute__((packed)) LEDCommand_t : command_t
{
	LEDCommand_t()
	{
		command_t::id = 1;
	}

	uint16_t value;
};

std::vector<commandBinding_t> commands;

bool registered = false;

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

	// Create a new instance of a command, and register it with the commands system
	LEDCommand_t *LEDCommand = new LEDCommand_t;
	commandBinding_t binding((command_t*)LEDCommand, (commandBinding)LEDBinding);
	commands.push_back(binding);

	// Set a low timeout so the receiving of commands can be fast (readBytes works, while adding a delay/etc. causes issues)
	Serial.setTimeout(10);
}

void loop()
{
	if (!registered)
	{
		//Serial.println(sizeof(LEDCommand_t));
		

		registered = true;
	}

	int readValue = analogRead(0);
	
	// Update the telemetry state/packet's light value
	lightTelemetry.lightValue = readValue;
	
	// Is the serial pending data? (AKA: is there a command pending?)
	if (Serial.available())
	{
		// Command pending! TODO: Should we also be sending a length parameter so if there are 2 commands "queued" they aren't interpreted as one command?

		// Attempt to read the command into a buffer (NOTE: Current command length limit is 256 bytes; Other solutions resulted in there being issues receiving the buffer ;( )
		uint8_t *buffer = (uint8_t*)malloc(256);
		int commandLength = Serial.readBytes(buffer, 256);
		uint8_t *commandBuffer = (uint8_t*)realloc(buffer, commandLength);
		
		// If we got here that means that all of the command's bytes should have come in!
		
		// Is the first byte (ie the ID) set?
		if (commandLength >= 1)
		{
			// The command's ID should be the first byte, so extract it to a regular integer for easier usage
			uint8_t commandID = commandBuffer[0];

			for (std::vector<commandBinding_t>::iterator it = commands.begin(); it != commands.end(); it++)
			{
				// Is the command/binding valid?
				if (!it->command || !it->binding)
				{
					// Something is wrong - flash the built in LED (13 on the Uno, my (personal) target) so we can see it even if the serial doesn't come through
					for (int i = 0; i < 10; i++)
					{
						digitalWrite(13, HIGH);
						delay(200);
						digitalWrite(13, LOW);
					}

					// Now log the error message (TODO: Should there be some meta setup with COSMOS for these kidns of logs?)
					Serial.print("Error with comands! Invalid command/binding! Target ID: ");
					Serial.println(commandID);
					
					// Something went wrong - an invalid command or binding was set up, but this doesn't mean that there are other valid commands, so continue with the next one
					continue;
				}
				
				// Is this the command COSMOS is trying to execute?
				if (it->command->id == commandID)
				{					// Found the command! Execute the function, passing in the commandBuffer casted to the command_t. Assume user has set up their struct they're typecasting to correctly so it won't cause any size mismatches -> issues
					it->binding((command_t *)commandBuffer);
////					LEDBinding((command_t *)commandBuffer);	
				}
			}

			// TODO Ensure the freeing of commandBuffer in all cases! We don't want any memory leaks!!!!!
		}
		else
		{
			for (int i = 0; i < 10; i++)
			{
				digitalWrite(13, HIGH);
				delay(500);
				digitalWrite(13, LOW);
			}
			Serial.println("Error parsing command - length is less than 1!!!! Command appears to have been invalid, or your serial port may be occupied");
		}
	}

	// Tick the states!
	States::Tick();
	
	// Delay for a second so we aren't constantly reading the analog data
	delay(5);
}
