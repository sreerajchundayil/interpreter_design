all: lex parse
main: lex parse main.cpp
	g++ lex.o parse.o main.cpp -g

lex:	lexer/lexer.cpp lexer/lexer.hxx lexer/token.hxx 
	g++ lexer/lexer.cpp -c -o lex.o -g

parse: parser/astClasses.hxx parser/parser.cpp parser/parser.hxx
	g++ parser/parser.cpp -c -o parse.o -g




clean : 
	rm a.out lexer parser
