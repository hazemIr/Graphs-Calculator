#include "shell.h"
#include "exceptions.h"

using std::endl;
using std::map;
using std::pair;
using std::set;
using std::string;
using std::vector;

bool isLegalFileName(const string &input) {
    if (input.size() <= 0) {
        return false;
    }
    if (input.find_first_not_of(' ') != 0) {
        // std::cout <<"input.find_first_not_of(' ')                 " << input.find_first_not_of(' ') << endl;
        return isLegalFileName(input.substr(input.find_first_not_of(' '), string::npos));
    }
    if (input.find_last_not_of(' ') != input.size()-1) {
        return isLegalFileName(input.substr(0, input.find_last_not_of(' ')+1));
    }
    unsigned int i = 0;
    while (i < input.size()) {
        if (input.at(i) == ')' or input.at(i) == '(' or input.at(i) == ',') {
            return false;
        }
        i++;
    }
    return true;
}

string stringRemoveSpaces(const string &input) {
    if (input.length() == 0) {
        return input;
    }

    string spaceless_input = input;
    string::iterator end_pos = std::remove(spaceless_input.begin(), spaceless_input.end(), ' ');
    spaceless_input.erase(end_pos, spaceless_input.end());
    while (spaceless_input.length() > 0 && (spaceless_input.at(spaceless_input.size()-1) == '\n' || spaceless_input.at(spaceless_input.size()-1) == '\r')) {
        spaceless_input = spaceless_input.substr(0, spaceless_input.size()-1);
    }
    return spaceless_input;
}

//Works on string without spaces ONLY.
string removeParenthesis(const string& input) {
    string result = input;
    if (input.at(0) == '(' && input.at(input.size()-1) == ')') {
        result = removeParenthesis(input.substr(1, input.size()-2));
    }
    return result;
}

bool isSingleWordCommand(const string &input, const string& word) {
    string spaceless_input = stringRemoveSpaces(input);
    return ((spaceless_input == word) &&(input.find(word) != string::npos));
}

bool isFunctionCommand(const string &input, const string &function_name) {
    string spaceless_input = stringRemoveSpaces(input);
    std::size_t pos = (spaceless_input.find(function_name));
    std::size_t pos3 = (input.find(function_name));
    std::size_t pos2 = (spaceless_input.find_last_of(')'));
    if ((pos == string::npos) || (pos3 == string::npos)) {
        return false;
    }
    if (pos > 0) {
        return false;
    }
    if (pos2 != spaceless_input.size() - 1) {
        return false;
    }
    string expression = input.substr(input.find_first_not_of(' ')+function_name.length(), string::npos);
    if (expression.at(expression.find_first_not_of(' ')) != '(' or expression.at(expression.find_last_not_of(' ')) != ')') {
        return false;
    }
    return true;
}

string graphNameFromCommand(const string& str, int function_name_length)
{
    string expression = str.substr(str.find_first_of('('), str.find_last_of(')') - str.find_first_of('(') + 1);
    return expression;
}

set<string> parseStringToVertices(const string& str, const std::size_t vertices_string_finish) {
    set<string> new_vertices;
    if (stringRemoveSpaces(str) == "{}" or stringRemoveSpaces(str) == "{|}") {
        return new_vertices;
    }
    string vertices_in_a_row;
    if (str.find_last_of('|') == string::npos) {
        vertices_in_a_row = str.substr(str.find_first_not_of(' ')+1, str.find_last_not_of(' ')-str.find_first_not_of(' ')-1);
    }
    else {
        vertices_in_a_row = str.substr(str.find_first_not_of(' ')+1, vertices_string_finish-str.find_first_not_of(' '));
    }
    if (vertices_in_a_row.at(vertices_in_a_row.find_last_not_of(' ')) == ',') { //more than one vertex, must have ',' separating
        throw(IllegalVertexName(vertices_in_a_row));
    }
    while (vertices_in_a_row.size() > 0) {
        if (isLegalVertexName(vertices_in_a_row)) { //only one vertex
            if (new_vertices.find(stringRemoveSpaces(vertices_in_a_row)) != new_vertices.end()) {
                throw(TwoOrMoreVertices());
            }
            new_vertices.insert(stringRemoveSpaces(vertices_in_a_row));
            return new_vertices;
        }
        else if (vertices_in_a_row.find_first_of(',') == string::npos) { //more than one vertex, must have ',' separating
            throw(IllegalVertexName(vertices_in_a_row));
        }
        string current_vertex = vertices_in_a_row.substr(vertices_in_a_row.find_first_not_of(' '), vertices_in_a_row.find_first_of(',')-vertices_in_a_row.find_first_not_of(' '));
        if (!isLegalVertexName(current_vertex)) {
            throw (IllegalVertexName(current_vertex));
        }
        if (new_vertices.find(vertices_in_a_row) != new_vertices.end()) {
            throw(TwoOrMoreVertices());
        }
        new_vertices.insert(stringRemoveSpaces(current_vertex));
        vertices_in_a_row.erase(0, vertices_in_a_row.find_first_of(',')+1);
    }
    return new_vertices;
}

