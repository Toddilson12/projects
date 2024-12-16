#include <iostream>
#include "common.h"
#include "pressure.h"
#include "io_manager.h"

//PLEASE USE C++/17 BECAUSE OF THE LAMBDA THING ON IO_MANAGER.CPP


bool IS_NUMBER(const std::string& s)
{
	return !s.empty() && std::find_if(s.begin(),
		s.end(), [](unsigned char c) { return !(std::isdigit(c) || (c == '.') || (c == '-') || (c == '+') || (c == 'e')); }) == s.end();
}
 
int main()
{
	common_input input_data;
	common_output output_data;
	
	io_manager io;
	int option;
	//int num_point;
	
	std::cout << "INPUT/OUTPUT FORMAT:\nFILE: ENTER 1\nTERMINAL (STANDARD IO): ENTER 2\n";
	
	std::cin >> option;

	combustion_chamber inst(&input_data, &output_data);

	if (option == 1)
	{
		io.ReadDataFile(input_data, "input/input.txt");
		inst.CalculateChamberProperties();
		io.PrintDataFile(output_data, "output/output.txt");
	}
	
	if (option == 2)
	{
		io.ReadDataCon(input_data);
		inst.CalculateChamberProperties();
		io.PrintDataCon(output_data);
	}

	//io.SetAmountPoints(num_point);
	//io.PlotDataInGraph<analytic_trajectory>(inst); //USE "|" para separar as configuracoes e.g. ...(inst, ASCENT_POS | ASCENT_VEL | ...);
	//fazer sistema de input/output de funcao

	system("pause");
	io.ClearTerminal();
	
	{
		io.DisplayOuput("p(%lf) = %lf\nEnter time input (to leave, type \"exit\" and hit Enter)\nt=", 0.0f, inst.GetTransientPressure(0.0f));
		io.GetInput();
		io.ClearTerminal();
		
	} 
	while (true)
	{
		if (*io.GetCurrentInput() == "exit")
			break;
		else if (IS_NUMBER(*io.GetCurrentInput()))
		{
			io.DisplayOuput("p(%lf) = %lf\nEnter time input (to leave, type \"exit\" and hit Enter)\nt=", std::stod(*io.GetCurrentInput()), inst.GetTransientPressure(std::stod(*io.GetCurrentInput())));
			io.GetInput();
			io.ClearTerminal();
		}
		else
		{
			io.DisplayOuput("p(%lf) = %lf\nEnter time input (to leave, type \"exit\" and hit Enter)\nt=", 0.0f, inst.GetTransientPressure(0.0f));
			io.GetInput();
			io.ClearTerminal();
		}

	}

	io.ClearTerminal();

	std::cout << "\nHit Enter to leave the program...\n";
	std::cin.get();
	std::cin.get();
	return 0;
}
