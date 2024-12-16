#include "io_manager.h"

io_manager::io_manager(const std::string& inputFile_, const std::string& outputFile_)
	: inputFile(inputFile_), outputFile(outputFile_), graphic_points(20), loop_stream(true), current_stream("\0")
{
}

void io_manager::ReadDataFile(common_input& input, const std::string& inputFile_)
{
	if (inputFile_ != ".")
		inputFile = inputFile_;
	fileHandler.open(inputFile, std::ios::in);
	if (!fileHandler.is_open())
	{
		std::cout << "ERROR(IO_MANAGER): Could not open file!\n";
		exit(-1);
	}

	std::string temp;
	
	for (unsigned int index = 0; std::getline(fileHandler, temp); index++)
	{
		double value = std::stod(temp);
		input.elem[index] = value;
	}

	fileHandler.close();
	/*
	//Gravidade
	std::getline();
	//Massa
	std::getline();
	//Coeficiente de arrasto
	std::getline();
	//Massa especifica do ar
	std::getline();
	//Area da seccao transversal
	std::getline();
	//Velocidade inicial de ascencao
	std::getline();
	//Altura inicial
	std::getline();
	*/

}

void io_manager::PrintDataFile(const common_output& output, const std::string& outputFile_)
{
	if (outputFile_ != ".")
		outputFile = outputFile_;
	fileHandler.open(outputFile, std::ios::out);
	if (!fileHandler.is_open())
	{
		std::cout << "ERROR(IO_MANAGER): Could not open file!\n";
		exit(-1);
	}

	for (int index = 0; index < (sizeof(common_output::elem) / sizeof(double)); index++)
	{
		double a = output.elem[index];
		fileHandler << common_output_names[index] << ": " << output.elem[index] << std::endl;
	}
	fileHandler.close();
}

void io_manager::ReadDataCon(common_input& input)
{
	
	//Read from console
	std::cout << "Enter all requested data:\n";
	
	for (int index = 0; index < (sizeof(common_input::elem) / sizeof(double)); index++)
	{
		std::cout << common_input_names[index] << ": ";
		std::cin >> input.elem[index];
	}
	
	std::cout << "All data was read!" << std::endl;
}

void io_manager::PrintDataCon(const common_output& output)
{
	for (int index = 0; index < (sizeof(common_output::elem) / sizeof(double)); index++)
	{
		std::cout << common_output_names[index] << ": " << output.elem[index] << "\n";
	}
}

void io_manager::ClearTerminal()
{
	system("cls");
}

bool io_manager::IsLoopStream()
{
	return this->loop_stream;
}

void io_manager::GetInput()
{
	std::cin >> this->current_stream;
}

const std::string* io_manager::GetCurrentInput()
{
	return &this->current_stream;
}

void io_manager::SetAmountPoints(int num)
{
	graphic_points = num;
}

