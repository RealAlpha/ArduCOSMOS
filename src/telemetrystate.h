#ifndef SRC_TELEMETRYSTATE_H_
#define SRC_TELEMETRYSTATE_H_

#include "state.h"
#include "globals.h"

#include <ArduinoSTL.h>
#include <vector>

/*
 * Basic implementation for a packet to be sent to COSMOS
 */
struct telemetryPacket_t
{
	uint8_t length = sizeof(telemetryPacket_t);
	uint8_t id;
	
	/* Further information (like the actual data to be logged) will be placed in "sub"-structs */
};

/*
 * Struct that keeps track of a registered telemetry packet.
 */
struct telemetryRegistration_t
{
	// Packet to track
	telemetryPacket_t *tel;
	
	// Interval at which to send it back to cosmos (in ms)
	int interval;

	// Runtime (in milliseconds) that the telemetry was last sent.
	long int lastCall;
};

/*
 * Telemetry state class - handles gathering/sending telemetry off to COSMOS to be analyzed.
 * NOTE - Data gathered needs to be set to the global variables in globals.h, and will be saved in the structures defined within the class
 */
class TelemetryState : public State
{
public:
	// Constructor - sets interval & calls super to begin initialization
	TelemetryState();
	
	// Called when this state ticks
	virtual void Call() override;

	// Registers a packet to keep sending to COSMOS at the provided interval (in ms. Min. interval is 5ms)
	void RegisterTelemetry(telemetryPacket_t *packet, int inverval);

private:
	long int last_time;

	std::vector<telemetryRegistration_t> telemetryRegistrations;
};
#endif
