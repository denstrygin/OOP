#include "figures.hpp"
#include "stack.hpp"
#include "allocator.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;

template <class T, class Alloc>
void remove_by_index(Stack<T, Alloc>& st, size_t idx) {
	auto it = st.begin();
	it = next(it, idx);
	st.erase(it);
}

template <class T, class Alloc>
void print(const Stack<T, Alloc>& st) {
	for_each(st.begin(), st.end(), [](const auto& i) {cout << i << "\n"; });
}

int main() {
	for (int test_count = 0;; ++test_count) {
		ifstream input("../tests/test" + to_string(test_count + 1) + ".txt");
		if (!input) {
			cout << "Программа завершила выполнение.\n";
			break;
		}

		cout << "test" << test_count + 1 << ":" << "\n\n";

		size_t count;
		size_t count_of_remove_requests;
		double target_square;
		input >> count >> count_of_remove_requests >> target_square;
		Stack<Rhombus<double>, custom_allocator<Rhombus<double>>> st;
		try {
			while (count-- > 0) {
				Rhombus<double> rb;
				if (!(input >> rb)) {
					throw runtime_error("некорректное значение");
				}
				st.push(rb);
			}
		}
		catch (exception& ex) {
			cerr << "Ошибка ввода test" << test_count + 1 << ": " << ex.what() << "\n" << endl;
			continue;
		}

		cout << "Стек:\n";
		print(st);
		cout << "Фигур с площадью, меньше чем " << target_square << ": ";
		cout << count_if(st.begin(), st.end(), [target_square](const auto& figure) {return figure.area() < target_square; }) << "\n";

		try {
			while (count_of_remove_requests-- > 0) {
				size_t idx;
				if (!(input >> idx)) {
					throw runtime_error("некорректное значение для удаления");
				}
				remove_by_index(st, idx);
			}
		}
		catch (exception& ex) {
			cerr << "Ошибка ввода test" << test_count + 1 << ": " << ex.what() << "\n" << endl;
			continue;
		}
		cout << "Стек после удалений:\n";
		print(st);
		cout << "\n";
	}
	return 0;
}