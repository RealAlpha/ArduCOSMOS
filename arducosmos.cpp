#include "arducosmos.h"

/* Define the static variables */
#ifndef NO_ARDUCOSMOS_TELEMETRY
	TelemetryState ArduCOSMOS::telemetryState = TelemetryState();
#endif
#ifndef NO_ARDUCOSMOS_COMMANDS
	CommandState ArduCOSMOS::commandState = CommandState();
#endif


void ArduCOSMOS::StartCosmos()
{
#ifndef NO_ARDUCOSMOS_TELEMETRY
	// Register the cosmos telemetry state (unless there is a guard against it)
	States::RegisterState(&telemetryState);
#endif
#ifndef NO_ARDUCOSMOS_COMMANDS
	// Register the COSMOS command state (unless there is a guard against it)
	States::RegisterState(&commandState);
#endif
}

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
