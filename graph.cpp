#include "graph.h"

using std::endl;
using std::map;
using std::pair;
using std::set;
using std::string;
using std::vector;

Graph::Graph(set<string> vertices, set<pair<string, string> > edges_to_add)
    : edges(map<string, destinations>()) {
    for (set<string>::iterator It = vertices.begin(); It != vertices.end(); ++It) {
        destinations j;
        edges.insert(pair<string, destinations>(*It, j));
    }
    for (set<string>::iterator ver_it = vertices.begin(); ver_it != vertices.end(); ++ver_it) {
        string current_vertex = *ver_it;
        for (set<pair<string, string> >::iterator edg_it = edges_to_add.begin(); edg_it != edges_to_add.end(); ++edg_it) {
            string current_source = edg_it->first;
            string current_dst = edg_it->second;
            if (current_vertex == current_source) {
                edges.at(current_vertex).insert(current_dst);
            }
        }
    }
}

Graph::Graph(const Graph &other) : edges(map<string, destinations>()) {
    this->edges = other.edges;
}

Graph Graph::operator+(const Graph &other) const {

    Graph result(*this);
    return result += other;
}

Graph &Graph::operator+=(const Graph &other) {
    for (map<string, destinations>::const_iterator other_iterator = other.edges.begin(); other_iterator != other.edges.end(); ++other_iterator) {
        if (edges.count(other_iterator->first) > 0) {
            for (set<string>::const_iterator edge_iterator = other_iterator->second.begin(); edge_iterator != other_iterator->second.end(); ++edge_iterator) {
                edges[other_iterator->first].insert(*edge_iterator);
            }
        }
        else if (edges.count(other_iterator->first) == 0) {
            pair<string, destinations> pair_to_add(other_iterator->first, other_iterator->second);
            edges.insert(pair_to_add);
        }
    }
    return *this;
}

Graph Graph::operator*(const Graph &other) const {
    Graph result;
    set<pair<string, string> > new_vertices;
    for (map<string, destinations>::const_iterator this_iterator = this->edges.begin(); this_iterator != this->edges.end(); ++this_iterator) {
        for (map<string, destinations>::const_iterator other_iterator = other.edges.begin(); other_iterator != other.edges.end(); ++other_iterator) {
            pair<string, string> vertex_to_add(this_iterator->first, other_iterator->first);
            new_vertices.insert(vertex_to_add);
        }
    }

    for (set<pair<string, string> >::const_iterator pair_iterator = new_vertices.begin(); pair_iterator != new_vertices.end(); ++pair_iterator) {
        string new_vertex = '[' + pair_iterator->first + ';' + pair_iterator->second + ']';
        destinations edges_to_add;
        pair<string, destinations> pair_to_add(new_vertex, edges_to_add);
        result.edges.insert(pair_to_add);
        for (set<pair<string, string> >::const_iterator other_pairs_iterator = new_vertices.begin(); other_pairs_iterator != new_vertices.end(); ++other_pairs_iterator) {
            // MTM 2020 Definition
            if (pair_iterator->first == other_pairs_iterator->first || pair_iterator->second == other_pairs_iterator->second) {
                continue;
            }
            else if (this->edges.at(pair_iterator->first).count(other_pairs_iterator->first) > 0 && other.edges.at(pair_iterator->second).count(other_pairs_iterator->second) >0) {
                string new_edge = '[' + other_pairs_iterator->first + ';' + other_pairs_iterator->second + ']';
                result.edges.at(new_vertex).insert(new_edge);
                continue;
            }

        }

    }
    return result;
}

Graph Graph::operator^(const Graph &other) const {
    set<string> intersection_vertices;
    Graph result;
    for (typename map<string, destinations>::const_iterator this_iterator = edges.begin(); this_iterator != edges.end(); ++this_iterator) {
        if (other.edges.count(this_iterator->first) == 0) {
            continue;
        }
        string current_new_vertex = this_iterator->first;

        intersection_vertices.insert(current_new_vertex);

        destinations intersection_edges;

        for (destinations::const_iterator edge_it = this_iterator->second.begin(); edge_it != this_iterator->second.end(); ++edge_it) {
            if (other.edges.at(current_new_vertex).count(*edge_it) > 0) {
                intersection_edges.insert(*edge_it);
            }
        }
        pair<string, destinations> pair_to_add(current_new_vertex, intersection_edges);
        result.edges.insert(pair_to_add);
    }
    return result;
}

