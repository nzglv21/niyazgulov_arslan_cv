#include "lex.hpp"
#include <cctype>
#include <cstdio>

extern std::vector<std::string> STRT;

Lex::Lex(){
    t_lex = LEX_NULL;
    v_lex = 0;
}

const char* typeOfLexNames[] = {
    "LEX_NULL",
    "LEX_TRUE", "LEX_FALSE",
    "LEX_AND", "LEX_OR", "LEX_NOT",
    "LEX_IF", "LEX_ELIF", "LEX_ELSE",
    "LEX_BREAK", "LEX_CONTINUE",
    "LEX_FOR", "LEX_WHILE", "LEX_IN",
    "LEX_PRINT", "LEX_INPUT",
    "LEX_RANGE",
    "LEX_INT", "LEX_STR",
    "LEX_FIN",
    "LEX_SPACE",
    "LEX_COLON", "LEX_COMMA",
    "LEX_EQ", "LEX_NEQ",
    "LEX_LSS", "LEX_GTR", "LEX_LEQ", "LEX_GEQ",
    "LEX_ASSIGN",
    "LEX_PLUS", "LEX_MINUS", "LEX_MULT", "LEX_DIV", "LEX_MOD", "LEX_POWER",
    "LEX_LPAREN", "LEX_RPAREN",
    "LEX_ID",
    "LEX_NUM",
    "LEX_UNPLUS", "LEX_UNMINUS",
    "LEX_STRING", "LEX_IDSTR", "LEX_IDFUNC",
    "LEX_INT_INPUT",
    "LEX_STR_INPUT",
    "LEX_IDINT", "LEX_IDBOOL",
    "POLIZ_LABEL",
    "POLIZ_ADDRESS",
    "POLIZ_GO",
    "POLIZ_FGO"
};

Lex::Lex(type_of_lex t, int v){
    t_lex = t;
    v_lex = v;
}

type_of_lex Lex::get_of_type() const{
    return t_lex;
}

int Lex::get_value() const{
    return v_lex;
}

ostream& operator<<(ostream &s, Lex& l){
    s << 1;
    return s;
}

// Scanner
const char * Scaner:: TW [] = {"", "true", "false", "and", "or", "not", "if", "elif", "else", "break", "continue",
                                "for", "while", "in", "print", "input", "range", "int", "str", NULL};

const char *Scaner:: TD  [] = {"", " ", ":", ",", "==", "!=", "<", ">", "<=", ">=", "=", "+", "-", "*", "/", "%", "**", "(", ")", NULL};
bool Scaner::is_newline = true;

Scaner::Scaner(const char * program = nullptr){
    f =  fopen(program, "r");
    is_prev = false;
    if (f == NULL)
        throw "can`t open file";
}

void Scaner::gc(){
    c = (char)fgetc(f);
}

void Scaner::putc(){
    if (c!=EOF)
        fseek(f, -1, SEEK_CUR);
}

int Scaner::look(const string buf, const char ** list){
    int i = 0;
    while (list[i] != NULL){
        if (buf == list[i])
            return i;
        ++i;
    }
    return 0;
}

int get_priority(string buf){
    if (buf == "(" || buf == ")")
        return 11;
    if (buf == "**")
        return 10;
    if (buf == "*" || buf=="/"||buf=="%")
        return 8;
    if (buf == "+"||buf == "-")
        return 7;
    if (buf == ">" || buf == ">=" || buf == "<" || buf == "<=") 
        return 6;
    if (buf == "==" || buf == "!=")
        return 5;
    if (buf == "in")
        return 4;
    if (buf == "not")
        return 3;
    if (buf == "and")
        return 2;
    if (buf == "or")
        return 1;
    if (buf == "=")
        return 0;
        
    return -1;
}

void Scaner::get_back(Lex l){
    is_prev = true;
    prev_lex = l;
}

