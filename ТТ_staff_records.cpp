// ТТ_staff_records.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "fstream"
#include "iostream"
#include "string"
#include "cstring"
#include <cstdlib>
#include <conio.h>
#include <mysql.h>

#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using namespace std;
#pragma comment(lib, "/libmysql.lib")

class worker
{
public:
	string fio;
	  int bday;
	  int bmonth;
	  int byear;
	  int wday;
	  int wmonth;
	  int wyear;
	
	string get_fio() { return this->fio; }
	string get_birth_date() { return to_string(bday) + '.' + to_string(bmonth) + '.' + to_string(byear); }
	string get_work_date() { return to_string(wday) + '.' + to_string(wmonth) + '.' + to_string(wyear); }

	void set_fio(char* fio) { this->fio = fio; }
	void set_bday(  int bday) { this->bday = bday; }
	void set_bmonth(  int bmonth) { this->bmonth = bmonth; }
	void set_byear(  int byear) { this->byear = byear; }
	void set_wday(  int wday) { this->wday = wday; }
	void set_wmonth(  int wmonth) { this->wmonth = wmonth; }
	void set_wyear(  int wyear) { this->wyear = wyear; }
	worker()
	{
		this->fio = "";
		this->bday = 0;
		this->bmonth = 0;
		this->byear = 0;
		this->wday = 0;
		this->wmonth = 0;
		this->wyear = 0;
	}

	virtual void show(int i)
	{
		cout << ++i <<":";
		cout.width(35);
		cout << left << get_fio() << "|  ";
		cout.width(13);
		cout << left <<get_birth_date() << "|  " << get_work_date() << endl;
	}	
	bool operator > (worker w)
	{		
		if (this->wyear > w.wyear)
		{
			return true;
		}
		else
			if (this->wyear == w.wyear)
				if (this->wmonth > w.wmonth)
				{
					return true;
				}
				else
					if (this->wmonth == w.wmonth)
						if (this->wday > w.wday)
						{
							return true;
						}
		return false;
	}
};
class manager : public worker
{
public:
	int numofwrs = 0;
	string workers[100];


	manager() :worker()
	{
		this->workers[0] = "";
		this->numofwrs = 0;
	}
	void set_workers(string s)
	{
		this->workers[this->numofwrs] = s;
		this->numofwrs++;
	}

	void get_list_of_workers()
	{
		for (int i = 0; i < numofwrs; i++) { cout << workers[i] << endl; }
	}

	void show(int i)
	{
		cout << ++i << ":";
		cout.width(35);
		cout.width(37);
		cout << left << get_fio() << "|  ";
		cout.width(13);
		cout << left << get_birth_date() << "|  " << get_work_date() << endl;
		cout <<"Люди в подчинении: ";
		for (int i = 0; i < numofwrs; i++) { cout << workers[i] << endl << "                   ";  }
		cout << endl;
	}
};
class other : public worker
{
public:
	string disc;

	other() :worker() { this->disc = ""; }

	string get_disc() { return this->disc; };
	void set_disc(string disc) { this->disc = disc; }
	void show(int i)
	{
		cout << ++i << ":";
		cout.width(35);
		cout.width(37);
		cout << left << get_fio() << "|  ";
		cout.width(13);
		cout << left << get_birth_date() << "|  " << get_work_date() << endl;
		cout << "Описание сотрудника: " << disc << endl << endl;
	}	
};

