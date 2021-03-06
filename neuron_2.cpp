#include "neuron_2.hpp"


using namespace std;

Neuron::Neuron(int Delay, double J)
:delay_(Delay),J_(J)
{
	membrane_pot_=Vreset_;
	nb_spikes_=0.0;
	time_buffer_ = vector<double>(4,0);
	
}
	
// GETTERS	
	double Neuron::getMembpot() const 
	{
		return membrane_pot_;
	}
	
	int Neuron::getNbSpikes() const
	{
		return nb_spikes_;
	}
	double Neuron::getH() const
	{
		return h_;
	}
	
	int Neuron::getInternalTime() const
	{
		return internal_time_;
	}
	
	vector<double> Neuron::getSpikesTime() 
	{
		return spikes_time_;
	}
	
	int Neuron::getDelay()  const
	{
		return delay_;
	}
	
// SETTERS	
	void Neuron::setMembPot(double m)
	{
		membrane_pot_= m;
	}
	void Neuron::setNbSpikes (int nbs)
	{
		nb_spikes_=nbs;
	}

	
//update the buffer time
void Neuron::updatebuffer(int pos)
	{
		time_buffer_[pos]+=J_;
	}
	
	
// determine if the potential max is reached
// and if, push into spiketime the internal time of the neuron.	
	bool Neuron::ifPotMaxReached()
	{
		if (membrane_pot_>= Vthreshold_)
		{ return true; } 
		else 
		{ return false; }
	}
	
	void Neuron::PotMaxReached ()
	{
		if (ifPotMaxReached())
		{
			++ nb_spikes_;
			
			spikes_time_.push_back(internal_time_*h_);
		}
	}
	
// set the increase of the potential 
// when the neuron is not in its refractory time	
	void Neuron::SetPot(double I)
	{
		double New;
		New = ((exp(-h_/tau_) * membrane_pot_)+ (I*R_*(1-exp(-h_/tau_))) + time_buffer_[internal_time_%4]);
		setMembPot(New);
		time_buffer_[internal_time_%4] =0.0;

	}
// set the potential at Vreset=0
// after a spike	
	void Neuron::SetNewPot()
	{
		double New;
		New= Vreset_;
		setMembPot(New);
	}
// update the neuron's potential every h time	
	void Neuron::update (double I)
	{
		internal_time_+=1;
		
		if (spikes_time_.empty())
		{
			SetPot(I);
			PotMaxReached();
		}		

		else 
		{		
			if (internal_time_ *h_- spikes_time_.back()>= tau_ref_) //determine if refractory time is over
			{
				SetPot(I);
				PotMaxReached();
			}
			else 
			{
				SetNewPot();
			}
		 
		}
	}
	
