#include<systemc-ams.h>
#include<systemc-ams>

SCA_TDF_MODULE(source)
{
	sca_tdf::sca_out<double> ref_value;

	SCA_CTOR(source)
		{
		}

	void set_attributes()
		{
			set_timestep(10,SC_US);
		}

	void processing()
	{

		ref_value.write(1.0);
	}
};
