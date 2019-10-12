#include<systemc-ams.h>
#include<systemc-ams>

SCA_TDF_MODULE(diff)
{
	sca_tdf::sca_in<double> in_ref;
	sca_tdf::sca_in<double> in_means;
	sca_tdf::sca_out<double> out_sig;

	SCA_CTOR(diff)
	{
	}

	void set_attributes()
	{
		set_timestep(10,SC_US);
	}

	void processing()
	{
		out_sig.write(in_ref.read()-in_means.read());

	}


private:
	//double error;
	};
