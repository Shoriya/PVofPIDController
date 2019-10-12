# PVofPIDController
Programmable View of PID Controller using TLM 

This is an on going project I am working on for the developing the PV which is described by the Register name and address map. 

The control loop should be connected to the master module (ECU) through TLM interface.
You are free to choose write/read TLM methods (either a blocking or non-blocking
transport) and a master module. The master module should be connected to the control
loop through TLM initiator->target techniques. The control loop system is here a slave
module. The initiator and target are in the master and slave module, resp.
The master module should execute required software functions through the PV:
• PID_configuration – configure the PID controller using the PV
• PID_GetStatus – returns current configuration
• PID_GetCoefficients – returns content of gain registers for enabled channels.
