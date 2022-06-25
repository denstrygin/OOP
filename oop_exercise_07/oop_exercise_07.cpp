#include <iostream>
#include <fstream>
#include <exception>
#include <list>
#include <vector>
#include <string>
#include <utility>
#include <memory>

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
		throw  runtime_error("Некорректный тип фигуры");
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

enum class ActionType {
	NONE,
	ADD,
	REMOVE
};

struct Action {
	ActionType type = ActionType::NONE;
	shared_ptr<Figure> figure_ptr;
};

class Document {
public:
	Document() = default;

	void Export(const string& filename) const {
		ofstream out(filename);
		for (const auto& ptr : data_) {
			out << *ptr << "\n";
		}
	}

	void Import(const string& filename) {
		data_.clear();
		last_action_.type = ActionType::NONE;

		ifstream in(filename);
		int type_int;
		while (in >> type_int) {
			FigureType type = static_cast<FigureType>(type_int);
			shared_ptr<Figure> figure_ptr;
			switch (type) {
			case FigureType::Trapeze:
				figure_ptr = make_shared<Trapeze>();
				break;
			case FigureType::Rhombus:
				figure_ptr = make_shared<Rhombus>();
				break;
			case FigureType::Pentagon:
				figure_ptr = make_shared<Pentagon>();
				break;
			default:
				throw  runtime_error("Некорректный тип фигуры");
				break;
			}
			in >> *figure_ptr;
			data_.push_back(move(figure_ptr));
		}
	}

	void Add(shared_ptr<Figure> figure_ptr) {
		last_action_.type = ActionType::ADD;
		last_action_.figure_ptr = figure_ptr;
		data_.push_back(move(figure_ptr));
	}

	void Remove(size_t idx) {
		if (idx >= data_.size()) {
			return;
		}
		last_action_.type = ActionType::REMOVE;
		auto it = next(data_.begin(), idx);
		last_action_.figure_ptr = *it;
		data_.erase(it);
	}

	void Print() const {
		for (const auto& ptr : data_) {
			ptr->Print();
		}
		cout << "\n";
	}

	void Undo() {
		switch (last_action_.type) {
		case ActionType::NONE:
			return;
		case ActionType::ADD:
			data_.pop_back();
			last_action_.type = ActionType::REMOVE;
			break;
		case ActionType::REMOVE:
			data_.push_back(last_action_.figure_ptr);
			last_action_.type = ActionType::ADD;
			break;
		default:
			throw  runtime_error("Некорректный тип фигуры");
			break;
		}
	}

private:
	list<shared_ptr<Figure>> data_;
	Action last_action_;
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
				if (cmd == "Import") {
					string filename;
					if (!(input >> filename)) {
						InputError();
					}
					doc.Import(filename);
				}
				else if (cmd == "Export") {
					string filename;
					if (!(input >> filename)) {
						InputError();
					}
					doc.Export(filename);
				}
				else if (cmd == "Trapeze") {
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
				else if (cmd == "Remove") {
					size_t idx;
					if (!(input >> idx)) {
						InputError();
					}
					doc.Remove(idx);
				}
				else if (cmd == "Print") {
					doc.Print();
				}
				else if (cmd == "Undo") {
					doc.Undo();
				}
				else {
					cout << cmd << " некорректная команда\n";
				}
			}
		}
		catch (exception& ex) {
			cerr << "Ошибка ввода test" << test_count + 1 << ": " << ex.what() << "\n" << endl;
			continue;
		}
	}
	return 0;
}