Lex Scaner::get_lex(){
    enum state {
        H, IDENT, NUMB, 
        COMMENT, SPACE, LBRCKT, 
        RBRCKT, NEQ, EQ,
        ALE, STR, POWER
    };

    int d = 0;
    int ind;
    int space_len = 0;
    int sign = 1;
    char str_start_with;
    bool bs = false;

    string buf;
    state CS = H;

    if (is_prev){
        is_prev = false;
        return prev_lex;
    }

    while (true) {
        gc();
        switch (CS){
            case H:
                if (c == EOF){
                    if (is_EOF)
                        return Lex(LEX_FIN, 0);
                    else{
                        is_EOF = true;
                        return Lex(LEX_SPACE, 0);
                    }
                }
                else if (is_newline){
                    is_newline = false;
                    if ((c == ' ' || c == '\t') ){
                        CS = SPACE;
                        space_len =  (c == ' ') ? 1 : 4;
                    }
                    else if (c == '#')
                        CS = COMMENT;
                    else {
                        if (c != '\n'){
                            putc();
                            return Lex(LEX_SPACE, 0);
                        }
                        else
                            is_newline = true;
                    }
                }
                else if (c == ' ' || c == '\t');

                else if (c == '\n')
                    is_newline = true;

                else if (isalpha(c)){
                    buf.push_back(c);
                    CS = IDENT;
                    is_newline = false;
                }

                else if (isdigit(c)){
                    d = c - '0';
                    CS = NUMB;
                    is_newline = false;
                }

                else if (c == '#'){
                    CS = COMMENT;
                    is_newline = false;
                }

                else if (c == '<' || c == '>' || c == '!' || c == '=' || c == '!'){
                    buf.push_back(c);
                    CS = ALE;
                    is_newline = false;
                }

                else if (c == '"' || c == '\''){
                    CS = STR;
                    str_start_with = c;
                    is_newline = false;
                }

                else if (c == '*'){
                    CS = POWER;
                    buf.push_back(c);
                }

                else {
                    is_newline = false;
                    buf.push_back(c);
                    if ((ind = look(buf, TD)))
                        return Lex((type_of_lex(ind + (int) LEX_FIN)), get_priority(buf));
                    else
                        throw c;
                }
                break;

            case POWER:
                if (c == '*')
                    return Lex(LEX_POWER, 10);
                else{
                    putc();
                    return Lex(LEX_MULT, 8);
                }
                break;

            case SPACE:
                if (c == ' ')
                    space_len += 1;
                else if (c == '\t')
                    space_len += 4;
                else if (c == '\n' || c == EOF){
                    space_len = 0;
                    is_newline = true;
                    CS = H;
                }
                else {
                    putc();
                    return Lex(LEX_SPACE, space_len);
                }
                break;

            case IDENT:
                if (isalpha(c) || isdigit(c))
                    buf.push_back(c);
                else {
                    putc();
                    if ((ind = look(buf, TW)))
                        return Lex((type_of_lex)ind, get_priority(buf));
                    else {
                        ind = put(buf);
                        return Lex(LEX_ID, ind);
                    }
                }
                break;
            
            case NUMB:
                if (isdigit(c))
                    d = d*10 + c-'0';
                else if (isalpha(c))
                    throw c;
                else {
                    putc();
                    return Lex(LEX_NUM, d);
                }
                break;
            case COMMENT:
                if (c == '\n'){
                    CS = H;
                    is_newline = true;
                }
                else if (c == EOF)
                    return Lex(LEX_FIN, 0);
                break;
            
            case ALE:
                if (c == '='){
                    buf.push_back(c);
                    ind = look(buf, TD);
                    return Lex((type_of_lex)(ind + (int)LEX_FIN), get_priority(buf));
                }
                else {
                    putc();
                    ind = look(buf, TD);
                    return Lex((type_of_lex)(ind + (int)LEX_FIN), get_priority(buf));
                }
                break;
            case STR:
                if (c == str_start_with){
                    STRT.push_back(buf);
                    return Lex(LEX_STRING, STRT.size() - 1);
                }
                if (c == EOF)
                    throw "Ошибка в строке";
                
                if (c == '\\' && !bs)
                    bs = true;

                else if (bs && c == 'n'){
                    bs = false;
                    buf.push_back('\n');
                }
                
                else if (bs){
                    bs = false;
                    buf.push_back('\\');
                    buf.push_back(c);
                }
                
                else {
                    buf.push_back(c);
                    bs = false;
                }
                break;
        }
    }
}

