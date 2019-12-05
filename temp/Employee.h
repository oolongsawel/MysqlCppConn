//Employee.h
#pragma once
#include "defs.h"

#ifndef _EMPLOYEE_H_
#define _EMPLOYEE_H_

#pragma pack(1) //struct memeber alignment 를 1로 설정
typedef struct st_employee{
    char  EMPNO[SIZE_EMPNO];
    char  NAME[SIZE_NAME];
    char  DEPT[SIZE_DEPT];
    int   AGE;
    char  HIREDATE[SIZE_HIREDATE];
    char  SEX[SIZE_SEX];
    float SALARY;
	char KEY[SIZE_KEY];
} EMPLOYEE;


typedef struct arr_employee{
    int  row;
    EMPLOYEE employees[1]; 
} ARR_EMPLOYEE;


#pragma pack() //struct memeber aligment 원상복구



#endif