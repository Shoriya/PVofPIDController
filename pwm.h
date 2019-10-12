#include<systemc-ams>
#include"systemc.h"
#include<cmath>


SCA_TDF_MODULE(pwm)
{
	sca_tdf::sca_in<double> in;
	sca_tdf::sca_out<double> vdrv;

	SCA_CTOR(pwm)
	{
		v0=0.0;
	    v1=1.0;
		const sca_core::sca_time& pperiod = sca_core::sca_time(5.0,sc_core::SC_MS);
		const sca_core::sca_time& rramp = sca_core::sca_time(0.05,sc_core::SC_MS);
		period=pperiod.to_seconds();
		ramp=rramp.to_seconds();
		duty_max=period-2.0*ramp;
		duty=duty_max;
	}

	void set_attributes()
	{
		set_timestep(10,SC_US);
	}

	void processing()
	{
		double t = get_time().to_seconds();
		double pos=std::fmod(t,period);

		if(pos<ramp)
		{
			duty=in.read()*duty_max;
			if(duty<0.0) duty=0.0;
			if(duty>duty_max) duty=duty_max;
		}

		double val = v0;
		if(pos<ramp)
			val= ((v1-v0)/ramp)*pos+v0;
		else if (pos<ramp+duty)
			val=v1;
		else if(pos<ramp+duty+ramp)
			val=((v0-v1)/ramp)*(pos-ramp-duty)+v1;

		vdrv.write(val);

		}

private:
	double v0,v1;
	double period,ramp;
	double duty_max;
	double duty;

	};
