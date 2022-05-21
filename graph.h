#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include <cctype>
#include <vector>
#include <utility>
#include <map>
#include <iterator>
#include <algorithm>



typedef std::set<std::string> destinations;

class Graph
{

private:
    std::map<std::string, destinations> edges;

public:
    Graph() = default;
    Graph(std::set<std::string> vertices, std::set<std::pair<std::string, std::string> > edges_to_add);
    Graph(const Graph& graph);
    ~Graph() = default;
    Graph& operator+=(const Graph& other);
    bool operator==(const Graph& other) const;
    Graph operator+(const Graph& other) const;
    Graph operator!() const;
    Graph operator-(const Graph& other) const;
    Graph operator^(const Graph& other) const;
    Graph operator*(const Graph& other) const;
    bool vertexInGraph(const std::string& str) const;
    bool edgeInGraph(const std::string& src, const std::string& dst) const;
    int verticesSum() const;
    int edgesSum() const;
    void exportGraphBinary(std::ofstream& outfile) const;

    friend std::ostream &operator<<(std::ostream &os, const Graph &graph);
    void insertEdge(const std::string& src, const std::string& dst);
    void insertVertex(const std::string& src);

    Graph Full() const;
};

Graph* create();
void destroy(Graph* graph);
Graph* addEdge(Graph* graph, const char* src, const char* dst);
Graph* addVertex(Graph* graph, const char* vetex);
void disp(const Graph* graph);

Graph* graphUnion(Graph* graph_in1, Graph* graph_in2, Graph* graph_out);
Graph* graphIntersection(Graph* graph_in1, Graph* graph_in2, Graph* graph_out);
Graph* graphDifference(Graph* graph_in1, Graph* graph_in2, Graph* graph_out);
Graph* graphProduct(Graph* graph_in1, Graph* graph_in2, Graph* graph_out);
Graph* graphComplement(const Graph* graph_in, Graph* graph_out);
Graph* graphOperation(const Graph* graph_in1, const Graph* graph_in2, Graph* graph_out, const char op);

bool isLegalGraphName(const std::string &input);
bool isLegalVertexName(const std::string &str);
bool isLegalCharacterInVertexName(const char& chararcter);
Graph parseBinaryOperation(const Graph& left_graph, const Graph& right_graph, char op);

#endif