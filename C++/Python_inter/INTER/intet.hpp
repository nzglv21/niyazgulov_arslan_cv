#include "../EXECUTOR/executor.hpp"

class Interpritator{
    Parser pars;
    Executer exec;
public:
    Interpritator (const char* program);
    void interpritation();
};

Interpritator::Interpritator(const char* program): pars(program){};

void Interpritator::interpritation(){
    pars.analyze();
    cout << "***POLIZ***" << endl;
    for (int i = 0; i < pars.poliz.size() ; ++i){
        cout << i<<"|"<< typeOfLexNames[pars.poliz[i].get_of_type()] <<" "<<pars.poliz[i].get_value() << endl;
    }
    cout << "***END OF POLIZ***" << endl << endl << "***PROGRAM:***" << endl;
    exec.execute(pars.poliz);
    cout << endl << "***YES!!!***" << endl << "***END OF PROGRAM***" << endl;
}