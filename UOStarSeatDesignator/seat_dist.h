#pragma once

#include <vector>
#include <string>

#include "person.h"

#define TEN_SEAT_TABLE_COUNT	6
#define FOUR_SEAT_TABLE_COUNT	6
#define SEAT_COUNT				(TEN_SEAT_TABLE_COUNT * 10 + FOUR_SEAT_TABLE_COUNT * 4)

#define ITERATION_LIMIT		100000

#define SEAT_NO_TO_INDEX(NO)	((NO) - 1)

class SeatDistributer
{
public:
	SeatDistributer(std::vector<Person>* people);
	void Iterate(void);
	void Print(void);

private:
	std::vector<Person>* people;
	Person* seats[SEAT_COUNT];

	void DistributeRandomly(void);
	bool CheckIfSeatIsValid10(int seat_no, int check_no);
	bool CheckIfSeatIsValid4(int seat_no, int check_no);
	void RandomlySwapSeats(int* p_no, int* q_no);
	void RevertSeats(int p_no, int q_no);

	// U(p, q) = Wg * G(gp, gq) + Wm * M(mp, mq) + E(p, q)
	// G: gender
	// M: MBTI
	// E: executive
	double NeighborSatisfactionScore(Person* p, Person* q);
	double CalculateAdjacentSeatScore(int seat_no);
	double CalculateTotalScore(void);
	bool Annealing(double temperature);

	double G(Person* p, Person* q);
	double M(Person* p, Person* q);
	double E(Person* p, Person* q);
};