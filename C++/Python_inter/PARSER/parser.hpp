#include "../LEX/lex.hpp"
#include "stack"

using namespace std;
class Parser{
    Lex cur_lex;
    type_of_lex c_type;
    int c_val;

    int cur_space;          //текущий отступ
    bool in_func;
    bool in_body;

    Scaner scan;
    stack <int> st_int;
    stack <Lex> st_lex;
    void gl();
    stack<int> address_break;           //номера в полизе в которых надо наисать адреса
    stack<int> address_continue;
    stack<int> range_jumps;
    Lex range_ident;
    void get_until_curlex();
    void get_all();
    bool is_printable();
    bool is_mult_oper();
    bool is_slozh_slag();
    bool is_relation_oper();

    //состояния    
    void S();           //стартовое состояние
    void S1();          //операторы
    void BODY();        //тело функции/цикла
    void EXP();         //выражение
    void ITER_OBJ();    //итерируемый объект
    void RANGE_ARGS();  //арументы range()
    void RANGE_ARG();   //аргумент range()
    void ARIF_EXP();    //выражение
    void SLAG();        //слагаемое
    void MNOZH();       //множитель
    void INPUT();       //input
    void IS_INPUT();
public:
    vector <Lex> poliz;
    Parser(const char* program = nullptr);
    void analyze();
};