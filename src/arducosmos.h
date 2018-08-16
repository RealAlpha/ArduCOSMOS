/*
 * Main include file for the Arduino COSMOS library written by Alpha-V (arducosmos).
 * Library is used for gathering telemetry from your arduino and sending it to COSMOS, and receiving commands from COSMOS to execute behavior using them.
 * 
 * This file will include all of the required files to get started with arducosmos. Please refer to the github repository for more information.
 */

#ifndef ARDUCOSMOS_ARDUCOSMOS_H_
#define ARDUCOSMOS_ARDUCOSMOS_H_

// Please uncomment the line below if you're compiling with a toolchain that supports the standard library, or more specifically, std::vector.
#define WITH_STD_LIB

#ifndef NO_ARDUCOSMOS_TELEMETRY
#include "telemetrystate.h"
#endif
#ifndef NO_ARDUCOSMOS_COMMANDS
#include "commandstate.h"
#endif

/*
 * Will help convert a telemetry name to the name of a struct to help differentiate type and variable name
 * NOTE: Works by suffixing _t to the name
 */
#define TO_STRUCT_NAME(NAME) NAME##_t

/*
 * Helper macro to declare the struct and implement its constructor. Missing final closing }; so variables can be implemented with other macros.
 */
#define DECLARE_TELEMETRY(NAME, ID) struct __attribute__((packed)) TO_STRUCT_NAME(NAME) : telemetryPacket_t { TO_STRUCT_NAME(NAME)() { telemetryPacket_t::id = ID; telemetryPacket_t::length = sizeof(TO_STRUCT_NAME(NAME)); };

/*
 * Declare / add a telemetry with a name, ID and one variable.
 *
 * @param Name The name of the newly created telemetry "packet". Can later be used with NAME.someVariable = someValue;
 * @param ID The ID that corresponds with what has been set up in COSMOS so it can be matched.
 * @param TypeX	The type of the variable with the corresponding Name
 * @param NameX The name of a telemetry item. Does not need to match order in cosmos, but does need to match the same Type & order.
 */
#define DECLARE_TELEMETRY_OneParam(NAME, ID, Type0, Name0) DECLARE_TELEMETRY(NAME, ID) Type0 Name0; }; TO_STRUCT_NAME(NAME) NAME;

/*
 * Declare / add a telemetry with a name, ID and two variables.
 *
 * @param Name The name of the newly created telemetry "packet". Can later be used with NAME.someVariable = someValue;
 * @param ID The ID that corresponds with what has been set up in COSMOS so it can be matched.
 * @param TypeX	The type of the variable with the corresponding Name
 * @param NameX The name of a telemetry item. Does not need to match order in cosmos, but does need to match the same Type & order.
 */
#define DECLARE_TELEMETRY_TwoParams(NAME, ID, Type0, Name0, Type1, Name1) DECLARE_TELEMETRY(NAME, ID) Type0 Name0; Type1 Name1; }; TO_STRUCT_NAME(NAME) NAME;

/*
 * Declare / add a telemetry with a name, ID and three variables.
 *
 * @param Name The name of the newly created telemetry "packet". Can later be used with NAME.someVariable = someValue;
 * @param ID The ID that corresponds with what has been set up in COSMOS so it can be matched.
 * @param TypeX	The type of the variable with the corresponding Name
 * @param NameX The name of a telemetry item. Does not need to match order in cosmos, but does need to match the same Type & order.
 */
#define DECLARE_TELEMETRY_ThreeParams(NAME, ID, Type0, Name0, Type1, Name1, Type2, Name2) DECLARE_TELEMETRY(NAME, ID) Type0 Name0; Type1 Name1; Type2 Name2; }; TO_STRUCT_NAME(NAME) NAME;

/*
 * Declare / add a telemetry with a name, ID and four variables.
 *
 * @param Name The name of the newly created telemetry "packet". Can later be used with NAME.someVariable = someValue;
 * @param ID The ID that corresponds with what has been set up in COSMOS so it can be matched.
 * @param TypeX	The type of the variable with the corresponding Name
 * @param NameX The name of a telemetry item. Does not need to match order in cosmos, but does need to match the same Type & order.
 */
#define DECLARE_TELEMETRY_FourParams(NAME, ID, Type0, Name0, Type1, Name1, Type2, Name2, Type3, Name3) DECLARE_TELEMETRY(NAME, ID) Type0 Name0; Type1 Name1; Type2 Name2; Type3 Name3; }; TO_STRUCT_NAME(NAME) NAME;

/*
 * Declare / add a telemetry with a name, ID and five variables.
 *
 * @param Name The name of the newly created telemetry "packet". Can later be used with NAME.someVariable = someValue;
 * @param ID The ID that corresponds with what has been set up in COSMOS so it can be matched.
 * @param TypeX	The type of the variable with the corresponding Name
 * @param NameX The name of a telemetry item. Does not need to match order in cosmos, but does need to match the same Type & order.
 */
#define DECLARE_TELEMETRY_FiveParams(NAME, ID, Type0, Name0, Type1, Name1, Type2, Name2, Type3, Name3, Type4, Name4) DECLARE_TELEMETRY(NAME, ID) Type0 Name0; Type1 Name1; Type2 Name2; Type3 Name3; Type4 Name4; }; TO_STRUCT_NAME(NAME) NAME;

