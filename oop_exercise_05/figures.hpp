#pragma once

#include <array>
#include <cmath>
#include <numeric>
#include <iostream>

#include "vertex_t.hpp"

template <class T>
class Rhombus {
private:
	vertex_t<T> a = { 0, 0 };
	vertex_t<T> b = { 0, 0 };
	vertex_t<T> center = { 0, 0 };

	bool near(double a, double b) {
		if (abs(a - b) > 0.001) {
			return true;
		}
		return false;
	}

	void check_vertexes() {
		double leg1 = distance(a, center);
		double leg2 = distance(b, center);
		double hypotenuse = distance(a, b);
		if (near(leg1 * leg1 + leg2 * leg2, hypotenuse * hypotenuse)) {
			throw std::runtime_error("incorrect vertexes");
		}
	}

public:
	Rhombus() = default;

	Rhombus(vertex_t<T> a, vertex_t<T> b, vertex_t<T> center)
		: a(a)
		, b(b)
		, center(center) {
		check_vertexes();
	}

	std::array<vertex_t<T>, 4> vertexes() const {
		vertex_t<T> c, d;
		c = reflect(b, center, a);
		d = reflect(a, c, b);
		return { a, b, c, d };
	}

	double area() const {
		double res = 0;
		auto v = vertexes();
		res += triangle_area(v[0], v[1], v[2]);
		res += triangle_area(v[0], v[2], v[3]);
		return res;
	}

	template <class K>
	friend std::istream& operator>>(std::istream& input, Rhombus<K>& rb);
	template <class K>
	friend std::ostream& operator<<(std::ostream& output, const Rhombus<K>& rb);
};

template <class T>
std::istream& operator>>(std::istream& input, Rhombus<T>& rb) {
	input >> rb.center >> rb.a >> rb.b;
	rb.check_vertexes();
	return input;
}

template <class T>
std::ostream& operator<<(std::ostream& output, const Rhombus<T>& rb) {
	for (const auto& p : rb.vertexes()) {
		output << p << " ";
	}
	return output;
}