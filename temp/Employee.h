//Employee.h
#pragma once

#ifndef _EMPLOYEE_H_
#define _EMPLOYEE_H_

#pragma pack(1) //struct memeber alignment 를 1로 설정
typedef struct st_employee{
    char  EMPNO[6];
    char  NAME[12];
    char  DEPT[3];
    int   AGE;
    char  HIREDATE[12];
    char  SEX[1];
    float SALARY;  
} EMPLOYEE;

typedef struct st_array_employee{
    int  row;
    EMPLOYEE * arr_employee[1]; 
} ARRAY_EMPLOYEE;

typedef struct arr_employee{
    int  row;
    EMPLOYEE employees[1]; 
} ARR_EMPLOYEE;


#pragma pack() //struct memeber aligment 원상복구



#endif