#include <iostream>
#include <thread>
#include <fstream>
#include <exception>
#include <list>
#include <vector>
#include <string>
#include <utility>
#include <memory>
#include <future>
#include <mutex>
#include <condition_variable>

using namespace std;

struct Vertex {
	int x = 0;
	int y = 0;

	void Print() const {
		cout << "[" << x << ", " << y << "]";
	}
};

istream& operator>>(istream& input, Vertex& vertex) {
	input >> vertex.x >> vertex.y;
	return input;
}

ostream& operator<<(ostream& output, const Vertex& vertex) {
	output << vertex.x << " " << vertex.y;
	return output;
}

enum class FigureType {
	Trapeze,
	Rhombus,
	Pentagon
};

string TypeToString(FigureType type) {
	switch (type) {
	case FigureType::Trapeze:
		return "Trapeze";
	case FigureType::Rhombus:
		return "Rhombus";
	case FigureType::Pentagon:
		return "Pentagon";
	default:
		throw  runtime_error("Undefined figure type");
	}
}

class Figure {
public:
	Figure() = default;
	Figure(vector<Vertex> vec) : stats_(vec) {}

	void Print() const {
		cout << TypeToString(GetType()) << ": (";
		bool is_first = true;
		for (const Vertex& v : stats_) {
			if (!is_first) {
				cout << ", ";
			}
			is_first = false;
			v.Print();
		}
		cout << ")\n";
	}

	virtual FigureType GetType() const = 0;
	virtual ~Figure() {}

	friend  istream& operator>>(istream& input, Figure& rb);
	friend  ostream& operator<<(ostream& output, const Figure& rb);

protected:
	vector<Vertex> stats_;
};

class Trapeze : public Figure {
public:
	Trapeze() : Figure(vector<Vertex>(3)) {}
	Trapeze(Vertex a, Vertex  b, Vertex  c) : Figure({ a, b, c }) {}
	virtual FigureType GetType() const override { return FigureType::Trapeze; }
};

class Rhombus : public Figure {
public:
	Rhombus() : Figure(vector<Vertex>(3)) {}
	Rhombus(Vertex a, Vertex  b, Vertex  center) : Figure({ a, b, center }) {}
	virtual FigureType GetType() const override { return FigureType::Rhombus; }
};

class Pentagon : public Figure {
public:
	Pentagon() : Figure(vector<Vertex>(2)) {}
	Pentagon(Vertex a, Vertex  center) : Figure({ a, center }) {}
	virtual FigureType GetType() const override { return FigureType::Pentagon; }
};

istream& operator>>(istream& input, Figure& figure) {
	for (Vertex& v : figure.stats_) {
		input >> v;
	}
	return input;
}

ostream& operator<<(ostream& output, const Figure& figure) {
	output << static_cast<int>(figure.GetType()) << " ";
	for (const Vertex& v : figure.stats_) {
		output << v << " ";
	}
	return output;
}

class Document {
public:
	Document() {
		fut_con_ = async([&]() {Logger(); });
	}

	~Document() {
		production_stopped_ = true;
		cv_consumption_.notify_all();
		fut_con_.get();
	}

	void Export() {
		static int i = 0;
		++i;
		ofstream out("log" + to_string(i) + ".txt");
		for (const auto& ptr : data_) {
			out << *ptr << "\n";
		}
		out << "\n";
	}

	void Add(shared_ptr<Figure> figure_ptr) {
		std::unique_lock<std::mutex> lock(m_);
		cv_production_.wait(lock, [&] { return data_.size() < critical_size_; });

		data_.push_back(move(figure_ptr));

		cv_consumption_.notify_all();
	}

	void Print() const {
		static int i = 0;
		++i;
		cout << i << ":\n";
		for (const auto& ptr : data_) {
			ptr->Print();
		}
		cout << "\n";
	}

private:
	list<shared_ptr<Figure>> data_;

	const size_t critical_size_ = 3;

	mutex m_;
	condition_variable cv_production_;
	condition_variable cv_consumption_;
	future<void> fut_con_;
	bool production_stopped_ = false;

	void Logger() {
		while (!production_stopped_) {
			std::unique_lock<std::mutex> lock(m_);
			cv_consumption_.wait(lock, [&] { return data_.size() >= critical_size_ || production_stopped_; });

			if (!data_.empty()) {
				Print();
				Export();
				data_.clear();
			}

			cv_production_.notify_all();
		}
	}
};


void InputError() {
	throw runtime_error("Неверный формат входных данных");
}

int main() {
	for (int test_count = 0;; ++test_count) {
		ifstream input("../tests/test" + to_string(test_count + 1) + ".txt");
		if (!input) {
			cout << "Программа завершила выполнение.\n";
			break;
		}

		cout << "test" << test_count + 1 << ":" << "\n\n";

		try {
			Document doc;
			string cmd;
			while (input >> cmd) {
				if (cmd == "Trapeze") {
					Vertex a, b, c;
					if (!(input >> a >> b >> c)) {
						InputError();
					}
					doc.Add(make_shared<Trapeze>(a, b, c));
				}
				else if (cmd == "Rhombus") {
					Vertex a, b, c;
					if (!(input >> a >> b >> c)) {
						InputError();
					}
					doc.Add(make_shared<Rhombus>(a, b, c));
				}
				else if (cmd == "Pentagon") {
					Vertex a, b;
					if (!(input >> a >> b)) {
						InputError();
					}
					doc.Add(make_shared<Pentagon>(a, b));
				}
				else {
					cout << cmd << " - некорректная команда\n";
				}
			}
		}
		catch (exception& ex) {
			cerr << "Ошибка ввода " << ex.what() << "\n" << endl;
			continue;
		}
	}
	return 0;
}