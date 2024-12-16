#pragma once
//Fazer funcao que identifica virguals e final e encontra elementos
//Tabela da ordem dos elementos
//Checador de linha (retorna se é dado ou comentário)
//Checa qual comentário é (hardcode o comentário final)
//Manejar arquivos

#include <iostream>
#include <string>
#include <fstream>


typedef unsigned int uint; //um saco ficar escrevendo tudo isso

enum class OR_DATA //essa ordem é a seguida no arquivo. Confira sempre que for adicionar algum dado
{
	TIME,
	ALTITUDE,
	MACH,
	RE,
	CD,
	CD_AXIAL,
	CD_FRICTION,
	CD_PRESSURE,
	CD_BASE,
	NORMAL_COEFF,
	TEMPERATURE,
	PRESSURE
};




class READER_OR_csv
{
private:
	std::ifstream mFileObj;
	uint mLineEnd; 
	
	std::string mCurrentLineRaw;
	std::string mEndKeyword;

	bool checkKeyword(const std::string& keyword);
	double getDataFromLineandIndex(const std::string& line, OR_DATA index);

	std::string mLastLine;
public:
	READER_OR_csv(const std::string& pathWithNameOfFile);

	bool checkLineType(); // 0 = comentario | 1 = dado
	double getDataFromLineIndex(OR_DATA index); // get a value from an index (starting from zero) of the current line (sem necessidade de ficar guardando toda a matriz ne?)
	void setEndLineCommentary(const std::string& keyword);
	
	bool checkIterationEnd(); 
	void iterate();
	double getLastDataFromLineIndex(OR_DATA index);

};