//receive string in XXX,YYY fomat
void singleEdgeVerticesValidator(const string& edge_name_without_parenthesis) {
    if (edge_name_without_parenthesis.find_first_of(',') != edge_name_without_parenthesis.find_last_of(',')) {
        throw(IllegalEdgeName(edge_name_without_parenthesis));
    }
    if (edge_name_without_parenthesis.length() <= 2 || edge_name_without_parenthesis == "<,>") {
        throw(IllegalEdgeName());
    }
    return;
}

set<pair<string, string> > parseStringToEdges(const string &input, set<string>& new_vertices, int vertices_string_finish) {
    set<pair<string, string> > new_edges;

    string edges_in_a_row = input.substr(vertices_string_finish+2, string::npos);
    if (edges_in_a_row.at(edges_in_a_row.find_last_not_of(' ')) != '}') {
        throw(UndefinedExpression(input));
    }
    edges_in_a_row = edges_in_a_row.substr(0, edges_in_a_row.find_last_not_of(' '));
    edges_in_a_row.erase(edges_in_a_row.find_last_not_of(' ')+1, string::npos);
    //remove '}' from end of string. remains with " < XXX  , YYY > , <XXX,YYY>  ,   <  [XXX; V]  ,  YYY  >   " string
    while (edges_in_a_row.size() > 0) {
        string current_pair_to_check = edges_in_a_row.substr(edges_in_a_row.find_first_of('<')+1, edges_in_a_row.find_first_of('>')-edges_in_a_row.find_first_of('<')-1);
        if (current_pair_to_check.find_first_of(',') != current_pair_to_check.find_last_of(',')) {
            throw(IllegalEdgeName(current_pair_to_check));
        }
        string source_vertex = current_pair_to_check.substr(current_pair_to_check.find_first_not_of(' '), current_pair_to_check.find_first_of(',')-current_pair_to_check.find_first_not_of(' '));
        string destination_vertex = current_pair_to_check.substr(current_pair_to_check.find_first_of(',') + 1, string::npos);


        if (!isLegalVertexName(source_vertex)) {
            throw(IllegalVertexName(source_vertex));
        }
        if (!isLegalVertexName(destination_vertex)) {
            throw(IllegalVertexName(destination_vertex));
        }
        source_vertex = stringRemoveSpaces(source_vertex);
        destination_vertex = stringRemoveSpaces(destination_vertex);
        if (new_vertices.count(source_vertex) == 0 or new_vertices.count(destination_vertex) == 0) {
            throw(EdgeDoesntExist());
        }
        if (source_vertex == destination_vertex) {
            throw(Loop());
        }
        pair<string, string> pair_to_add(source_vertex, destination_vertex);
        if (new_edges.count(pair_to_add) > 0) {
            throw(DuplicateEdges());
        }
        new_edges.insert(pair_to_add);
        edges_in_a_row.erase(0, edges_in_a_row.find_first_of('>')+1);

        if (edges_in_a_row.find_first_not_of(' ') == string::npos) {
            return new_edges;
        }

        if (edges_in_a_row.at(edges_in_a_row.find_first_not_of(' ')) != ',') {
            throw(UndefinedExpression(input));
        }
    }
    return new_edges;
}

void splittingValidator(const string& no_spaces_input, const char& delimiter) {
    if (no_spaces_input.find(delimiter) == string::npos) {
        throw(UnrecognizedCommand());
    }
    else if (no_spaces_input.find_first_of(delimiter) != no_spaces_input.find_last_of(delimiter)) {
        throw(UnrecognizedCommand());
    }
    else if (delimiter == *no_spaces_input.rbegin()) {
        throw(MissingRightHandSide());
    }
}

