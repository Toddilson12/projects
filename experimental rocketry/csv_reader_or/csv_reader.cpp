#include "csv_reader.h"

bool READER_OR_csv::checkKeyword(const std::string& keyword)
{
	if (mCurrentLineRaw.size() < (8 + keyword.size()))
		return 0;
	for (int i = 8; i < (8 + keyword.size()); i++) //i = 2 is the first letter of keyword
	{
		if (mCurrentLineRaw[i] != keyword[i - 8])
			return 0; //if smth differnt, it aint the same word
	}
	return 1; //if every check ok, then its the same; It should end now
}

double READER_OR_csv::getDataFromLineandIndex(const std::string& line, OR_DATA index)
{
	//suponha que nao é um comentario

	//iterate thru line to find commas
	std::string raw_data = "\0";
	bool started = false;
	uint k = 0;
	for (int i = 0; i < line.size(); i++)
	{
		if (line[i] == ',') //\n is discarded because of getline
			k++;
		else if (k == (uint)index)
		{
			raw_data.push_back(line[i]);
			started = true; //fica repetindo, mas to com preguica de fazer melhor
		}

		if (started && k == ((uint)index + 1))
			break;
	}

	return std::stod(raw_data);
}

READER_OR_csv::READER_OR_csv(const std::string& pathWithNameOfFile)
	: mLineEnd(0), mEndKeyword("APOGEE") //ja começa com a primeira iter feita
{
	mFileObj.open(pathWithNameOfFile);
	if (!mFileObj.is_open())
	{
		std::cout << "Erro ao abrir o arquivo\n";
		exit(-1);
	}

	//iterate to start (keyword "LAUNCH"); Could remove?
	while(!checkKeyword("LAUNCH"))
		std::getline(mFileObj, mCurrentLineRaw); 
	this->iterate();
	mLastLine = mCurrentLineRaw; //first last line
}

bool READER_OR_csv::checkLineType()
{
	if (mCurrentLineRaw.size() != 0)
		return (mCurrentLineRaw[0] == '#') ? 0 : 1;
	else
		return 0;
}

double READER_OR_csv::getDataFromLineIndex(OR_DATA index)
{
	return getDataFromLineandIndex(this->mCurrentLineRaw, index);
}

void READER_OR_csv::setEndLineCommentary(const std::string& keyword)
{
	mEndKeyword = keyword;
}


bool READER_OR_csv::checkIterationEnd()
{
	//check if commentary and then if "# keyword"
	if (!this->checkLineType()) //if commentary
	{
		return this->checkKeyword(mEndKeyword);
	}
	else
		return 0;// not even a commentary...
}

void READER_OR_csv::iterate()
{
	if (this->checkLineType()) 
		mLastLine = mCurrentLineRaw; //keep dumping current line
	
	std::getline(mFileObj, mCurrentLineRaw);
}

double READER_OR_csv::getLastDataFromLineIndex(OR_DATA index)
{
	return getDataFromLineandIndex(this->mLastLine, index);
}
