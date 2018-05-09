#ifndef ARDUCOSMOS_TELEMETRYSTATE_H_
#define ARDUCOSMOS_TELEMETRYSTATE_H_

#include "state.h"

#include <ArduinoSTL.h>
#include <vector>

/*
 * Basic implementation for a packet to be sent to COSMOS
 */
struct __attribute__((packed)) telemetryPacket_t
{
	uint8_t length = sizeof(telemetryPacket_t);
	uint8_t id;

	/* Further information (like the actual data to be logged) will be placed in "sub"-structs. A constructor should also be implemented in child structs that sets the ID/length!!! */
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
 * NOTE - For telemetry to be gathered a child struct of telemetryPacket_t (with a constructor & the to be tracked data) should be created, and registered using RegisterTelemetry.
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
