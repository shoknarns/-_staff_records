// Migration_TT_to_MySql.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <conio.h>
#include <cstdlib>
#include <mysql.h>
#include <windows.h>

#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>


#pragma comment(lib, "libmysql.lib")

using namespace std;

int staff_count_global=0;

struct record
{
	int staff_count;
	char *fio= (char *)malloc(51);
	char *bday = (char *)malloc(3);
	char *bmonth = (char *)malloc(3);
	char *byear = (char *)malloc(5);
	char *wday = (char *)malloc(3);
	char *wmonth = (char *)malloc(3);
	char *wyear = (char *)malloc(5);
	char *numofwrs = (char *)malloc(4);
	int workers[100];
	string disc;
} rec[200];

char* initiate(fstream &iput,char *result, int num, int NUMBER_OF_WORKERS,int i)
{
	string CHEK;
	char p[100], *s = p;
	int count=0;
	do
	{
		iput.getline(rec[i].fio, 50);
		CHEK = rec[i].fio;
	} while (CHEK == "");	
	iput >> rec[i].bday;	
	iput >> rec[i].bmonth;
	iput >> rec[i].byear;
	iput >> rec[i].wday;
	iput >> rec[i].wmonth;
	iput >> rec[i].wyear;	
	if (num == 1)
	{
		iput >> rec[i].numofwrs;
		for (int k = 0; k < atoi(rec[i].numofwrs); k++)
		{
			do
			{
				iput.getline(s, 50);
				CHEK = s;				
			} while (CHEK == "");
			for (int j = 0; j < NUMBER_OF_WORKERS; j++)
			{
				if (rec[j].fio==CHEK)	
				{
					rec[i].workers[count]= rec[j].staff_count;
					count++;
				}
			}			
		}		
	}
	else
		if (num == 2)
		{
			iput >> rec[i].disc;
		}

	int len1 = strlen(rec[i].fio);
	int len2 = strlen(rec[i].bday);
	int len3 = strlen(rec[i].bmonth);
	int len4 = strlen(rec[i].byear);
	int len5 = strlen(rec[i].wday);
	int len6 = strlen(rec[i].wmonth);
	int len7 = strlen(rec[i].wyear);		
	int len8 = strlen(rec[i].numofwrs);	
	
	if (num == 0) { memcpy_s(result, 100, "INSERT INTO `workers` (`fio`,`bday`,`bmonth`,`byear`,`wday`,`wmonth`,`wyear`) VALUES('", 87); }
	if (num == 1) { memcpy_s(result, 125, "INSERT INTO `managers` (`fio`,`bday`,`bmonth`,`byear`,`wday`,`wmonth`,`wyear`,`numofwrs`,`workers`) VALUES('", 109); }
	if (num == 2) { memcpy_s(result, 105, "INSERT INTO `others` (`fio`,`bday`,`bmonth`,`byear`,`wday`,`wmonth`,`wyear`,`discription`) VALUES('", 100); }	
	memcpy_s(result + strlen(result), len1 + strlen(result), rec[i].fio, len1 + strlen(result));
	memcpy_s(result + strlen(result), 2 + strlen(result), "',", 2 + strlen(result));
	memcpy_s(result + strlen(result), len2 + strlen(result), rec[i].bday, len2 + strlen(result));
	memcpy_s(result + strlen(result), 1 + strlen(result), ",", 1 + strlen(result));
	memcpy_s(result + strlen(result), len3 + strlen(result) + 1, rec[i].bmonth , len3 + strlen(result));
	memcpy_s(result + strlen(result), 1 + strlen(result), ",", 1 + strlen(result));
	memcpy_s(result + strlen(result), len4 + strlen(result), rec[i].byear, len4 + strlen(result) );
	memcpy_s(result + strlen(result), 1 + strlen(result), ",",1 + strlen(result));
	memcpy_s(result + strlen(result), len5 + strlen(result), rec[i].wday , len5 + strlen(result));
	memcpy_s(result + strlen(result), 1 + strlen(result), ",", 1 + strlen(result));
	memcpy_s(result + strlen(result), len6 + strlen(result), rec[i].wmonth, len6 + strlen(result));
	memcpy_s(result + strlen(result), 1 + strlen(result), ",", 1 + strlen(result));
	memcpy_s(result + strlen(result), len7 + strlen(result), rec[i].wyear, len7 + strlen(result));	
	if (num==1)
	{
		memcpy_s(result + strlen(result), 1 + strlen(result), ",", 1 + strlen(result));
		memcpy_s(result + strlen(result), len8 + strlen(result), rec[i].numofwrs, len8 + strlen(result));
		memcpy_s(result + strlen(result), 2 + strlen(result), ",'", 2 + strlen(result));
		for (int q = 0; q < count; q++)
		{			
			CHEK=to_string(rec[i].workers[q]);
			memcpy_s(result + strlen(result), strlen(CHEK.c_str()) + strlen(result), CHEK.c_str(), strlen(CHEK.c_str()) + strlen(result));
			memcpy_s(result + strlen(result), 1 + strlen(result), ";", 1 + strlen(result));
		}
		memcpy_s(result + strlen(result), 2 + strlen(result), "'", 2 + strlen(result));
	}
	if (num == 2)
	{
		memcpy_s(result + strlen(result), 2 + strlen(result), ",'",  2 + strlen(result));
		memcpy_s(result + strlen(result), strlen(rec[i].disc.c_str()) + strlen(result), rec[i].disc.c_str(), strlen(rec[i].disc.c_str()) + strlen(result));
		memcpy_s(result + strlen(result), 2 + strlen(result), "'", 2 + strlen(result));
	}
	memcpy_s(result + strlen(result), strlen(result) + 3, ");", 3 + strlen(result));	
	staff_count_global++;
	rec[i].staff_count = staff_count_global;	
	return result;
}

