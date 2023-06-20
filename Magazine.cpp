#include "Magazine.h"
#include "Exceptions/AnalysisExceptions.h"

void Magazine::ptrUp()
{
	this->curMagPtr++;
}

void Magazine::ptrDown()
{
	this->curMagPtr--;
}
/*
void Magazine::setLexFromScanner(Lexeme iLex)
{
	this->magazine[this->curMagPtr].lex = iLex;
}*/

Element Magazine::getTop()
{
    return magazine[curMagPtr];
}

Element* Magazine::getTopPtr()
{
    return &(magazine[curMagPtr]);
}

Element Magazine::getTop(int shift)
{
    return magazine[curMagPtr + shift];
}

void Magazine::setTop(Element element)
{
    magazine[curMagPtr] = element;
}

/*
void Magazine::analyzeNonTerm(Lexeme lex)
{
    switch (getTop().lex.type)
    {
    case LexemeType::TypeProgNonTerm:


    default:
        break;
    }
}*/
