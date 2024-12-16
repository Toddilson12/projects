#include <chrono>
#include <iostream>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <fstream>
#include <vector>
#include <cstdlib>

struct fitted_int_col
{
    std::string col_name;
    double fitted_coef[4];
};

const double temperature_column[12] = 
{
	300,     // 0
	500,     // 1
	600, 	 // 2
	1000,	 // 3
	2000,	 // 4
	4000,	 // 5
	5000, 	 // 6
	6000, 	 // 7
	8000, 	 // 8
	10000, 	 // 9
	15000, 	 // 10
	20000	 // 11
};

const int start_temp_at = 3;
/*
We iterate each 2 lines
We choose 4 temperatures (the first 4 valid ones, starting from the 4th element)
We ln these temperatures
We ln the int col factor of these temperature
We build a matrix system and solve it
Store in format
*/


std::vector<fitted_int_col> list_int_col;


bool state = 0;
fitted_int_col temp;

std::vector<double> solve_system(const std::vector<double>& omegas, const std::vector<double>& temperatures)
{
	Eigen::Matrix4f A = Eigen::MatrixXf::Random(4, 4);
	Eigen::Vector4f b = Eigen::VectorXf::Random(4);

	//Construct matrices
	for (int i = 0; i < 4; i++)
	{
		A(i, 0) = std::pow(std::log(temperatures[i]), 3);
		A(i, 1) = std::pow(std::log(temperatures[i]), 2);
		A(i, 2)	= std::pow(std::log(temperatures[i]), 1);
		A(i, 3)	= 1;

		b(i) = std::log(omegas[i]);
	}
	//std::chrono::steady_clock sc;
	//std::cout << "Here is the matrix A:\n" << A << std::endl;
	//std::cout << "Here is the vector b:\n" << b << std::endl;
	//auto start = sc.now();
	Eigen::Vector4f solution_vector = A.colPivHouseholderQr().solve(b);
	std::vector<double> vec(solution_vector.data(), solution_vector.data() + solution_vector.rows() * solution_vector.cols());
	//std::cout << "The solution is:\n" << solution_vector << std::endl;
	return vec;
	//auto end = sc.now();
	
	//auto time_span = static_cast<std::chrono::duration<double>>(end - start);
	//std::cout << "Operation took: " << time_span.count() << " seconds !!!";
	 //A.colPivHouseholderQr().solve(b);
}

void parse_line(const std::string& line)
{
	if(state == 0)
		temp.col_name = line;
	else
	{
		std::vector<double> value_temp; //sizes = 4
		std::vector<double> temperatures_temp;
		

		std::string temp_num = "\0";
		
		int spaces = 0; //defines the column (temperature)
		int count_valid_omega = 0;

		for(int i = 0; i < line.size(); i++)
		{
			if(line[i] == ' ')
			{
				if (count_valid_omega >= 4)
					break;
				if ((spaces >= start_temp_at))
				{
					if (temp_num == "\0") //Teve um 'x'
					{
						spaces++;
						continue;
					}

					temperatures_temp.push_back(temperature_column[spaces]);
					value_temp.push_back(std::stod(temp_num));
					temp_num = "\0";

					count_valid_omega++;
				}
				spaces++;
				
			}
			else if(line[i] != 'x')
			{
				if ((spaces >= start_temp_at))
				{
					temp_num += line[i];
				}
			}
			
		}
		//temperatures_temp.push_back(temperature_column[spaces]);
		//value_temp.push_back(std::stod(temp_num));
		std::vector<double> sol = (solve_system(value_temp, temperatures_temp));

		temp.fitted_coef[0] = sol[0];
		temp.fitted_coef[1] = sol[1];
		temp.fitted_coef[2] = sol[2];
		temp.fitted_coef[3] = sol[3];
	}
	
	list_int_col.push_back(temp);
	
	state = !state;
}

