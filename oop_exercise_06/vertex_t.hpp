#pragma once

#include <utility>
#include <iostream>

template <class T>
using vertex_t = std::pair<T, T>;

template <class T>
bool operator==(const vertex_t<T>& lhs, const vertex_t<T>& rhs) {
	return lhs.first == rhs.first && lhs.second == rhs.second;
}

template <class T>
bool operator!=(const vertex_t<T>& lhs, const vertex_t<T>& rhs) {
	return !(lhs == rhs);
}

template <class T>
vertex_t<T> operator+(const vertex_t<T>& lhs, const vertex_t<T>& rhs) {
	return { lhs.first + rhs.first, lhs.second + rhs.second };
}

template <class T>
vertex_t<T> operator-(const vertex_t<T>& lhs, const vertex_t<T>& rhs) {
	return { lhs.first - rhs.first, lhs.second - rhs.second };
}

template <class T>
vertex_t<T> operator/(const vertex_t<T>& lhs, const double& rhs) {
	return { lhs.first / rhs, lhs.second / rhs };
}

template <class T>
double distance(const vertex_t<T>& lhs, const vertex_t<T>& rhs) {
	return sqrt((lhs.first - rhs.first) * (lhs.first - rhs.first)
		+ (lhs.second - rhs.second) * (lhs.second - rhs.second));
}

template <class T>
double triangle_area(const vertex_t<T>& a, const vertex_t<T>& b, const vertex_t<T>& c) {
	return abs(
		(b.first - a.first) * (c.second - a.second)
		- (c.first - a.first) * (b.second - a.second)
	) / 2;
}

//отражение точки s относительно прямой a-b
template <class T>
vertex_t<T> reflect(const vertex_t<T>& a, const vertex_t<T>& b, const vertex_t<T>& s) {
	vertex_t<T> n;
	n.first = (a.second - b.second);
	n.second = (b.first - a.first);
	double len = sqrt(n.first * n.first + n.second * n.second);
	n.first /= len;
	n.second /= len;
	double dot2 = 2 * (n.first * s.first + n.second * s.second);
	vertex_t<T> res;
	res.first = s.first - dot2 * n.first;
	res.second = s.second - dot2 * n.second;
	return res;
}

template<class T>
std::istream& operator>>(std::istream& input, vertex_t<T>& vertex) {
	input >> vertex.first >> vertex.second;
	return input;
}

template<class T>
std::ostream& operator<<(std::ostream& output, const vertex_t<T>& vertex) {
	output << "[" << vertex.first << ", " << vertex.second << "]";
	return output;
}