int Graph::verticesSum() const {
    return this->edges.size();
}

int Graph::edgesSum() const {
    int edges_counter = 0;
    for (map<string, destinations>::const_iterator res_iterator = edges.begin(); res_iterator != edges.end(); ++res_iterator) {
        edges_counter += res_iterator->second.size();
    }
    return edges_counter;
}

Graph Graph::operator-(const Graph &other) const {
    Graph result(*this);
    Graph temp(other);
    set<string> deleted_vertices;

    for (map<string, destinations>::const_iterator other_iterator = other.edges.begin(); other_iterator != other.edges.end(); ++other_iterator) {
        if (result.edges.count(other_iterator->first) > 0) {
            deleted_vertices.insert(other_iterator->first);
            result.edges.erase(other_iterator->first);
        }
    }

    for (map<string, destinations>::iterator res_iterator = result.edges.begin(); res_iterator != result.edges.end(); ++res_iterator) {
        for (destinations::iterator dst_iterator = res_iterator->second.begin(); dst_iterator != res_iterator->second.end(); ++dst_iterator) {
            if (deleted_vertices.find(*dst_iterator) != deleted_vertices.end()) {
                res_iterator->second.erase(dst_iterator);
            }
        }
    }
    return result;
}

bool Graph::operator==(const Graph &other) const {
    return (this->edges == other.edges);
}

std::ostream &operator<<(std::ostream &os, const Graph &graph) {
    for (map<string, destinations>::const_iterator this_iterator = graph.edges.begin(); this_iterator != graph.edges.end(); ++this_iterator) {
        os << this_iterator->first << endl;
    }

    os << '$' << endl;

    for (map<string, destinations>::const_iterator this_iterator = graph.edges.begin(); this_iterator != graph.edges.end(); ++this_iterator) {
        for (destinations::const_iterator edges_iterator = this_iterator->second.begin(); edges_iterator != this_iterator->second.end(); ++edges_iterator) {
            os << this_iterator->first << ' ' << *edges_iterator << endl;
        }
    }
    return os;
}

Graph Graph::Full() const {
    Graph Full_result;
    destinations all_vertices;
    for (map<string, destinations>::const_iterator this_iterator = edges.begin(); this_iterator != edges.end(); ++this_iterator) {
        all_vertices.insert(this_iterator->first);
    }
    for (map<string, destinations>::const_iterator second_iterator = edges.begin(); second_iterator != edges.end(); ++second_iterator) {
        destinations all_vertices_but_current = all_vertices;
        all_vertices_but_current.erase(second_iterator->first);
        pair<string, destinations> pair_to_add(second_iterator->first, all_vertices_but_current);
        Full_result.edges.insert(pair_to_add);
    }
    return Full_result;
}

Graph Graph::operator!() const {
    Graph Full_Graph(this->Full());
    Graph result;
    for (typename map<string, destinations>::const_iterator full_iterator = Full_Graph.edges.begin(); full_iterator != Full_Graph.edges.end(); ++full_iterator) {
        string new_vertex = full_iterator->first;
        destinations new_edges;
        for (destinations::const_iterator edges_iterator = full_iterator->second.begin(); edges_iterator != full_iterator->second.end(); ++edges_iterator) {
            if (this->edges.at(full_iterator->first).find(*edges_iterator) != this->edges.at(full_iterator->first).end()) {
                continue;
            }
            new_edges.insert(*edges_iterator);
        }
        pair<string, destinations> pair_to_add(new_vertex, new_edges);
        result.edges.insert(pair_to_add);
    }
    return result;
}

