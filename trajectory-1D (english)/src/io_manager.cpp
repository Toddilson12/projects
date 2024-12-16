#include "io_manager.h"

io_manager::io_manager(const std::string& inputFile_, const std::string& outputFile_)
	: inputFile(inputFile_), outputFile(outputFile_), graphic_points(20)
{
}

void io_manager::ReadDataFile(common_input& input, const std::string& inputFile_)
{
	if (inputFile_ != ".")
		inputFile = inputFile_;
	fileHandler.open(inputFile, std::ios::in);
	if (!fileHandler.is_open())
	{
		std::cout << "ERRO(IO_MANAGER): Nao foi possivel abrir o arquivo!\n";
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
		std::cout << "ERRO(IO_MANAGER): Nao foi possivel abrir o arquivo!\n";
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
	
	std::cout << "Gravitional acceleration (m s^-2): ";
	std::cin >> input.gravity;
	std::cout << "Mass (kg): ";
	std::cin >> input.mass;
	std::cout << "Drag Coefficient (adim): ";
	std::cin >> input.drag_coeff;
	std::cout << "Air Specific Mass (kg m^-3): ";
	std::cin >> input.air_specmass;
	std::cout << "Cross-sectional area (reference area) (m^2): ";
	std::cin >> input.area_ref;
	std::cout << "Initial Ascension Velocity (m/s): ";
	std::cin >> input.vel_ascent0;
	std::cout << "Inicial Height(m): ";
	std::cin >> input.h0;

	std::cout << "Todos os dados foram armazenados!" << std::endl;
}

void io_manager::PrintDataCon(const common_output& output)
{
	std::cout << "Calculations complete!\Results:\n";
	std::cout << "Time to apogee				(s): " << output.t_toApogee << "\n";
	std::cout << "Height of apogee              (m): " << output.h_ofApogee << "\n";
	std::cout << "Maximum Acceleration	        (m s^-2): " << output.accel_max << "\n";
	std::cout << "Time to fall (from apogee)    (t): " << output.t_toFall << "\n";
	std::cout << "Total Flight Time				(t): " << output.t_flight << "\n";
	std::cout << "Impact Velocity				(m/s): " << output.vel_impact << "\n";
	std::cout << "Acceleration at Impact		(m s^-2): " << output.accel_impact << "\n";
}

void io_manager::SetAmountPoints(int num)
{
	graphic_points = num;
}

