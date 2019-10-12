#include"PID.h"
#include"Diff.h"
#include"pwm.h"
#include"motor.h"
#include"source.h"
#include"Initiator.h"
#include"target.h"

int sc_main(int argc,char* argv[])
{
	/*definition of PID Controller signals*/
	sca_tdf::sca_signal<double> iref;
	sca_tdf::sca_signal<double>error_sig;
	sca_tdf::sca_signal<double>pid_sig;
	sca_tdf::sca_signal<double>vdrv;
	sca_tdf::sca_signal<double>imeans;

	sc_signal<double>configure1;
	sc_signal<double>configure2;
	sc_signal<double>configure3;


	diff* Diff=new diff ("diff");	//module to get the error signal
	source* s_tb=new source("tb");	//source module
	pid* Pid=new pid("Pid");		//calculated PID
	pwm Pwm("pwm");
	motor Motor("motor");
	Initiator *initiator;
	Memory    *memory;


	// Instantiate components
	initiator = new Initiator("initiator");
	memory    = new Memory   ("memory");



    // Bind initiator socket to target socket
	initiator->socket.bind( memory->socket );


	s_tb->ref_value(iref);

	Diff->in_ref(iref);
	Diff->in_means(imeans);
	Diff->out_sig(error_sig);

	memory->Channel1_Out(configure1);
	memory->Channel2_Out(configure2);
	memory->Channel3_Out(configure3);

	Pid->channel1_in(configure1);
	Pid->channel2_in(configure2);
	Pid->channel3_in(configure3);

	Pid->Pid_in(error_sig);
	Pid->Pid_out(pid_sig);


	Pwm.in(pid_sig);
	Pwm.vdrv(vdrv);

	Motor.motor_in(vdrv);
	Motor.motor_out(imeans);



	/*Signal Tracing*/
	sca_util::sca_trace_file* file=sca_util::sca_create_tabular_trace_file("control.dat");

	sca_util::sca_trace(file,iref,"Iref");
	sca_util::sca_trace(file,error_sig,"error_sig");
	sca_util::sca_trace(file,pid_sig,"pid_sig");
	sca_util::sca_trace(file,vdrv,"vdrv");
	sca_util::sca_trace(file,imeans,"imeans");

	/*starting the trace*/
	sc_start(100,SC_MS);
	return 0;

}
