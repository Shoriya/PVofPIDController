#include "systemc"
using namespace sc_core;
using namespace sc_dt;
using namespace std;

#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "register.h"

struct Initiator: sc_module
{
	tlm_utils::simple_initiator_socket<Initiator> socket;

	SC_CTOR(Initiator): socket("socket")
	{
		SC_THREAD(Functions);
	}

	void Functions()
	{
		channel_Status_Register[0]=1; //Setting the value to be used as PId Controller
		channel_Status_Register[1]=1;
		channel_Status_Register[2]=1;

		here:
		int choice;
		cout<<"\n\n 1. PID_Configuration \n 2. PID_GetStatus \n 3. PID_GetCoefficients \n Enter Your choice \n\n";
		cin>>choice;
		switch(choice)
		{
		case 1:
			PID_Configuration();
			break;
		case 2:
			PID_GetStatus();
			break;
		case 3:
			PID_GetCoEfficients();
			break;
		}

		char opt;
		cout<<"\n\n Do you want to continue (Y N)\n\n";
		cin>>opt;
		if(opt=='Y'||opt=='y')
		{
			goto here;
		}
		else if(opt=='N'||opt=='n')
		{
		}
		else
		{
			cout<<"Wrong input";

		}
	}
	void PID_Configuration()
	{
		int choice;
		cout<<"\n\n 1.Disable PID \n 2.Enable PID"<<"\n Enter your Choice \n\n";
		cin>>choice;

		switch(choice)
		{
		case 1:
		cout<<"case1:PID Disabled";
		control_Register=1;
		channel_Status_Register=0;
		control_Enable_Register=0;
		control_Disable_Register[0]=control_Disable_Register[1]=control_Disable_Register[2]=1;
		cmd=tlm::TLM_WRITE_COMMAND;
		thread_process(control_Register,cmd,CONTROL_REGISTER);
		thread_process(control_Enable_Register,cmd,CHANNEL_ENABLE_REGISTER);
		thread_process(control_Disable_Register,cmd,CHANNEL_DISABLE_REGISTER);

		break;

		case 2:
			cout<<"\ncase1:PID Enabled\n";
			cout<<"\nEnter the value for kp\n";
			cin>>Channel1_Out;
			cout<<"\nEnter the value for kd\n";
			cin>>Channel2_Out;
			cout<<"\nEnter the value for ki\n";
			cin>>Channel3_Out;
			control_Register=2;
			(Channel1_Out>0)?(control_Enable_Register[0]=1):(control_Enable_Register[0]=0);
			(Channel2_Out>0)?(control_Enable_Register[1]=1):(control_Enable_Register[1]=0);
			(Channel3_Out>0)?(control_Enable_Register[2]=1):(control_Enable_Register[2]=0);
			cmd=tlm::TLM_WRITE_COMMAND;
			thread_process(control_Register,cmd,CONTROL_REGISTER);
			thread_process(control_Enable_Register,cmd,CHANNEL_ENABLE_REGISTER);
			thread_process(control_Disable_Register,cmd,CHANNEL_DISABLE_REGISTER);
			thread_process(Channel1_Out,cmd,CHANNEL_GAIN_REGISTER1);
			thread_process(Channel2_Out,cmd,CHANNEL_GAIN_REGISTER2);
			thread_process(Channel3_Out,cmd,CHANNEL_GAIN_REGISTER3);
			cout<<"\n\n PID Enabled with data values as:\n";
			cout<<"Ki:"<<Channel1_Out<<"\n";
			cout<<"Kp:"<<Channel2_Out<<"\n";
			cout<<"Kd:"<<Channel3_Out<<"\n";
		}
	}
	void PID_GetStatus()
	{
		cout<<"Current Configuration \n";
		cmd=tlm::TLM_READ_COMMAND;
		thread_process(control_Register,cmd,CONTROL_REGISTER);
		thread_process(control_Enable_Register,cmd,CHANNEL_ENABLE_REGISTER);
		thread_process(control_Disable_Register,cmd,CHANNEL_DISABLE_REGISTER);
		thread_process(channel_Status_Register,cmd,CHANNEL_STATUS_REGISTER);

		cout<<"\nControl Register:"<<control_Register;
		cout<<"\nControl Enable Register:"<<control_Enable_Register;
		cout<<"\nControl Disable Register:"<<control_Disable_Register;
		cout<<"\nControl Status Register:"<<channel_Status_Register;
	}

	void PID_GetCoEfficients()
	{
		cout<<"Get CoEfficients Values \n";
		cmd=tlm::TLM_READ_COMMAND;
		thread_process(Channel1_Out,cmd,CHANNEL_GAIN_REGISTER1);
		thread_process(Channel2_Out,cmd,CHANNEL_GAIN_REGISTER2);
		thread_process(Channel3_Out,cmd,CHANNEL_GAIN_REGISTER3);
		cout<<"\nKp:"<<Channel1_Out;
		cout<<"\nKi:"<<Channel2_Out;
		cout<<"\nKd:"<<Channel3_Out;


	}
	template<typename T>
	void  thread_process(T data,tlm::tlm_command cmd,sc_dt::uint64 adr)
	{
		tlm::tlm_generic_payload* trans = new tlm::tlm_generic_payload;
		sc_time delay = sc_time(0, SC_NS);

		trans->set_command(cmd);
		trans->set_address(adr);
		trans->set_data_ptr(reinterpret_cast<unsigned char*> (&data));
		trans->set_data_length( 32 );
		trans->set_streaming_width( 32 ); // = data_length to indicate no streaming
		trans->set_byte_enable_ptr( 0 ); // 0 indicates unused
		trans->set_dmi_allowed( false ); // Mandatory initial value
		trans->set_response_status( tlm::TLM_INCOMPLETE_RESPONSE ); // Mandatory initial value

		socket->b_transport( *trans, delay );  // Blocking transport call

		if ( trans->is_response_error() )
			SC_REPORT_ERROR("TLM-2", "Response error from b_transport");

		wait(delay);

	}

	// Internal data buffer used by initiator with generic payload
	tlm::tlm_command cmd;
	sc_dt::uint64    adr;
	sc_bv<7>  control_Register;
	sc_bv<32>  control_Enable_Register;
	sc_bv<32>  control_Disable_Register;
	sc_bv<32>  channel_Status_Register;
	double Channel1_Out;
	double Channel2_Out;
	double Channel3_Out;
};

