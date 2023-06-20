#pragma once

#ifndef COMPILER_SYNTAX_ANALYZATOR_H
#define COMPILER_SYNTAX_ANALYZATOR_H

#include "../Lexical/Scanner.h"
//#include "../Semantics/SemanticTree.h"
#include "../Magazine.h"

enum TypeSymb
{
	Prog, Description,
};


class SyntaxAnalyser
{
public:
	SyntaxAnalyser(const std::istream& srcStream)
	{
		scanner = new Scanner(srcStream);
        magazine = new Magazine();
	}

	void run();

    static bool isTypeData(const Lexeme& lex);
    static bool isComparisonSign(const Lexeme& lex);
    static bool isConst(const Lexeme& lex);

private:

    void epsilon();
    void analyzeNonTerm(Lexeme lex);
	void setLexFromScanner(Lexeme iLex);

	Scanner* scanner;
    Magazine* magazine;
};


#endif //COMPILER_SYNTAX_ANALYZATOR_H