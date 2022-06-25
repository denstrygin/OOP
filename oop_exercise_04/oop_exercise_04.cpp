/*Разработать шаблоны классов согласно варианту задания. Параметром шаблона должен являться скалярный тип 
данных задающий тип данных для оси координат. Классы должны иметь публичные поля. Фигуры являются 
фигурами вращения. Для хранения координат фигур необходимо использовать шаблон std::pair
Создать набор шаблонов, создающих функции, реализующие:
1. Вычисление геометрического центра фигуры;
2. Вывод в стандартный поток вывода std::cout координат вершин фигуры;
3. Вычисление площади фигуры;
Параметром шаблона должен являться тип класса фигуры ( например Square<int>). Помимо самого класса 
фигуры, шаблонная функция должна уметь работать с tuple. Например, std::tuple<std::pair<int,int>, std::pair<int,int>, 
std::pair<int,int>> должен интерпретироваться как треугольник. std::tuple<std::pair<int,int>, std::pair<int,int>, 
std::pair<int,int>, std::pair<int,int>> - как квадрат. Каждый std::pair<int,int> - соответствует координатам вершины 
фигуры вращения.
Создать программу, которая позволяет:
• Вводить из стандартного ввода std::cin фигуры, согласно варианту задания (как в виде класса, так и в виде 
std::tuple).
• Вызывать для нее шаблонные функции (1-3).
При реализации шаблонных функций допускается использование вспомогательных шаблонов std::enable_if, 
std::tuple_size, std::is_same.
Вариант 36: Трапеция, Ромб, Пятиугольник*/

#include <iostream>
#include <tuple>
#include <fstream>
#include <string>

#include "vertex_t.hpp"
#include "figures.hpp"

using namespace std;

template <class T>
void print(const Trapeze<T>& tr) {
	for (const auto& p : find_trapeze_vertexes(tr)) {
		cout << p << " ";
	}
	cout << "\n";
}

template <class T>
void print(const Rhombus<T>& rb) {
	for (const auto& p : find_rhombus_vertexes(rb)) {
		cout << p << " ";
	}
	cout << "\n";
}

template <class T>
void print(const Pentagon<T>& pt) {
	for (const auto& p : find_pentagon_vertexes(pt)) {
		cout << p << " ";
	}
	cout << "\n";
}

template <class... Elems>
void print(const tuple<Elems...>& tup) {
	if (tuple_size<tuple<Elems...>>::value != 6) {
		throw runtime_error("Должно быть ровно 6 фигур");
	}
	print(get<0>(tup));
	print(get<1>(tup));
	print(get<2>(tup));
	print(get<3>(tup));
	print(get<4>(tup));
	print(get<5>(tup));
}

template <class... Elems>
double area(const tuple<Elems...>& tup) {
	if (tuple_size<tuple<Elems...>>::value != 6) {
		throw runtime_error("Должно быть ровно 6 фигур");
	}
	double res = 0;
	res += area(get<0>(tup));
	res += area(get<1>(tup));
	res += area(get<2>(tup));
	res += area(get<3>(tup));
	res += area(get<4>(tup));
	res += area(get<5>(tup));
	return res;
}

int main() {
	for (int test_count = 0;; ++test_count) {
		ifstream input("../tests/test" + to_string(test_count + 1) + ".txt");
		if (!input) {
			cout << "Программа завершила выполнение." << "\n";
			break;
		}

		cout << "test " << test_count + 1 << ":" << "\n\n";

		Trapeze<int> tr_i;
		Trapeze<double> tr_d;
		Rhombus<int> rb_i;
		Rhombus<double> rb_d;
		Pentagon<int> pt_i;
		Pentagon<double> pt_d;

		if (!(input >> tr_i >> tr_d >> rb_i >> rb_d >> pt_i >> pt_d)) {
			cerr << "Ошибка ввода на тесте " << test_count + 1 << "\n\n";
			continue;
		}

		auto tup = make_tuple(tr_i, tr_d, rb_i, rb_d, pt_i, pt_d);
		print(tup);

		cout << "Суммарная площадь: " << area(tup) << "\n\n";
	}
}