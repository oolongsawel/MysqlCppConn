#define SOCKET int
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>  //memcpy
#include <mysql.h>

#include "Dbinfo.h"
#include "Employee.h"
#include "queue.h"
#include "sqlQuery.h"




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
		strcpy_s(ptr[0]->EMPNO, empno);
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

int LoadDataForEmployees(struct q_block * p_q_blk, int row){
	int query_stat;
	char buff[1024];
	int count = row;
    int i = 0 ;

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
	p_q_blk->p_employee = (ARR_EMPLOYEE *)malloc(sizeof(ARR_EMPLOYEE) + (sizeof(EMPLOYEE) * count));
	EMPLOYEE * datas = p_q_blk->p_employee->employees;

	while((sql_row = mysql_fetch_row(sql_result)) != NULL)
	{	
		char tempkey[100];
		memset(tempkey,0x00,sizeof(tempkey));
		//printf("%s \n", sql_row[1]);
		strcpy_s(datas[i].EMPNO, SIZE_EMPNO, sql_row[0]);
		strcpy_s(datas[i].NAME, SIZE_NAME, sql_row[1]);
		strcpy_s(datas[i].DEPT, SIZE_DEPT, sql_row[2]);
		datas[i].AGE = (int)sql_row[3];
		strcpy_s(datas[i].HIREDATE,SIZE_HIREDATE,  sql_row[4]);
		strcpy_s(datas[i].SEX, SIZE_SEX,  sql_row[5]);
		datas[i].SALARY = (int)sql_row[6];
		strcpy_s(tempkey,SIZE_EMPNO,datas[i].EMPNO);
		strcat_s(tempkey, "#");
		strcat_s(tempkey,datas[i].NAME);
		strcpy_s(datas[i].KEY,SIZE_KEY, tempkey);
	i++;	
	}

	p_q_blk->p_employee->row = count;
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
	
	//printf("이름 %s\n", (*ptr)[0].NAME);
	//printf("이름 %s\n", (*ptr)[1].NAME);
	//printf("key %s\n", (*ptr)[1].KEY);
	

	insert_table_employee(*ptr,2);
	select_table_employee(TABLE_NAME_EMPLOYEE);
	
	struct q_block * q_blk;
	LoadDataForEmployees(q_blk, 2);
	printf("%s\n", q_blk->p_employee->employees[0].NAME);
	printf("%s\n", q_blk->p_employee->employees[0].KEY);
	return 0;
}
