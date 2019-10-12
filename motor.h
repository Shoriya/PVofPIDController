#include<systemc-ams.h>

SCA_TDF_MODULE(motor)
{
	sca_tdf::sca_in<double> motor_in;
	sca_tdf::sca_out<double> motor_out;

	SC_CTOR(motor)
		{
			h0=15.0;
			w0=(20*M_PI);
		}

	void initialize()
	{
		//num(0)=h0;
		num(0)=1.0;
		den(0)=1.0;
		den(1)=(1/w0);
	}

	void set_attributes()
	{
			set_timestep(10,SC_US);
			motor_out.set_delay(1);
	}

	void processing()
	{

		motor_out.write(lt1_nd(num,den,motor_in.read(),h0));
	}

private:

	double w0;
	double h0;
	sca_util::sca_vector<double> num,den;
	sca_tdf::sca_ltf_nd lt1_nd;

};
