#pragma once

#include <fstream>
#include <vector>

#include "person.h"

#define DATA_CSV_NAME	L"S:\\data.csv"
#define EXEC_TXT_NAME	L"S:\\exec.txt"

class Parser
{
public:
	Parser(void);
	~Parser(void);

	bool Parse(void);
	std::vector<Person>* GetPeople(void);

private:
	std::wifstream csv_stream;
	std::wifstream txt_stream;
	std::vector<Person> people;
	std::vector<std::wstring> executives;

	bool MakePersonStruct(std::vector<std::wstring>& ptokens, Person* pperson);
	void ValidateMBTI(const wchar_t* mbti, MBTI* res);
	bool GetExecutivesName(void);
	bool IsExecutive(std::wstring name);
	int GetNumericMBTI(const wchar_t* mbti);
};