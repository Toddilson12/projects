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



union common_input
{
	struct 
	{
		double gravity;
		double mass;
		double drag_coeff;
		double air_specmass;
		double area_ref;
		double vel_ascent0;
		double h0;
	};
	double elem[7];
};

union common_output
{
	struct
	{
		// Ascent
		double t_toApogee;
		double h_ofApogee;
		// Descent
		double accel_max;
		double t_toFall; // From apogee
		// Final
		double t_flight; // Total flight time
		double vel_impact;
		double accel_impact;
	};
	double elem[7];
};

const std::string common_input_names[] = {
	"Graviational Acceleration (m s^-2)",
	"Mass (kg)",
	"Drag Coefficient (adim)",
	"Air Specific Mass (kg m^-3)",
	"Cross-section area (reference area) (m^2)",
	"Initial Ascension Velocity (m/s)",
	"Initial Height (m)"
};


const std::string common_output_names[] = {
	"Time to Apogee(s)",
	"Height of Apogee(m)",
	"Maximum Acceleration (m s ^ -2)",
	"Time to fall from apogee (t)",
	"Total flight time (t)",
	"Impact Velocity(m / s)",
	"Acceleration at Impact (m s ^ -2)"
};

