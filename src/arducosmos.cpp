#include "arducosmos.h"

// For checking if the serial is active/has been started and starting it if that is not the case
#include "Arduino.h"

/* Define the static variables */
#ifndef NO_ARDUCOSMOS_TELEMETRY
	TelemetryState ArduCOSMOS::telemetryState = TelemetryState();
#endif
#ifndef NO_ARDUCOSMOS_COMMANDS
	CommandState ArduCOSMOS::commandState = CommandState();
#endif


void ArduCOSMOS::StartCosmos()
{
	// Has the serial interface already been started?
	if (!Serial)
	{
		// It hasn't yet, so start it!
		Serial.begin(9600);
	}

#ifndef NO_ARDUCOSMOS_TELEMETRY
	// Register the cosmos telemetry state (unless there is a guard against it)
	States::RegisterState(&telemetryState);
#endif
#ifndef NO_ARDUCOSMOS_COMMANDS
	// Register the COSMOS command state (unless there is a guard against it)
	States::RegisterState(&commandState);
#endif
}

#ifndef NO_ARDUCOSMOS_COMMANDS
void ArduCOSMOS::StartCosmos(bool bNoCommands)
{
	if (!bNoCommands)
	{
		// Not no commands -> register it like without the parameter
		StartCosmos();
	}
	else
	{
		// User doesn't want us to "block" the serial listening for commands, so only start the telemetry part

		// Has the serial interface already been started?
		if (!Serial)
		{
			// It hasn't yet, so start it!
			Serial.begin(9600);
		}

#ifndef NO_ARDUCOSMOS_TELEMETRY
		// Register the cosmos telemetry state (unless there is a guard against it)
		States::RegisterState(&telemetryState);
#endif
	}
}
#endif

#ifndef NO_ARDUCOSMOS_TELEMETRY
void ArduCOSMOS::RegisterTelemetry(telemetryPacket_t *packet, int interval)
{
	// Forward the request to the (private) state variable to create a layer of encapsulation
	telemetryState.RegisterTelemetry(packet, interval);
}
#endif

#ifndef NO_COSMOS_COMMANDS
void ArduCOSMOS::RegisterCommand(command_t command, commandBinding binding)
{
	// Forward the request to the (private) command state variable to create a layer of encapsulation
	commandState.RegisterCommand(command, binding);
}
#endif
