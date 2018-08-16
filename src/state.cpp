#include "state.h"
#include "Arduino.h"

#ifdef WITH_STD_LIB
	std::vector<State*> States::StatesArray;
#else
	NS_ArduCOSMOS::LinkedList<State*> States::StatesArray;
#endif

State::State()
{
	last_call = millis();
	Call();
}

void State::Tick()
{
	unsigned long current_time = millis();

	if (current_time >= (last_call + interval))
	{
		last_call = current_time;
		Call();
	}
}

void State::Call()
{
	// Default behavior - do nothing! (derrived classes should override Call and add their own behavior)
}

#ifdef WITH_STD_LIB
	std::vector<State*> States::GetStates()
#else
	NS_ArduCOSMOS::LinkedList<State*> States::GetStates()
#endif
{
	return StatesArray;
}

void States::RegisterState(State *state)
{
	StatesArray.push_back(state);
}

bool States::UnregisterState(State *state)
{
#ifdef WITH_STD_LIB
	for (std::vector<State*>::const_iterator it = StatesArray.begin(); it != StatesArray.end(); it++)
#else
	for (NS_ArduCOSMOS::LinkedList<State*>::Iterator it = StatesArray.begin(); it; it++)
#endif
	{
		if (*it == state)

		{
			// TODO *actually* remove the state.
			StatesArray.erase(it.GetLowLevel());
			return true;
		}
	}

	return false;
}

void States::Tick()
{
	// Iterate the array, and invoke the tick functionality for all of them.
#ifdef WITH_STD_LIB
	for (std::vector<State*>::const_iterator it = StatesArray.begin(); it != StatesArray.end(); it++)
#else
	for (NS_ArduCOSMOS::LinkedList<State*>::Iterator it = StatesArray.begin(); it; it++)
#endif
	{
		(*it)->Tick();
	}
}
