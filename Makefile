CXX = /usr/local/bin/g++
EXEC = gcalc
OBJS = graph.o final.o exceptions.o calculator.o shell.o
HFLS = graph.h calculator.h shell.h final.h exceptions.h
CPPFLS = graph.cpp calculator.cpp shell.cpp final.cpp exceptions.cpp
COMP_FLAG = -std=c++11  -Wall -pedantic-errors -Werror -g -fPIC
DEBUG_FLAG = -DNDEBUG

$(EXEC): $(OBJS) Makefile
	$(CXX) $(DEBUG_FLAG) $(OBJS) -o $@
	make libgraph.a
	clear

graph.o: graph.h graph.cpp
	$(CXX) -c $(COMP_FLAG) $(DEBUG_FLAG) $*.cpp

final.o: final.cpp final.h graph.h calculator.h exceptions.h shell.h
	$(CXX) -c $(COMP_FLAG) $(DEBUG_FLAG) $*.cpp

exceptions.o: exceptions.h exceptions.cpp graph.h
	$(CXX) -c $(COMP_FLAG) $(DEBUG_FLAG) $*.cpp

calculator.o: graph.h calculator.cpp calculator.h
	$(CXX) -c $(COMP_FLAG) $(DEBUG_FLAG) $*.cpp

shell.o: shell.cpp shell.h calculator.h exceptions.h graph.h
	$(CXX) -c $(COMP_FLAG) $(DEBUG_FLAG) $*.cpp

libgraph.a: $(OBJS)
	ar -rs $@ $^

tar:
	make clean
	make
	zip gcalc.zip design.pdf test_in.txt test_out.txt Makefile gcalc $(HFLS) $(CPPFLS)

final: $(HFLS) $(CPPFLS) Makefile test_in.txt test_out.txt #design.pdf
	make tar
	/home/mtm/public/1920b/project/finalCheck gcalc.zip



clean:
	rm -f $(OBJS) $(EXEC) libgraph.a

val:
	valgrind --leak-check=full --track-origins=yes --suppressions=supress ./gcalc

valpy:
	valgrind --track-origins=yes --leak-check=full -v /usr/local/bin/python3.6 /home/shir.breuer/Final/main.py > pythontemp.txt 2> pythonerrors.txt

swig: graph.i
	make libgraph.a
	swig -python -o graph_wrap.c  graph.i
	g++ -std=c++11 -DNDEBUG --pedantic-errors -Wall -Werror -I/usr/local/include/python3.6m -fPIC -o _graph.so -shared graph_wrap.c libgraph.a 
