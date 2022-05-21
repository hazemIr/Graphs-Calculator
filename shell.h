#ifndef SHELL_H_
#define SHELL_H_

#include <iostream>
#include <fstream>
#include <iterator>
#include "calculator.h"

class Shell {
private:
    std::istream& is;
    std::ostream& os;
    Calculator calc;
    bool is_shell;

public:
    Shell(bool is_shell = true, std::istream& is = std::cin, std::ostream& os = std::cout);
    ~Shell() = default;
    void run();
    void who() const;
    void reset();
    void parseCommand(const std::string& str);

    void printGraph(const std::string& str) const;
    void deleteGraph(const std::string &str);
    void saveGraph(const std::string &str, const std::string& file_name);

    Graph parseGraphExpression(const std::string& str) const;
    Graph parseGraphFromFile(const std::string& str) const;
    Graph parseGraphDeclaration(const std::string& str) const;

    void isAssignmentCommand(const std::string &input);
};

std::string stringRemoveSpaces(const std::string &input);
std::set<std::string> parsestringToVertices(const std::string& str, const std::size_t vertices_string_finish);
std::string graphNameFromCommand(const std::string& spaceless_input, int function_name_length);
std::size_t findLastOuterBinaryOp(const std::string& input);
std::string removeParenthesis(const std::string& input);
std::set<std::pair<std::string, std::string> > parsestringToEdges(const std::string &input, std::set<std::string>& new_vertices, int vertices_string_finish);

void graphDeclarationValidator(const std::string& str);
void edgesDefinitionValidator(const std::string& str, int vertices_string_finish);
void singleEdgeVerticesValidator(const std::string& edge_name_without_parenthesis);
void verticesDefinitionValidator(const std::string& str, int vertices_string_finish);

bool isLegalFileName(const std::string &input);
bool isSingleWordCommand(const std::string &input, const std::string& word);
bool isFunctionCommand(const std::string &input, const std::string &function_name);

std::pair<std::string, std::string> splitstringToCommand(const std::string& input);
std::pair<std::string, std::string> parseSaveCommand(const std::string& str);


#endif