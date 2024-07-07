# Introduction.

The ArduRTC project converts an arduino into a simple RTC timer that may perform a trigger at a specified time.

# Example of Usage

An example of use would be with a Raspberry Pi and an Arduino Pro Mini connected via the i2c interface.
The raspberry is attached to a UPS. When it shuts down at some time in the night, it configures the arduino pro mini, which serves as an RTC, to trigger an event on a pin. The pin is connected to a switch on the UPS. When the event occurs, the UPS goes on and powers the Raspberry Pi.