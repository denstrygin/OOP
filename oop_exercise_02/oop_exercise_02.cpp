//Стрыги Д.Д. М8О-306Б Лабораторная работа №2 вариант13

/*Создать класс Long для работы с целыми беззнаковыми числами из 64 бит. Число должно быть 
представлено двумя полями unsigned int. Должны быть реализованы арифметические операции, 
присутствующие в С++, и сравнения.
Операции необходимо реализовать в виде перегрузки операторов. 
Необходимо реализовать пользовательский литерал для работы с константами типа Long*/

#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <typeinfo>

using namespace std;

class Long {
	uint32_t left_;
	uint32_t right_;
public:
	// Конструкторы класса
	Long() {
		left_ = 0;
		right_ = 0;
	}

	Long(const uint32_t &left, const uint32_t &right) {
		left_ = left;
		right_ = right;
	}

	// Функции класса
	uint64_t toLong() const {                                    // Приведение к беззнаковому числу из 64 битов
		uint64_t res = left_;
		res <<= 32;
		res |= right_;
		return res;
	}
};

Long longToLong(const uint64_t &a) {                             // Приведение беззнакового числа из 64 битов к типу Long
	uint32_t l = a >> 32;
	uint64_t mask = 1ull << 32;
	mask = ~mask;
	uint32_t r = (uint32_t)(a & mask);
	return {l, r};
}

// Пользовательский литерал
Long operator "" _myL(unsigned long long val) {
	return longToLong(val);
}

// Перегрузка операторов
Long operator + (const Long &lvtp, const Long &rgtp) {
	uint64_t res = lvtp.toLong() + rgtp.toLong();
	return longToLong(res);
}

Long operator - (const Long &lvtp, const Long &rgtp) {
	uint64_t res = lvtp.toLong() - rgtp.toLong();
	return longToLong(res);
}

Long operator * (const Long &lvtp, const Long &rgtp) {
	uint64_t res = lvtp.toLong() * rgtp.toLong();
	return longToLong(res);
}

Long operator / (const Long &lvtp, const Long &rgtp) {
	uint64_t res = lvtp.toLong() / rgtp.toLong();
	return longToLong(res);
}

Long operator / (const uint64_t &lvtp, const Long &rgtp) {
	uint64_t res = lvtp / rgtp.toLong();
	return longToLong(res);
}

bool operator < (const Long &lvtp, const Long &rgtp) {
	return lvtp.toLong() < rgtp.toLong();
}

bool operator > (const Long &lvtp, const Long &rgtp) {
	return rgtp < lvtp;
}

bool operator <= (const Long &lvtp, const Long &rgtp) {
	return !(lvtp > rgtp);
}

bool operator >= (const Long &lvtp, const Long &rgtp) {
	return !(lvtp < rgtp);
}

bool operator == (const Long &lvtp, const Long &rgtp) {
	return !(lvtp < rgtp) && !(lvtp > rgtp);
}

bool operator != (const Long &lvtp, const Long &rgtp) {
	return !(lvtp == rgtp);
}

istream& operator >> (istream &input, Long &l) {
	uint64_t val;
	input >> val;
	l = longToLong(val);
	return input;
}

ostream& operator << (ostream &output, const Long &l) {
	output << l.toLong();
	return output;
}

int main() {

	// Пример пользовательских литералов
	Long a = 1'000'000'000_myL;
	Long b = 0_myL;
	int n = 0;

    while (true) {
        ifstream input("../tests/test" + to_string(n + 1) + ".txt");
        if (!input)
            break;
        cout << "test " << n + 1 << ":" << "\n";
        while (true) {
            if (input >> a >> b) {
                cout << "A = " << a << ", B = " << b << "\n";
                cout << "A + B = " << a + b << "\n";
                cout << "A - B = " << a - b << "\n";
                cout << "A * B = " << a * b << "\n";
                cout << "A / B = ";
                try {
                    cout << a / b << "\n";
                }
                catch (exception& ex) {
                    cout << ex.what() << endl;
                }
                cout << "1 / A = ";
                try {
                    cout << 1 / a << "\n";
                }
                catch (exception& ex) {
                    cout << ex.what() << endl;
                }
                cout << endl;
            }
            else {
                if (!input.eof()) {
                    cout << "Ошибка ввода\n\n";
                }
                break;
            }
        }
        ++n;
    }
}