int main(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "rus");
	MYSQL *conn;	
	MYSQL_RES *res;
	MYSQL_ROW row;
	string login="", password="";
	int NUMBER_OF_WORKERS, NUMBER_OF_MANAGERS, NUMBER_OF_OTHERS, NUMBER_OF_SUBORDINATE;

	{
		conn = mysql_init(NULL);
		if (conn == NULL)
		{			
			fprintf(stderr, "Ошибка: Невозможно создать дескриатор mysql\n");
			system("Pause");
			return 0;
		}
		// Подключаемся к серверу
		while (true)
		{
			system("cls");
			cout << "Введите логин: ";
			cin >> login;
			cout << "Введите пароль: ";
			cin >> password;
			if (!mysql_real_connect(conn, "localhost", login.c_str(), password.c_str(), NULL, NULL, NULL, 0))  //"TT_staff_records"
			{				
				fprintf(stderr, "Ошибка: невозможно подключиться к серверу\n", mysql_error(conn));
				system("Pause");
				mysql_close(conn);
				return 0;				
			}
			else
			{
				cout << "Подключено!" << endl;
				break;
			}
		}
		cout << "connection_character_set: " << mysql_character_set_name(conn) << endl;
	}

	char p[100];		
	fstream iput;
	string CHEK;
	char *result = (char *)malloc(500);

	iput.open("iput.txt", ios::in);
	iput >> p;
	NUMBER_OF_WORKERS = atoi(p);
	iput >> p;
	NUMBER_OF_MANAGERS = atoi(p);
	iput >> p;
	NUMBER_OF_OTHERS = atoi(p);
	
	if (mysql_query(conn, "CREATE DATABASE tt_staff_records;"))
	{
		cout << "Ошибка создания базы данных 'tt_staff_records':\n" << mysql_error(conn) << endl;		
	}
	else cout << "База данных 'TT_staff_records' успешно создана." << endl;
	if (mysql_select_db(conn, "tt_staff_records"))
	{
		cout << "Ошибка:\n" << mysql_error(conn) << endl;

	}
	if (mysql_query(conn, "CREATE TABLE `tt_staff_records`.`workers` (`idworker` INT NOT NULL,`Fio` VARCHAR(100) NULL DEFAULT '\"\"',`bday` DECIMAL(2) ZEROFILL NULL,`bmonth` DECIMAL(2) ZEROFILL NULL,`byear` DECIMAL(4) ZEROFILL NULL,`wday` DECIMAL(2) ZEROFILL NULL,`wmonth` DECIMAL(2) ZEROFILL NULL,`wyear` DECIMAL(4) ZEROFILL NULL,PRIMARY KEY(`idworker`));"))
	{
		cout << "Ошибка создания таблицы 'workers':\n" << mysql_error(conn)<<endl;
	}
	else
	{
	    cout << "Таблица 'workers' успешно создана." << endl;
		for (int i = 0; i < NUMBER_OF_WORKERS; i++)
		{
			if (!mysql_query(conn, initiate(iput, result, 0, NUMBER_OF_WORKERS, i)))
				cout << "Запись " << i << " добавлена" << endl;
			else
				cout << "Ошибка записи в таблицу 'worker'\n" << mysql_error(conn) << endl << endl;
		}
	}
	if (mysql_query(conn, "CREATE TABLE `tt_staff_records`.`managers` (`idmanagers` INT NOT NULL,`fio` VARCHAR(100) NOT NULL,`bday` INT ZEROFILL NULL,`bmonth` INT ZEROFILL NULL,`byear` DECIMAL(4) ZEROFILL NULL,`wday` DECIMAL(2) ZEROFILL NULL,`wmonth` DECIMAL(2) ZEROFILL NULL,`wyear` DECIMAL(4) ZEROFILL NULL,`numofwrs` DECIMAL(2) ZEROFILL NULL,`workers` VARCHAR(100) NULL,PRIMARY KEY(`idmanagers`));"))
	{
		cout << "Ошибка создания таблицы 'managers'\n" << mysql_error(conn) << endl;
	}
	else
	{
		cout << "Таблица 'managers' успешно создана." << endl;
		for (int i = NUMBER_OF_WORKERS; i < NUMBER_OF_WORKERS + NUMBER_OF_MANAGERS; i++)
		{
			if (!mysql_query(conn, initiate(iput, result, 1, NUMBER_OF_WORKERS, i)))
				cout << "Запись " << i << " добавлена" << endl;
			else
				cout << "Ошибка записи в таблицу 'managers'\n" << mysql_error(conn) << endl << endl;
		}
	}
	if (mysql_query(conn, "CREATE TABLE `tt_staff_records`.`others` (`idother` INT NOT NULL,`Fio` VARCHAR(100) NULL DEFAULT '\"\"',`bday` DECIMAL(2) ZEROFILL NULL,`bmonth` DECIMAL(2) ZEROFILL NULL,`byear` DECIMAL(4) ZEROFILL NULL,`wday` DECIMAL(2) ZEROFILL NULL,`wmonth` DECIMAL(2) ZEROFILL NULL,`wyear` DECIMAL(4) ZEROFILL NULL,`discription` VARCHAR(45) NULL, PRIMARY KEY(`idother`));"))
	{
		cout << "Ошибка создания таблицы 'others'\n" << mysql_error(conn) << endl;	
	}
	else
	{
		cout << "Таблица 'others' успешно создана." << endl;
		for (int i = NUMBER_OF_WORKERS + NUMBER_OF_MANAGERS; i < NUMBER_OF_WORKERS + NUMBER_OF_MANAGERS + NUMBER_OF_OTHERS; i++)
		{
			if (!mysql_query(conn, initiate(iput, result, 2, NUMBER_OF_WORKERS, i)))
				cout << "Запись " << i <<" добавлена"<< endl;
			else
				cout << "Ошибка записи в таблицу 'others'\n" << mysql_error(conn) << endl << endl;
		}
	}
	// Закрываем соединение с сервером базы данных		
	mysql_close(conn);
	free(result);
	iput.close();
	system("Pause");	
	return 0;
}