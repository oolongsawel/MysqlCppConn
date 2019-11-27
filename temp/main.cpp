#define SOCKET int
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>  //memcpy
#include <mysql.h>

#include "Dbinfo.h"


//EMPLOYEE 테이블
#define TABLE_NAME_EMPLOYEE "employee"
#define SQL_CREATE_TABLE_EMPLOYEE "CREATE TABLE %s \
    (EMPNO      CHAR(6)         NOT NULL,\
    NAME        VARCHAR(12)     NOT NULL,\
    DEPT        CHAR(10)                 ,\
    AGE         INT                 ,\
    HIREDATE    DATE                    ,\
    SEX         CHAR(1)                 ,\
    SALARY      DECIMAL(9,2)            ,\
    PRIMARY KEY (EMPNO)) ENGINE = myISAM DEFAULT CHARSET = utf8"

#define SQL_DROP_TABLE "DROP TABLE IF EXISTS %s"


#define SQL_INSERT_DATA_EMPLOYEE "INSERT INTO %s \
    (EMPNO, NAME, DEPT, AGE, HIREDATE, SEX, SALARY) values \
('%s', '%s', '%s',%d, sysdate(), '%s', %f)"

#define SQL_SELECT_DATA_EMPLOYEE "SELECT EMPNO, NAME, DEPT, HIREDATE, SEX, SALARY FROM %s "    



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

int connect();
int close();
int create_table_employee();
int drop_table(char * table_name);
EMPLOYEE * add_employee(int cnt, EMPLOYEE employee_list[], char * empno, char * name, char * dept, int age, char * hiredate, char * sex , float salary);
int insert_table_employee(EMPLOYEE * employee_list, int count);
int select_table_employee(char * table_name);

MYSQL * connection, conn;


/** DB접속 **/
int connect(){
    //mysql 초기화
    mysql_init(&conn);

    //DB연결
    connection = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME,3306
    ,(char *)NULL, 0);

    if(connection == NULL){
        fprintf(stderr, "MYSQL connection error : %s", mysql_error(&conn));
        exit(0);
    } else {
        printf("MYSQL connected\n");
    }
    return 0;
}

/** DB접속 닫음 **/
int close(){
    mysql_close(connection);
    printf("mysql closed\n");
    return 0;
}

/** 테이블생성 **/
int create_table_employee(){
    int query_stat;
    char buff[1024];

    printf("buff size %d\n", sizeof(buff));

    memset(buff, 0x00, sizeof(buff));
    sprintf_s(buff, SQL_CREATE_TABLE_EMPLOYEE, TABLE_NAME_EMPLOYEE);
    query_stat = mysql_query(connection, buff);
    if(query_stat != 0){
        fprintf(stderr, "Mysql query error : %s\n", mysql_error(&conn));
        exit(0);
    } else {
        printf("created table\n");
    }
    return 0;
}

int insert_table_employee(EMPLOYEE * employee_list, int count){
	int query_stat;
	char buff[1024];

	printf("buff size %d\n", sizeof(buff));
	memset(buff, 0x00, sizeof(buff));
	for(int i = 0; i < count; i++){
		sprintf_s(buff, SQL_INSERT_DATA_EMPLOYEE, TABLE_NAME_EMPLOYEE
			, employee_list[i].EMPNO, employee_list[i].NAME, employee_list[i].DEPT, employee_list[i].AGE, employee_list[i].SEX, employee_list[i].SALARY);
		query_stat = mysql_query(connection, buff);
		if(query_stat != 0){
			fprintf(stderr, "Mysql query error : %s\n", mysql_error(&conn));
			exit(0);
		} else {
			printf("data insert success\n");
		}
	}
	return 0;
	
}
/** 기존 테이블 삭제 **/
int drop_table(char * table_name){
    int query_stat;
    char buff[1024];

    //기존테이블 삭제
    memset(buff,0x00,sizeof(buff));
    sprintf(buff, SQL_DROP_TABLE, table_name);
    query_stat = mysql_query(connection, buff);

    if(query_stat != 0){
        fprintf(stderr,"Mysql query error : %s\n", mysql_error(&conn));
        exit(0);
    } else {
        printf("table dropped\n");
    }

    return 0;
}


