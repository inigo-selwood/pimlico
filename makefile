CXX = g++-7
CXXFLAGS += -std=c++17 -g

all : binaries/compiler

pimlico/parsing/parser.tab.cpp pimlico/parsing/parser.tab.hpp : pimlico/parsing/parser.y
	bison -v -d pimlico/parsing/parser.y -o pimlico/parsing/parser.tab.cpp

pimlico/lexing/lexer.yy.cpp : pimlico/lexing/lexer.flex pimlico/parsing/parser.tab.hpp
	flex -o pimlico/lexing/lexer.yy.cpp pimlico/lexing/lexer.flex

binaries/compiler : pimlico/main.cpp pimlico/parsing/parser.tab.o pimlico/lexing/lexer.yy.o
	mkdir -p ./binaries
	$(CXX) $(CXXFLAGS) -o binaries/compiler $^

clean :
	rm -f binaries/*
	rm -f pimlico/lexing/lexer.yy.cpp \
			pimlico/lexing/lexer.yy.o
	rm -f pimlico/parsing/parser.output \
		pimlico/parsing/parser.tab.cpp \
		pimlico/parsing/parser.tab.hpp \
		pimlico/parsing/parser.tab.o
