//Стрыгин Д.Д. М8О-306Б Лабораторная работа №1 вариант4

/*Реализовать класс FazzyNumber для работы с нечеткими числами, которые представляются тройками
чисел (x – el, x, x + er). Для чисел A = (A – al, A, A + ar) и B = (B – bl, B, B + br) арифметические операции
выполняются по следующим формулам:
- сложение A + B = (A + B – al – bl, A + B, A + B + ar + br);
- вычитание A – B = (A – B – al – bl, A – B, A – B + ar + br);
- умножение A · B = (A ´ B – B ´ al – A ´ bl + al ´ bl, A ´ B, A ´ B + B ´ al + A ´ bl + al ´ bl);
- обратное число A = (1 / (A + ar), 1 / A, 1 / (A – al)), A > 0;
- деление A / B = ((A – al) / (B + br), A / B, (A + ar) / (B – bl)), B > 0;
Считать el = er, то есть число представлено парой <x, e>.
Реализовать операции сравнения по х.*/

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

//Классы

class FazzyNumber {
    double x_;
    double e_;

public:

    //Конструктор объекта класса

    FazzyNumber(const double &x = 0, const double &e = 0) {
        x_ = x;
        e_ = e;
    }

    //Функция работы с объектами класса

    double left() const {
        return x_ - e_;
    }

    double num() const {
        return x_;
    }

    double& num() {
        return x_;
    }

    double right() const {
        return x_ + e_;
    }

    double epsilon() const {
        return e_;
    }

    double& epsilon() {
        return e_;
    }
};

//Перегрузка операторов

FazzyNumber operator + (const FazzyNumber &lftp, const FazzyNumber &rgtp) {
    return {lftp.num() + rgtp.num(), lftp.epsilon() + rgtp.epsilon()};
}//(A+B)+(a+b)

FazzyNumber operator - (const FazzyNumber &lftp, const FazzyNumber &rgtp) {
    return { lftp.num() - rgtp.num(), lftp.epsilon() + rgtp.epsilon() };
}//(A-B)+(a+b)

FazzyNumber operator * (const FazzyNumber &lftp, const FazzyNumber &rgtp) {
    return { lftp.num() * rgtp.num(), lftp.num() * rgtp.epsilon() + lftp.epsilon() * rgtp.num() + lftp.epsilon() * rgtp.epsilon() };
}//(A*B)+(A*b+B*A*a*b)

void IsPvCheck(const FazzyNumber &fazzyNumber) {
    if (fazzyNumber.left() <= 0) {
        throw runtime_error("Делитель меньше или равен 0");
    }
}

FazzyNumber operator / (const FazzyNumber &lftp, const FazzyNumber &rgtp) {
    IsPvCheck(rgtp);
    double right = (lftp.num() + lftp.epsilon()) / (rgtp.num() - rgtp.epsilon());
    double num = lftp.num() / rgtp.num();
    return { num, right - num };
}//(A+a)/(B-b) -> (A/B)+(...)

FazzyNumber operator / (const double &lftp, const FazzyNumber &rgtp) {
    IsPvCheck(rgtp);
    double right = lftp / (rgtp.num() - rgtp.epsilon());
    double num = lftp / rgtp.num();
    return { num, right - num };
}

ostream& operator << (ostream &output, const FazzyNumber &fuzzyNumber) {
    output << "(" << fuzzyNumber.left() << ", " << fuzzyNumber.num() << ", " << fuzzyNumber.right() << ")";
    return output;
}

istream& operator >> (istream &input, FazzyNumber &fuzzyNumber) {
    input >> fuzzyNumber.num() >> fuzzyNumber.epsilon();
    return input;
}

void isCorrect(const FazzyNumber &fz) {
    if (fz.epsilon() < 0) {
        throw invalid_argument("Эпсилон < 0");
    }
}

int main() {
    FazzyNumber a;
    FazzyNumber b;
    int n = 0;

    while (true) {
        ifstream input("../tests/test" + to_string(n + 1) + ".txt");
        if (!input)
            break;
        cout << "test " << n + 1 << ":" << "\n";
        while (true) {
            if (input >> a >> b) {
                try {
                    isCorrect(a);
                    isCorrect(b);
                }
                catch (exception& ex) {
                    cout << ex.what() << "\n";
                    break;
                }

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