#pragma once
#ifndef COMPILER_SYNTAX_ANALYZATOR_MAGAZINE_H
#define COMPILER_SYNTAX_ANALYZATOR_MAGAZINE_H

#include "Types/LexemeType.h"
#include "Lexical/Scanner.h"

#define MAX_LEN_MAGAZINE 1000		    // Размер магазина

struct Element 
{
    bool term;
    Lexeme lex;
};

class Magazine
{
private:
    Element magazine[MAX_LEN_MAGAZINE];
    int curMagPtr;                 // Указатель на верхушку магазина

public:
    Magazine()
    {
        curMagPtr = 0;
    }

    void ptrUp();
    void ptrDown();

    Element getTop();
    Element getTop(int shift);
    Element* getTopPtr();
    void setTop(Element element);
};


#endif //COMPILER_SYNTAX_ANALYZATOR_MAGAZINE_H