void leftSideValidator(const string& input, const string& original_lhs, const string& spaceless_lhs) {
    if (spaceless_lhs != removeParenthesis(spaceless_lhs)) {
        throw(LeftHandSide(spaceless_lhs));
    }
    if (input.find(spaceless_lhs) == string::npos) {
        throw(IllegalGraphName(original_lhs));
    }
}

pair<string, string> splitStringToCommand(const string& input) {
    string spaceless_input = stringRemoveSpaces(input);
    char delimiter = '=';

    string left_hand_side;
    string right_hand_side;

    splittingValidator(spaceless_input, delimiter);

    string lhs_str = input.substr(0, input.find(delimiter));
    left_hand_side = spaceless_input.substr(0, spaceless_input.find(delimiter));

    leftSideValidator(input, lhs_str, left_hand_side);

    right_hand_side = spaceless_input.substr(spaceless_input.find(delimiter)+1, spaceless_input.size());
    pair<string, string> full_command(left_hand_side, right_hand_side);
    return full_command;
}

std::size_t findLastOuterBinaryOp(const string& input) {
    int opening_parenthesis = 0;
    int closing_parenthesis = 0;
    int i;
    for (i = input.size()-1; i>=0; i--) {
        opening_parenthesis = (input.at(i) == '(') ? opening_parenthesis + 1 : opening_parenthesis;
        closing_parenthesis = (input.at(i) == ')') ? closing_parenthesis + 1 : closing_parenthesis;
        if (input.at(i) == '+' or input.at(i) == '^' or input.at(i) == '-' or input.at(i) == '*') {
            if (closing_parenthesis == opening_parenthesis) {
                return i; //G1+(G2*G3)+G4
            }
        }
    }
    return i;
}

Shell::Shell(bool is_shell, std::istream& is, std::ostream& os) :
    is(is), os(os), calc(), is_shell(is_shell) {}

void Shell::parseCommand(const string& str) {
    string spaceless_str = stringRemoveSpaces(str);
    if (spaceless_str.length() == 0) {
        if (is_shell) {
            os << "";
        }
        return;
    }
    if (isFunctionCommand(str, "print")) {
        printGraph(str);
        return;
    }
    if (isFunctionCommand(str, "delete")) {
        deleteGraph(str);
        return;
    }
    if (isFunctionCommand(str, "save")) {
        pair<string, string> saveExpression = parseSaveCommand(str);
        saveGraph(saveExpression.first, saveExpression.second);
        return;
    }
    if (isSingleWordCommand(str, "who")) {
        who();
        return;
    }
    if (isSingleWordCommand(str, "reset")) {
        reset();
        return;
    }
    if (isSingleWordCommand(str, "quit")) {
        reset();
        throw(quitException());
    }
    isAssignmentCommand(str);
    return;
}

void Shell::run() {
    string name;

    while (std::getline(is, name)) {
        try {
            if (name.empty()) {
                continue;
            }
            if (is_shell) {
                std::cout << "Gcalc> ";
            }
            parseCommand(name);
        }
        catch (const quitException& e) {
            return;
        }
        catch (const MatamProjectException& e) {
            os << e.what() << '\n';
        }
        catch (const std::exception& e) {
            std::cerr << e.what() << endl;
            return;
        }
    }
    if (is_shell) {
        std::cout << "Gcalc> ";
    }
}

void Shell::printGraph(const string &input) const {
    string expression = graphNameFromCommand(input, 5);
    os << parseGraphExpression(expression);
}

void Shell::deleteGraph(const string &str) {
    string graph_to_delete = graphNameFromCommand(str, 6);

    graph_to_delete = graph_to_delete.substr(graph_to_delete.find_first_not_of(' ')+1, graph_to_delete.find_last_not_of(' ')-graph_to_delete.find_first_not_of(' ')-1);
    graph_to_delete = graph_to_delete.substr(graph_to_delete.find_first_not_of(' '), graph_to_delete.find_last_not_of(' ')-graph_to_delete.find_first_not_of(' ')+1);

    if (!isLegalGraphName(graph_to_delete)) {
        throw(CantDeleteExpression(graph_to_delete));
    }
    if (calc.getGraphs().count(graph_to_delete) > 0) {
        calc.getGraphs().erase(graph_to_delete);
    }
    else if (calc.getGraphs().count(graph_to_delete) == 0) {
        throw(NoSuchGraph(graph_to_delete));
    }
}

