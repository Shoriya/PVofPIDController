#include "systemc-ams"
#include "systemc.h"

SCA_TDF_MODULE(pid)
{
	sca_tdf::sca_in<double> Pid_in;
	sca_tdf::sca_out<double> Pid_out;
	sca_tdf::sca_de::sca_in<double>channel1_in;
	sca_tdf::sca_de::sca_in<double>channel2_in;
	sca_tdf::sca_de::sca_in<double>channel3_in;

	SCA_CTOR(pid)
	{
		//g0=1.0;

	}

	void set_attributes()
	{
		set_timestep(10,SC_US);

	}

	void initialize()
	{
		dd(1)=1;
	}

	void processing()
	{
		nn(0)=(double)channel1_in.read();//ki
		nn(1)=(double)channel2_in.read();//kp
		nn(2)=(double)channel3_in.read();//kd
		Pid_out.write(lt_nd(nn,dd,Pid_in.read()));
		cout<<nn(0);
		cout<<nn(1);
		cout<<nn(2);
	}

private:
	double g0;
	sca_util::sca_vector<double> nn;
	sca_util::sca_vector<double> dd;
	sca_tdf::sca_ltf_nd lt_nd;
};