void read(const std::string& filename)
{
	std::ifstream file_;
	file_.open(filename);
	if(!file_.is_open())
	{
		std::cout << "Read error\n";
		exit(-1);
	}
	std::string line_;
	while(std::getline(file_, line_))
	{
		parse_line(line_);
	}
	
}

void print_file(const std::string& filename)
{
	std::ofstream file_;
	file_.open(filename);
	if (!file_.is_open())
	{
		std::cout << "print error\n";
		exit(-1);
	}

	for (int i = 0; i < list_int_col.size(); i++)
	{
		if((i % 2) != 0)
			file_ << "(" << list_int_col[i].col_name << ")   " << "("
			  << list_int_col[i].fitted_coef[0] << " "
			  << list_int_col[i].fitted_coef[1] << " "
			  << list_int_col[i].fitted_coef[2] << " "
			  << list_int_col[i].fitted_coef[3] << " "
			  << ")\n";
	}
}

// Falta programar um skip quando tiver o x, pq o programa incrementa space mesmo com o x


struct pgcoeffs
{
	double A0 = 0;
	double A1 = 0;
	double A2 = 0;
	double A3 = 0;
	double A4 = 0;
} GUPTAcoeff;

struct p90coeffs
{
	double A1 = 0;
	double A2 = 0;
	double A3 = 0;
	double A4 = 0;
	double A5 = 0;
} PARKcoeff;

double lnKeqGUPTA(double T)
{
	double Z = 10000 / T;
	return GUPTAcoeff.A0 + GUPTAcoeff.A1 * std::log(Z) + GUPTAcoeff.A2 * Z + GUPTAcoeff.A3 * Z * Z + GUPTAcoeff.A4 * Z * Z * Z;
}

double guptareactions[] =
{
1.146, 0.328, -8.546, -0.089, 0.005,
4.366, 1.157, -13.829, 0.116, -0.006,
0.810, -0.311, -7.021, -0.075, 0.004,
3.172, -0.174, -5.058, -0.187, 0.009,
-1.479, -0.780, -1.287, -0.020, 0.000,
1.458, -0.415, -3.557, -0.022, 0.001,
2.077, 0.687, -8.094, 0.172, -0.009,
3.040, 0.738, -6.905, 0.153, -0.008,
3.220, 0.828, -5.282, 0.205, -0.011,
1.122, -1.054, -2.031, -0.008, 0.000,
0.336, 0.639, -1.525, -0.014, 0.001,
-0.179, -0.091, -1.623, -0.053, 0.003,
0.884, -0.643, 0.616, -0.132, 0.006,
-0.687, -0.963, 0.145, -0.148, 0.007,
0.286, -1.871, -1.813, -0.144, 0.006,
1.644, 0.542, -2.773, 0.052, -0.002,
-0.128, 0.238, -2.299, 0.078, -0.004,
-0.600, 0.231, -1.370, 0.004, 0.000,
-0.492, -1.201, -3.915, -0.116, 0.005,
-7.781, -1.540, -3.307, 0.021, -0.001,
-8.274, -2.741, -7.222, -0.095, 0.005,
-9.407, -4.337, -1.085, -0.264, 0.012,
-6.113, -2.034, -15.255, -0.073, 0.004,
-5.328, -1.309, -13.101, -0.003, 0.000
};


