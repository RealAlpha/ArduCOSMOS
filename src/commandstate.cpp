#include "commandstate.h"

// Include the arduino library for Serial functionality - NOTE: Assume that the serial connection has already been started
#include "Arduino.h"

NS_ArduCOSMOS::LinkedList<commandBinding_t> CommandState::commandBindings;

CommandState::CommandState()
{
	// Since we need to constantly be checking if new serial data is available, we will set the interval to 0ms, thus ensuring it "always" invokes the Call() function.
	State::interval = 0;

	// Call the parent constructor
	State();
}

void CommandState::Call()
{
	// Is the serial pending data? (AKA: is there a command pending?)
	if (Serial.available())
	{
		// Attempt to read the command into a buffer (NOTE: Current command length limit is 256 bytes; Other solutions resulted in there being issues receiving the buffer ;( )
		uint8_t *buffer = (uint8_t*)malloc(256);
		int commandLength = Serial.readBytes(buffer, 256);
		uint8_t *commandBuffer = (uint8_t*)realloc(buffer, commandLength);
		
		/// If we got here that means that all of the command's bytes should have come in!
		// Is the first byte (ie the ID) set?
		if (commandLength >= 1)
		{
			// The command's ID should be the first byte, so extract it to a regular integer for easier usage
			uint8_t commandID = commandBuffer[0];
			
			// Iterate over the registered commands to try and find the one matching the ID we registered
#ifdef WITH_STD_LIB
			for (std::vector<commandBinding_t>::iterator it = commandBindings.begin(); it != commandBindings.end(); it++)
#else
			for (NS_ArduCOSMOS::LinkedList<commandBinding_t>::ListNode *it = commandBindings.begin(); it; it++)
#endif
			{
				// Extract the data into a variable to avoid having to write the below code
#ifdef WITH_STD_LIB
				commandBinding_t data = *it;
#else
				commandBinding_t data = **it;
#endif
				// Is the command/binding valid?
				if (!data.binding)
				{
					// TODO - Find a better solution for this error (hanlding)

					// Something is wrong with this binding, so log the error message (TODO: Should there be some meta setup with COSMOS for these kidns of logs?)
					Serial.print("Error with comands! Invalid command/binding! Target ID: ");
					Serial.println(commandID);
					
					// Something went wrong - an invalid command or binding was set up, but this doesn't mean that there are other valid commands, so continue with the next one
					continue;
				}
				
				// Is this the command COSMOS is trying to execute?
				if (data.command.id == commandID)
				{
					// Found the command! Execute the function, passing in the commandBuffer casted to the command_t. Assume user has set up their struct they're typecasting to correctly so it won't cause any size mismatches -> issues
					data.binding((command_t *)commandBuffer);
				}
			}

			// TODO Ensure the freeing of commandBuffer in all cases! We don't want any memory leaks!!!!!
		}
		else
		{
			// The command buffer somehow has a length of 0 (or less). Generally means that the Serial was available but buffer wasn't there within timeout?
			// TODO - Add an error handler, but this shouldn't ever be happening!	
		}
	}
}

void CommandState::RegisterCommand(command_t command, commandBinding binding)
{
	// Register this command by creating a binding (internal representation of a command & it's associated function) and adding it to the command bindings array so it can be searched for & found when receiving a command buffer.
	commandBinding_t commandBinding = commandBinding_t(command, binding);
	commandBindings.push_back(commandBinding);
}
