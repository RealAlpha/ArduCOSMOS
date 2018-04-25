#include "state.h"
#include "Arduino.h"

std::vector<State*> States::StatesArray;

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

std::vector<State*> States::GetStates()
{
	return StatesArray;
}

void States::RegisterState(State *state)
{
	StatesArray.push_back(state);
}

bool States::UnregisterState(State *state)
{
	for (std::vector<State*>::const_iterator it = StatesArray.begin(); it != StatesArray.end(); it++)
	{
		if (*it == state)
		{
			// TODO *actually* remove the state.
			StatesArray.erase(it);
			return true;
		}
	}

	return false;
}

void States::Tick()
{
	// Iterate the array, and invoke the tick functionality for all of them.
	for (std::vector<State*>::const_iterator it = StatesArray.begin(); it != StatesArray.end(); it++)
	{
		(*it)->Tick();
	}
}
