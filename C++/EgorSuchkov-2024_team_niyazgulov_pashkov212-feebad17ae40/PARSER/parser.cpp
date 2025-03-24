#include "parser.hpp"

Parser::Parser(const char* program): scan(program), cur_lex(LEX_NULL, 0){
    cur_space = 0;
    in_body = false;
    in_func = false;
}

void Parser::gl(){
    cur_lex = scan.get_lex();
    c_type = cur_lex.get_of_type();
    c_val = cur_lex.get_value();
}

void Parser::analyze(){
    gl();
    S();
    poliz.push_back(Lex(LEX_FIN, 42));
}

void Parser::S(){
    switch (c_type){
        case LEX_SPACE:
            if (cur_space > c_val)
                throw "Ошибка в отсупе S";
            
            gl();

            if (c_type == LEX_FIN)
                return;
            
            else
                S1();

            S();
            break;

        case LEX_FIN:
            break;

        default:
            throw cur_lex;
    }
}

void Parser::S1(){
    bool in_body1;
    int addr_back;
    int addr_if_false;              //переход к elif||else или выход из цикла while
    int addr_if_true;               //выход из body
    int jump_break_address;         //адрес для выхода break;
    vector <int> addr_exits;
    int break_count_befor;          //после выхода body будем знать, сколько раз встретился break. Потом додставим в полизе адреса для break;
    bool is_if;
    Lex save_space;
    switch(c_type){
        case LEX_FIN:
            break;
        
        case LEX_PRINT:
            gl();
            if (c_type != LEX_LPAREN)
                throw cur_lex;

            gl();
            if (c_type == LEX_RPAREN){
                gl();
                break;
            }
            
            ARIF_EXP();
            get_all();
            poliz.push_back(Lex(LEX_PRINT, 42));
            while (c_type != LEX_RPAREN){
                if (c_type != LEX_COMMA)
                    throw cur_lex;
                
                gl();
                ARIF_EXP();
                get_all();
                poliz.push_back(Lex(LEX_PRINT, 52));
            }
            gl();
            break;

        case LEX_ID:
            poliz.push_back(Lex(POLIZ_ADDRESS, cur_lex.get_value()));
            gl();
            if (c_type == LEX_ASSIGN){
                st_lex.push(cur_lex);
                gl();
                EXP();
                poliz.push_back(Lex(LEX_NULL, 0));
            }
            else
                throw cur_lex;
            break;
        
        case LEX_WHILE:
            gl();

            addr_back = poliz.size();
            address_continue.push(addr_back);        //для continue

            EXP();
            
            poliz.push_back(Lex(POLIZ_FGO, 0));
            poliz.push_back(Lex(POLIZ_LABEL, 0));
            addr_if_false = poliz.size() - 1;

            if (c_type != LEX_COLON)
                throw cur_lex;
            
            cur_space += 4;         //отступ
            in_body1 = in_body;
            in_body = true;         //находимся внутри цикла

            break_count_befor = address_break.size();    //после выхода body будем знать, сколько раз встретился break. Потом додставим в полизе адреса для break;

            gl();

            BODY();
            address_continue.pop();
            cur_space -=4;

            in_body = in_body1;
            poliz.push_back(Lex(POLIZ_LABEL, addr_back));

            jump_break_address = poliz.size();                          //jump на конец poliz если break или не выполнено условие
            poliz[addr_if_false] = Lex(POLIZ_LABEL, poliz.size());    //не выолнено условие у while      
                           
            while (address_break.size() != break_count_befor){           //заполняем break(количесво break - на сколько увеличился стек) в полизе, теперь знаем куда прыгать
                poliz[address_break.top()] = Lex(POLIZ_LABEL, jump_break_address);
                address_break.pop();
            }
            break;
        
        case LEX_IF:
            is_if = true;
            while (c_type == LEX_ELIF || is_if){
                is_if = false;

                gl();
                ARIF_EXP();
                get_all();

                poliz.push_back(Lex(POLIZ_FGO, 0));
                poliz.push_back(Lex(LEX_NULL, 0));
                addr_if_false = poliz.size()-1;         //переход на следующий elif/else
                cur_space += 4;
                gl();
                BODY();
                cur_space -= 4;
                poliz.push_back(Lex(POLIZ_LABEL, 0));     //выход при выполнении body
                addr_exits.push_back(poliz.size()-1);
                poliz[addr_if_false] = Lex(POLIZ_LABEL, poliz.size());

                if (cur_space == c_val){
                    save_space = cur_lex;
                    gl();
                }
            }
            if (c_type == LEX_ELSE){
                
                gl();
                if (c_type != LEX_COLON)
                    throw "*LEX_ELSE*";
                gl();
                cur_space += 4;
                BODY();
                cur_space -= 4;
            }
            else if (c_type != LEX_SPACE){
                scan.get_back(cur_lex);
                cur_lex = save_space;
                c_type = save_space.get_of_type();
                c_val = save_space.get_value();   
            }

            //если выполнилось body значит выходим из конструкции if/elif
            for (int i = 0; i< addr_exits.size(); ++i){
                poliz[addr_exits[i]] = Lex(POLIZ_LABEL, poliz.size());
            }
            break;
        
        case LEX_FOR:
            gl();
            if (c_type != LEX_ID)
                throw "*LEX_FOR*";
            
            range_ident = cur_lex;
            gl();
            if (c_type != LEX_IN)
                throw cur_lex;
            gl();
            ITER_OBJ();
            if (c_type != LEX_COLON)
                throw cur_lex;

            cur_space += 4;         //отступ
            in_body1 = in_body;
            in_body = true;         //находимся внутри цикла
            break_count_befor = address_break.size();    //после выхода body будем знать, сколько раз встретился break. Потом додставим в полизе адреса для break;

            gl();
            BODY();

            cur_space -= 4;
            in_body = in_body1;
            address_continue.pop();

            poliz.push_back(Lex(POLIZ_LABEL, range_jumps.top()));
            range_jumps.pop();
            poliz[range_jumps.top()] = Lex(POLIZ_LABEL, poliz.size());
            range_jumps.pop();

            jump_break_address = poliz.size();                          //jump на конец poliz если break или не выполнено условие
                           
            while (address_break.size() != break_count_befor){           //заполняем break(количесво break - на сколько увеличился стек) в полизе, теперь знаем куда прыгать
                poliz[address_break.top()] = Lex(POLIZ_LABEL, jump_break_address);
                address_break.pop();
            }
            break;  

        case LEX_BREAK:
            if (!in_body)
                throw cur_lex;
            
            poliz.push_back(Lex(POLIZ_LABEL, 0));
            address_break.push(poliz.size()-1);         //потом подставим аддресс выхода после выхода из BODY в while/for
            gl();
            break;
        
        case LEX_CONTINUE:
            if (!in_body)
                throw cur_lex;
            
            poliz.push_back(Lex(POLIZ_LABEL, address_continue.top()));        //прыгаем на начало цикла, аддресс хранится в address_continue;
            gl();
            break;
    }
}