int main()
{
	/*
    std::chrono::steady_clock sc;
   
    Eigen::Matrix4f A = Eigen::MatrixXf::Random(4,4);
    Eigen::Vector4f b = Eigen::VectorXf::Random(4);
    //A << 1, 1, 1, -1;
    //b << 1, 3;
    std::cout << "Here is the matrix A:\n" << A << std::endl;
    std::cout << "Here is the vector b:\n" << b << std::endl;
    auto start = sc.now();
    Eigen::Vector4f x = A.colPivHouseholderQr().solve(b);
    auto end = sc.now();
    std::cout << "The solution is:\n" << x << std::endl;
    auto time_span = static_cast<std::chrono::duration<double>>(end - start);
    std::cout << "Operation took: " << time_span.count() << " seconds !!!";
	*/
	//read("C:\\Users\\enzog\\source\\repos\\solvelinear\\solvelinear\\IO\\input2.txt");
	//print_file("C:\\Users\\enzog\\source\\repos\\solvelinear\\solvelinear\\IO\\output2.txt");
	/*
	std::chrono::steady_clock sc;

	Eigen::Matrix3f A;
	Eigen::Vector3f b;
	double temperatures[3] = { 600, 3000, 10000 };
	double lnetas[3] = {5.9617884540039, 7.0591451087184, 7.894663412146};
	const double correction = 6 * std::log(10);
	for (int i = 0; i < 3; i++)
	{
		A(i, 0) = std::pow(std::log(temperatures[i]), 2);
		A(i, 1) = std::log(temperatures[i]);
		A(i, 2) = 1;

		b(i) = lnetas[i] - correction;
	}


	std::cout << "Here is the matrix A:\n" << A << std::endl;
	std::cout << "Here is the vector b:\n" << b << std::endl;
	auto start = sc.now();
	Eigen::Vector3f x = A.colPivHouseholderQr().solve(b);
	auto end = sc.now();
	std::cout << "The solution is:\n" << x << std::endl;
	auto time_span = static_cast<std::chrono::duration<double>>(end - start);
	std::cout << "Operation took: " << time_span.count() << " seconds !!!";
	*/
	std::chrono::steady_clock sc;
	srand((unsigned)time(NULL));
	//from 1000 to 20000
	//int random = 1000 + (rand() % 19000);
	//random * 500 + 1000
	

	Eigen::Matrix<double, 5, 5> A;
	Eigen::Vector<double, 5> b;
	for (int j = 0; j < 24; j++)
	{
		GUPTAcoeff.A0 = guptareactions[j *	5 + 0];
		GUPTAcoeff.A1 = guptareactions[j * 5 + 1];
		GUPTAcoeff.A2 = guptareactions[j * 5 + 2];
		GUPTAcoeff.A3 = guptareactions[j * 5 + 3];
		GUPTAcoeff.A4 = guptareactions[j * 5 + 4];

		Eigen::Vector<double, 5> mean_solution;

		mean_solution(0) = 0;
		mean_solution(1) = 0;
		mean_solution(2) = 0;
		mean_solution(3) = 0;
		mean_solution(4) = 0;
		auto start = sc.now();

		const int iterations = 10000; //140 segundos 
		for (int k = 0; k < iterations; k++)
		{
			for (int i = 0; i < 5; i++)
			{
				int random_temp = 0 + (rand() % 2990);
				double random_Z = 10000 / (random_temp * 10.0 + 100.0);
				A(i, 0) = 1.0 / random_Z;
				A(i, 1) = 1;
				A(i, 2) = std::log(random_Z);
				A(i, 3) = random_Z;
				A(i, 4) = random_Z * random_Z;

				b(i) = lnKeqGUPTA(random_temp * 10.0 + 100.0);
				//std::cout << "temperature: "<<  random_temp * 10.0 + 100.0 << std::endl;
			}



			//std::cout << "Here is the matrix A:\n" << A << std::endl;
			//std::cout << "Here is the vector b:\n" << b << std::endl;

			Eigen::Vector<double, 5> x = A.colPivHouseholderQr().solve(b);

			//std::cout << "The solution is:\n" << x << "\n=========================" << std::endl;

			mean_solution += x;
		}
		mean_solution = mean_solution / iterations;
		auto end = sc.now();
		auto time_span = static_cast<std::chrono::duration<double>>(end - start);
		std::cout << "Operation took: " << time_span.count() << " seconds !!!\n==========================\n";
		std::cout << "\n\n\nThe MEAN solution is:\n" << mean_solution << "\a\n\n" << std::endl;
	}
	std::cin.get();
	system("pause");
}