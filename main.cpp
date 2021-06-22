#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <list>
#include <algorithm>

using namespace std;

#define MAX_LENGTH 4096
#define PATH "db.txt"

class Fio { // ����� ���

private: // ��������� ����������
	char *surname;
	char *name;
	char *patronymic;

public:
	Fio() { // ����������� �� ���������
		set_surname((char *) "I");
		set_name((char *) "I");
		set_patronymic((char *) "I");
	}

	Fio(const Fio &fio) { // ����������� �����������
		set_surname(fio.surname);
		set_name(fio.name);
		set_patronymic(fio.patronymic);
	}

	Fio &operator=(const Fio &fio) { // �������� ������������
		set_surname(fio.surname);
		set_name(fio.name);
		set_patronymic(fio.patronymic);
		return *this;
	}

	// ������� � �������
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

	// ������������� �������� ���������� ������ (�����)
	friend ostream &operator<<(ostream &out, const Fio &fio);

	friend Fio enter_fio(); // ������������� (������� ����������� �����) ������� ����� � ����� ���
};

class Bank {
private:
	int id;
	float sum;
	Fio client;

public:
	static int count; // ����������� �������

	Bank() { // ����������� �� ���������
		id = 1;
		sum = 100;
	}

	Bank(const Bank &bank) { // ����������� �����������
		set_client(bank.client);
		set_sum(bank.sum);
		set_id(bank.id);
	}

	Bank &operator=(const Bank &bank) { // �������� ������������
		set_client(bank.client);
		set_sum(bank.sum);
		set_id(bank.id);
		return *this;
	}

	// ������� � �������
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

	// ������������� �������� �������� Bank � ����� float
	Bank &operator+(float amount) {
		this->sum += amount;
		return *this;
	}

	// ������������� ��������� ����� (�����)
	friend ostream &operator<<(ostream &out, const Bank &bank);

	friend Bank enter_bank(); // ������� ����� ����� � ����������

	friend bool find_by_id(list<Bank> arr, Bank &bank, int id); // ���������� ������ �� ������ �����

	friend void sort_by_sum(list<Bank>& arr); // ���������� ���������� �� ������� �� �����

	friend void save(list<Bank> arr); // ���������� ������� ����������

	friend list<Bank> readStorages(); // ���������� ������� ������ �� �����

	friend void print_banks(list<Bank> arr); // ���������� ������� ������ ����� ���������
};

void print_banks(list<Bank> arr) { // ����� ����� ������������� �������
	for (std::list<Bank>::iterator it = arr.begin(); it != arr.end(); it++) {

		cout << *it;

		if (it != --arr.end()) {
			cout << endl << endl;
		}
	}
}

ostream &operator<<(ostream &out, const Fio &fio) { // ����� ���
	out << "Person: " << fio.surname << " " << fio.name << " " << fio.patronymic;
	return out;
}

ostream &operator<<(ostream &out, const Bank &bank) { // ����� �����
	out << "Client - " << bank.client << endl <<
		"Account ID - " << bank.id << endl << "Account sum - " << bank.sum;
	return out;
}

Fio enter_fio() { // ���� ��� � �����
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

Bank enter_bank() { // ���� ����� � �����
	Bank bank;

	bank.id = Bank::count + 1;

	cout << "Enter client" << endl;
	bank.client = enter_fio();

	cout << "Enter sum: ";
	cin >> bank.sum;

	return bank;
}

bool find_by_id(list<Bank> arr, Bank &bank, int id) { // ����� �� ������
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

void sort_by_sum(list<Bank>& list) { // ���������� �������� �� �������
	list.sort(comp);
}

void save(list<Bank> arr) { // ���������� ����� � ���� � ��������� "db.txt"
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

std::list<Bank> readStorages() { // ������ �� �����
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

		file.getline(buff, MAX_LENGTH, ';'); // ������ surname
		fio.set_surname(buff);

		file.getline(buff, MAX_LENGTH, ';'); // ������ name
		fio.set_name(buff);

		file.getline(buff, MAX_LENGTH, ';'); // ������ patronymic
		fio.set_patronymic(buff);

		int temp_int;
		file >> temp_int; // ������ ����� �����
		t.set_id(temp_int);
		file.ignore();

		float sum;
		file >> sum; // ������ ����� �� �����

		t.set_sum(sum);
		t.set_client(fio);

		arr.push_back(t);

		delete[] buff;
	}

	file.close();
	return arr;
}

int menu() { // ����� ������ � ����
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

int Bank::count = 0; // ������������� ����������� ����������
int main() {
	int opt;
	int am;
	int temp;
	Bank bank;
	list<Bank> arr = readStorages();

	Bank banker;
	cout << "Was" << endl << banker << endl << endl;

	banker = banker + 205; // �������� ����������������� ���������� �������� ��������

	cout << "Became" << endl << banker << endl << endl;

	while (true) {
		opt = menu();
		cout << endl;
		switch (opt) {
		case 1: { // ���� ������ ����� � �����
			arr.push_back(enter_bank());
			++Bank::count;
			cout << endl << endl;
			break;
		}
		case 2: { // �������� �� �������
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
		case 3: { // ������� �������
			arr.pop_front();
			--Bank::count;
			cout << endl << endl;
			break;
		}
		case 4: { // ���������� ��� �������
			print_banks(arr);
			cout << endl << endl;
			break;
		}
		case 5: { // ���������� �� ����� (�� �������� � ��������, �� ����������)
			sort_by_sum(arr);
			print_banks(arr);
			cout << endl << endl;
			break;
		}
		case 6: { // ����� �� �������
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
		case 7: { // ����� � ����������
			save(arr);
			return 0;
		}
		default: {
			break;
		}
		}
	}
}