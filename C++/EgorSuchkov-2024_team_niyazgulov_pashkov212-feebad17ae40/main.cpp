#include "INTER/intet.hpp"

void make(char *program){
    Interpritator inter(program);

    try{
        inter.interpritation();
    }

    catch (Lex &l){
        cout << "*"<< typeOfLexNames[l.get_of_type()] << "*\n";
    }
    
    catch (const char* c){
        cout << c << endl;
    }

    catch (char c){
        cout << c << endl;
    }
}

int main(int argc, char **argv){
    if (argc == 1){
        cout << "Интерпритатор подмножества питон.\nЧтобы запустить файл введите ./task [PATH] или make myprog, чтобы запустить my_program.txt\n";
        return 0;
    }
    else if (argc == 2)
        make(argv[1]);
}