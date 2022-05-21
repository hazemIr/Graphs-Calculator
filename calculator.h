#ifndef CALC_H
#define CALC_H

#include "graph.h"

class Calculator
{

private:
    std::map<std::string, Graph> variables;

public:
    Calculator();
    Calculator(const std::map<std::string, Graph>& new_map);
    Calculator(const Calculator &other);
    ~Calculator() = default;

    void insert(const std::string& new_graph_name, const Graph& new_graph_value);
    void insertGraphBySets(const std::string& lhs_graph, std::set<std::string> new_vertices, std::set<std::pair<std::string, std::string> > new_edges);

    std::map<std::string, Graph>& getGraphs();
    std::map<std::string, Graph> getGraphs() const;

};


#endif