bool Parser::is_slozh_slag(){
    return (c_type == lEX_PLUS)
        || (c_type == LEX_MINUS);
}

bool Parser::is_mult_oper(){
    return (c_type== LEX_MULT)
        || (c_type == LEX_DIV)
        || (c_type == LEX_MOD)
        || (c_type == LEX_POWER)
        || (c_type == LEX_AND)
        || (c_type == LEX_IN);
}

bool Parser::is_relation_oper(){
    return (c_type == LEX_EQ) || 
            (c_type == LEX_NEQ) ||
            (c_type == LEX_LSS) || 
            (c_type == LEX_LEQ) ||
            (c_type == LEX_GTR) || 
            (c_type == LEX_GEQ) ||
            (c_type == LEX_OR);        
}

void Parser::BODY(){
    if (c_type != LEX_SPACE)
        throw "*BODY*";
    
    if (c_val < cur_space)
        throw "Ошибка в отступе BODY";

    gl();
    S1();

    if (c_type != LEX_SPACE)
        throw "*BODY*";
    
    if (c_val == cur_space)
        BODY();
}

void Parser::EXP(){
    if (c_type == LEX_INT || c_type == LEX_STR || c_type == LEX_INPUT){
        st_lex.pop();
        INPUT();
    }
    else{
        ARIF_EXP();
        get_all();
    }
}

void Parser::IS_INPUT(){
    if (c_type != LEX_INPUT)
        throw cur_lex;
    gl();
    if (c_type != LEX_LPAREN)
        throw cur_lex;
    gl();
    if (c_type != LEX_RPAREN)
        throw cur_lex;
    gl();
}

void Parser::INPUT(){
    if (c_type == LEX_INT){
        gl();
        if (c_type != LEX_LPAREN)
            throw cur_lex;
        gl();

        IS_INPUT();

        if (c_type != LEX_RPAREN)
            throw cur_lex;
        gl();
        poliz.push_back((Lex(LEX_INT_INPUT, 0)));
    }

    else if (c_type == LEX_STR){
        gl();
        if (c_type != LEX_LPAREN)
            throw cur_lex;
        
        gl();
        IS_INPUT();
        if (c_type != LEX_RPAREN)
            throw cur_lex;
        gl();
        poliz.push_back((Lex(LEX_STR_INPUT, 0)));
    }

    else {
        IS_INPUT();
        poliz.push_back((Lex(LEX_STR_INPUT, 0)));
    }
}

void Parser::get_until_curlex(){
    int priority;
    if (c_type != LEX_RPAREN){
        priority = cur_lex.get_value();
        while (!st_lex.empty() && (priority <= st_lex.top().get_value()) && (st_lex.top().get_of_type() != LEX_LPAREN)){
            poliz.push_back(st_lex.top());
            st_lex.pop();
            if (st_lex.empty())
                break;
        }
        st_lex.push(cur_lex);
    } 

    else {
        while (st_lex.top().get_of_type() != LEX_LPAREN){
                poliz.push_back(st_lex.top());
                st_lex.pop();
                if (st_lex.empty())
                    throw "Несогласование скобок";
        }
        st_lex.pop();
    }
}

