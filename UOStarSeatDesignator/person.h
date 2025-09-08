#pragma once

#define NAME_MAX_LEN	16
#define STUDENT_NUM_LEN	11

typedef enum
{
	Invalid = -1,
	INFP = 0,
	ENFP,
	INFJ,
	ENFJ,
	INTJ,
	ENTJ,
	INTP,
	ENTP,
	ISFP,
	ESFP,
	ISTP,
	ESTP,
	ISFJ,
	ESFJ,
	ISTJ,
	ESTJ
} MBTI;

typedef struct
{
	wchar_t name[NAME_MAX_LEN];
	wchar_t student_number[STUDENT_NUM_LEN];
	MBTI mbti;
	bool is_male;
	bool is_executive;
}	Person;