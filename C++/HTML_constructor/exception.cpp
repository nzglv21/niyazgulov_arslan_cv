#include "exception.hpp"
#include "iostream"
exception::exception(const char* class_type_){
    class_type =  class_type_;
}

void exception::what(){
    std::cout << "Недопустимый класс " << class_type << " в методе addElement.\n";
}
