# ArduCOSMOS
### An Arduino Library that Helps Implement [COSMOS](https://cosmosrb.com/) Telemetry & Command Functionality.

[![Build Status](https://travis-ci.org/RealAlpha/ArduCOSMOS.svg?branch=master)](https://travis-ci.org/RealAlpha/ArduCOSMOS) [![Platform](https://img.shields.io/badge/platform-Arduino-blue.svg)](https://www.arduino.cc/) 
[![License](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/RealAlpha/ArduCOSMOS/blob/master/LICENSE)

## What is ArduCOSMOS?
Arducosmos is a library for the Arduino/Genuino platform to help simplify working with telemetry and commands using [Ball Aerospace's COSMOS](https://cosmosrb.com/). We aim to reduce the arduino side of things to just a few lines! See the below example for adding some telemetry to an analog sensor:

```cpp
// Declare/create a telemetry packet containing an integer value and a packet ID of 0
DECLARE_TELEMETRY_OneParam(sensorTelemetry, 0, uint16_t, value);
```

```cpp
void setup()
{
	// Start COSMOS (NOTE: This will automatically start a serial connection)
	ArduCOSMOS::StartCosmos();

	// Register our sensor telemetry to replicate every 100ms
	REGISTER_TELEMETRY(sensorTelemetry, 100);
}
```

```cpp
void loop()
{
	// Obtain the sensor reading
	int readValue = analogRead(0);
	
	// Update the telemetry
	sÿEensorTelemetry.value = readValue;

	// Tick all of the states so it checks for new commands/replicates the telemetry if required
	States::Tick();
}
```

As you can see, if we remove the value updating line, sensor reading line, and the boilerplate, we're left with three lines to declare and replicate the telemetry!

## Cool, but what is COSMOS...really?
COSMOS is a telemetry gathering and command sending tool developed by Ball Aerospace. It's something that allows you to easily analyze, debug, and control (embedded) devices. COSMOS is being used in a whole range of projects, including satelites! COSMOS is currently a pain to configure, but there are plenty of resources online to help you with this, and trust me, once you get used to it, it'll be amazing to use. It's also open source, meaning you can look at the source code, and even contribute to the project itself! Find the COSMOS site here: [cosmosrb.com](http://cosmosrb.com)

## I'm in! How do I get started using ArduCOSMOS?
We're currently still in the process of tying up a few loose ends of the library, and writing documentation to simplify the usage of it. If you have experience using COSMOS (and its configuration) and just want to give it a try, you can download the GitHub repository, place it in your arduino libraries, and then include the `arducosmos.h` header. Also check the dependencies section!

If you're new to COSMOS we highly reccomend you check out our [getting started tutorial](https://github.com/RealAlpha/ArduCOSMOS/wiki/Creating-your-first-telemetry-gathering-program!), which covers everything from setting up a COSMOS project to creating a simple telemetry circuit and switch. 

## Dependencies
ArduCOSMOS currently bundles it's own "state machine" to multiplex all of it's functionality. Previously, ArduCOSMOS also depended on ArduinoSTL for `std::vector`, but we have now bundled an `std::vector`-like linked list implementation with ArduCOSMOS. If your toolchain support the STL library, and more specifically, `std::vector`, you may switch over to using that by defining `WITH_STD_LIB` in `state.h`. There are currently no other dependancies ArduCOSMOS.

## Supported Platforms
As of right now, we have only been able to verify full support for the following (Arduino) boards:
- Arduino Uno r3
- Arduino Mega r3
- Generic STM32F103C8-based development board with STM32Arduino (NOTE: Works both with `WITH_STD_LIB` and without `WITH_STD_LIB`)

We do not (currently) have any platform-specific code, so we expect ArduCOSMOS to work on all (Arduino) boards, but can not guarantee this. Have you successfully managed to get ArduCOSMOS to work on another board? Then please consider submitting a PR adding your board to the above list! Experiencing issues (with a board)? Please open up an issue with as much detailed information as you can so we can look into the issue!

# WARNING: ArduCOSMOS is currently still in beta stages!
ArduCOSMOS is currently still in development. We're still working on ironing out a few bugs, and writing documentation. If you decide to still use ArduCOSMOS, and are experiencing issues, then please report them by opening up an [issue](https://github.com/RealAlpha/ArduCOSMOS/issues) here on GitHub. We're open to relevant suggestions too, and in order to put in your suggestion, you can open up an issue, and clearly mark it as a suggestion in the first 1-2 lines.