void initiate_from_DB(MYSQL_ROW row, worker *obj)
{	
	obj->fio = row[1];
	obj->bday = atoi(row[2]);
	obj->bmonth = atoi(row[3]);
	obj->byear = atoi(row[4]);
	obj->wday = atoi(row[5]);
	obj->wmonth = atoi(row[6]);
	obj->wyear = atoi(row[7]);
}
void initiate(fstream &iput,worker *obj)
{
	string CHEK;
	char p[100], *s = p;	
	do
	{
		iput.getline(s, 50);
		CHEK = s;
	} while (CHEK == "");
	obj->set_fio(s);
	iput >> s;
	obj->set_bday(atoi(s));
	iput >> s;
	obj->set_bmonth(atoi(s));
	iput >> s;
	obj->set_byear(atoi(s));
	iput >> s;
	obj->set_wday(atoi(s));
	iput >> s;
	obj->set_wmonth(atoi(s));
	iput >> s;
	obj->set_wyear(atoi(s));
}
void save(fstream &iput, worker *wrk, manager *mgrs, other *oth, int NUMBER_OF_WORKERS, int NUMBER_OF_MANAGERS, int NUMBER_OF_OTHERS)
{
	iput.open("iput.txt", ios::out);
	if (!iput.is_open()) 
		cout << "Не удалось сохранить!\n";
	else
	{
		iput << NUMBER_OF_WORKERS << endl;
		iput << NUMBER_OF_MANAGERS << endl;
		iput << NUMBER_OF_OTHERS << endl;
		for (int i = 0; i < NUMBER_OF_WORKERS; i++)
		{
			iput << wrk[i].fio << endl;
			iput << wrk[i].bday << endl;
			iput << wrk[i].bmonth << endl;
			iput << wrk[i].byear << endl;
			iput << wrk[i].wday << endl;
			iput << wrk[i].wmonth << endl;
			iput << wrk[i].wyear << endl;
		}
		for (int i = 0; i < NUMBER_OF_MANAGERS; i++)
		{
			iput << mgrs[i].fio << endl;
			iput << mgrs[i].bday << endl;
			iput << mgrs[i].bmonth << endl;
			iput << mgrs[i].byear << endl;
			iput << mgrs[i].wday << endl;
			iput << mgrs[i].wmonth << endl;
			iput << mgrs[i].wyear << endl;			
			iput << mgrs[i].numofwrs << endl;
			for (int j = 0; j < mgrs[i].numofwrs; j++)
			{
				iput << mgrs[i].workers[j] << endl;
			}
		}
		for (int i = 0; i < NUMBER_OF_OTHERS; i++)
		{
			iput << oth[i].fio << endl;
			iput << oth[i].bday << endl;
			iput << oth[i].bmonth << endl;
			iput << oth[i].byear << endl;
			iput << oth[i].wday << endl;
			iput << oth[i].wmonth << endl;
			iput << oth[i].wyear << endl;
			iput << oth[i].disc << endl;
		}
		iput.close();
	}
}

