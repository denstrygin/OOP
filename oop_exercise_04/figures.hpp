#pragma once

#include <array>
#include <cmath>
#include <numeric>
#include <iostream>

#include "vertex_t.hpp"

template <class T>
class Trapeze {
public:
	vertex_t<T> a, b, c, d;
};

template <class T>
class Rhombus {
public:
	vertex_t<T> a, b, center;
};

template <class T>
class Pentagon {
public:
	vertex_t<T> a, center;
};

template <class T>
istream& operator>>(istream& input, Trapeze<T>& tr) {
	input >> tr.a >> tr.b >> tr.c >> tr.d;
	return input;
}

template <class T>
istream& operator>>(istream& input, Rhombus<T>& rb) {
	input >> rb.a >> rb.b >> rb.center;
	return input;
}

template <class T>
istream& operator>>(istream& input, Pentagon<T>& pt) {
	input >> pt.a >> pt.center;
	return input;
}

template <class T>
array<vertex_t<T>, 4> find_trapeze_vertexes(const Trapeze<T>& tr) {
	return { tr.a, tr.b, tr.c, tr.d };
}

template <class T>
array<vertex_t<T>, 4> find_rhombus_vertexes(const Rhombus<T>& rb) {
	vertex_t<T> c, d;
	c = reflect(rb.b, rb.center, rb.a);
	d = reflect(rb.a, c, rb.b);
	return { rb.a, rb.b, c, d };
}

constexpr double PI = 3.1415;

template <class T>
vertex_t<T> rofi_to_vertex(double ro, double fi) {
	return { ro * cos(fi),ro * sin(fi) };
}

template <class T>
array<vertex_t<T>, 5> find_pentagon_vertexes(const Pentagon<T>& pt) {
	vertex_t<T> ast, b, c, d, e;
	ast = pt.a - pt.center;
	double ro = distance(pt.a, pt.center);

	double fi = 0;
	if (ast.second >= 0) {
		if (ast.first != 0) {
			fi = atan(ast.second / ast.first);
		}
		else {
			fi = PI / 2;
		}
	}
	else {
		if (ast.first != 0) {
			fi = atan(ast.second / ast.first) + PI / 2;
		}
		else {
			fi = 3 * PI / 2;
		}
	}

	fi -= 2 * PI / 5;
	b = rofi_to_vertex<T>(ro, fi);
	fi -= 2 * PI / 5;
	c = rofi_to_vertex<T>(ro, fi);
	fi -= 2 * PI / 5;
	d = rofi_to_vertex<T>(ro, fi);
	fi -= 2 * PI / 5;
	e = rofi_to_vertex<T>(ro, fi);

	const auto& ce = pt.center;
	return { pt.a, b + ce, c + ce, d + ce, e + ce };
}

template <class T>
double area(const Trapeze<T>& tr) {
	double res = 0;
	auto v = find_trapeze_vertexes(tr);
	res += triangle_area(v[0], v[1], v[2]);
	res += triangle_area(v[0], v[2], v[3]);
	return res;
}

template <class T>
double area(const Rhombus<T>& rb) {
	double res = 0;
	auto v = find_rhombus_vertexes(rb);
	res += triangle_area(v[0], v[1], v[2]);
	res += triangle_area(v[0], v[2], v[3]);
	return res;
}

template <class T>
double area(const Pentagon<T>& pt) {
	double res = 0;
	auto v = find_pentagon_vertexes(pt);
	res += triangle_area(v[0], v[1], v[2]);
	res += triangle_area(v[0], v[2], v[3]);
	res += triangle_area(v[0], v[3], v[4]);
	return res;
}