#include "calculator.h"

Calculator::Calculator()
{
}

Calculator::Calculator(const std::map<std::string, Graph>& new_map) : variables(new_map)
{
}

Calculator::Calculator(const Calculator &other)
{
    this->variables = other.variables;
}

std::map<std::string, Graph>& Calculator::getGraphs()
{
    return this->variables;
}

std::map<std::string, Graph> Calculator::getGraphs() const
{
    return this->variables;
}

void Calculator::insert(const std::string& new_graph_name, const Graph& new_graph_value)
{
    if (this->variables.count(new_graph_name) > 0)
    {
        this->variables.erase(new_graph_name);
    }
    std::pair<std::string, Graph> pair_to_add(new_graph_name, new_graph_value);
    this->variables.insert(pair_to_add);
}

void Calculator::insertGraphBySets(const std::string& new_graph_name, std::set<std::string> new_vertices, std::set<std::pair<std::string, std::string>> new_edges)
{
    if (this->variables.count(new_graph_name) > 0)
    {
        this->variables.erase(new_graph_name);
    }
    Graph new_graph(new_vertices, new_edges);
    std::pair<std::string, Graph> pair_to_add(new_graph_name, new_graph);
    this->variables.insert(pair_to_add);
}