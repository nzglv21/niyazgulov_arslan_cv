#include "executor.hpp"
#include <cmath>
extern std::vector<Ident> TID;
extern std::vector<std::string> STRT;

template <class T, class T_EL>
void from_st(T & st, T_EL & i){
    i = st.top(); 
    st.pop();
}

void Executer::execute(vector <Lex> &poliz){
    Lex pol_el;
    stack<Data> args;
    Data element_1, element_2;
    int i, id;
    bool is_print = false;
    string str;

    for (int index = 0; index < poliz.size(); index++){
        pol_el = poliz[index];

        switch (pol_el.get_of_type()){
            case LEX_ID:
                i = pol_el.get_value();
                args.push(TID[i].get_value());
                break;
            
            case LEX_NUM: case POLIZ_ADDRESS:
                args.push(pol_el.get_value());
                break;
            
            case POLIZ_LABEL:
                index = poliz[index].get_value() - 1;
                break;
            
            case LEX_ASSIGN:
                from_st(args, element_2);
                from_st(args, element_1);
                TID[element_1.get_int()].put_value(element_2);
                break;

            case LEX_EQ:
                from_st(args, element_2);
                from_st(args, element_1);
                if ((element_1.get_type() == element_2.get_type()) and (element_1.get_type() != NULL_TYPE))
                    if (element_1.get_type() == INT_TYPE)
                        args.push(element_1.get_int() == element_2.get_int());
                    else
                        args.push(element_1.get_str() == element_2.get_str());
                else
                    throw "Wrong types (==)";
                break;
 
            case LEX_LSS:
                from_st(args, element_2);
                from_st(args, element_1);
                if ((element_1.get_type() == element_2.get_type()) and (element_1.get_type() != NULL_TYPE))
                    if (element_1.get_type() == INT_TYPE)
                        args.push(element_1.get_int() < element_2.get_int());
                    else
                        args.push(element_1.get_str() < element_2.get_str());
                else
                    throw "Wrong types (<)";
                break;
 
            case LEX_GTR:
                from_st(args, element_2);
                from_st(args, element_1);
                if ((element_1.get_type() == element_2.get_type()) and (element_1.get_type() != NULL_TYPE))
                    if (element_1.get_type() == INT_TYPE)
                        args.push(element_1.get_int() > element_2.get_int());
                    else
                        args.push(element_1.get_str() > element_2.get_str());
                else
                    throw "Wrong types (>)";
                break;
 
            case LEX_LEQ:
                from_st(args, element_2);
                from_st(args, element_1);
                if ((element_1.get_type() == element_2.get_type()) and (element_1.get_type() != NULL_TYPE))
                    if (element_1.get_type() == INT_TYPE)
                        args.push(element_1.get_int() <= element_2.get_int());
                    else
                        args.push(element_1.get_str() <= element_2.get_str());
                else
                    throw "Wrong types (<=)";
                break;
 
            case LEX_GEQ:
                from_st(args, element_2);
                from_st(args, element_1);
                if ((element_1.get_type() == element_2.get_type()) and (element_1.get_type() != NULL_TYPE))
                    if (element_1.get_type() == INT_TYPE)
                        args.push(element_1.get_int() >= element_2.get_int());
                    else
                        args.push(element_1.get_str() >= element_2.get_str());
                else
                    throw "Wrong types (>=)";
                break;
 
            case LEX_NEQ:
                from_st(args, element_2);
                from_st(args, element_1);
                if ((element_1.get_type() == element_2.get_type()) and (element_1.get_type() != NULL_TYPE))
                    if (element_1.get_type() == INT_TYPE)
                        args.push(element_1.get_int() != element_2.get_int());
                    else
                        args.push(element_1.get_str() != element_2.get_str());
                else
                    throw "Wrong types (!=)";
                break;

            case LEX_TRUE:
                args.push(1);
                break;
            
            case LEX_FALSE:
                args.push(0);
                break;
            
            case LEX_AND:
                from_st(args, element_1);
                from_st(args, element_2);
                if ((element_1.get_type() == element_2.get_type()) and (element_1.get_type() == INT_TYPE))
                    args.push(element_1.get_int() and element_2.get_int());
                else
                    throw "Wrong types (and)";
                break;
            
            case LEX_OR:
                from_st(args, element_1);
                from_st(args, element_2);
                if ((element_1.get_type() == element_2.get_type()) and (element_1.get_type() == INT_TYPE))
                    args.push(element_1.get_int() or element_2.get_int());
                else
                    throw "Wrong types (or)";
                break;
            
            case LEX_NOT:
                from_st(args, element_1);
                if ((element_1.get_type() == INT_TYPE))
                    args.push(!element_1.get_int());
                else
                    throw "Wrong types (not)";
                break;
            
            case LEX_UNPLUS:
                from_st(args, element_1);
                if (element_1.get_type() == INT_TYPE)
                    args.push(+element_1.get_int());
                else
                    throw "Wrong types (unplus)";
                break;
            
            case LEX_UNMINUS:
                from_st(args, element_1);
                if (element_1.get_type() == INT_TYPE)
                    args.push(-element_1.get_int());
                else
                    throw "Wrong types (unminus)";
                break;
            
            case LEX_STRING:
                i = pol_el.get_value();
                args.push(STRT[i]);
                break;
            
            case lEX_PLUS:
                from_st(args, element_2);
                from_st(args, element_1);
                if ((element_1.get_type() == element_2.get_type()) and (element_1.get_type() != NULL_TYPE))
                    if (element_1.get_type() == INT_TYPE)
                        args.push(element_1.get_int() + element_2.get_int());
                    else
                        args.push(element_1.get_str() + element_2.get_str());
                else
                    throw "Wrong types (+)";
                break;
            
            case LEX_MINUS:
                from_st(args, element_2);
                from_st(args, element_1);
                if ((element_1.get_type() == element_2.get_type()) and (element_1.get_type() != NULL_TYPE) and (element_1.get_type() != STRING_TYPE))
                    args.push(element_1.get_int() - element_2.get_int());
                else
                    throw "Wrong types (-)";
                break;
            
            case LEX_MULT:
                from_st(args, element_1);
                from_st(args, element_2);
                if ((element_1.get_type() == element_2.get_type()) and (element_1.get_type() != NULL_TYPE) and (element_1.get_type() != STRING_TYPE))
                    args.push(element_1.get_int() * element_2.get_int());
                else
                    throw "Wrong types (*)";
                break;
            
            case LEX_DIV:
                from_st(args, element_2);
                from_st(args, element_1);
                if ((element_1.get_type() == element_2.get_type()) and (element_1.get_type() != NULL_TYPE) and (element_1.get_type() != STRING_TYPE))
                    args.push(element_1.get_int() / element_2.get_int());
                else
                    throw "Wrong types (/)";
                break;
            
            case LEX_MOD:
                from_st(args, element_2);
                from_st(args, element_1);
                if ((element_1.get_type() == element_2.get_type()) and (element_1.get_type() != NULL_TYPE) and (element_1.get_type() != STRING_TYPE))
                    args.push(element_1.get_int() % element_2.get_int());
                else
                    throw "Wrong types (%)";
                break;
            
            case LEX_POWER:
                from_st(args, element_2);
                from_st(args, element_1);
                if ((element_1.get_type() == element_2.get_type()) and (element_1.get_type() != NULL_TYPE) and (element_1.get_type() != STRING_TYPE))
                    args.push((int)pow(element_1.get_int(), element_2.get_int()));
                else
                    throw "Wrong types (**)";
                break;
            
            case POLIZ_GO:
                from_st(args, element_1);
                index = element_1.get_int() - 1;
                break;
 
            case POLIZ_FGO:
                from_st(args, element_1);
                if (!element_1.get_int()) 
                    index = poliz[index+1].get_value() - 1;
                else
                    index = index + 1;
                break;
            
            case LEX_PRINT:
                if (poliz[index].get_value() == 42 && is_print)
                    cout << "\n";
                is_print = true;
                from_st(args, element_1);
                cout << element_1;
                break;
            
            case LEX_INT_INPUT:
                from_st(args, element_1);
                id = element_1.get_int();
                cin >> i;
                TID[id].put_value(i);
                break;
            
            case LEX_STR_INPUT:
                from_st(args, element_1);
                id = element_1.get_int();
                cin >> str;
                TID[id].put_value(str);
                break;
            
            case LEX_NULL:
                break;
            
            case LEX_IN:
                from_st(args, element_2);
                from_st(args, element_1);
                if ((element_1.get_type() == element_2.get_type()) && (element_1.get_type() == STRING_TYPE))
                    args.push(element_2.get_str().find(element_1.get_str()) != -1);
                else
                    throw "Wrong types (in)";
                break;
            
            case LEX_FIN:
                break;
            
            default:
                throw "POLIZ: undefined lex";
        }
    }
}