void add_employee(int cnt, EMPLOYEE ** ptr, char * empno, char * name, char * dept, int age, char * hiredate, char * sex , float salary){ 
	if(cnt == 0){
		cnt ++;
		strcpy(ptr[0]->EMPNO, empno);
		strcpy(ptr[0]->NAME, name);
		strcpy(ptr[0]->DEPT, dept);
		ptr[0]->AGE = age;
		strcpy(ptr[0]->HIREDATE, hiredate);
		strcpy(ptr[0]->SEX, sex);
		ptr[0]->SALARY = salary;
		
	} else {
		cnt++;
		//임시배열생성
		EMPLOYEE * temp_list = (EMPLOYEE*)malloc(sizeof(EMPLOYEE) * cnt);
		//기존값을 임시배열에 복사
		memcpy(temp_list,(*ptr), sizeof(EMPLOYEE) * (cnt-1));

		strcpy(temp_list[cnt-1].EMPNO, empno);
		strcpy(temp_list[cnt-1].NAME, name);
		strcpy(temp_list[cnt-1].DEPT, dept);
		temp_list[cnt-1].AGE = age;
		strcpy(temp_list[cnt-1].HIREDATE, hiredate);
		strcpy(temp_list[cnt-1].SEX, sex);
		temp_list[cnt-1].SALARY = salary;
			
		//메모리재할당
		*ptr = (EMPLOYEE *)realloc(*ptr, sizeof(EMPLOYEE) * cnt);
		//배열요소 복사
		memcpy(*ptr,temp_list,sizeof(EMPLOYEE) * cnt);
		//메모리해제
		free(temp_list);
	}
}

int select_table_employee(char * table_name){
	int query_stat;
	char buff[1024];
	int count = 0;
    int i ;

	MYSQL_RES * sql_result;
    MYSQL_ROW sql_row;
	
	printf("buff size %d\n", sizeof(buff));
	memset(buff, 0x00, sizeof(buff));
	
	sprintf_s(buff, SQL_SELECT_DATA_EMPLOYEE, TABLE_NAME_EMPLOYEE);
	query_stat = mysql_query(connection, buff);

	if(query_stat != 0){
			fprintf(stderr, "Mysql query error : %s\n", mysql_error(&conn));
			exit(1);
	} 

	sql_result = mysql_store_result(connection);
    while((sql_row = mysql_fetch_row(sql_result)) != NULL)
	{
		printf("%s \n", sql_row[1]);
		//printf("%s %s %s\n",sql_row[0],sql_row[1],sql_row[2]);
	}
	mysql_free_result(sql_result);

	return 0;
}

int main(){
	connect();
	drop_table(TABLE_NAME_EMPLOYEE);
	create_table_employee();
	char table_name[10] = "EMPLOYEE";
	
	char * no = "1";
	char * nm = "sunyoung";
	char * dep = "ito";
	int age = 28;
	char * date = "20180630";
	char * sex = "f";
	float salary = 200.0;

	EMPLOYEE * arr_employee[1];
	EMPLOYEE ** ptr = arr_employee;
	
	add_employee(0, ptr, no,nm,dep,28,date,sex,200);
	add_employee(1, ptr, "2","hyen","si",31,"191024","m",300);
	
	printf("이름 %s\n", (*ptr)[0].NAME);
	printf("이름 %s\n", (*ptr)[1].NAME);
	
	insert_table_employee(*ptr,2);

	select_table_employee(TABLE_NAME_EMPLOYEE);
	
	/*
	ARRAY_EMPLOYEE  * arremp_ptr;
	ARRAY_EMPLOYEE ** dptr = &arremp_ptr;
	EMPLOYEE  ** demp = (*dptr)->arr_employee;
	(*dptr)->row = 0;
	add_employee2(dptr, demp , "1","maria","si",25,"190514","m",300);
	add_employee2(dptr, demp , "2","kimukimu","cloud",27,"191115","m",300);
	
	printf("arr구조체 이름 %s\n", (*dptr)->arr_employee[0]->NAME);
	printf("arr구조체 이름%s\n", (*dptr)->arr_employee[1]->NAME);
	*/


 return 0;
}
