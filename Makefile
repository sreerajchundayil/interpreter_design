a.out:	lexer/lexer.cpp main.cpp lexer/lexer.hxx lexer/token.hxx
	g++ -g lexer/lexer.cpp main.cpp

ast:	parser/generateAst.cpp
	g++ -g parser/generateAst.cpp

clean : 
	rm a.out
