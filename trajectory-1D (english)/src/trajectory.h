#pragma once
#include <iostream>
#include "common.h"

//talvez fazer classe pai para todos os "solvers"?
//checar para NULL
//Cuidar com a ordem dos calculos!
//Sempre comece pela ascendente
//Prioridade para encontrar tempo
//Depois encontre a grandeza associada temporalmente
//e.g. tempo apogeu -> altura apogeu

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

class analytic_trajectory
{
	common_input* ptr_InputData;
	common_output* ptr_OutputData;

	double kappa;

	void calculate_t_toApogee();
	void calculate_h_ofApogee();
	void calculate_accel_max();
	 
	void calculate_t_toFall();
	void calculate_t_flight();
	void calculate_vel_impact();
	void calculate_accel_impact();

public:
	analytic_trajectory(common_input* inptr, common_output* outptr);

	void CalculateTrajectoryData();

	double CalculatePosAscent(double t); // t0 = 0
	double CalculateVelAscent(double t);
	double CalculateAccelAscent(double t);

	double CalculatePosDescent(double t); // t0 = t_apogee
	double CalculateVelDescent(double t);
	double CalculateAccelDescent(double t);

	common_output* const GetOutputData() const;

};

//t_toApogee;
//h_ofApogee;
//
//accel_max;
//t_toFall; 
//
//t_flight; 
//vel_impact;
//accel_impact;