#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <sstream>
#include <iostream>
#include <codecvt>

#include "csvparser.h"

Parser::Parser(void)
{
	csv_stream.open(DATA_CSV_NAME, std::ios::binary);
	csv_stream.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
	txt_stream.open(EXEC_TXT_NAME, std::ios::binary);
	txt_stream.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
}

Parser::~Parser(void)
{
	csv_stream.close();
	txt_stream.close();
}

bool Parser::MakePersonStruct(std::vector<std::wstring>& ptokens, Person* pperson)
{
	memset(pperson, 0, sizeof(Person));

	if (!wcscmp(ptokens[4].c_str(), L"불참"))
		return false;

	// name
	wcsncpy(pperson->name, ptokens[1].c_str(), NAME_MAX_LEN);
	wcsncpy(pperson->student_number, ptokens[2].c_str(), STUDENT_NUM_LEN);
	ValidateMBTI(ptokens[5].c_str(), &pperson->mbti);
	if (!wcscmp(L"남", ptokens[7].c_str()))
		pperson->is_male = true;
	else
		pperson->is_male = false;
	if (IsExecutive(ptokens[1]))
		pperson->is_executive = true;
	else
		pperson->is_executive = false;
	
	return true;
}

// MBTI는 대문자로
void Parser::ValidateMBTI(const wchar_t* mbti, MBTI* res)
{
	wchar_t temp[5];
	int i;
	int numeric_mbti;

	for (i = 0; i < 4; i++)
		temp[i] = towupper(mbti[i]);
	temp[4] = 0;
	if (temp[0] == L'E' || temp[0] == L'I' ||
		temp[1] == L'S' || temp[1] == L'N' ||
		temp[2] == L'T' || temp[2] == L'F' ||
		temp[3] == L'J' || temp[3] == L'P')
		*res = (MBTI)GetNumericMBTI(temp);
	else
		*res = Invalid;
}

bool Parser::Parse(void)
{
	std::wstring csv_line;
	Person p;

	if (!csv_stream.is_open()) return false;
	if (!GetExecutivesName()) return false;

	for (; std::getline(csv_stream, csv_line);)
	{
		std::wstringstream ss(csv_line);
		std::wstring token;
		std::vector<std::wstring> tokens;

		while (std::getline(ss, token, L','))
			tokens.push_back(token);

		if (!MakePersonStruct(tokens, &p))
			continue;
		people.push_back(p);
	}

	return true;
}

bool Parser::GetExecutivesName(void)
{
	std::wstring txt_line;

	if (!txt_stream.is_open()) return false;

	for (; std::getline(txt_stream, txt_line);)
	{
		std::wstringstream ss(txt_line);
		std::wstring token;

		while (std::getline(ss, token, L','))
			executives.push_back(token);
	}
}

bool Parser::IsExecutive(std::wstring name)
{
	for (auto it = executives.begin();
		it != executives.end();
		it++)
		if (name._Equal(*it))
			return true;
	return false;
}

std::vector<Person>* Parser::GetPeople(void)
{
	return &people;
}

int Parser::GetNumericMBTI(const wchar_t* mbti)
{
	int ret = 0;

	if (mbti[1] == 'S')
	{
		ret |= 0b1000;

		if (mbti[0] == 'E')
			ret |= 0b0001;
		else
			ret &= 0b1110;

		if (mbti[2] == 'T')
			ret |= 0b0010;
		else
			ret &= 0b1101;

		if (mbti[3] == 'J')
			ret |= 0b0100;
		else
			ret &= 0b1011;
	}
	else
	{
		ret &= 0b0111;

		if (mbti[0] == 'E')
			ret |= 0b0001;
		else
			ret &= 0b1110;

		if (mbti[2] == 'T')
		{
			ret |= 0b0100;

			if (mbti[3] == 'P')
				ret |= 0b0010;
			else
				ret &= 0b1101;
		}
		else
		{
			ret &= 0b1011;

			if (mbti[3] == 'J')
				ret |= 0b0010;
			else
				ret &= 0b1101;
		}
	}

	return ret;
}