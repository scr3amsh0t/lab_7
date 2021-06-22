#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <list>
#include <algorithm>

using namespace std;

#define MAX_LENGTH 4096
#define PATH "db.txt"

class Fio { // класс ‘»ќ

private: // приватные переменные
	char *surname;
	char *name;
	char *patronymic;

public:
	Fio() { // конструктор по умолчанию
		set_surname((char *) "I");
		set_name((char *) "I");
		set_patronymic((char *) "I");
	}

	Fio(const Fio &fio) { // конструктор копировани€
		set_surname(fio.surname);
		set_name(fio.name);
		set_patronymic(fio.patronymic);
	}

	Fio &operator=(const Fio &fio) { // оператор присваивани€
		set_surname(fio.surname);
		set_name(fio.name);
		set_patronymic(fio.patronymic);
		return *this;
	}

	// сеттеры и геттеры
	void set_surname(char *surname) {
		this->surname = new char[strlen(surname) + 1];
		strcpy_s(this->surname, strlen(surname) + 1, surname);
	}

	void set_name(char *name) {
		this->name = new char[strlen(name) + 1];
		strcpy_s(this->name, strlen(name) + 1, name);
	}

	void set_patronymic(char *patronymic) {
		this->patronymic = new char[strlen(patronymic) + 1];
		strcpy_s(this->patronymic, strlen(patronymic) + 1, patronymic);
	}

	char *get_surname() {
		return surname;
	}

	char *get_name() {
		return name;
	}

	char *get_patronymic() {
		return patronymic;
	}

	// перегруженный оператор побитового сдвига (вывод)
	friend ostream &operator<<(ostream &out, const Fio &fio);

	friend Fio enter_fio(); // дружественна€ (поэтому определение потом) функци€ ввода с клавы ‘»ќ
};

class Bank {
private:
	int id;
	float sum;
	Fio client;

public:
	static int count; // статический счетчик

	Bank() { // конструктор по умолчанию
		id = 1;
		sum = 100;
	}

	Bank(const Bank &bank) { // конструктор копировани€
		set_client(bank.client);
		set_sum(bank.sum);
		set_id(bank.id);
	}

	Bank &operator=(const Bank &bank) { // оператор присваивани€
		set_client(bank.client);
		set_sum(bank.sum);
		set_id(bank.id);
		return *this;
	}

	// геттеры и сеттеры
	Fio get_client() {
		return client;
	}

	void set_client(Fio client) {
		this->client = client;
	}

	int get_id() {
		return id;
	}

	void set_id(int id) {
		this->id = id;
	}

	float get_sum() {
		return sum;
	}

	void set_sum(float sum) {
		this->sum = sum;
	}

	// перегруженна€ операци€ сложени€ Bank с типом float
	Bank &operator+(float amount) {
		this->sum += amount;
		return *this;
	}

	// перегруженный побитовый сдвиг (вывод)
	friend ostream &operator<<(ostream &out, const Bank &bank);

	friend Bank enter_bank(); // функци€ ввода счета с клавиатуры

	friend bool find_by_id(list<Bank> arr, Bank &bank, int id); // объ€вление поиска по номеру счета

	friend void sort_by_sum(list<Bank>& arr); // объ€вление сортировки по балансу на счете

	friend void save(list<Bank> arr); // объ€вление функции сохранени€

	friend list<Bank> readStorages(); // объ€вление фукнции чтени€ из файла

	friend void print_banks(list<Bank> arr); // объ€вление функции вывода листа аккаунтов
};

void print_banks(list<Bank> arr) { // вывод листа определенного размера
	for (std::list<Bank>::iterator it = arr.begin(); it != arr.end(); it++) {

		cout << *it;

		if (it != --arr.end()) {
			cout << endl << endl;
		}
	}
}

ostream &operator<<(ostream &out, const Fio &fio) { // вывод фио
	out << "Person: " << fio.surname << " " << fio.name << " " << fio.patronymic;
	return out;
}

ostream &operator<<(ostream &out, const Bank &bank) { // вывод счета
	out << "Client - " << bank.client << endl <<
		"Account ID - " << bank.id << endl << "Account sum - " << bank.sum;
	return out;
}

Fio enter_fio() { // ввод фио с клавы
	Fio fio;

	char *buff = new char[MAX_LENGTH];

	cout << "Enter surname: ";
	cin.getline(buff, MAX_LENGTH);
	fio.set_surname(buff);

	cout << "Enter name: ";
	cin.getline(buff, MAX_LENGTH);
	fio.set_name(buff);

	cout << "Enter patronymic: ";
	cin.getline(buff, MAX_LENGTH);
	fio.set_patronymic(buff);

	delete[] buff;
	return fio;
}

Bank enter_bank() { // ввод счета с клавы
	Bank bank;

	bank.id = Bank::count + 1;

	cout << "Enter client" << endl;
	bank.client = enter_fio();

	cout << "Enter sum: ";
	cin >> bank.sum;

	return bank;
}

