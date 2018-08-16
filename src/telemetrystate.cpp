#include "telemetrystate.h"

#include "Arduino.h"

// Declare helper functions

// Helper function to send a telemetry packet over the serial connection (assuming it's connected)
void sendTelemetry(telemetryPacket_t *packet);

// Define statics
#ifdef WITH_STD_LIB
std::vector<telemetryRegistration_t> TelemetryState::telemetryRegistrations;
#else
NS_ArduCOSMOS::LinkedList<telemetryRegistration_t> TelemetryState::telemetryRegistrations;
#endif

TelemetryState::TelemetryState()
{
	// We have our own "state" checking, so set this to a pretty minimal interval (the min. telemetry interval)
	State::interval = 5;
	// Call the parent constructor
	State();
}

void TelemetryState::Call()
{
	int current_time = millis();
#ifdef WITH_STD_LIB
	for (std::vector<telemetryRegistration_t>::iterator it = telemetryRegistrations.begin(); it != telemetryRegistrations.end(); it++)
#else
	for (NS_ArduCOSMOS::LinkedList<telemetryRegistration_t>::ListNode *it = telemetryRegistrations.begin(); it; it++)
#endif
	{
		// Extract the data into a variable to avoid having to write the below code
#ifdef WITH_STD_LIB
		telemetryRegistration_t data = *it;
#else
		telemetryRegistration_t data = **it;
#endif
		int delta_t = current_time - data.lastCall;

		// Was there enough of a time diference to relog this telemetry packet
		if (delta_t >= data.interval)
		{
			sendTelemetry(data.tel);
			
			data.lastCall = current_time;
		}
	}
}

void TelemetryState::RegisterTelemetry(telemetryPacket_t *packet, int interval)
{
	// Check if the packet is valid (ie not a nullptr). If it isn't valid, then don't try to register it.
	if (!packet)
	{
		return;
	}
	
	// Create the registration and set it's details to match the desired values
	telemetryRegistration_t registration;
	
	registration.tel = packet;
	registration.interval = interval;
	// Set the last call to 0 to "ensure" that it'll run soon/on the next it?
	registration.lastCall = 0;

	// Add this registration to the vector of tracked telemetry packets
	telemetryRegistrations.push_back(registration);
}

void sendTelemetry(telemetryPacket_t *packet)
{
	// Ensure the packet is valid (not nullptr) and the serial is valid (available)
	if (!packet || !Serial)
	{
		return;
	}

	// NOTE - Since sizeof is calculated at compiletime, we can safely assume that overriding it in all of the subclasses and setting it right away will work. This avoids any complications to do with getting the size of a pointer to a "sub"-struct
	// Convert the packet into a const char* so it can more easily be transmitted over the serial connection
	const char* packet_char = (const char*)packet;	

	for (int i = 0; i < packet->length; i++)
	{
		Serial.write(packet_char[i]);
	}
}
