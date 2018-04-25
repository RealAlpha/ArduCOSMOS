#ifndef SRC_STATE_H_
#define SRC_STATE_H_

// Use STL vectors as I'm too lazy to create a linekd list from scratch
#include <ArduinoSTL.h>
#include <vector>

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
	static std::vector<State*> GetStates();

	// "Register" / add a state
	static void RegisterState(State *state);

	// "Unregister" / remove a state. (pointers should match)
	static bool UnregisterState(State *state);

	// "Process"/tick all of the states
	static void Tick();

private:
	// The variable that actually stores all of the states.
	static std::vector<State*> StatesArray;
};
#endif
