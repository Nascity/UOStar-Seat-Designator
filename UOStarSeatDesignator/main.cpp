#include <iostream>
#include <list>

#include "csvparser.h"
#include "seat_dist.h"

void SetLocale(void)
{
	// Do not touch this code. This part sets the locale.
	std::setlocale(LC_ALL, "ko_KR.UTF-8");
	system("chcp 65001 > nul");
	// Do not touch this code. This part sets the locale.
}

void WelcomeMessage(void)
{
	std::wcout << L"+----------------------------------+\n";
	std::wcout << L"��ǻ�Ͱ��к� 23�й� �Ž¸��� ����\n";
	std::wcout << L"������Ÿ ������ȸ �ڸ� ���� ���α׷�\n";
	std::wcout << L"+----------------------------------+\n";
}

void Init(void)
{
	SetLocale();
	WelcomeMessage();
}

void OnError(void)
{
	std::wcout << L"���!!! ��������!!!\n";
	std::wcout << L"�����ڿ��� �����ϼ���.\n";
	std::wcout << L"victory8500@naver.com\n";
}

// ������ data.csv�� ���·� �ް�, �Ľ��ؾ� ��.
int main(void)
{
	Init();

	Parser p;
	if (!p.Parse())
	{
		OnError();
		return -1;
	}

	SeatDistributer sd(p.GetPeople());
	sd.Iterate();

	return 0;
}