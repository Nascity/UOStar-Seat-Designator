#include <iostream>

#include "seat_dist.h"

static const int mbti_compat[16][16] =
{
	{ 3, 3, 3, 4, 3, 4, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 3, 3, 4, 3, 4, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 3, 4, 3, 3, 3, 3, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 4, 3, 3, 3, 3, 3, 3, 3, 4, 0, 0, 0, 0, 0, 0, 0 },
	{ 3, 4, 3, 3, 3, 3, 3, 4, 2, 2, 2, 2, 1, 1, 1, 1 },
	{ 4, 3, 3, 3, 3, 3, 4, 3, 2, 2, 2, 2, 2, 2, 2, 2 },
	{ 3, 3, 3, 3, 3, 4, 3, 3, 2, 2, 2, 2, 1, 1, 1, 4 },
	{ 3, 3, 4, 3, 4, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1 },
	{ 0, 0, 0, 4, 2, 2, 2, 2, 1, 1, 1, 1, 2, 4, 2, 4 },
	{ 0, 0, 0, 0, 2, 2, 2, 2, 1, 1, 1, 1, 4, 2, 4, 2 },
	{ 0, 0, 0, 0, 2, 2, 2, 2, 1, 1, 1, 1, 2, 4, 2, 4 },
	{ 0, 0, 0, 0, 2, 2, 2, 2, 1, 1, 1, 1, 4, 2, 4, 2 },
	{ 0, 0, 0, 0, 1, 2, 1, 1, 2, 4, 2, 4, 3, 3, 3, 3 },
	{ 0, 0, 0, 0, 1, 2, 1, 1, 4, 2, 4, 2, 3, 3, 3, 3 },
	{ 0, 0, 0, 0, 1, 2, 1, 1, 2, 4, 2, 4, 3, 3, 3, 3 },
	{ 0, 0, 0, 0, 1, 2, 4, 1, 4, 2, 4, 2, 3, 3, 3, 3 }
};

SeatDistributer::SeatDistributer(std::vector<Person>* people)
	: people(people)
{
	int i;

	for (i = 0; i < SEAT_COUNT; i++)
		seats[i] = NULL;
	DistributeRandomly();
}

void SeatDistributer::Iterate(void)
{
	std::wcout << L"제 점수는요: " << CalculateTotalScore() << '\n';
}

void SeatDistributer::DistributeRandomly(void)
{
	// implement!!!!!
	int i = 0;

	for (auto it = people->begin();
		it != people->end();
		it++, i++)
		seats[i] = &*it;
}

double SeatDistributer::CalculateTotalScore(void)
{
	int i;
	double sum;

	for (i = 1, sum = 0; i < SEAT_COUNT; i++)
		sum += CalculateAdjacentSeatScore(i);

	return sum;
}

double SeatDistributer::CalculateAdjacentSeatScore(int seat_no)
{
	int add[] = { 1, 5, 6 };
	int check_no;
	int i;
	double sum;

	for (i = 0, sum = 0; i < sizeof(add) / sizeof(int); i++)
	{
		check_no = seat_no + add[i];
		if (seat_no <= 10 * TEN_SEAT_TABLE_COUNT
			&& !CheckIfSeatIsValid10(seat_no, check_no))
			break;
		else if (seat_no > 10 * TEN_SEAT_TABLE_COUNT
			&& !CheckIfSeatIsValid4(seat_no, check_no))
			break;

		sum += NeighborSatisfactionScore(
			seats[SEAT_NO_TO_INDEX(seat_no)],
			seats[SEAT_NO_TO_INDEX(check_no)]
		);
	}
	
	return sum;
}

double SeatDistributer::NeighborSatisfactionScore(Person* p, Person* q)
{
	if (!p || !q)
		return 0.0;
	return 1.5 * G(p, q) + 2.0 * M(p, q) + 1.1 * E(p, q);
}

bool SeatDistributer::CheckIfSeatIsValid10(int seat_no, int check_no)
{
	return ((seat_no - 1) / 10) == ((check_no - 1) / 10);
}

bool SeatDistributer::CheckIfSeatIsValid4(int seat_no, int check_no)
{
	return ((seat_no - 1) / 4) == ((check_no - 1) / 4);
}

double SeatDistributer::G(Person* p, Person* q)
{
	if (p->is_male ^ q->is_male)
		return 1.0;
	return 0.8;
}

double SeatDistributer::M(Person* p, Person* q)
{
	return (double)mbti_compat[(int)p->mbti][(int)q->mbti];
}

double SeatDistributer::E(Person* p, Person* q)
{
	if (p->is_executive || q->is_executive)
		return 1.0;
	return 0.75;
}