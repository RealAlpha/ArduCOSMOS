/*
 * Main include file for the Arduino COSMOS library written by Alpha-V (arducosmos).
 * Library is used for gathering telemetry from your arduino and sending it to COSMOS, and receiving commands from COSMOS to execute behavior using them.
 * 
 * This file will include all of the required files to get started with arducosmos. Please refer to the github repository for more information.
 */

#ifndef ARDUCOSMOS_ARDUCOSMOS_H_
#define ARDUCOSMOS_ARDUCOSMOS_H_

#ifndef NO_ARDUCOSMOS_TELEMETRY
#include "telemetrystate.h"
#endif
#ifndef NO_ARDUCOSMOS_COMMANDS
#include "commandstate.h"
#endif

// TODO: Implement static functionality to start/register the states (start COMSOS) functionality, and (static) functionality to register commands/telemetry
class ArduCOSMOS
{
public:
	/*
	 * Start the COSMOS telemetry sender/command sender.
	 *
	 * NOTE: This does NOT register any telemetry or commands yet!
	 */
	static void StartCosmos();

#ifndef NO_ARDUCOSMOS_COMMANDS
	/*
	 * Start the COSMOS telemetry sender/command sender, selectingi if it should listen for commands or not.
	 *
	 * NOTE: This does NOT register any telemetry or commands yet!
	 *
	 * @param bUseCommands Should ArduCOSMOS use commands, or should it not parse all serial data for them?
	 */
	static void StartCosmos(bool bUseCommands);
#endif
	
#ifndef NO_ARDUCOSMOS_TELEMETRY
	/*
	 * Register a COSMOS telemetry packet to send to the computer.
	 *
	 * @param Packet Pointer to the telemetry packet (Must inheret from telemetry_t) to send to COSMOS.
	 * @param Interval Time in miliseconds in which to replicate it to COSMOS. Must be a multiple of 5.
	 */
	static void RegisterTelemetry(telemetryPacket_t *packet, int interval);
#endif

#ifndef NO_ARDUCOSMOS_COMMANDS
	/*
	 * Register a COSMOS command (binding) to execute behavior based upon
	 *
	 * @param Command The command to register with cosmos (should EXACTLY match the size/variables in cosmos, or it could cause HUGE issues / segmentation fauls / corruption)
	 * @param Binding The function to execute when the command gets sent from cosmos. Should be typecasted to commandBinding
	 */
	static void RegisterCommand(command_t command, commandBinding binding);
#endif
private:
	/*
	 * In the case the user has a define to exclude any of the telemetry/cosmos parts of ARDUCOSMOS, we won't create the variables either.
	 */
#ifndef NO_ARDUCOSMOS_TELEMETRY
	static TelemetryState telemetryState;
#endif
#ifndef NO_ARDUCOSMOS_COMMANDS
	static CommandState commandState;
#endif

#endif
};
