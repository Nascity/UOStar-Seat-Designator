#include <iostream>
#include <fstream>
#include <stdlib.h>

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

void SeatDistributer::Print(void)
{
	std::wofstream of(L"seat.csv", std::ios_base::binary);
	int i;

	for (i = 0; i < SEAT_COUNT; i++)
	{
		if (!seats[i])
			continue;
		std::wcout << i + 1 << L',' << seats[i]->name << L',' << seats[i]->student_number << L'\n';
		of << i + 1 << L',' << seats[i]->name << L',' << seats[i]->student_number << L'\n';
	}

	of.close();
}

void SeatDistributer::Iterate(void)
{
	int i;
	double cur_score, new_score;
	double temperature;
	int p, q;

	std::ofstream of("iter.csv");

	for (i = 0, cur_score = CalculateTotalScore(), temperature = 1.0;
		i < ITERATION_LIMIT; i++, temperature *= 0.999)
	{
		RandomlySwapSeats(&p, &q);
		new_score = CalculateTotalScore();

		if (new_score <= cur_score && Annealing(temperature))
			RevertSeats(p, q);
		else
			cur_score = new_score;

		of << i << "," << new_score << '\n';
		if (i % 10000 == 0)
			std::wcout << i << L"번째 iteration | 점수: " << cur_score << '\n';
	}

	of.close();
}

void SeatDistributer::RandomlySwapSeats(int* p_no, int* q_no)
{
	Person* temp;

	*p_no = rand() % SEAT_COUNT + 1;
	do
	{
		*q_no = rand() % SEAT_COUNT + 1;
	} while (*p_no == *q_no);

	temp = seats[*p_no - 1];
	seats[*p_no - 1] = seats[*q_no - 1];
	seats[*q_no - 1] = temp;
}

void SeatDistributer::RevertSeats(int p_no, int q_no)
{
	Person* temp;

	temp = seats[p_no - 1];
	seats[p_no - 1] = seats[q_no - 1];
	seats[q_no - 1] = temp;
}

bool SeatDistributer::Annealing(double temperature)
{
	int random;

	random = rand() % 10000;

	return (double)random / 10000.0 >= temperature;
}

void SeatDistributer::DistributeRandomly(void)
{
	std::vector<int> seat_no_indices(SEAT_COUNT);
	int i, j;

	for (i = 0; i < SEAT_COUNT; i++)
		seat_no_indices[i] = i + 1;

	for (i = SEAT_COUNT - 1; i > 0; i--)
	{
		j = rand() % (i + 1);
		std::swap(seat_no_indices[i], seat_no_indices[j]);
	}

	i = 0;
	for (auto it = people->begin();
		it != people->end();
		it++, i++)
	{
		int seat_no = seat_no_indices[i];

		seats[seat_no - 1] = &*it;
	}
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