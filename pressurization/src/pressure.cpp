#include "pressure.h"
#include <cmath>

#define pid ptr_InputData
#define pod ptr_OutputData

void combustion_chamber::CalculateGamma()
{
	this->Gamma = pid->gamma * std::pow( (double)2.0f / (pid->gamma + (double)1.0f), ((pid->gamma + (double)1.0f)/((double)2.0f * (pid->gamma - (double)1.0f))));
	pod->Gamma = this->Gamma;
}

void combustion_chamber::CalculateSpeedSound()
{
	this->c = std::sqrt(pid->gamma * pid->R * pid->T_0);
	pod->c = this->c;
}

void combustion_chamber::CalculateSteadyPressure()
{
	double K = pid->A_b / pid->A_t;
	double a_ = (pid->rho_p * pid->a * this->c * K) / (this->Gamma);
	double b_ = (double)1.0f / ((double)1.0f - pid->n);

	this->SteadyPressure = std::pow(
	a_
	,
	b_
	);

	pod->p_steady = this->SteadyPressure;

}

void combustion_chamber::CalculateAlphaTime()
{
	double a_ = (((double)1.0f) / (pid->n - (double)1.0f));
	double b_ = ((pid->V_0 * this->c) / (pid->R * pid->T_0 * this->Gamma * pid->A_t));
	double c_ = (std::pow(pid->alpha, ((double)1.0f - pid->n)) - (double)1.0f);
	double d_ = ((std::pow(this->SteadyPressure, (double)1.0f - pid->n)) / (std::pow(pid->p_a, (double)1.0f - pid->n) - std::pow(this->SteadyPressure, (double)1.0f - pid->n)));
	this->AlphaTime =
	(
		a_
		*

		b_ 
		
		*


		std::log
		(
			c_
				*
			d_
			
		)
	);
	pod->time_steady = this->AlphaTime;
}

combustion_chamber::combustion_chamber(common_input* inptr, common_output* outptr)
{
	if (inptr == NULL || outptr == NULL)
	{
		std::cout << "ERRO (SOLVER): ponteiros NULOS\n";
		exit(-1);
	}
	ptr_InputData = inptr;
	ptr_OutputData = outptr;

	this->Gamma = 0.0f;
	this->SteadyPressure = 0.0f;
	this->AlphaTime = 0.0f;
	this->c = 0.0f;

	
}
double combustion_chamber::GetTransientPressure(double time)
{
	if (SteadyPressure != 0.0f)
	{


		return

			std::pow
			(
			
				(std::pow(pid->p_a, (double)1.0f - pid->n) - std::pow(this->SteadyPressure, (double)1.0f - pid->n)) *
				std::exp
				(
					(pid->n - (double)1.0f) *
					((pid->R * pid->T_0 * this->Gamma * pid->A_t)		/		(pid->V_0 * this->c))*
					time
				)

				+

				std::pow(this->SteadyPressure, (double)1.0f - pid->n)
				, 
				
				
				(((double)1.0)/((double)1.0f - pid->n))
			)
			
			
			
			;
	}
	return -1.0f;
}
void combustion_chamber::CalculateChamberProperties()
{
	if (ptr_InputData == NULL || ptr_OutputData == NULL)
	{
		std::cout << "ERROR (SOLVER): NULL POINTERS\n";
		exit(-1);
	}

	CalculateGamma();
	CalculateSpeedSound();
	CalculateSteadyPressure();
	CalculateAlphaTime();
}
double combustion_chamber::GetGamma()
{
	return this->Gamma;
}
double combustion_chamber::GetSpeedSound()
{
	return this->c;
}
double combustion_chamber::GetSteadyPressure()
{
	return this->SteadyPressure;
}
double combustion_chamber::GetAlphaTime()
{
	return this->SteadyPressure;
}


common_output* const combustion_chamber::GetOutputData() const
{
	if (ptr_InputData == NULL || ptr_OutputData == NULL)
	{
		std::cout << "ERROR (SOLVER): NULL POINTERS\n";
		exit(-1);
	}
	return ptr_OutputData;
}


