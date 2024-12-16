#include <iostream>
#include "csv_reader.h"

/*
NaN = Not a Number
Comma separates values (without whitespaces)
Ends in \n
Chronological order***
FORMAT: Altitude,Mach,Reynolds,CD,CD axial,CD friction,CD pressure,CD base,Normal Force Coefficient,Temperature (°C),Air pressure (mbar)


Objective:
-Calculate mean CD (Until parachute deploy event)

Numerical Integration
-Riemann Sum: CD * dt

*/


int main()
{
	double cd1 = 0, cd2 = 0;
	{
		READER_OR_csv myReader("sem_garras_2.csv");
		myReader.setEndLineCommentary("APOGEE");
		double sum_step = 0.0;
		while (!myReader.checkIterationEnd())
		{
			if (myReader.checkLineType()) // if it is data
			{
				sum_step += myReader.getDataFromLineIndex(OR_DATA::CD) * (myReader.getDataFromLineIndex(OR_DATA::TIME) - myReader.getLastDataFromLineIndex(OR_DATA::TIME));
				//std::cout << myReader.getDataFromLineIndex(OR_DATA::CD) << " | " << sum_step << " | " << (myReader.getDataFromLineIndex(OR_DATA::TIME) - myReader.getLastDataFromLineIndex(OR_DATA::TIME)) << "\n";
			}

			myReader.iterate();
		}
		cd1 = sum_step / myReader.getLastDataFromLineIndex(OR_DATA::TIME);
		std::cout << "Mean CD without launch grip: " << cd1 << "\n";
	}
	{
		READER_OR_csv myReader("com_garras_2.csv");
		myReader.setEndLineCommentary("APOGEE");
		double sum_step = 0.0;
		while (!myReader.checkIterationEnd())
		{
			if (myReader.checkLineType()) // se for dado
			{
				sum_step += myReader.getDataFromLineIndex(OR_DATA::CD) * (myReader.getDataFromLineIndex(OR_DATA::TIME) - myReader.getLastDataFromLineIndex(OR_DATA::TIME));
				//std::cout << myReader.getDataFromLineIndex(OR_DATA::CD) << " | " << sum_step << " | " << (myReader.getDataFromLineIndex(OR_DATA::TIME) - myReader.getLastDataFromLineIndex(OR_DATA::TIME)) << "\n";
			}

			myReader.iterate();
		}
		cd2 = sum_step / myReader.getLastDataFromLineIndex(OR_DATA::TIME);
		std::cout << "Mean CD with launch grip: " << cd2;
	}
	std::cout << "\n\nThe percentual increase in the mean CD due to the launch grip: " << 100*((cd2-cd1)/cd1) << "%\n";

	return 0;
}