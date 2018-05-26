// ТТ_staff_records.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <fstream>
#include "iostream"
#include "string"
#include "stdlib.h"

using namespace std;

class worker
{
private:
	string fio;
	short int bday;
	short int bmonth;
	short int byear;
	short int wday;
	short int wmonth;
	short int wyear;

public:

	string get_fio() { return this->fio; }
	string get_birth_date() { return to_string(bday) + '.' + to_string(bmonth) + '.' + to_string(byear); }
	string get_work_date() { return to_string(wday) + '.' + to_string(wmonth) + '.' + to_string(wyear); }

	void set_fio(char* fio) { this->fio = fio; }
	void set_bday(short int bday) { this->bday = bday; }
	void set_bmonth(short int bmonth) { this->bmonth = bmonth; }
	void set_byear(short int byear) { this->byear = byear; }
	void set_wday(short int wday) { this->wday = wday; }
	void set_wmonth(short int wmonth) { this->wmonth = wmonth; }
	void set_wyear(short int wyear) { this->wyear = wyear; }
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

	virtual void show()
	{
		cout.width(30);
		cout << left << get_fio();
		cout.width(13);
		cout<< left << get_birth_date() << get_work_date()<<endl;
	}	
};

class manager: public worker
{
private:
	short int numofwrs=0;
	string workers[100];

public:
	manager():worker()
	{
		this->workers[0] = "";
		this->numofwrs=0;
	}
	void set_workers(string s) 
	{
		this->workers[this->numofwrs] = s;
		this->numofwrs ++; 
	}

	short int get_numbor_of_workes() {		return this->numofwrs;	}

	void get_list_of_workers()
	{
		for (int i = 0; i < numofwrs; i++)	{	cout << workers[i]<<endl;	}
	}

	void show()
	{
		cout.width(30);
		cout << left << get_fio();
		cout.width(13);
		cout << left << get_birth_date();
		cout.width(13);
		cout<< get_work_date() ;		
		for (int i = 0; i < numofwrs; i++)	{ cout << right << workers[i] << endl; cout.width(79);	}
	}	
};

class other : public worker
{
private:
	string disc;

public:

	other() :worker()	{	this->disc = "";	}
	
	void set_disc(string disc) { this->disc = disc; }
	void show()			
	{
		cout.width(30);
		cout << left << get_fio();
		cout.width(13);
		cout << left << get_birth_date();
		cout.width(13);
		cout << get_work_date();
		cout << disc << endl;	
	}
};

int main()
{
	setlocale(LC_ALL, "rus");	
	ifstream iput;	
	iput.open("iput.txt");
	
	if (!iput.is_open()) // если файл не открыт
		cout << "Файл не может быть открыт!\n"; // сообщить об этом
	else
	{
		int NUMBER_OF_WORKERS, NUMBER_OF_MANAGERS, NUMBER_OF_OTHERS, NUMBER_OF_SUBORDINATE;
		const int LENGTH = 100;
		char p[100];
		char *s, *s1;
		int i = 0;
		worker wrs[LENGTH];
		manager mgr[LENGTH];
		other oth[LENGTH];

		system("cls");
		iput >> p;
		NUMBER_OF_WORKERS = atoi(p);
		iput >> p;
		NUMBER_OF_MANAGERS = atoi(p);
		iput >> p;
		NUMBER_OF_OTHERS = atoi(p);

		s = p; s1 = p;
		for (i = 0; i < NUMBER_OF_WORKERS; i++)
		{
			iput.getline(s, 50);
			iput.getline(s, 50);
			wrs[i].set_fio(s);
			iput >> s;
			wrs[i].set_bday(atoi(s));
			iput >> s;
			wrs[i].set_bmonth(atoi(s));
			iput >> s;
			wrs[i].set_byear(atoi(s));
			iput >> s;
			wrs[i].set_wday(atoi(s));
			iput >> s;
			wrs[i].set_wmonth(atoi(s));
			iput >> s;
			wrs[i].set_wyear(atoi(s));			
		}
		for (i = 0; i < NUMBER_OF_MANAGERS; i++)
		{
			iput.getline(s, 50); //вечная для меня проблема, строка просто игнорируется, приходится дублировать. На разных компуктерах может и сработать ¯\_(ツ)_/¯
			iput.getline(s, 50);
			mgr[i].set_fio(s);
			iput >> s;
			mgr[i].set_bday(atoi(s));
			iput >> s;
			mgr[i].set_bmonth(atoi(s));
			iput >> s;
			mgr[i].set_byear(atoi(s));
			iput >> s;
			mgr[i].set_wday(atoi(s));
			iput >> s;
			mgr[i].set_wmonth(atoi(s));
			iput >> s;
			mgr[i].set_wyear(atoi(s));
			iput >> s;
			NUMBER_OF_SUBORDINATE = atoi(s);
			for (int j = 0; j < NUMBER_OF_SUBORDINATE; j++)
			{
				iput.getline(s, 50);
				mgr[i].set_workers(s);
			}
		}
		for (i = 0; i < NUMBER_OF_OTHERS; i++)
		{
			iput.getline(s, 50);
			iput.getline(s, 50);
			oth[i].set_fio(s);
			iput >> s;
			oth[i].set_bday(atoi(s));
			iput >> s;
			oth[i].set_bmonth(atoi(s));
			iput >> s;
			oth[i].set_byear(atoi(s));
			iput >> s;
			oth[i].set_wday(atoi(s));
			iput >> s;
			oth[i].set_wmonth(atoi(s));
			iput >> s;
			oth[i].set_wyear(atoi(s));
			iput >> s;
			oth[i].set_disc(s);
		}
		iput.close();
		for (i = 0; i < NUMBER_OF_WORKERS; i++)	{ wrs[i].show(); }	
		cout.width(60);
		cout << right << endl<< "Список менеджеров" << endl << endl;
		for (i = 0; i < NUMBER_OF_MANAGERS; i++) { mgr[i].show(); }
		cout.width(60);
		cout << right << endl << "Список других работников" << endl << endl;
		for (i = 0; i < NUMBER_OF_OTHERS; i++) { oth[i].show(); }
		system("pause");
		return 0;
	}
}