void Graph::insertEdge(const string& src, const string& dst) {
    pair<string, string> pair_to_add(src, dst);
    this->edges.at(src).insert(dst);
}

void Graph::insertVertex(const std::string& src) {
    destinations new_edges;
    pair<string, destinations> pair_to_add(src, new_edges);
    this->edges.insert(pair_to_add);
}

Graph* create() {
    Graph* graph = new(Graph);
    if (!graph) {
        std::cout << "Error: Memory allocation failed!" << endl;
        return nullptr;
    }
    return graph;
}

void destroy(Graph* graph) {
    delete(graph);
}

void disp(const Graph* graph) {
    if (!graph) {
        std::cout << "Error: Can't print nullptr!" << endl;
        return;
    }
    std::cout << *graph;
}

Graph* addEdge(Graph* graph, const char* src, const char* dst) {
    if (!graph) {
        std::cout << "Error: nullptr" << endl;
        return graph;
    }
    std::string dststr(dst);
    std::string srcstr(src);

    if (!isLegalVertexName(srcstr) or !isLegalVertexName(dststr)) {
        std::cout << "Error: Illegal Vertex Name." << endl;
        return nullptr;
    }
    if (!graph->vertexInGraph(srcstr) or !graph->vertexInGraph(dststr)) {
        std::cout << "Error: Unidentified Vertex Name." << endl;
        return nullptr;
    }
    if (graph->edgeInGraph(src, dst)) {
        std::cout << "Error: Edge Already in Graph" << endl;
        return nullptr;
    }
    if (srcstr == dststr) {
        std::cout << "Error: trying to add Loop" << endl;
        return nullptr;
    }
    graph->insertEdge(srcstr, dststr);
    return graph;
}

bool Graph::vertexInGraph(const std::string& str) const {
    return (this->edges.count(str) > 0);
}

bool Graph::edgeInGraph(const std::string& src, const std::string& dst) const {
    return (this->edges.at(src).count(dst) > 0);
}

Graph* addVertex(Graph* graph, const char* vertex) {
    if (!graph) {
        std::cout << "Error: nullptr" << endl;
        return nullptr;
    }
    if (!isLegalVertexName(vertex)) {
        std::cout << "Error: Illegal Vertex Name." << endl;
        return nullptr;
    }
    if (graph->vertexInGraph(vertex)) {
        std::cout << "Error: vertex Already in Graph" << endl;
        return nullptr;
    }
    graph->insertVertex(vertex);
    return graph;
}

Graph* graphUnion(Graph* graph_in1, Graph* graph_in2, Graph* graph_out) {
    return graphOperation(graph_in1, graph_in2, graph_out, '+');
}
Graph* graphIntersection(Graph* graph_in1, Graph* graph_in2, Graph* graph_out) {
    return graphOperation(graph_in1, graph_in2, graph_out, '^');
}
Graph* graphDifference(Graph* graph_in1, Graph* graph_in2, Graph* graph_out) {
    return graphOperation(graph_in1, graph_in2, graph_out, '-');
}
Graph* graphProduct(Graph* graph_in1, Graph* graph_in2, Graph* graph_out) {
    return graphOperation(graph_in1, graph_in2, graph_out, '*');
}

Graph* graphOperation(const Graph* graph_in1, const  Graph* graph_in2, Graph* graph_out, const char op) {
    if (!graph_in1 or !graph_in2 or !graph_out) {
        std::cout << "Error: nullptr" << endl;
        return nullptr;
    }
    Graph* graph = new(Graph);
    if (!graph) {
        std::cout << "Error: Memory allocation failed!" << endl;
        return nullptr;
    }
    *graph = parseBinaryOperation(*graph_in1, *graph_in2, op);
    *graph_out = parseBinaryOperation(*graph_in1, *graph_in2, op);
    return graph;

}

Graph* graphComplement(const Graph* graph_in, Graph* graph_out) {
    if (!graph_in or !graph_out) {
        std::cout << "Error: nullptr" << endl;
        return nullptr;
    }

    Graph* graph = new(Graph);
    if (!graph) {
        std::cout << "Error: Memory allocation failed!" << endl;
        return nullptr;
    }
    *graph = (!*graph_in);
    *graph_out = (!*graph_in);
    return graph;
}