void Parser::ARIF_EXP(){
    if (is_slozh_slag()){
        if (c_type == lEX_PLUS){
            cur_lex = Lex(LEX_UNPLUS, 9);
        }
        else{
            cur_lex = Lex(LEX_UNMINUS, 9);
        }
        get_until_curlex();
        gl();
    }
    SLAG();

    while (is_slozh_slag() || is_relation_oper()){
        get_until_curlex();
        gl();
        SLAG();
    }
}

void Parser::SLAG(){
    MNOZH();
    while (is_mult_oper()){
        get_until_curlex();
        gl();
        MNOZH();
    } 
}

void Parser::MNOZH(){
    switch(c_type){
        case LEX_NUM: case LEX_TRUE: case LEX_FALSE: case LEX_ID: case LEX_STRING:
            poliz.push_back(cur_lex);
            gl();
            break;

        case LEX_MINUS:
            cur_lex = Lex(LEX_UNMINUS, 9);
            c_val = 9;
            get_until_curlex();
            gl();
            ARIF_EXP();
            break;
        
        case LEX_LPAREN:
            st_lex.push(cur_lex);
            gl();
            ARIF_EXP();

            if (c_type != LEX_RPAREN)
                throw "*MNOZH_RPAREN*";
                
            get_until_curlex();
            gl();
            break;
        
        case LEX_IN:
            get_until_curlex();
            gl();
            break;
        
        case LEX_NOT:
            st_lex.push(cur_lex);
            gl();
            MNOZH();
            break;
        
        default:
            throw "*Unexpected construction*";
    }
}

void Parser::ITER_OBJ(){
    if (c_type == LEX_RANGE){
        gl();
        if (c_type != LEX_LPAREN)
            throw cur_lex;
        
        gl();
        RANGE_ARGS();
        gl();
    } 

    else if ((c_type == LEX_STRING) or (c_type == LEX_ID)){
        poliz.push_back(cur_lex);
        gl();
    }
    
    else
        throw cur_lex;
}

void Parser::RANGE_ARGS(){
    Lex from(LEX_NUM, 0);
    Lex to(LEX_NUM, 0);
    Lex step(LEX_NUM, 1);
    Lex buf;

    if (c_type == LEX_NUM || c_type == LEX_ID){
        buf = cur_lex;
        gl();

        if (c_type != LEX_RPAREN){
            if (c_type != LEX_COMMA)
                throw cur_lex;
            gl();

            if (c_type == LEX_NUM || c_type == LEX_ID){
                to = cur_lex;
                from = buf;
                gl();
                
                if (c_type != LEX_RPAREN){
                    if (c_type != LEX_COMMA)
                        throw cur_lex;
                    gl();

                    if (c_type == LEX_NUM || c_type == LEX_ID){
                        step = cur_lex;
                        gl(); 
                    }

                    else 
                        throw cur_lex;
                }
            }
            else
                throw cur_lex;
        }
        
        else {
            to = buf;
        }
    }
    
    else 
        throw cur_lex;

    poliz.push_back(Lex(POLIZ_ADDRESS, range_ident.get_value()));
    poliz.push_back(from);
    poliz.push_back(Lex(LEX_ASSIGN, 0));
    poliz.push_back(Lex(LEX_NULL, 0));
    int E2 = poliz.size();
    //разделитель
    poliz.push_back(range_ident);
    poliz.push_back(to);
    poliz.push_back(Lex(LEX_LSS, 2));
    poliz.push_back(Lex(POLIZ_FGO, 0));
    poliz.push_back(Lex(POLIZ_LABEL, 0));
    range_jumps.push(poliz.size() - 1);

    poliz.push_back(Lex(POLIZ_LABEL, 0));
    int goE4 = poliz.size() - 1;
    range_jumps.push(poliz.size());

    address_continue.push(poliz.size());        //continue address;

    poliz.push_back(Lex(POLIZ_ADDRESS, range_ident.get_value()));
    poliz.push_back(range_ident);
    poliz.push_back(step);
    poliz.push_back(Lex(lEX_PLUS, 0));
    poliz.push_back(Lex(LEX_ASSIGN, 0));
    poliz.push_back(Lex(LEX_NULL, 0));
    poliz.push_back(Lex(POLIZ_LABEL, E2));
    poliz[goE4] = Lex(POLIZ_LABEL, poliz.size());
}

void Parser::RANGE_ARG(){
    switch (c_type){
        case LEX_ID: case LEX_NUM:
            poliz.push_back(cur_lex);
            gl();
            break;
    
        default:
            throw cur_lex;
    }
}

void Parser::get_all(){
    for(;!st_lex.empty();){
        poliz.push_back(st_lex.top());
        st_lex.pop();
    }
}

bool Parser::is_printable(){
    return (c_type == LEX_ID) || 
            (c_type == LEX_NUM) || 
            (c_type == LEX_STRING);
}