template <class first>
void print(first* f, int NUMBER_OF_RECORDS)
{	
	string s;

	system("cls");
	s = typeid(f).name();
	if (s == "class worker * __ptr64") s = "Список работников";
	if (s == "class manager * __ptr64") s = "Список менеджеров";
	if (s == "class other * __ptr64") s = "Список других сотрудников";	
	cout.width(40);
	cout << right << endl << s << endl;
	cout << "________________________________________________________________________________";
	cout << "       ФИО сотруднки                 | Дата рождения | Дата принятия на работу\n";
	cout << "--------------------------------------------------------------------------------";
	for (int i = 0; i < NUMBER_OF_RECORDS; i++) { f[i].show(i); }
	cout << "________________________________________________________________________________";
	system("pause");
}
template <class first>
 int search(first* f, int NUMBER_OF_RECORDS)
{
	 int q = 0,i = 0;
	 string CHEK;

	 system("cls");
	 cout << "Введите фамилию сотрудника: ";
	 cin >> CHEK;	 
	 for (int i = 0; i < NUMBER_OF_RECORDS; i++)
	 {
		 if (strstr(f[i].fio.c_str(), CHEK.c_str()))
		 {	
			 q++;
			 f[i].show(i);
		 }
	 }
	 if (q == 0)
	 {
		 cout << "Сотрудников с такой фамилией нет!\n\n"; 		 
		 system("pause");
		 return -1;
	 }
	 else
	 {
		 do
		 {			
			cout << "Введите номер нужного сотрудника: ";
			cin >> i;			
		 } while (i < 0); 
		 if (!(i>NUMBER_OF_RECORDS))	{ return i; }
		 else		{ return -1; }
	 }
}
template <class first>
 int del(first* f, int& NUMBER_OF_RECORDS, MYSQL *conn,int SOURCE)
{
	string s;
	int i;
	i = search(f, NUMBER_OF_RECORDS);
	if (i != (-1))
	{
		if (SOURCE == 2)
		{
			s = typeid(f).name();
			if (s == "class worker * __ptr64") s = "DELETE FROM tt_staff_records.others WHERE `idworker`=";
			if (s == "class manager * __ptr64") s = "DELETE FROM tt_staff_records.others WHERE `idmanagers`=";
			if (s == "class other * __ptr64") s = "DELETE FROM tt_staff_records.others WHERE `idother`=";			
			s += to_string(i) + ";";
			if (mysql_query(conn, s.c_str()))
				cout << endl << mysql_errno(conn) << endl;;
		}
		for (int j = i; j < NUMBER_OF_RECORDS; j++)		{ f[j] = f[j + 1]; }		
		NUMBER_OF_RECORDS--;		
		return 1;
	}
	return 0;
}
template <class first, class second >
 void move(first* f, second* s,int POSITION_OF_FIRST, int POSITION_OF_SECOND)
{
	f[POSITION_OF_FIRST].fio = s[POSITION_OF_SECOND].fio;
	f[POSITION_OF_FIRST].bday = s[POSITION_OF_SECOND].bday;
	f[POSITION_OF_FIRST].bmonth = s[POSITION_OF_SECOND].bmonth;
	f[POSITION_OF_FIRST].byear = s[POSITION_OF_SECOND].byear;
	f[POSITION_OF_FIRST].wday = s[POSITION_OF_SECOND].wday;
	f[POSITION_OF_FIRST].wmonth = s[POSITION_OF_SECOND].wmonth;
	f[POSITION_OF_FIRST].wyear = s[POSITION_OF_SECOND].wyear;	
}
template <class first>
void sort(first* f, int NUMBER_OF_RECORDS)
{
	first* temp=new first();
	char CHEK;

	system("cls");
	cout << "Выберите вариант сортироваки\n";
	cout << "1:По фамилиям\n";
	cout << "2:По дате принятия\n";
	cin >> CHEK;
	if (CHEK == '1')
	{
		for (int i = 0; i < NUMBER_OF_RECORDS - 1; i++)
			for (int j = i+1; j < NUMBER_OF_RECORDS; j++)
			{
				if (f[i].fio > f[j].fio)
				{
					*temp = f[i];
					f[i] = f[j];
					f[j] = *temp;
				}
			}
	}
	if (CHEK == '2')
	{
		for (int i = 0; i < NUMBER_OF_RECORDS-1; i++)
			for (int j = i+1; j < NUMBER_OF_RECORDS; j++)
			{
				if (f[i] > f[j]) 
				{
					*temp = f[i];
					f[i] = f[j];
					f[j] = *temp;
				}													
			}
	}	
	cout << "Таблица отсортирована!\n\n" << endl;
	system("pause");
 }

