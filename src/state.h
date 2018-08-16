#ifndef ARDUCOSMOS_STATE_H_
#define ARDUCOSMOS_STATE_H_

#ifdef WITH_STD_LIB
// If the C++ standard library is available (-> #define in arducosmos.h is uncommented), then use std.
#include <vector>
#else
// Otherwise, use a simple linked-list implementation.
#include "linkedlist.h"
#endif

/*
 * State class - a base class for states/tasks that need to be executed at a specific interval.
 *
 * Subclasses should should have a constructor that calls super and sets the interval (in ms). Classes should also override the Call() function and execute the desired behavior there.
 */
class State
{
public:
  	// Constructor
	State();

	// Should be called every loop() iteration; Will handle the ticking of the function, and calling the behavior at the required interval.
	void Tick();

	// Override this - the behavior that should be executed every interval ms.
	virtual void Call();

private:
	// Last uptime ms at which we executed Call()
	unsigned long last_call;

protected:
	// Intervall at which Call() should be executed in milliseconds. (should be set in constructor; default is 500ms/half a second)
	int interval = 500;
};

class States
{
public:
	// Return all of the available states.
#ifdef WITH_STD_LIB
	static std::vector<State*> GetStates();
#else
	static ArduCOSMOS::LinkedList<State*> GetStates();
#endif
	// "Register" / add a state
	static void RegisterState(State *state);

	// "Unregister" / remove a state. (pointers should match)
	static bool UnregisterState(State *state);

	// "Process"/tick all of the states
	static void Tick();

private:
	// The variable that actually stores all of the states.
#ifdef WITH_STD_LIB
	static std::vector<State*> StatesArray;
#else
	static ArduCOSMOS::LinkedList<State*> StatesArray;
#endif
};
#endif
