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

class combustion_chamber
{
	common_input* ptr_InputData;
	common_output* ptr_OutputData;

	double Gamma;
	double SteadyPressure;
	double AlphaTime;
	double c;

	void CalculateGamma();
	void CalculateSpeedSound();
	void CalculateSteadyPressure();
	void CalculateAlphaTime();

public:
	combustion_chamber(common_input* inptr, common_output* outptr);
	
	double GetTransientPressure(double time);
	void CalculateChamberProperties();
	
	double GetGamma();
	double GetSpeedSound();
	double GetSteadyPressure();
	double GetAlphaTime();

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