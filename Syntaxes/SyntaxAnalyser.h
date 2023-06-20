#pragma once

#ifndef COMPILER_SYNTAX_ANALYZATOR_H
#define COMPILER_SYNTAX_ANALYZATOR_H

#include "../Lexical/Scanner.h"
#include "../Semantics/SemanticTree.h"
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
	


    /*
    //void epsilon();
    void progRule();
    void descriptionRule1();
    void descriptionRule2();
    void descriptionRule3();

    void structRule();
    void structVarsRule1();
    void structVarsRule2();
    void typeRuleInt();
    void typeRuleShort();
    void typeRuleLong();
    void typeRuleDouble();
    void typeRuleIdent();
    void mainOrVarsRule1();
    void mainOrVarsRule2();
    void mainRule();
    void varsRule();
    void endofVarListRule();
    void mayEqualRule();
    void constVarsRule();
    void endofConstVarListRule();

    void compoundOperatorRule();
    void compoundBodyRule1();
    void compoundBodyRule2();
    void compoundBodyRule3();

    void operatorRule1();
    void operatorRule2();
    void operatorRule3();
    void simpleOperatorRule1();
    void simpleOperatorRule2();
    void forRule();
    void equalRule();

    void exprRule();
    void endofExprRule();
    void xorBitRule();
    void endofXorRule();
    void andBitRule();
    void endofAndRule();
    void comparisonRule();
    void endofComparisonRule();
    void compSignE();
    void compSignNE();
    void compSignG();
    void compSignGOE();
    void compSignL();
    void compSignLOE();
    void additierRule();
    void endofAdditierRule1();
    void endofAdditierRule2();
    void multiplierRule();
    void endofMultiplierRule1();
    void endofMultiplierRule2();
    void endofMultiplierRule3();
    void basicExprRule1();
    void basicExprRule2Int();
    void basicExprRule2Hex();
    void basicExprRule2Exp();
    void basicExprRule3();

    void preIdentRule();
    void endofIdentRule();

    std::string parseType(int lexType);
    int lookForward(int toForward);*/

	Scanner* scanner;
    Magazine* magazine;
};


#endif //COMPILER_SYNTAX_ANALYZATOR_H