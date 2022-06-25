//Стрыги Д.Д. М8О-306Б Лабораторная работа №3 вариант20

/*Разработать классы согласно варианту задания, классы должны наследоваться от базового класса Figure. Фигуры 
являются фигурами вращения. Все классы должны поддерживать набор общих методов:
1. Вычисление геометрического центра фигуры;
2. Вывод в стандартный поток вывода std::cout координат вершин фигуры;
3. Вычисление площади фигуры;
Создать программу, которая позволяет:
• Вводить из стандартного ввода std::cin фигуры, согласно варианту задания.
• Сохранять созданные фигуры в динамический массив std::vector<Figure*>
• Вызывать для всего массива общие функции (1-3 см. выше).Т.е. распечатывать для каждой фигуры в массиве 
геометрический центр, координаты вершин и площадь.
• Необходимо уметь вычислять общую площадь фигур в массиве.
• Удалять из массива фигуру по индексу;

20 Трапеция Ромб 5-угольник*/

#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <cmath>
#include <numeric>
#include <string>

using namespace std;

struct Point {
	double x = 0;
	double y = 0;

	Point operator + (const Point &rhs) const {
		return { x + rhs.x, y + rhs.y };
	}
	Point &operator += (const Point &rhs) {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	Point operator / (const double &rhs) const {
		return { x / rhs, y / rhs };
	}
};

istream &operator >> (istream &input, Point &point) {
	input >> point.x >> point.y;
	return input;
}

ostream &operator << (ostream &output, const Point &point) {
	output << "[" << point.x << ", " << point.y << "]";
	return output;
}

class Figure {
public:
	Figure(size_t number_of_vertexes)
		: vertexes_(number_of_vertexes) {
	}

	Point center() const {
		Point res;
		for (const auto &p : vertexes_) {
			res += p;
		}
		return res / vertexes_.size();
	}

	virtual double area() const = 0;

	friend istream& operator >> (istream &input, Figure &figure);
	friend ostream& operator << (ostream &output, const Figure &figure);

protected:
	vector<Point> vertexes_;

	const Point& getv(size_t idx) const {
		return vertexes_[idx];
	}
	Point& getv(size_t idx) {
		return vertexes_[idx];
	}
};

istream& operator >> (istream &input, Figure &figure) {
	for (auto &vertex : figure.vertexes_) {
		input >> vertex;
	}
	return input;
}

ostream& operator << (ostream &output, const Figure &figure) {
	for (const auto &vertex : figure.vertexes_) {
		output << vertex << " ";
	}
	return output;
}

class Triangle : public Figure {
public:
	Triangle() : Figure(3) {}
	Triangle(const Point &a, const Point &b, const Point &c) : Triangle() {
		vertexes_[0] = a;
		vertexes_[1] = b;
		vertexes_[2] = c;
	}

	virtual double area() const override {
		return abs(
			(getv(1).x - getv(0).x) * (getv(2).y - getv(0).y)
			- (getv(2).x - getv(0).x) * (getv(1).y - getv(0).y)
		) / 2;
	}
};

class Rhombus : public Figure {
public:
	Rhombus() : Figure(4) {}
	Rhombus(const Point &a, const Point &b, const Point &c, const Point &d) : Rhombus() {
		vertexes_[0] = a;
		vertexes_[1] = b;
		vertexes_[2] = c;
		vertexes_[3] = d;
	}

	virtual double area() const override {
		return Triangle(getv(0), getv(1), center()).area() * 4;
	}
};

class Pentagon : public Figure {
public:
	Pentagon() : Figure(5) {}
	Pentagon(const Point &a, const Point &b, const Point &c, const Point &d, const Point &e) : Pentagon() {
		vertexes_[0] = a;
		vertexes_[1] = b;
		vertexes_[2] = c;
		vertexes_[3] = d;
		vertexes_[4] = e;
	}

	virtual double area() const override {
		return Triangle(getv(0), getv(1), getv(2)).area()
			+ Triangle(getv(0), getv(2), getv(3)).area()
			+ Triangle(getv(0), getv(3), getv(4)).area();
	}
};

enum class FigureType {
	Triangle,
	Rhombus,
	Pentagon
};

void readInputForType(istream &input, vector<Figure*> &figures, int number, const FigureType &type) {
	while (number-- > 0) {
		Figure* f = nullptr;
		switch (type) {
		case FigureType::Triangle:
			f = new Triangle;
			break;
		case FigureType::Rhombus:
			f = new Rhombus;
			break;
		case FigureType::Pentagon:
			f = new Pentagon;
			break;
		default:
			return;
		}
		input >> *f;
		figures.push_back(f);
	}
}

int main() {
	int test_count = 0;
	while (true) {
		ifstream input("../tests/test" + to_string(test_count + 1) + ".txt");
		if (!input) {
			cout << "Программа завершила выполнение." << "\n";
			break;
		}

		cout << "test" << test_count + 1 << ":" << "\n\n";

		int number_of_triangles, number_of_rhombuses, number_of_pentagons;
		input >> number_of_triangles >> number_of_rhombuses >> number_of_pentagons;

		vector<Figure*> figures;
		readInputForType(input, figures, number_of_triangles, FigureType::Triangle);
		readInputForType(input, figures, number_of_rhombuses, FigureType::Rhombus);
		readInputForType(input, figures, number_of_pentagons, FigureType::Pentagon);

		double sum_area = 0;
		for (const auto& figure : figures) {
			sum_area += figure->area();
			cout << "Координаты вершин: " << *figure << "\nКоординаты центра: " << figure->center() << "\nПлощадь: " << figure->area() << "\n\n";
			delete figure;
		}

		cout << "Суммарная площадь: " << sum_area << "\n\n";

		++test_count;
	}
}