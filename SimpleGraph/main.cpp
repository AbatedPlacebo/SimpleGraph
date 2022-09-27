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

	class abstractedgeiterator {

	};

	class abstractoutedgeiterator {

	};

	friend ostream& operator<<(ostream& out, const AbstractGraph& a) {
		return a.print(out);
	}
	virtual ostream& print(ostream& out) const {
		out << "Base class (please, initialise L- or M-Graph)";
		return out;
	}
	virtual void insertEdge(Edge&, int, int) = 0;
	virtual void insertVertex(int) = 0;
	virtual void deleteVertex(int) = 0;
	virtual void deleteEdge(Edge&) = 0;
};


class LGraph : public AbstractGraph {
private:
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

	void insertEdge(Edge& e, int v1, int v2) override {
		listform[v1].push_back(&e);
	}

	void insertVertex(int index) override {
		listform.insert(listform.begin() + index, list<Edge*>());
	}

	void deleteVertex(int index) override {
		listform.erase(listform.begin() + index);
	}

	void deleteEdge(Edge& e) override{
		for (int i = 0; i < listform.size(); i++) {
			for (auto it = listform[i].begin();
				it != listform[i].end(); it++) {
				if (*it == &e) {
					listform[i].erase(it);
					return;
				}
			}
		}
	}
};


class MGraph : public AbstractGraph {
private:
	vector<vector<Edge*>> matform;
public:
	
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

	void insertEdge(Edge& e, int v1, int v2) override {
		matform[v1][v2] = &e;
	}

	void insertVertex(int index) override {
		int size = matform.size();
		matform.insert(matform.begin() + index, vector<Edge*>(size, nullptr));
		size++;
		for (int i = 0; i < size; i++) {
			matform[i].insert(matform[i].begin() + index, nullptr);
		}
	}

	void deleteVertex(int index) override {
		matform.erase(matform.begin() + index);
		for (int i = 0; i < matform.size(); i++) {
			matform[i].erase(matform[i].begin() + index);
		}
	}

	void deleteEdge(Edge& e) override {
		for (int i = 0; i < matform.size(); i++) {
			for (auto it = matform[i].begin();
				it != matform[i].end(); it++) {
				if (*it == &e) {
					*it = nullptr;
					return;
				}
			}
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

	class vertex_iterator {
	private:
		vector<Vertex*> vertices;
		int index;
	public:
		vertex_iterator() {
			index = 0;
		}
		vertex_iterator(vector<Vertex*> elem, int index) {
			this->index = index;
			vertices = elem;
		}
		void operator+(int index) {
			this->index += index;
		}
		void operator--(int index) {
			this->index -= index;
		}
		Vertex* operator*() {
			return this->vertices[index];
		}
	};

	class edge_iterator {
		edge_iterator() {

		}
	};

	class outedge_iterator {
		outedge_iterator() {

		}
	};

	vertex_iterator vertexBegin() {
		return vertex_iterator(vertices,0);
	}

	edge_iterator edgeBegin() {

	}

	outedge_iterator outEdgeBegin() {

	}

	vertex_iterator vertexEnd() {
		return vertex_iterator(vertices, vertices.size());
	}

	edge_iterator edgeEnd() {

	}

	outedge_iterator outEdgeEnd() {

	}


	SimpleGraph(bool graphType, bool directed) {
		this->directed = directed;
		this->graphType = graphType;
		if (graphType == false)
			absgraph = new MGraph();
		else
			absgraph = new LGraph();
	}


	void insertVertex(Vertex& v) {
		auto it = vertices.begin();
		for (; it != vertices.end(); it++) {
			if (*it == &v)
				return;
		}
		vertices.push_back(&v);
		absgraph->insertVertex(vertices.size() - 1);
	}

	void insertEdge(Edge& e) {
		insertVertex(*e.getV1());
		insertVertex(*e.getV2());
		auto it1 = vertices.begin();
		for (; *it1 != e.getV1(); it1++);
		auto it2 = vertices.begin();
		for (; *it2 != e.getV2(); it2++);
		absgraph->insertEdge(e, it1 - vertices.begin(), it2 - vertices.begin());
		if (!directed)
			absgraph->insertEdge(e, it2 - vertices.begin(), it1 - vertices.begin());
	}

	void deleteVertex(Vertex& v) {
		auto it1 = vertices.begin();
		for (it1 = vertices.begin(); *it1 != &v; it1++);
		if (it1 != vertices.end())
			absgraph->deleteVertex(it1 - vertices.begin());
		vertices.erase(it1);
	}

	void deleteEdge(Edge& e) {
		absgraph->deleteEdge(e);
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

	SimpleGraph g = SimpleGraph(true,true);
	g.insertEdge(e1);
	g.insertEdge(e2);
	g.insertEdge(e3);
	g.insertEdge(e4);
	g.deleteVertex(v3);
	g.deleteEdge(e4);

	cout << g;
}