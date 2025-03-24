#pragma once
extern const char* typeOfLexNames[];

enum type_of_lex {
    LEX_NULL,
    LEX_TRUE, LEX_FALSE,
    LEX_AND, LEX_OR, LEX_NOT, 
    LEX_IF, LEX_ELIF, LEX_ELSE,                         //if
    LEX_BREAK, LEX_CONTINUE,
    LEX_FOR, LEX_WHILE, LEX_IN,
    LEX_PRINT, LEX_INPUT,
    LEX_RANGE,
    LEX_INT, LEX_STR,

    LEX_FIN,
    LEX_SPACE,                                 //Новая строка
    LEX_COLON, LEX_COMMA,                               // : ,
    LEX_EQ, LEX_NEQ,                                    //логические
    LEX_LSS, LEX_GTR, LEX_LEQ, LEX_GEQ, 
    LEX_ASSIGN,                                         //присваивание
    lEX_PLUS, LEX_MINUS, LEX_MULT, LEX_DIV, LEX_MOD, LEX_POWER,    //математические
    LEX_LPAREN, LEX_RPAREN,                             //Скобка ( || )
    
    LEX_ID,
    LEX_NUM,
    LEX_UNPLUS, LEX_UNMINUS,
    LEX_STRING, LEX_IDSTR, LEX_IDFUNC,
    LEX_INT_INPUT,
    LEX_STR_INPUT,
    LEX_IDINT, LEX_IDBOOL,
    POLIZ_LABEL,                                                                                /*38*/
    POLIZ_ADDRESS,                                                                              /*39*/
    POLIZ_GO,                                                                                   /*40*/
    POLIZ_FGO
};