/*
 * Declare / add a telemetry with a name, ID and six variables.
 *
 * @param Name The name of the newly created telemetry "packet". Can later be used with NAME.someVariable = someValue;
 * @param ID The ID that corresponds with what has been set up in COSMOS so it can be matched.
 * @param TypeX	The type of the variable with the corresponding Name
 * @param NameX The name of a telemetry item. Does not need to match order in cosmos, but does need to match the same Type & order.
 */
#define DECLARE_TELEMETRY_SixParams(NAME, ID, Type0, Name0, Type1, Name1, Type2, Name2, Type3, Name3, Type4, Name4, Type5, Name5) DECLARE_TELEMETRY(NAME, ID) Type0 Name0; Type1 Name1; Type2 Name2; Type3 Name3; Type4 Name4; Type5 Name5; }; TO_STRUCT_NAME(NAME) NAME;

/*
 * Declare / add a telemetry with a name, ID and seven variables.
 *
 * @param Name The name of the newly created telemetry "packet". Can later be used with NAME.someVariable = someValue;
 * @param ID The ID that corresponds with what has been set up in COSMOS so it can be matched.
 * @param TypeX	The type of the variable with the corresponding Name
 * @param NameX The name of a telemetry item. Does not need to match order in cosmos, but does need to match the same Type & order.
 */
#define DECLARE_TELEMETRY_SevenParams(NAME, ID, Type0, Name0, Type1, Name1, Type2, Name2, Type3, Name3, Type4, Name4, Type5, Name5, Type6, Name6) DECLARE_TELEMETRY(NAME, ID) Type0 Name0; Type1 Name1; Type2 Name2; Type3 Name3; Type4 Name4; Type5 Name5; Type6 Name6; }; TO_STRUCT_NAME(NAME) NAME;

/*
 * Declare / add a telemetry with a name, ID and eight variables.
 *
 * @param Name The name of the newly created telemetry "packet". Can later be used with NAME.someVariable = someValue;
 * @param ID The ID that corresponds with what has been set up in COSMOS so it can be matched.
 * @param TypeX	The type of the variable with the corresponding Name
 * @param NameX The name of a telemetry item. Does not need to match order in cosmos, but does need to match the same Type & order.
 */
#define DECLARE_TELEMETRY_EightParams(NAME, ID, Type0, Name0, Type1, Name1, Type2, Name2, Type3, Name3, Type4, Name4, Type5, Name5, Type6, Name6, Type7, Name7) DECLARE_TELEMETRY(NAME, ID) Type0 Name0; Type1 Name1; Type2 Name2; Type3 Name3; Type4 Name4; Type5 Name5; Type6 Name6; Type7 Name7; }; TO_STRUCT_NAME(NAME) NAME;

/*
 * Declare / add a telemetry with a name, ID and nine variables.
 *
 * @param Name The name of the newly created telemetry "packet". Can later be used with NAME.someVariable = someValue;
 * @param ID The ID that corresponds with what has been set up in COSMOS so it can be matched.
 * @param TypeX	The type of the variable with the corresponding Name
 * @param NameX The name of a telemetry item. Does not need to match order in cosmos, but does need to match the same Type & order.
 */
#define DECLARE_TELEMETRY_NineParams(NAME, ID, Type0, Name0, Type1, Name1, Type2, Name2, Type3, Name3, Type4, Name4, Type5, Name5, Type6, Name6, Type7, Name7, Type8, Name8) DECLARE_TELEMETRY(NAME, ID) Type0 Name0; Type1 Name1; Type2 Name2; Type3 Name3; Type4 Name4; Type5 Name5; Type6 Name6; Type7 Name7; Type8 Name8; }; TO_STRUCT_NAME(NAME) NAME;

/*
 * Declare / add a telemetry with a name, ID and ten variables.
 *
 * @param Name The name of the newly created telemetry "packet". Can later be used with NAME.someVariable = someValue;
 * @param ID The ID that corresponds with what has been set up in COSMOS so it can be matched.
 * @param TypeX	The type of the variable with the corresponding Name
 * @param NameX The name of a telemetry item. Does not need to match order in cosmos, but does need to match the same Type & order.
 */
#define DECLARE_TELEMETRY_TenParams(NAME, ID, Type0, Name0, Type1, Name1, Type2, Name2, Type3, Name3, Type4, Name4, Type5, Name5, Type6, Name6, Type7, Name7, Type8, Name8, Type9, Name9) DECLARE_TELEMETRY(NAME, ID) Type0 Name0; Type1 Name1; Type2 Name2; Type3 Name3; Type4 Name4; Type5 Name5; Type6 Name6; Type7 Name7; Type8 Name8; Type9 Name9; }; TO_STRUCT_NAME(NAME) NAME;

/*
 * NOTE: Further macro definitions do not (currently) exist. If you want to define more variables, please implement the struct from "scratch". Please see our documentation for more information about this.
 */

/*
 * Macro to help (further) simplify registering telemetry. Intended to be used in conjunction with DECLARE_TELEMETRY_SomeParams.
 *
 * NOTE: Should be used within a function (most likely void Setup()!). Will cause compile issues due to the function invocation otherwise.
 */
#ifndef NO_ARDUCOSMOS_TELEMETRY
#define REGISTER_TELEMETRY(NAME, INTERVAL) ArduCOSMOS::RegisterTelemetry(&NAME, INTERVAL);
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

};
#endif
