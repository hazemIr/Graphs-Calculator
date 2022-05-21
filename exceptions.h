#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <string>
#include <iterator>
#include "graph.h"

class MatamProjectException : public std::exception {
protected:
    std::string result;
    std::string message;

public:
    virtual const char *what() const noexcept;
    MatamProjectException() : message("Error: ") {};
};

class IllegalGraphName : public MatamProjectException {
public:
    IllegalGraphName(const std::string &command) {
        result = message + "Illegal Graph Name: '" + command + "'";
    };
};

class EdgeDoesntExist : public MatamProjectException {
public:
    EdgeDoesntExist() {
        result = message + "Edge contains vertex that does not belong to the graph.";
    };
};


class NoSuchGraph : public MatamProjectException {
public:
    NoSuchGraph(const std::string &command) {
        result = message + "No such graph with name: '" + command + "'";
    };
    NoSuchGraph() {
        result = message + "Illegal name";
    };
};

class EmptyVertexName : public MatamProjectException {
public:
    EmptyVertexName() {
        result = message + "Empty Vertex Name";
    };
};

class MissingRightHandSide : public MatamProjectException {
public:
    MissingRightHandSide() {
        result = message + "Missing Right Hand Side";
    };
};

class IllegalVertexName : public MatamProjectException {
public:
    IllegalVertexName(const std::string &command) {
        if (command.length() == 0)
        {
            result = message + "Illegal Vertex Name: (empty name)";
        }
        else
        {
            result = message + "Illegal Vertex Name: '" + command + "'";
        }
    };
    IllegalVertexName(const char &command) {
        result = message + "Illegal Vertex Name, Contains: '" + command + "'";
    };
    IllegalVertexName() {
        result = message + "Illegal Vertex Name: (empty)";
    };
};

class IllegalEdgeName : public MatamProjectException {
public:
    IllegalEdgeName(const std::string &command) {
        result = message + "Illegal Edge Name: '" + command + "'";
    };
    IllegalEdgeName(const char &command) {
        result = message + "Illegal Edge Name, Contains: '" + command + "'";
    };
    IllegalEdgeName() {
        result = message + "Illegal Edge Name - Contains (empty) vertex";
    };
};

class VertexIsMissing : public MatamProjectException {
public:
    VertexIsMissing(const std::string &command) {
        result = message + "Vertex Is Missing From The Graph: '" + command + "'";
    };
};

class UnrecognizedCommand : public MatamProjectException {
public:
    UnrecognizedCommand() {
        result = message + "Unrecognized Command.";
    };
};

class UndefinedVariable : public MatamProjectException {
public:
    UndefinedVariable(const std::string &command) {
        result = message + "Undefined Variable '" + command + "'";
    };
};

class UndefinedIdentifier : public MatamProjectException {
public:
    UndefinedIdentifier(const std::string &command) {
        result = message + "Undefined Identifier - " + command;
    };
};

class UndefinedExpression : public MatamProjectException {
public:
    UndefinedExpression(const std::string &command) {
        result = message + "Undefined Expression '" + command + "'";
    };
};

class quitException : public MatamProjectException {
public:
    quitException() {};
};

class LeftHandSide : public MatamProjectException {
public:
    LeftHandSide(const std::string &command) {
        result = message + "Left hand side is an expression: " + command;
    };
};

class CantDeleteExpression : public MatamProjectException {
public:
    CantDeleteExpression(const std::string &command) {
        result = message + "Cannot Delete Expression: " + command;
    };
};

class invalidGraphDeclaration : public MatamProjectException {
public:
    invalidGraphDeclaration(const std::string &command) {
        result = message + "Invalid Graph Declaration: " + command;
    };
};

class TwoOrMoreVertices : public MatamProjectException {
public:
    TwoOrMoreVertices() {
        result = message + "Two or more vertices with the same name in this graph";
    };
};

class DuplicateEdges : public MatamProjectException {
public:
    DuplicateEdges() {
        result = message + "Two or more edges with the same name in this graph";
    };
};

class Loop : public MatamProjectException {
public:
    Loop() {
        result = message + "Trying to add loop to the graph!!";
    };
};

class EmptyCommand : public MatamProjectException {
public:
    EmptyCommand() {
        result = "";
    };
};

class IllegalFileName : public MatamProjectException {
public:
    IllegalFileName(const std::string &command) {
        result = message + "Illegal File Name: '" + command + "'";
    };
};

class FileNotFound : public MatamProjectException {
public:
    FileNotFound(const std::string &command) {
        result = message + "File Not Found: '" + command + "'";
    };
};

class FileError : public MatamProjectException {
public:
    FileError(const std::string &command) {
        result = message + "File Error: '" + command + "'";
    };
};

class EmptyFile : public MatamProjectException {
public:
    EmptyFile(const std::string &command) {
        result = message + "File is Empty: '" + command + "'";
    };
};
#endif