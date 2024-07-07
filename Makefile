all: lex par
lex:	lexer/lexer.cpp main.cpp lexer/lexer.hxx lexer/token.hxx
	g++ -g lexer/lexer.cpp main.cpp -o lex

par:	parser/generateAst.cpp
	g++ -g parser/generateAst.cpp -o par


clean : 
	rm a.out lexer parser
