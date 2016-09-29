# projector-adapter

ProjectorAdapter is a solution for timing camera exposure time with a Digital Micromirror Device (DMD) trigger signal for use in 3D recording.

### Hardware
The firmware runs on a [Teensy 3.2](https://www.pjrc.com/store/teensy32.html).

A custom made PCB was made with the same footprint as the Teensy3.2 with molex connectors for interfacing with a camera and projector DMD.

The PCB sits below the Teensy, with the 6 pin connectors to and from the projector and the 4 pin header routed to the camera trigger cable.

![Custom PCB Shield](http://github.com/ruffner/projector-adapter/raw/master/images/v3pcb.jpg "Custom PCB Shield")

It is written to interface with Ximea cameras using [this](https://www.ximea.com/support/wiki/apis/XiAPI_Camera_Trigger_and_Synchronization_Signals) protocall.

Here is a scope trace of the signals involved: ![Scope Trace](http://github.com/ruffner/projector-adapter/raw/master/images/scope-trace.jpg "Scope Trace")

### Firmware and operation

The green trace is the trigger output from the Teensy3.2, at 1Hz with a 20% duty cycle. This signal is routed to the projector. Once the projector has been triggered it sends out trigger pulses (blue), intended to trigger the camera. However, it sends pulses until the next rising edge of its 20% duty cycle trigger signal. This would result in the camera exposing more times than desired.

The solution is to intercept the projector output pulses and only pass one through to the camera. The trigger pulse recreated by the Teensy, seen in pink, must be centered around the pulse it is replicating to avoid skewing of camera exposure timing. The orange trace is the output from the camera indicating that it has indeed been triggered.