void Shell::who() const {
    if (this->calc.getGraphs().size() > 0) {
        for (pair<string, Graph> calculator_iterator : this->calc.getGraphs()) {
            os << calculator_iterator.first << endl; //Do not delete this!!!
        }
    }
}

void Shell::reset() {
    this->calc.getGraphs().clear();
}

Graph Shell::parseGraphExpression(const string& str) const {
    std::size_t binary_operation_index = findLastOuterBinaryOp(str);

    if (binary_operation_index != string::npos) {
        return parseBinaryOperation(\
            parseGraphExpression(str.substr(0, binary_operation_index)), \
            parseGraphExpression(str.substr(binary_operation_index + 1, string::npos)), \
            str[binary_operation_index]\
        );
    }
    std::size_t first_nospace_index = str.find_first_not_of(' ');

    if (first_nospace_index == string::npos) {
        throw(UndefinedExpression(str));
    }

    if (str[first_nospace_index] == '!') {
        return !parseGraphExpression(str.substr(first_nospace_index + 1, string::npos));
    }
    std::size_t last_nospace_index = str.find_last_not_of(' ');
    string nospace_substr = str.substr(first_nospace_index, last_nospace_index - first_nospace_index + 1);

    if (str[first_nospace_index] == '(') {
        if ((last_nospace_index == string::npos) || (str[last_nospace_index] != ')')) {
            throw(UndefinedExpression(str));
        }
        return parseGraphExpression(nospace_substr.substr(1, nospace_substr.length() - 2));
    }
    else if (calc.getGraphs().count(nospace_substr) > 0) {
        return calc.getGraphs().at(nospace_substr);
    }
    else if (str[first_nospace_index] == '{') {
        return parseGraphDeclaration(str);
    }
    else if (isLegalGraphName(nospace_substr)) {
        is_shell ? throw(UndefinedIdentifier(nospace_substr)) : throw(NoSuchGraph());
        //trinary operator because MTM tests different between shell and batch.
    }
    else if (isFunctionCommand(nospace_substr, "load")) {
        return parseGraphFromFile(nospace_substr);
    }
    else {
        throw(UndefinedExpression(nospace_substr));
    }
}

Graph Shell::parseGraphFromFile(const string &str) const {
    string file_name = graphNameFromCommand(str, 4);
    string clean_file_name = file_name.substr(1, file_name.size()-2);

    if (!isLegalFileName(clean_file_name)) {
        throw(IllegalFileName(clean_file_name));
    }
    clean_file_name = clean_file_name.substr(clean_file_name.find_first_not_of(' '), clean_file_name.find_last_not_of(' ')-clean_file_name.find_first_not_of(' ')+1);

    Graph result;
    std::ifstream infile(clean_file_name, std::ios_base::binary);
    if (infile.fail()) {
        throw(FileNotFound(clean_file_name));

    }
    if (infile.eof()) {
        throw(EmptyFile(clean_file_name));

    }
    unsigned int import_vertices;
    infile.read((char*)&import_vertices, sizeof(int));

    unsigned int import_edges;
    infile.read((char*)&import_edges, sizeof(int));

    if (import_edges == 0 and import_vertices == 0) {
        return result;
    }
    for (unsigned int i = 0; i< import_vertices; i++) {
        unsigned int vertex_length;
        infile.read((char*)&vertex_length, sizeof(unsigned int));

        char* vertex_name = new char[vertex_length+1];
        if (!vertex_name) {
            throw(std::bad_alloc());
        }

        infile.read(vertex_name, vertex_length*sizeof(char));
        vertex_name[vertex_length] = '\0';
        if (result.vertexInGraph(vertex_name)) {
            delete[] vertex_name;
            throw (TwoOrMoreVertices());
        }
        result.insertVertex(vertex_name);
        delete[] vertex_name;
    }

    for (unsigned int j = 0; j< import_edges; j++) {
        unsigned int source_vertex_length;
        infile.read((char*)&source_vertex_length, sizeof(unsigned int));

        char* source_vertex_name = new char[source_vertex_length+1];
        infile.read(source_vertex_name, source_vertex_length*sizeof(char));
        if (!source_vertex_name) {
            throw(std::bad_alloc());
        }

        unsigned int destination_vertex_length;
        infile.read((char*)&destination_vertex_length, sizeof(unsigned int));

        char* destination_vertex_name = new char[destination_vertex_length+1];
        infile.read(destination_vertex_name, destination_vertex_length*sizeof(char));
        if (!destination_vertex_name) {
            delete[] source_vertex_name;
            throw(std::bad_alloc());
        }
        source_vertex_name[source_vertex_length] = '\0';
        destination_vertex_name[destination_vertex_length] = '\0';

        if (!result.vertexInGraph(source_vertex_name) or !result.vertexInGraph(destination_vertex_name)) {
            delete[] source_vertex_name;
            delete[] destination_vertex_name;
            throw (NoSuchGraph());
        }
        if (result.edgeInGraph(source_vertex_name, destination_vertex_name)) {
            delete[] source_vertex_name;
            delete[] destination_vertex_name;
            throw(DuplicateEdges());
        }
        result.insertEdge(source_vertex_name, destination_vertex_name);
        delete[] source_vertex_name;
        delete[] destination_vertex_name;
    }
    return result;
}

