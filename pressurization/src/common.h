#pragma once

// Data structures
// Input file structure:
/*
[gravity]
[drag_coeff]
[air_specmas]
[ref_area]
[vel_ascent0]
[h0]
*/

//the values in the unions are of only one type because of the array

union common_input
{
	struct 
	{
		double gamma;
		double R;
		double A_b;
		double A_t;
		double T_0;
		double rho_p;
		double a;
		double n;
		double p_a;
		double V_0;
		double alpha;
		
	};
	double elem[11];
};

union common_output
{
	struct
	{
		double Gamma;
		double c;
		double p_steady;
		double time_steady;
	};
	double elem[4];
};

const std::string common_input_names[] = {
	"Specific Heat Ratio (propellant gas) [adim.]",
	"Gas Constant of the Propellant Gas [J/kg*K]",
	"Burning Cross-section Area [m^2]",
	"Throat Cross-section Area [m^2]",
	"Combustion Chamber Temperature [K]",
	"Specific Mass of Solid Propellant [kg*m^-3]",
	"Multiplicative Constant of Saint-Robert's Law [adim.]",
	"Exponential Constant of Saint-Robert's Law [adim.]",
	"Ambient Pressure (initial pressure in the chamber) [Pa]",
	"Initial Free Space Volume In The Chamber [m^3]",
	"Fraction of Ideal Steady State Pressure (used to find the time to steady state) [0 to 1]"
};


const std::string common_output_names[] = {
	"Gamma",
	"Speed of Sound [m/s]",
	"Combustion Chamber Pressure at Steady State (assymptote) [Pa]",
	"Time to Reach ((Alpha)) Fraction of Steady State Pressure [s]"
};

