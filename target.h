#include "systemc"
using namespace sc_core;
using namespace sc_dt;
using namespace std;

#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"
#include "register.h"

// Target module representing a simple memory

struct Memory: sc_module
{
  // TLM-2 socket, defaults to 32-bits wide, base protocol
  tlm_utils::simple_target_socket<Memory> socket;


  SC_CTOR(Memory)
  : socket("socket")
  {
    // Register callback for incoming b_transport interface method call
    socket.register_b_transport(this, &Memory::b_transport);
  }

  // TLM-2 blocking transport method
  virtual void b_transport( tlm::tlm_generic_payload& trans, sc_time& delay )
  {
    tlm::tlm_command cmd = trans.get_command();
    sc_dt::uint64    adr = trans.get_address();
    unsigned char*   ptr = trans.get_data_ptr();
    unsigned int     len = trans.get_data_length();
    unsigned char*   byt = trans.get_byte_enable_ptr();
    unsigned int     wid = trans.get_streaming_width();

    // Obliged to check address range and check for unsupported features,
    //   i.e. byte enables, streaming, and bursts
    // Can ignore DMI hint and extensions
    // Using the SystemC report handler is an acceptable way of signalling an error

    if ( byt != 0 || len > 32 || wid < len)
      SC_REPORT_ERROR("TLM-2", "Target does not support given generic payload transaction");

    // Obliged to implement read and write commands

    switch(adr)
    {
    case CONTROL_REGISTER:
    	if(cmd == tlm::TLM_WRITE_COMMAND)
    	{
    		memcpy(&control_Register, ptr, len);
    	}

    	break;
    case CHANNEL_ENABLE_REGISTER:
    	if(cmd == tlm::TLM_WRITE_COMMAND)
    	{
    	    memcpy(&control_Enable_Register, ptr, len);
    	    memcpy(&channel_Status_Register, ptr, len);
    	    cout<<"control status register:"<<control_Enable_Register;
    	}
    	break;

    case CHANNEL_DISABLE_REGISTER:
        	if(cmd == tlm::TLM_WRITE_COMMAND)
        	{
        	    memcpy(&control_Disable_Register, ptr, len);
        	}
        	break;

    case CHANNEL_GAIN_REGISTER1:
        	if(cmd == tlm::TLM_WRITE_COMMAND)
        	{
        	    memcpy(&Channel1, ptr, len);
        	    Channel1_Out.write(Channel1);
        	}
        	break;
    case CHANNEL_GAIN_REGISTER2:
          if(cmd == tlm::TLM_WRITE_COMMAND)
          {
            memcpy(&Channel2, ptr, len);
            Channel2_Out.write(Channel2);
           }

           break;
    case CHANNEL_GAIN_REGISTER3:
          if(cmd == tlm::TLM_WRITE_COMMAND)
          {
            memcpy(&Channel3, ptr, len);
            Channel3_Out.write(Channel3);
          }

          break;
    case CHANNEL_STATUS_REGISTER:
    	if(cmd == tlm::TLM_READ_COMMAND)
    	{
    		memcpy(ptr,&channel_Status_Register ,len);
    	}
   }


    // Obliged to set response status to indicate successful completion
    trans.set_response_status( tlm::TLM_OK_RESPONSE );
  }
  double Channel1,Channel2,Channel3;
  sc_bv<7>  control_Register;
  sc_bv<32>  control_Enable_Register;
  sc_bv<32>  control_Disable_Register;
  sc_bv<32>  channel_Status_Register ;
  sc_out<double > Channel1_Out;
  sc_out<double > Channel2_Out;
  sc_out<double > Channel3_Out;
};