bool find_by_id(list<Bank> arr, Bank &bank, int id) { // поиск по номеру
	for (list<Bank>::iterator it = arr.begin(); it != arr.end(); it++) {
		if (it->id == id) {
			bank = *it;
			return true;
		}
	}
	return false;
}

bool comp(Bank f, Bank s) {
	return f.get_sum() > s.get_sum();
}

void sort_by_sum(list<Bank>& list) { // сортировка вставкой по балансу
	list.sort(comp);
}

void save(list<Bank> arr) { // сохранение листа в файл с названием "db.txt"
	remove(PATH);
	ofstream file(PATH);

	file << Bank::count << endl;
	for (list<Bank>::iterator it = arr.begin(); it != arr.end(); it++) {
		Bank t = *it; // surname;name;patr;id;sum
		file << t.client.get_surname() << ';' << t.client.get_name() << ';' << t.client.get_patronymic() << ';'
			<< t.id << ';' << t.sum << ';' << endl;
	}
	file.close();
}

std::list<Bank> readStorages() { // чтение из файла
	list<Bank> arr = list<Bank>();

	ifstream file(PATH);
	if (!file) {
		cout << "File isn't open" << endl;
		return arr;
	}

	file >> Bank::count;

	// surname;name;patr;id;sum
	for (int i = 0; i < Bank::count; ++i) {
		if (file.eof()) {
			Bank::count = i;
			cout << "Something went wrong";
			return arr;
		}
		Bank t;
		Fio fio;

		file.ignore();

		char *buff = new char[MAX_LENGTH + 1];

		file.getline(buff, MAX_LENGTH, ';'); // читаем surname
		fio.set_surname(buff);

		file.getline(buff, MAX_LENGTH, ';'); // читаем name
		fio.set_name(buff);

		file.getline(buff, MAX_LENGTH, ';'); // читаем patronymic
		fio.set_patronymic(buff);

		int temp_int;
		file >> temp_int; // читаем номер счета
		t.set_id(temp_int);
		file.ignore();

		float sum;
		file >> sum; // читаем сумму на счете

		t.set_sum(sum);
		t.set_client(fio);

		arr.push_back(t);

		delete[] buff;
	}

	file.close();
	return arr;
}

int menu() { // опции выбора в меню
	int opt;
	cout << "1. Enter new bank account" << endl;
	cout << "2. Enter new bank account at positon" << endl;
	cout << "3. Delete bank account at start" << endl;
	cout << "4. Print all the bank accounts" << endl;
	cout << "5. Sort by decreasing sum" << endl;
	cout << "6. Search by ID" << endl;
	cout << "7. Exit and save" << endl << endl;

	do {
		cout << "Enter option: ";
		cin >> opt;
		if (opt >= 1 && opt <= 7) {
			break;
		}
		cout << "Wrong option, try again" << endl;
	} while (true);
	cin.ignore();

	return opt;
}

int Bank::count = 0; // инициализаци€ статической переменной
int main() {
	int opt;
	int am;
	int temp;
	Bank bank;
	list<Bank> arr = readStorages();

	Bank banker;
	cout << "Was" << endl << banker << endl << endl;

	banker = banker + 205; // проверка работоспособности перегрузки операции сложени€

	cout << "Became" << endl << banker << endl << endl;

	while (true) {
		opt = menu();
		cout << endl;
		switch (opt) {
		case 1: { // ввод нового счета с клавы
			arr.push_back(enter_bank());
			++Bank::count;
			cout << endl << endl;
			break;
		}
		case 2: { // добавить по позиции
			list<Bank>::iterator iter = arr.begin();
			int ind;
			cout << "Enter position: ";
			cin >> ind;
			cin.ignore();
			for (int i = 0; i < ind; i++) {
				iter++;
			}
			arr.insert(iter, enter_bank());
			++Bank::count;
			cout << endl << endl;
			break;
		}
		case 3: { // удалить спереди
			arr.pop_front();
			--Bank::count;
			cout << endl << endl;
			break;
		}
		case 4: { // напечатать все объекты
			print_banks(arr);
			cout << endl << endl;
			break;
		}
		case 5: { // сортировка по сумме (от большего к меньшему, по уменьшению)
			sort_by_sum(arr);
			print_banks(arr);
			cout << endl << endl;
			break;
		}
		case 6: { // поиск по индексу
			cout << "Enter ind: ";
			cin >> temp;
			if (find_by_id(arr, bank, temp)) {
				cout << "Has found an account!" << endl << endl << bank << endl;
			}
			else {
				cout << "Nothing has found!" << endl;
			}
			cout << endl << endl;
			break;
		}
		case 7: { // конец и сохранение
			save(arr);
			return 0;
		}
		default: {
			break;
		}
		}
	}
}