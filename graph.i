%module graph
%include "typemaps.i"
// %include "graph.h"
%{
    #include "graph.h"
    // typedef struct olympics* Olympics;
    %}

// TODO: insert more code here.add missing functions.


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