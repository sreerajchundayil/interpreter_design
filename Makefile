a.out:	lexer.cpp main.cpp lexer.hxx token.hxx
	g++ -g lexer.cpp main.cpp

clean : 
	rm a.out
