#include <iostream>
#include <vector>
#include <list>
#include <algorithm>

using namespace std;

class Vertex {
private:
	int data;
public:
	Vertex() {
		data = 0;
	}
	Vertex(int data) {
		this->data = data;
	}
	friend ostream& operator<<(ostream& s, Vertex& v) {
		s << v.data;
		return s;
	}
};

class Edge {
private:
	Vertex* v1;
	Vertex* v2;
	int weight;
	friend class SimpleGraph;
public:
	Edge() {
		v1 = nullptr;
		v2 = nullptr;
		weight = 1;
	}
	Edge(Vertex& v1, Vertex& v2) {
		this->v1 = &v1;
		this->v2 = &v2;
		weight = 1;
	}
	Edge(Vertex& v1, Vertex& v2, int w) {
		this->v1 = &v1;
		this->v2 = &v2;
		weight = w;
	}
	friend ostream& operator<<(ostream& s, Edge& e) {
		s << e.v1 << " - ";
		s << e.v2 << std::endl;
		return s;
	}
	int getWeight() {
		return weight;
	}
	Vertex* getV1() {
		return v1;
	}
	Vertex* getV2() {
		return v2;
	}
};


class AbstractGraph {
private:
	friend class SimpleGraph;
	friend class MGraph;
	friend class LGraph;
public:
	friend ostream& operator<<(ostream& out, const AbstractGraph& a) {
		return a.print(out);
	}
	virtual ostream& print(ostream& out) const {
		out << "Base class (please, initialise L- or M-Graph)";
		return out;
	}
	virtual void insertEdge(Edge, int, int) = 0;
	virtual void insertVertex(int) = 0;
};


class LGraph : public AbstractGraph {
	vector<list<Edge*>> listform;

public:
	ostream& print(ostream& out) const override {
		for (int i = 0; i < listform.size(); i++) {
			out << i << ": ";
			for (auto it = listform[i].begin();
				it != listform[i].end(); it++) {
				out << *((*it)->getV2()) << " ";
			}
			out << endl;
		}
		return out;
	}
	void insertEdge(Edge e, int v1, int v2) override {
		listform[v1].push_back(new Edge(e));
	}
	void insertVertex(int index) {
		listform.insert(listform.begin() + index, list<Edge*>());
	}
};


class MGraph : public AbstractGraph {
	vector<vector<Edge*>> matform;
public:

	void deleteVertex() {

	}
	
	ostream& print(ostream& out) const override {
		for (int i = 0; i < matform.size(); i++) {
			for (int j = 0; j < matform[i].size(); j++) {
				if (matform[i][j] == nullptr)
					out << 0 << " ";
				else
					out << (matform[i][j])->getWeight() << " ";
			}
			out << endl;
		}
		return out;
	}

	void insertEdge(Edge e, int v1, int v2) override {
		matform[v1][v2] = new Edge(e);
	}

	void insertVertex(int index) override {
		int size = matform.size();
		matform.insert(matform.begin() + index, vector<Edge*>(size, nullptr));
		size++;
		for (int i = 0; i < size; i++) {
			matform[i].insert(matform[i].begin() + index, nullptr);
		}
	}

};

class SimpleGraph {
private:
	AbstractGraph* absgraph;
	vector<Vertex*> vertices;
	bool graphType;
	bool directed;
public:
	SimpleGraph(bool graphType, bool directed) {
		this->directed = directed;
		this->graphType = graphType;
		if (graphType == false)
			absgraph = new MGraph();
		else
			absgraph = new LGraph();
	}

	void insertEdge(Vertex& v1, Vertex& v2, int w) {
		auto it1 = vertices.begin();
		for (; it1 != vertices.end(); it1++) {
			if (*it1 == &v1)
				break;
		}
		if (it1 == vertices.end()) {
			vertices.push_back(&v1);
			absgraph->insertVertex(vertices.size()-1);
		}
		auto it2 = vertices.begin();
		for (; it2 != vertices.end(); it2++) {
			if (*it2 == &v2)
				break;
		}
		if (it2 == vertices.end()) {
			vertices.push_back(&v2);
			absgraph->insertVertex(vertices.size()-1);
		}
		for (it1 = vertices.begin(); *it1 != &v1; it1++);
		for (it2 = vertices.begin(); *it2 != &v2; it2++);
		absgraph->insertEdge(Edge(v1,v2,w), it1 - vertices.begin(), it2 - vertices.begin());
		if (!directed)
			absgraph->insertEdge(Edge(v1, v2, w), it2 - vertices.begin(), it1 - vertices.begin());
	}
	void insertEdge(Edge& e) {
		insertEdge(*e.v1,*e.v2, e.getWeight());
	}

	friend ostream& operator<<(ostream& s, SimpleGraph& g) {
		s << *(g.absgraph);
		return s;
	}

};

int main() {
	Vertex v1 = Vertex(1);
	Vertex v2 = Vertex(2);
	Vertex v3 = Vertex(3);
	Edge e1 = Edge(v1, v2, 3);
	Edge e2 = Edge(v2, v3, 2);
	Edge e3 = Edge(v1, v3, 1);
	Edge e4 = Edge(v1, v1, 1);

	SimpleGraph g = SimpleGraph(false,true);

	g.insertEdge(e1);
	g.insertEdge(e2);
	g.insertEdge(e3);
	g.insertEdge(e4);

	cout << g;
}