bool isLegalGraphName(const string &input) {
    if (input.size() <= 0) {
        return false;
    }
    unsigned int i = 0;
    if (!isalpha(input.at(i++))) {
        return false;
    }
    if ((input == "who") || (input == "print") || (input == "delete") || (input == "reset") || (input == "quit") || (input == "load") || (input == "save")) {
        return false;
    }
    while (i < input.size()) {
        if (!isalnum(input.at(i))) {
            return false;
        }
        i++;
    }
    return true;
}

bool isLegalVertexName(const std::string &str) {
    if (str.find_first_not_of(' ') == string::npos) {
        return false;
    }
    string vertex_name = str.substr(str.find_first_not_of(' '), str.find_last_not_of(' ')-str.find_first_not_of(' ')+1);

    int opening_parenthesis;
    int closing_parenthesis;
    int vertex_length = vertex_name.length();
    if (vertex_length == 0) {
        return false;
    }
    for (int i = 1; i <= vertex_length; i++) {
        opening_parenthesis = 0;
        closing_parenthesis = 0;
        std::string opening_part = vertex_name.substr(0, i);
        for (unsigned int j = 0; j < opening_part.length(); j++) {
            char current = opening_part.at(j);
            if (!isLegalCharacterInVertexName(current)) {

                return false;
            }
            if (current == '[') {
                opening_parenthesis++;
            }
            if (current == ']') {
                closing_parenthesis++;
            }
            if (current == ';') {
                if (closing_parenthesis >= opening_parenthesis) {
                    return false;
                }
            }
        }
        if (closing_parenthesis > opening_parenthesis) {
            return false;
        }
    }
    return true;
}

bool isLegalCharacterInVertexName(const char& chararcter) {
    return (isalnum(chararcter) || chararcter == ';' || chararcter == '[' || chararcter == ']');
}

Graph parseBinaryOperation(const Graph& left_graph, const Graph& right_graph, char op) {
    Graph result;
    switch (op) {
    case '+':
        result = left_graph+right_graph;
        break;
    case '*':
        result = left_graph*right_graph;
        break;
    case '-':
        result = left_graph-right_graph;
        break;
    case '^':
        result = left_graph^right_graph;
        break;
    default:
        break;
    }
    return result;
}

void Graph::exportGraphBinary(std::ofstream& outfile) const {
    unsigned int export_vertices = verticesSum();
    outfile.write(reinterpret_cast<char*>(&export_vertices), sizeof(unsigned int));

    unsigned int export_edges = edgesSum();
    outfile.write(reinterpret_cast<char*>(&export_edges), sizeof(unsigned int));

    for (map<string, destinations>::const_iterator graph_it = edges.begin(); graph_it != edges.end(); ++graph_it) {
        unsigned int vertex_length = graph_it->first.size(); // length of vertex to export
        string vertex_name_string = graph_it->first; // vertex to export

        outfile.write(reinterpret_cast<char*>(&vertex_length), sizeof(unsigned int)); //write to file length of vertex
        outfile.write(vertex_name_string.c_str(), vertex_length*sizeof(char)); //write to file the vertex (minus \0)
    }
    for (map<string, destinations>::const_iterator graph_it = edges.begin(); graph_it != edges.end(); ++graph_it) {
        for (destinations::iterator edge_it = graph_it->second.begin(); edge_it != graph_it->second.end(); ++edge_it) {
            unsigned int source_length = graph_it->first.size();
            outfile.write(reinterpret_cast<char*>(&source_length), sizeof(unsigned int));
            outfile.write(graph_it->first.c_str(), source_length*sizeof(char));

            unsigned int destination_length = edge_it->size();
            outfile.write(reinterpret_cast<char*>(&destination_length), sizeof(unsigned int));
            outfile.write(edge_it->c_str(), destination_length*sizeof(char));
        }
    }
}