int main()
{	
	setlocale(LC_ALL, "rus");

	const int LENGTH = 100;
	int SOURCE, i = 0, q = 0, NUMBER_OF_WORKERS, NUMBER_OF_MANAGERS, NUMBER_OF_OTHERS, NUMBER_OF_SUBORDINATE;
	char p[100], *s = p;
	string CHEK,login,password;	
	fstream iput;
	worker wrs[LENGTH], w;
	manager mgr[LENGTH];
	other oth[LENGTH];
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;	

	cout <<"Выберите источник данных\n";
	cout << "1:Файл;\n";
	cout << "2:БД MySql;\n";
	cin >> SOURCE;
	if (SOURCE == 1)
	{
		
		iput.open("iput.txt", ios::in);
		if (!iput.is_open())
			cout << "Файл не может быть открыт!\n";
		else
		{			
			iput >> p;
			NUMBER_OF_WORKERS = atoi(p);
			iput >> p;
			NUMBER_OF_MANAGERS = atoi(p);
			iput >> p;
			NUMBER_OF_OTHERS = atoi(p);

			for (i = 0; i < NUMBER_OF_WORKERS; i++)
			{
				initiate(iput, &wrs[i]);
			}
			for (i = 0; i < NUMBER_OF_MANAGERS; i++)
			{
				initiate(iput, &mgr[i]);
				iput >> s;
				NUMBER_OF_SUBORDINATE = atoi(s);
				for (int j = 0; j < NUMBER_OF_SUBORDINATE; j++)
				{
					do
					{
						iput.getline(s, 50);
						CHEK = s;
					} while (CHEK == "");
					mgr[i].set_workers(s);
				}
			}
			for (i = 0; i < NUMBER_OF_OTHERS; i++)
			{
				initiate(iput, &oth[i]);
				iput >> s;
				oth[i].set_disc(s);
			}
			iput.close();			
		}
	}	else
		if (SOURCE==2)
		{
			//инициализация соединения с сервером
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
					if (!mysql_real_connect(conn, "localhost", login, password, "TT_staff_records", NULL, NULL, 0))
					{
						cout<<"Ошибка: невозможно подключиться к серверу\n" << mysql_error(conn);
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
			}
			mysql_query(conn, "");
			//сбор данных из БД
			mysql_query(conn, "SELECT * FROM `tt_staff_records`.workers");
			if (mysql_query(conn, "SELECT * FROM `tt_staff_records`.workers"))
			{
				if (res = mysql_store_result(conn))
				{
					while (row = mysql_fetch_row(res))
					{
						initiate_from_DB(row, &wrs[q]);
						q++;
					}
					NUMBER_OF_WORKERS = q;
					mysql_free_result(res);
				}
			}
			else cout << mysql_error(conn);
			q = 0; 
			CHEK = "";
			mysql_query(conn, "SELECT * FROM tt_staff_records.managers");
			if (mysql_query(conn, "SELECT * FROM tt_staff_records.managers"))
			{
				if (res = mysql_store_result(conn))
				{
					while (row = mysql_fetch_row(res))
					{
						initiate_from_DB(row, &mgr[q]);
						mgr[q].numofwrs = atoi(row[8]);
						s = row[9];
						for (int i = 0; i < strlen(s); i++)
						{
							if (s[i] != ';') CHEK += s[i];
							else
							{
								mgr[q].numofwrs--;
								mgr[q].workers[mgr[q].numofwrs] = wrs[atoi(CHEK.c_str()) - 1].fio;
								CHEK = "";
							}
						}
						mgr[q].numofwrs = atoi(row[8]);
						q++;						
					}
					NUMBER_OF_MANAGERS = q;
					mysql_free_result(res);
				}
			}			
			else cout << mysql_error(conn);			
			q = 0;
			mysql_query(conn, "SELECT * FROM tt_staff_records.others");
			if (mysql_query(conn, "SELECT * FROM tt_staff_records.others"))
			{
				if (res = mysql_store_result(conn))
				{
					while (row = mysql_fetch_row(res))
					{
						initiate_from_DB(row, &oth[q]);
						oth[q].disc = row[8];
						q++;
					}
					NUMBER_OF_OTHERS = q;
					mysql_free_result(res);
				}
			}
			else cout << mysql_error(conn);					
			system("Pause");
		}
		else
		{
			cout << "Nain!\n";
			system("pause");
			return 0;
		}
	while (true)
	{
		system("cls");
		cout << "1:Просмотр таблиц;" << endl;
		cout << "2:Добавление записи в таблицу;" << endl;
		cout << "3:Удалить запись из таблицы;" << endl;
		cout << "4:Изменить тип сотрудника;" << endl;
		cout << "5:Привязать сотрудника к менеджеру;" << endl;
		cout << "6:Сортировка;" << endl;
		cout << "7:Выход." << endl;
		CHEK[0] = _getch();
		switch (CHEK[0])
		{
		case '1':
		{
			system("cls");
			cout << "1:Таблица Работники;" << endl;
			cout << "2:Таблица Менеджеры;" << endl;
			cout << "3:Таблица Другие сотрудники;" << endl;
			cout << "4:Отмена." << endl;
			cout << endl << "Выберите вариант вывода: ";
			cin >> i;
			switch (i)
			{
			case 1:
			{
				print(wrs, NUMBER_OF_WORKERS);
				break;
			}
			case 2:
			{
				print(mgr, NUMBER_OF_MANAGERS);
				break;
			}
			case 3:
			{
				print(oth, NUMBER_OF_OTHERS);
				break;
			}
			case 4:
			{
				break;
			}
			}
			break;
		}
		case '2':
		{
			system("cls");
			cout << "1:Помощь(Только для файла);" << endl;
			cout << "2:Добавить в Таблицу работников;" << endl;
			cout << "3:Добавить в Таблицу менеджеров;" << endl;
			cout << "4:Добавить в Таблицу других сотрудников" << endl;
			cout << "5:Отмена;" << endl;
			cin >> i;
			switch (i)
			{
			case 1:
			{
				system("cls");
				cout << "!!!Структура файла!!!" << endl << "Количество добавляемых записей" << endl << "ФИО" << endl << "День рождения" << endl << "Месяц" << endl << "Год" << endl << "День трудоустройства" << endl << "Месяц" << endl << "Год" << endl << "Количество людей в подчинении(Только для менеджеров)" << endl << "ФИО каждого сотрудника в полдчинении в отдельной строке(Количество должно совпадать с предыдущим параметром. Только для менеджеров)" << endl << "Описание сотрудника(Только для других сотрудников)\n" << endl;
				system("pause");
				break;
			}
			case 2:
			{
				system("cls");
				cout << "Добавить в таблицу Работники;" << endl;
				cout << "Введите путь к файлу: " << endl;
				cin >> CHEK;
				iput.open(CHEK, ios::in);
				if (!iput.is_open())
				{
					cout << "Файл не может быть открыт!\n";
					system("pause");
					break;
				}
				else
				{
					iput >> p;
					q = atoi(p);
					for (i = NUMBER_OF_WORKERS; i < NUMBER_OF_WORKERS + q; i++)
					{
						initiate(iput, &wrs[i]);
						if (SOURCE == 2) {
							CHEK = "INSERT INTO tt_staff_records.workers (`fio`,`bday`,`bmonth`,`byear`,`wday`,`wmonth`,`wyear`) VALUES('" + wrs[i].fio +"',"+ to_string(wrs[i].bday) + "," + to_string(wrs[i].bmonth) + "," + to_string(wrs[i].byear) + "," + to_string(wrs[i].wday) + "," + to_string(wrs[i].wmonth) + "," + to_string(wrs[i].wyear)+");";
							if (mysql_query(conn, CHEK.c_str())) 
								cout << endl << mysql_errno(conn);
						}
					}
					NUMBER_OF_WORKERS += q;
					iput.close();					
					save(iput, wrs, mgr, oth, NUMBER_OF_WORKERS, NUMBER_OF_MANAGERS, NUMBER_OF_OTHERS);
					cout << "Запись добавлена.\n";
					system("pause");
				}
				break;
			}
			case 3:
			{
				system("cls");
				cout << "Добавить в таблицу Менеджеры;" << endl;
				cout << "Введите путь к файлу: " << endl;
				cin >> CHEK;
				iput.open(CHEK, ios::in);
				if (!iput.is_open())
				{
					cout << "Файл не может быть открыт!\n";
					system("pause");
					break;
				}
				else
				{
					iput >> p;
					q = atoi(p);
					for (i = NUMBER_OF_MANAGERS; i < NUMBER_OF_MANAGERS + q; i++)
					{
						initiate(iput, &mgr[i]);
						iput >> s;
						NUMBER_OF_SUBORDINATE = atoi(s);
						for (int j = 0; j < NUMBER_OF_SUBORDINATE; j++)
						{
							do
							{
								iput.getline(s, 50);
								CHEK = s;
							} while (CHEK == "");
							mgr[i].set_workers(s);
						}
						if (SOURCE == 2)
						{
							CHEK = "INSERT INTO tt_staff_records.managers (`fio`,`bday`,`bmonth`,`byear`,`wday`,`wmonth`,`wyear`,`nomofwrs`,`workers`) VALUES('" + mgr[i].fio + "'," + to_string(mgr[i].bday) + "," + to_string(mgr[i].bmonth) + "," + to_string(mgr[i].byear) + "," + to_string(mgr[i].wday) + "," + to_string(mgr[i].wmonth) + "," + to_string(mgr[i].wyear) + "," + to_string(mgr[i].numofwrs)+ ",'";
							for (int n = 0; n < mgr[i].numofwrs; n++)
							{
								for (int m = 0; m < NUMBER_OF_WORKERS; m++)
								{
									if (wrs[m].fio == mgr[i].workers[n])
										CHEK += to_string(m+1)+";";
								}
							}
							if (mgr[i].numofwrs == 0) CHEK += "";
							CHEK += "');";
							if (mysql_query(conn, CHEK.c_str()))
								cout << endl << mysql_errno(conn);
						}
					}
					NUMBER_OF_MANAGERS += q;
					iput.close();
					save(iput, wrs, mgr, oth, NUMBER_OF_WORKERS, NUMBER_OF_MANAGERS, NUMBER_OF_OTHERS);
					cout << "Запись добавлена.\n";
					system("pause");
				}
				break;
			}
			case 4:
			{
				system("cls");
				cout << "Добавить в таблицу Другие сотрудники;" << endl;
				cout << "Введите путь к файлу: " << endl;
				cin >> CHEK;
				iput.open(CHEK, ios::in);
				if (!iput.is_open())
				{
					cout << "Файл не может быть открыт!\n";
					system("pause");
					break;
				}
				else
				{
					iput >> p;
					q = atoi(p);
					for (i = NUMBER_OF_OTHERS; i < NUMBER_OF_OTHERS + q; i++)
					{
						initiate(iput, &oth[i]);
						iput >> s;
						oth[i].set_disc(s);
						if (SOURCE == 2)
						{
							CHEK = "INSERT INTO tt_staff_records.others (`fio`,`bday`,`bmonth`,`byear`,`wday`,`wmonth`,`wyear`,`discription`) VALUES('" + oth[i].fio + "'," + to_string(oth[i].bday) + "," + to_string(oth[i].bmonth) + "," + to_string(oth[i].byear) + "," + to_string(oth[i].wday) + "," + to_string(oth[i].wmonth) + "," + to_string(oth[i].wyear) + ",'";
							CHEK += oth[i].disc+"');";
							if (mysql_query(conn, CHEK.c_str()))
								cout << endl << mysql_errno(conn);
						}
					}
					NUMBER_OF_OTHERS += q;
					iput.close();
					save(iput, wrs, mgr, oth, NUMBER_OF_WORKERS, NUMBER_OF_MANAGERS, NUMBER_OF_OTHERS);
					cout << "Запись добавлена.\n";
					system("pause");
				}
				break;
			}
			case 5:
			{
				break;
			}
			}
			break;
		}
		case '3':
		{
			system("cls");
			cout << "Удалить из:" << endl;
			cout << "1:Таблица Работники;" << endl;
			cout << "2:Таблица Менеджеры;" << endl;
			cout << "3:Таблица Другие сотрудники;" << endl;
			cout << "4:Отмена." << endl;
			cin >> i;
			switch (i)
			{
			case 1:
			{
				if (del(wrs, NUMBER_OF_WORKERS,conn,SOURCE))
				{
					save(iput, wrs, mgr, oth, NUMBER_OF_WORKERS, NUMBER_OF_MANAGERS, NUMBER_OF_OTHERS);
					cout << "Запись удалена.\n\n";
					system("pause");
				}
				break;
			}
			case 2:
			{
				if (del(mgr, NUMBER_OF_MANAGERS, conn, SOURCE))
				{
					save(iput, wrs, mgr, oth, NUMBER_OF_WORKERS, NUMBER_OF_MANAGERS, NUMBER_OF_OTHERS);
					cout << "Запись удалена.\n\n";
					system("pause");
				}
				break;
			}
			case 3:
			{
				if (del(oth, NUMBER_OF_OTHERS, conn, SOURCE))
				{
					save(iput, wrs, mgr, oth, NUMBER_OF_WORKERS, NUMBER_OF_MANAGERS, NUMBER_OF_OTHERS);
					cout << "Запись удалена.\n\n";
					system("pause");
				}
				break;
			}
			case 4: break;
			default:
				break;
			}
			break;
		}
		case '4':
		{
			system("cls");
			cout << "Укажите тип сотрудника:\n";
			cout << "1:Работник:\n";
			cout << "2:Менеджер:\n";
			cout << "3:Другие:\n";
			cout << "4:Отмена.\n";
			cin >> i;
			switch (i)
			{
			case 1:
			{
				i = search(wrs, NUMBER_OF_WORKERS);
				if (i != -1)
				{
					system("cls");
					cout << "Укажите новый тип сотрудника:\n";
					cout << "1:Менеджер:\n";
					cout << "2:Другие:\n";
					cout << "3:Отмена:\n";
					cin >> CHEK[0];
					if (CHEK[0] == '1')
					{
						move(mgr, wrs, NUMBER_OF_MANAGERS, i);
						for (int j = i; j < NUMBER_OF_WORKERS; j++) { wrs[j] = wrs[j + 1]; }
						NUMBER_OF_WORKERS--;
						mgr[NUMBER_OF_MANAGERS].numofwrs = 0;
						system("cls");
						for (int i = 0; i < NUMBER_OF_WORKERS; i++)
						{
							cout << i << ":";
							wrs[i].show(i);
						}
						while (true)
						{
							cout << "\nУкажите сотрудника в подчинени: ";
							cin >> i;
							mgr[NUMBER_OF_MANAGERS].numofwrs++;
							mgr[NUMBER_OF_MANAGERS].workers[mgr[NUMBER_OF_MANAGERS].numofwrs - 1] = wrs[i].fio;
							cout << "\nСотрудник добавлен в подчинение. Продолжить?" << "\n1:Да" << "\n2:Нет\n";
							cin >> CHEK[0];
							if (CHEK[0] != '1')
								if (CHEK[0] == '2') break;
								else break;
						}
						NUMBER_OF_MANAGERS++;
						save(iput, wrs, mgr, oth, NUMBER_OF_WORKERS, NUMBER_OF_MANAGERS, NUMBER_OF_OTHERS);
					}
					if (CHEK[0] == '2')
					{
						move(oth, wrs, NUMBER_OF_OTHERS, i);
						for (int j = i; j < NUMBER_OF_WORKERS; j++) { wrs[j] = wrs[j + 1]; }
						NUMBER_OF_WORKERS--;
						cout << "\nУкажите описание сотрудника: ";
						cin >> oth[NUMBER_OF_OTHERS].disc;
						NUMBER_OF_OTHERS++;
						save(iput, wrs, mgr, oth, NUMBER_OF_WORKERS, NUMBER_OF_MANAGERS, NUMBER_OF_OTHERS);
					}
				}
				break;
			}
			case 2:
			{
				i = search(mgr, NUMBER_OF_MANAGERS);
				if (i != -1)
				{
					system("cls");
					cout << "Укажите новый тип сотрудника:\n";
					cout << "1:Работник:\n";
					cout << "2:Другие:\n";
					cout << "3:Отмена:\n";
					cin >> CHEK[0];
					if (CHEK[0] == '1')
					{
						move(wrs, mgr, NUMBER_OF_WORKERS, i);
						for (int j = i; j < NUMBER_OF_MANAGERS; j++) { mgr[j] = mgr[j + 1]; }
						NUMBER_OF_MANAGERS--;
						NUMBER_OF_WORKERS++;
						save(iput, wrs, mgr, oth, NUMBER_OF_WORKERS, NUMBER_OF_MANAGERS, NUMBER_OF_OTHERS);
					}
					if (CHEK[0] == '2')
					{
						move(oth, mgr, NUMBER_OF_OTHERS, i);
						for (int j = i; j < NUMBER_OF_MANAGERS; j++) { mgr[j] = mgr[j + 1]; }
						NUMBER_OF_MANAGERS--;
						cout << "\nУкажите описание сотрудника: ";
						cin >> oth[NUMBER_OF_OTHERS].disc;
						NUMBER_OF_OTHERS++;
						save(iput, wrs, mgr, oth, NUMBER_OF_WORKERS, NUMBER_OF_MANAGERS, NUMBER_OF_OTHERS);
					}
				}
				break;
			}
			case 3:
			{
				i = search(oth, NUMBER_OF_OTHERS);
				if (i != -1)
				{
					system("cls");
					cout << "Укажите новый тип сотрудника:\n";
					cout << "1:Работник:\n";
					cout << "2:Менеджер:\n";
					cout << "3:Отмена:\n";
					cin >> CHEK[0];
					if (CHEK[0] == '1')
					{
						move(wrs, oth, NUMBER_OF_WORKERS, i);
						for (int j = i; j < NUMBER_OF_OTHERS; j++) { oth[j] = oth[j + 1]; }
						NUMBER_OF_OTHERS--;
						NUMBER_OF_WORKERS++;
						save(iput, wrs, mgr, oth, NUMBER_OF_WORKERS, NUMBER_OF_MANAGERS, NUMBER_OF_OTHERS);
					}
					if (CHEK[0] == '2')
					{
						move(mgr, oth, NUMBER_OF_MANAGERS, i);
						for (int j = i; j < NUMBER_OF_WORKERS; j++) { oth[j] = oth[j + 1]; }
						NUMBER_OF_OTHERS--;
						mgr[NUMBER_OF_MANAGERS].numofwrs = 0;
						system("cls");
						for (int i = 0; i < NUMBER_OF_WORKERS; i++)
						{
							cout << i << ":";
							wrs[i].show(i);
						}
						while (true)
						{
							cout << "\nУкажите сотрудника в подчинени: ";
							cin >> i;
							mgr[NUMBER_OF_MANAGERS].numofwrs++;
							mgr[NUMBER_OF_MANAGERS].workers[mgr[NUMBER_OF_MANAGERS].numofwrs - 1] = wrs[i].fio;
							cout << "\nСотрудник добавлен в подчинение. Продолжить?" << "\n1:Да" << "\n2:Нет\n";
							cin >> CHEK[0];
							if (CHEK[0] != '1')
								if (CHEK[0] == '2') break;
								else break;
						}
						NUMBER_OF_MANAGERS++;
						save(iput, wrs, mgr, oth, NUMBER_OF_WORKERS, NUMBER_OF_MANAGERS, NUMBER_OF_OTHERS);
					}
				}
				break;
			}
			case 4:
			{
				break;
			}
			}
			cout << "Сотрудник переведен.\n"; //оставил тут, чтобы не оздавать доп строк. Это не правильно, да.
			system("pause");
			break;
		}
		case '5':
		{
			i = search(mgr, NUMBER_OF_MANAGERS);
			if (i != -1)
			{
				CHEK = "";
				for (int n = 0; n < mgr[i].numofwrs; n++)
				{
					for (int m = 0; m < NUMBER_OF_WORKERS; m++)
					{
						if (wrs[m].fio == mgr[i].workers[n])
							CHEK += to_string(m + 1) + ";";
					}
				}
				system("cls");
				for (int i = 0; i < NUMBER_OF_WORKERS; i++)
				{
					cout << i + 1 << ":";
					wrs[i].show(i);
				}
				while (true)
				{
					cout << "\nУкажите сотрудника в подчинени: ";
					cin >> q;
					if ((q < 1) || (q > NUMBER_OF_WORKERS))
					{
						cout << "Сотрудника с такой фамилией нет!\n\n" << endl;
						break;
					}
					else
					{	
						mgr[i].numofwrs++;
						login = "UPDATE tt_staff_records.managers SET workers='" + CHEK + ";" + to_string(q-1) + "',numofwrs="+to_string(mgr[i].numofwrs)+" WHERE idmanagers=" +to_string(q - 1) + ";";
						mysql_query(conn, login.c_str());						
						mgr[i].workers[mgr[i].numofwrs - 1] = wrs[q - 1].fio;
						cout << "\nСотрудник добавлен в подчинение. Продолжить?" << "\n1:Да" << "\n2:Нет\n";
						cin >> CHEK[0];
						if (CHEK[0] != '1')
							if (CHEK[0] == '2') break;
							else break;
					}
				}
				save(iput, wrs, mgr, oth, NUMBER_OF_WORKERS, NUMBER_OF_MANAGERS, NUMBER_OF_OTHERS);
			}
			system("pause");
			break;
		}
		case '6':
		{
			system("cls");
			cout << "Выберите таблицу для сортирвки:" << endl;
			cout << "1:Таблица Работники;" << endl;
			cout << "2:Таблица Менеджеры;" << endl;
			cout << "3:Таблица Другие сотрудники;" << endl;
			cout << "4:Отмена.\n\n" << endl;
			cin >> CHEK[0];
			switch (CHEK[0])
			{
			case '1': {	sort(wrs, NUMBER_OF_WORKERS); break; }
			case '2': { sort(mgr, NUMBER_OF_MANAGERS); break; }
			case '3': {	sort(oth, NUMBER_OF_OTHERS); break; }
			case '4': {	break; }
					  break;
			}
			save(iput, wrs, mgr, oth, NUMBER_OF_WORKERS, NUMBER_OF_MANAGERS, NUMBER_OF_OTHERS);
			break;
		}
		case '7':
		{
			exit(0);
		}
		default: break;
		}
	}
	return 0;
}
