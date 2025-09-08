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
	std::wcout << L"컴퓨터과학부 23학번 신승리가 만든\n";
	std::wcout << L"유오스타 개강총회 자리 배정 프로그램\n";
	std::wcout << L"+----------------------------------+\n";
}

void Init(void)
{
	SetLocale();
	WelcomeMessage();
}

void OnError(void)
{
	std::wcout << L"비상!!! 오류났다!!!\n";
	std::wcout << L"제작자에게 문의하세요.\n";
	std::wcout << L"victory8500@naver.com\n";
}

// 파일을 data.csv의 형태로 받고, 파싱해야 함.
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