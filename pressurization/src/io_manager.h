#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <functional>
#include <vector>
#include <initializer_list>
#include <utility>
#include <type_traits>
#include "common.h"



enum graphic_mode
{
	_mask = 0xff
};

/*
0x01
0x02
0x04
0x08
0x10
0x40
0x80
0x20
*/

const graphic_mode ASCENT_POS	= static_cast<graphic_mode>(0x01);
const graphic_mode ASCENT_VEL	= static_cast<graphic_mode>(0x02);
const graphic_mode ASCENT_ACCEL	= static_cast<graphic_mode>(0x04);
const graphic_mode DESCENT_POS	= static_cast<graphic_mode>(0x08);
const graphic_mode DESCENT_VEL	= static_cast<graphic_mode>(0x10);
const graphic_mode DESCENT_ACCEL= static_cast<graphic_mode>(0x20);
//Lembrar de fazer sistema para descida com velocidade inicial != 0


class io_manager
{
	std::fstream fileHandler;

	std::string outputFile;
	std::string inputFile;

	int graphic_points;

	bool loop_stream;

	std::string current_stream;

public:
	io_manager(const std::string& inputFile_ = ".", const std::string& outputFile_ = ".");

	// File system
	void ReadDataFile(common_input& input, const std::string& inputFile_ = ".");
	void PrintDataFile(const common_output& output, const std::string& outputFile_ = ".");

	// Console system
	void ReadDataCon(common_input& input);
	void PrintDataCon(const common_output& output);
	
	//Console continuous stream system
	void ClearTerminal(); //simple
	bool IsLoopStream(); // are we in a loop?
	
	template<class...A> 
	void DisplayOuput(const std::string& msg, A...args); //just printf

	void GetInput();

	const std::string* GetCurrentInput();


	// Graphic system
	void SetAmountPoints(int num);
	//template<class SOLVER_CLASS>
	//void PlotDataInGraph(SOLVER_CLASS solver, unsigned char mode = ASCENT_POS | ASCENT_VEL | ASCENT_ACCEL | DESCENT_POS | DESCENT_VEL | DESCENT_ACCEL);
};
/*
static std::string doubleToString(double num)
{
	std::ostringstream stream;
	stream << num;
	return stream.str();
}

struct VECTOR2
{
	double x;
	double y;
};

template<class SOLVER_CLASS>
void io_manager::PlotDataInGraph(SOLVER_CLASS solver, unsigned char mode)
{
	//Mask = 0xff
	unsigned char fun_count = 0;
	unsigned char mask = 1;
	Gnuplot gnuInst("\"C:\\Program Files\\gnuplot\\bin\\gnuplot.exe\"");
	
	
	if (solver.GetOutputData() == NULL)
		exit(-2);

	auto lambda_print = [&](std::function<double(double)> calculate, double time_limit)
	{
		double time = 0.0f;
		double increment = time_limit / graphic_points;
		//create points
		for (int i = 0; i < graphic_points; i++)
		{
			VECTOR2 point_;
			point_.x = time;
			point_.y = calculate(time);
			//std::cout << "(" << point_.x << "; " << point_.y << ")\n";
			gnuInst << doubleToString(point_.x) + " " + doubleToString(point_.y) << std::endl;
			
			time += increment;
		}
		//push to gnuplot
		gnuInst << "e" << std::endl;
	};

	gnuInst << "set arrow from graph 1, 0 to graph 1.05, 0 size screen 0.025, 15, 60  filled ls 11" << std::endl; //set range
	gnuInst << "set arrow from graph 0,1 to graph 0,1.05 size screen 0.025,15,60" << std::endl; // set range

	gnuInst << "set xrange[0 : 10]" << std::endl; //set range
	gnuInst << "set yrange[0 : 10]" << std::endl; // set range
	std::cout << "Para trocar de gráfico, aperte espaço. Não feche as janelas!\n";
	double time_final = 0.0f;
	while (mode)
	{
		switch (mode & mask)
		{
		case ASCENT_POS:
			time_final = solver.GetOutputData()->t_toApogee;
			gnuInst << "plot \'-\' with linespoint title \'Altura (Asc)\'" << std::endl;
			
			lambda_print([&](double time)
			{
				return (solver.CalculatePosAscent(time));
			}, time_final);
			gnuInst << "pause mouse key" << std::endl;
			gnuInst << "while (MOUSE_KEY != 32) { pause mouse key }" << std::endl;
			
		
			break;
		case ASCENT_VEL:
			time_final = solver.GetOutputData()->t_toApogee;
			gnuInst << "plot \'-\' with linespoint title \'Velocidade (Asc)\'" << std::endl;
			lambda_print([&](double time)
				{
					return (solver.CalculateVelAscent(time));
				}, time_final);
			gnuInst << "pause mouse key" << std::endl;
			gnuInst << "while (MOUSE_KEY != 32) { pause mouse key }" << std::endl;


			break;
		case ASCENT_ACCEL:
			time_final = solver.GetOutputData()->t_toApogee;
			gnuInst << "plot \'-\' with linespoint title \'Aceleração (Asc)\'" << std::endl;
			lambda_print([&](double time)
				{
					return (solver.CalculateAccelAscent(time));
				}, time_final);
			gnuInst << "pause mouse key" << std::endl;
			gnuInst << "while (MOUSE_KEY != 32) { pause mouse key }" << std::endl;


			break;
		case DESCENT_POS:
			time_final = solver.GetOutputData()->t_toFall;
			gnuInst << "plot \'-\' with linespoint title \'Altura (Dsc)\'" << std::endl;
			lambda_print([&](double time)
				{
					return (solver.CalculatePosDescent(time));
				}, time_final);
			gnuInst << "pause mouse key" << std::endl;
			gnuInst << "while (MOUSE_KEY != 32) { pause mouse key }" << std::endl;


			break;
		case DESCENT_VEL:
			time_final = solver.GetOutputData()->t_toFall;
			gnuInst << "plot \'-\' with linespoint title \'Velocidade (Dsc)\'" << std::endl;
			lambda_print([&](double time)
				{
					return (solver.CalculateVelDescent(time));
				}, time_final);
			gnuInst << "pause mouse key" << std::endl;
			gnuInst << "while (MOUSE_KEY != 32) { pause mouse key }" << std::endl;


			break;
		case DESCENT_ACCEL:
			time_final = solver.GetOutputData()->t_toFall;
			gnuInst << "plot \'-\' with linespoint title \'Aceleração (Dsc)\'" << std::endl;
			lambda_print([&](double time)
				{
					return (solver.CalculateAccelDescent(time));
				}, time_final);
			gnuInst << "pause mouse key" << std::endl;
			gnuInst << "while (MOUSE_KEY != 32) { pause mouse key }" << std::endl;


			break;
		}
		mode &= ~mask;
		mask <<= 1;
	}
}
*/


template<class ...A>
inline void io_manager::DisplayOuput(const std::string& msg, A ...args)
{
	printf(msg.c_str(), args...);
}
