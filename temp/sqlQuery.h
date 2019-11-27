#ifndef _SQLQUERY_H
#define _SQLQUERY_H

//EMPLOYEE Å×ÀÌºí
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

#endif