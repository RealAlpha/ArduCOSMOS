#ifndef NS_ArduCOSMOS_COMMANDSTATE_H_
#define NS_ArduCOSMOS_COMMANDSTATE_H_

#include "state.h"

#include <stdint.h>

#ifdef WITH_STD_LIB
// If the C++ standard library is available (-> #define in NS_ArduCOSMOS.h is uncommented), then use std.
#include <vector>
#else
// Otherwise, use a simple linked-list implementation.
#include "linkedlist.h"
#endif

/*
 * Basic implementation for a command recieved from COSMOS
 */
struct __attribute__((packed)) command_t
{
	uint8_t id;

	/*
	 * Further parameters left up to children. NOTE: Children need to have the same order/datatypes & need to be packed
	 */
};

/*
 * Macro used to simplify the creation of a command instance. Gets rid of having to take care of the packed/inheretence stuff
 */
#define command(A)		struct __attribute__((packed)) A : command_t

/*
 * Type used for functions that are (going to be) called when a command matching the binding's ID comes in.
 */
typedef void (*commandBinding)(command_t *command);

/*
 * Used for an internal representation of a command bound to a given function
 */
struct commandBinding_t
{
	// Default commandBinding_t constructor that does nothing; used so the linked list won't freak out.
	commandBinding_t()
	{

	}

	// commandBinding_t constructor that intiializes a command binding using a command, and a binding -> function to call when the command is detected.
	commandBinding_t(command_t _command, commandBinding command_binding)
	{
		command = _command;
		binding = command_binding;
	}

	command_t command;

	// NOTE: Should be able to safely use the provided command as there is only one thread -> can't have a new instance while this one is still processing
	commandBinding binding;
};

/*
 * Command state class - will "listen" to the serial port for registered commands (sent from COSMOS) and invoke the bound functions when it receives them.
 * NOTE - When serial data comes in (eg. serial is available) it will take (at most) the serial timeout's time & block the thread. Reccomended action is to set the serial time out to a low(-ish) value.
 * NOTE - Commands can NOT be more than 256 bytes!!!
 */
class CommandState : public State
{
public:
	// Constructor - sets interval & calls super to begin initialization
	CommandState();
	
	// Called when this state ticks
	virtual void Call() override;

	// Registers a command to "listen" for when receiving serial data (from COSMOS)
	void RegisterCommand(command_t command, commandBinding binding);

private:
	// Define statics
	#ifdef WITH_STD_LIB
	std::vector<commandBinding_t> commandBindings;
	#else
	static NS_ArduCOSMOS::LinkedList<commandBinding_t> commandBindings;
	#endif

};

#endif
