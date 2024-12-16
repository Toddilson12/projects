#include "trajectory.h"
#include <cmath>



analytic_trajectory::analytic_trajectory(common_input* inptr, common_output* outptr)
{
	if (inptr == NULL || outptr == NULL)
	{
		std::cout << "ERROR (SOLVER): NULL POINTER\n";
		exit(-1);
	}
	ptr_InputData = inptr;
	ptr_OutputData = outptr;
	kappa = 0;
}

void analytic_trajectory::CalculateTrajectoryData()
{
	if (ptr_InputData == NULL || ptr_OutputData == NULL)
	{
		std::cout << "ERROR (SOLVER): NULL POINTER\n";
		exit(-1);
	}
	kappa = (ptr_InputData->air_specmass * ptr_InputData->area_ref * ptr_InputData->drag_coeff) / (2 * ptr_InputData->mass);
	calculate_t_toApogee();
	calculate_h_ofApogee();
	calculate_accel_max();
	calculate_t_toFall();
	calculate_vel_impact();
	calculate_accel_impact();
	calculate_t_flight();
}

double analytic_trajectory::CalculatePosAscent(double t)
{
	if (ptr_InputData == NULL || ptr_OutputData == NULL)
	{
		std::cout << "ERROR (SOLVER): NULL POINTER\n";
		exit(-1);
	}
	return (ptr_InputData->h0 + (1/kappa) * std::log((std::cos(std::atan(ptr_InputData->vel_ascent0 * std::sqrt(kappa/ptr_InputData->gravity))-t*std::sqrt(kappa*ptr_InputData->gravity))) * std::sqrt(1 + (kappa * ptr_InputData->vel_ascent0 * ptr_InputData->vel_ascent0)/(ptr_InputData->gravity))));
}

double analytic_trajectory::CalculateVelAscent(double t)
{
	if (ptr_InputData == NULL || ptr_OutputData == NULL)
	{
		std::cout << "ERROR (SOLVER): NULL POINTER\n";
		exit(-1);
	}
	return (std::sqrt(ptr_InputData->gravity/kappa) * std::tan(std::atan(ptr_InputData->vel_ascent0 * std::sqrt(kappa/ptr_InputData->gravity))-t*std::sqrt(ptr_InputData->gravity * kappa)));
}

double analytic_trajectory::CalculateAccelAscent(double t)
{
	if (ptr_InputData == NULL || ptr_OutputData == NULL)
	{
		std::cout << "ERROR (SOLVER): NULL POINTER\n";
		exit(-1);
	}
	return (-ptr_InputData->gravity) / (std::cos(std::atan(ptr_InputData->vel_ascent0 * std::sqrt(kappa / ptr_InputData->gravity))- t * std::sqrt(kappa * ptr_InputData->gravity)) * std::cos(std::atan(ptr_InputData->vel_ascent0 * std::sqrt(kappa / ptr_InputData->gravity)) - t * std::sqrt(kappa * ptr_InputData->gravity)));
}

double analytic_trajectory::CalculatePosDescent(double t)
{
	if (ptr_InputData == NULL || ptr_OutputData == NULL)
	{
		std::cout << "ERROR (SOLVER): NULL POINTER\n";;
		exit(-1);
	}
	return (ptr_OutputData->h_ofApogee - (1/kappa)*std::log(std::cosh(t * std::sqrt(kappa * ptr_InputData->gravity))));
}

double analytic_trajectory::CalculateVelDescent(double t)
{
	if (ptr_InputData == NULL || ptr_OutputData == NULL)
	{
		std::cout << "ERROR (SOLVER): NULL POINTER\n";
		exit(-1);
	}
	return (-std::sqrt(ptr_InputData->gravity / kappa) * std::tanh(t * std::sqrt(ptr_InputData->gravity * kappa)));
}

double analytic_trajectory::CalculateAccelDescent(double t)
{
	if (ptr_InputData == NULL || ptr_OutputData == NULL)
	{
		std::cout << "ERROR (SOLVER): NULL POINTER\n";
		exit(-1);
	}
	return (-ptr_InputData->gravity)/(std::cosh(t * std::sqrt(ptr_InputData->gravity * kappa)) * std::cosh(t * std::sqrt(ptr_InputData->gravity * kappa)));
}

common_output* const analytic_trajectory::GetOutputData() const
{
	if (ptr_OutputData == NULL)
	{
		std::cout << "ERROR (SOLVER): NULL POINTER\n";
		exit(-1);
	}
	return ptr_OutputData;
}

/*
Ordem atual:
-----------------
Tempo apogeu
Altura apogeu
Aceleracao maxima (tempo=0, por definicao)
-----------------
Tempo queda
Velocidade impact
Aceleracao impact
-----------------
Tempo total
-----------------
*/ 

void analytic_trajectory::calculate_t_toApogee()
{
	ptr_OutputData->t_toApogee = (1/(std::sqrt(kappa*ptr_InputData->gravity)))*std::atan(ptr_InputData->vel_ascent0 * (std::sqrt(kappa / ptr_InputData->gravity)));
}

void analytic_trajectory::calculate_h_ofApogee()
{
	ptr_OutputData->h_ofApogee = ptr_InputData->h0 + (1 / kappa) * std::log(std::cos( std::atan (ptr_InputData->vel_ascent0 * std::sqrt(kappa/ ptr_InputData->gravity)) - ptr_OutputData->t_toApogee * std::sqrt(kappa * ptr_InputData->gravity)) * std::sqrt(1 + (ptr_InputData->vel_ascent0 * ptr_InputData->vel_ascent0 * kappa)/(ptr_InputData->gravity)));
}

void analytic_trajectory::calculate_accel_max()
{
	ptr_OutputData->accel_max = -ptr_InputData->gravity - ptr_InputData->vel_ascent0 * ptr_InputData->vel_ascent0 * kappa;
}

void analytic_trajectory::calculate_t_toFall()
{
	ptr_OutputData->t_toFall = (1 / std::sqrt(kappa * ptr_InputData->gravity)) * std::acosh(std::exp(ptr_OutputData->h_ofApogee * kappa));
}

void analytic_trajectory::calculate_t_flight()
{
	ptr_OutputData->t_flight = ptr_OutputData->t_toFall + ptr_OutputData->t_toApogee;
}

void analytic_trajectory::calculate_vel_impact()
{
	ptr_OutputData->vel_impact = -std::sqrt(ptr_InputData->gravity / kappa) * std::tanh(ptr_OutputData->t_toFall * std::sqrt(ptr_InputData->gravity * kappa));
}

void analytic_trajectory::calculate_accel_impact()
{
	ptr_OutputData->accel_impact = -ptr_InputData->gravity / (std::cosh(ptr_OutputData->t_toFall * std::sqrt(ptr_InputData->gravity * kappa)) * std::cosh(ptr_OutputData->t_toFall * std::sqrt(ptr_InputData->gravity * kappa)));
}
