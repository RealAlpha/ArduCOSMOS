# ArduCOSMOS
### A (Work-in-Progress) Arduino Library that Helps Implement COSMOS Telemetry & Command Functionality.

## What is ArduCOSMOS?
Arducosmos is a Work-in-Progress library for the Arduino/Genuino platform to help simplify working with telemetry and commands using Ball Aerospace's COSMOS. We aim to reduce the arduino side of things to just a few lines! See the below example for adding some telemetry to an analog sensor:

```cpp
// Declare/create a telemetry packet containing an integer value and a packet ID of 0
DECLARE_TELEMETRY_OneParam(sensorTelemetry, 0, uint16_t, value);
```

```cpp
void Setup()
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
	sensorTelemetry.value = readValue;

	// Tick all of the states so it checks for new commands/replicates the telemetry if required
	States::Tick();
}
```

As you can see, if we remove the value updating line, sensor reading line, and the boilerplate, we're left with three lines to declare and replicate the telemetry!

## Cool, but what is COSMOS...really?
COSMOS is a telemetry gathering and command sending tool developed by Ball Aerospace. It's something that allows you to easily analyze, debug, and control embedded devices. COSMOS is being used in a whole range of projects, including satelites! COSMOS is currently a pain to configure, but there are plenty of resources online to help you with this, and it's open source, meaning you can look at the source code, and even contribute to the project itself! Find the COSMOS site here: [cosmosrb.com](http://cosmosrb.com)

## I'm in! How do I get started using ArduCOSMOS?
We're currently still in the process of finishing up the library and writing documentation to simplify the usage of it. If you have experience using COSMOS (and its configuration) and just want to give it a try, you can download the GitHub repository, place it in your arduino libraries, and then include the `arducosmos.h` header. 