//receive string in a "save(GRAPH (OR GRAPH EXPRESSION), file_name) format"
pair<string, string> parseSaveCommand(const string& str) {
    string expression = str.substr(str.find_first_of('e') + 1, str.size());
    if (expression.at(expression.find_first_not_of(' ')) != '(' || expression.at(expression.find_last_not_of(' ')) != ')') {
        throw(UndefinedExpression(expression));
    }
    char delimiter = ',';
    string graph_expression = expression.substr(expression.find_first_not_of(' ')+1, expression.find_last_of(delimiter)-expression.find_first_not_of(' ')-1);
    string file_name = expression.substr(expression.find_last_of(delimiter)+1, expression.find_last_not_of(' ')-expression.find_last_of(delimiter)-1);

    pair<string, string> result(graph_expression, file_name);
    return result;
}

void Shell::saveGraph(const string &str, const string& file_name) {
    if (!isLegalFileName(file_name)) {
        throw (IllegalFileName(file_name));
    }
    string clean_file_name = file_name.substr(file_name.find_first_not_of(' '), file_name.find_last_not_of(' ') - file_name.find_first_not_of(' ')+1);
    Graph result = parseGraphExpression(str);

    std::ofstream outfile(clean_file_name, std::ios_base::binary);
    result.exportGraphBinary(outfile);
    outfile.close();
    return;

}

Graph Shell::parseGraphDeclaration(const string& str) const {
    if (str.at(str.find_last_not_of(' ')) != '}' or str.at(str.find_first_not_of(' ')) != '{') {
        throw(UndefinedExpression(str));
    }
    if (str.find_first_of('|') != str.find_last_of('|')) { //More than one '|' character
        throw(UndefinedExpression(str));
    }

    set<string> new_vertices;
    set<pair<string, string> > new_edges;

    if (str.find('|') == string::npos) { // no-edges
        new_vertices = parseStringToVertices(str, str.size()-2);
    }
    else if (str.find('|') == str.size()-2) { //no edges but with | sign
        new_vertices = parseStringToVertices(str, str.find('|')-1);
    }
    else {
        if (str.find('|')-1 == 0 and stringRemoveSpaces(str) != "{|}") {
            throw(UndefinedExpression(str));
        }
        new_vertices = parseStringToVertices(str, str.find('|')-1);
        new_edges = parseStringToEdges(str, new_vertices, str.find('|')-1);
        if (new_vertices.size() == 1 and new_edges.size() > 0) {
            throw(UndefinedExpression(str));
        }
    }
    Graph result(new_vertices, new_edges);
    return result;
}

void Shell::isAssignmentCommand(const string &input) {
    pair<string, string> full_command = splitStringToCommand(input);
    string left_hand_side = full_command.first;
    if (!isLegalGraphName(left_hand_side)) {
        throw(IllegalGraphName(left_hand_side));
    }

    string right_hand_side = full_command.second;
    Graph result;
    result = this->parseGraphExpression(right_hand_side);
    this->calc.insert(left_hand_side, result);
}