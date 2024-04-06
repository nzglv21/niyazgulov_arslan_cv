#include "Polynomial.hpp"
#include <iostream>

#define catch_and_print \
catch (Polynom_exception_string e) {std::cerr<<e<<std::endl;} \
catch (Polynom_exception e) {std::cerr<<e<<std::endl;}

void try_valid_constructors() {
    try {

        // Проверка конструктора с использованием строки
        Polynomial p1("1+x^2-2x+3x^3");
        std::cout << "Polynomial p1: " << p1 << std::endl;

        // Проверка конструктора копирования
        Polynomial p2(p1);
        std::cout << "Copied polynomial p2 from p1: " << p2 << std::endl;

        // Проверка оператора присваивания
        Polynomial p3;
        p3 = p1;
        std::cout << "Assigned polynomial p3 from p1: " << p3 << std::endl;

        // Дополнительные проверки
        Polynomial p4("2x^3+5x^2-3x+1");
        std::cout << "Polynomial p4: " << p4 << std::endl;

        Polynomial p5("x^4-2x^3+5x^2-x-4");
        std::cout << "Polynomial p5: " << p5 << std::endl;

        Polynomial p6("-x^5+3x^4-5x^3+x^2+2x-1");
        std::cout << "Polynomial p6: " << p6 << std::endl;

        Polynomial p7("4x^6-2x^5+x^4+3x^3-5x^2+2x-1");
        std::cout << "Polynomial p7: " << p7 << std::endl;

        Polynomial p8("x^2-2x+1");
        std::cout << "Polynomial p8: " << p8 << std::endl;

        Polynomial p9("10");
        std::cout << "Polynomial p9: " << p9 << std::endl;

        Polynomial p10("-x^2");
        std::cout << "Polynomial p10: " << p10 << std::endl;

        Polynomial p11("0");
        std::cout << "Polynomial p11: " << p11 << std::endl;

        Polynomial p12("3x^3+4x^2+2x+1");
        std::cout << "Polynomial p12: " << p12 << std::endl;

    } catch_and_print;
}


void try_invalid_constructors() {
    try {

        // Неверное использование строки (несоответствие формату)
        std::cout << "Trying Polynomial p(\"1+2x^3-4x\")" << std::endl;
        try {
            Polynomial p("1+2x^3-4x"); // Несоответствующий формат строки
        } catch_and_print

        // Другие недопустимые случаи
        std::cout << "Trying Polynomial p(\"\")" << std::endl;
        try {
            Polynomial p(""); // Пустая строка
        } catch_and_print

        std::cout << "Trying Polynomial p(\"x^2+3x+-2\")" << std::endl;
        try {
            Polynomial p("x^2+3x+-2"); // Дублирование оператора
        } catch_and_print

        // Другие недопустимые случаи
        std::cout << "Trying Polynomial p(\"3x^3+4x^2+2x+1.5a\")" << std::endl;
        try {
            Polynomial p("3x^3+4x^2+2x+1.5a"); // Несоответствующий символ
        } catch_and_print

        std::cout << "Trying Polynomial p(\"3.14x^2+2x+1\")" << std::endl;
        try {
            Polynomial p("3.14x^2+2x+1"); // Неверное использование десятичной точки
        } catch_and_print

        std::cout << "Trying Polynomial p(\"x^2+3x-\")" << std::endl;
        try {
            Polynomial p("x^2+3x-"); // Незавершенный полином
        } catch_and_print

        std::cout << "Trying Polynomial p(\"2x^2+-3x+1\")" << std::endl;
        try {
            Polynomial p("2x^2+-3x+1"); // Незавершенный полином
        } catch_and_print

        std::cout << "Trying Polynomial p(\"2x^2+3x+-1\")" << std::endl;
        try {
            Polynomial p("2x^2+3x+-1"); // Незавершенный полином
        } catch_and_print

    } catch_and_print;
}

void try_plus_minus(Polynomial p1, Polynomial p2) {
    try {
        std::cout << "==================================================" << std::endl;
        std::cout << "   ~~~~~~~~~~ + ~~~~~~~~~~" << std::endl;
        std::cout << "Polynomial p1: " << p1 << std::endl;
        std::cout << "Polynomial p2: " << p2 << std::endl;
        std::cout << "p1 + p2 = " << (p1 + p2) << std::endl;
    } catch_and_print

    try {
        std::cout << "   ~~~~~~~~~~ - ~~~~~~~~~~" << std::endl;
        std::cout << "Polynomial p1: " << p1 << std::endl;
        std::cout << "Polynomial p2: " << p2 << std::endl;
        std::cout << "p1 - p2 = " << (p1 - p2) << std::endl;
    } catch_and_print


    try {
        std::cout << "   ~~~~~~~~~~ - ~~~~~~~~~~" << std::endl;
        std::cout << "Polynomial p2: " << p2 << std::endl;
        std::cout << "Polynomial p1: " << p1 << std::endl;
        std::cout << "p1 - p2 = " << (p2 - p1) << std::endl;
    } catch_and_print
}

void try_multiply(Polynomial p1, Polynomial p2) {
    try {
        std::cout << "==================================================" << std::endl;
        std::cout << "   ~~~~~~~~~~ * ~~~~~~~~~~" << std::endl;
        std::cout << "Polynomial p1: " << p1 << std::endl;
        std::cout << "Polynomial p2: " << p2 << std::endl;
        std::cout << "p1 * p2 = " << (p1 * p2) << std::endl;
    } catch_and_print

    try {
        std::cout << "   ~~~~~~~~~~ *= ~~~~~~~~~~" << std::endl;
        std::cout << "Polynomial p1: " << p1 << std::endl;
        std::cout << "Polynomial p2: " << p2 << std::endl;
        p1 *= p2;
        std::cout << "p1 *= p2 = " << p1 << std::endl;
    } catch_and_print
}

void try_div(Polynomial p1, Polynomial p2) {
    try {
        std::cout << "==================================================" << std::endl;
        std::cout << "   ~~~~~~~~~~ / ~~~~~~~~~~" << std::endl;
        std::cout << "Polynomial p1: " << p1 << std::endl;
        std::cout << "Polynomial p2: " << p2 << std::endl;
        std::cout << "p1 / p2 = " << (p1 / p2) << std::endl;
    } catch_and_print

    try {
        std::cout << "   ~~~~~~~~~~ /= ~~~~~~~~~~" << std::endl;
        std::cout << "Polynomial p1: " << p1 << std::endl;
        std::cout << "Polynomial p2: " << p2 << std::endl;
        p1 /= p2;
        std::cout << "p1 /= p2 = " << p1 << std::endl;
    } catch_and_print
}





void try_mod(Polynomial p1, Polynomial p2) {
    std::cout << "=========================================" << std::endl;
    try {
        std::cout << "Polynomial p1: " << p1 << std::endl;
        std::cout << "Polynomial p2: " << p2 << std::endl;

        // Trying modulus operator
        Polynomial mod_result = p1 % p2;
        
        std::cout << "   ~~~~~~~~~~ % ~~~~~~~~~~" << std::endl;

        std::cout <<  "p1 % p2 = " << mod_result << std::endl;

        // Trying modulus assignment operator
        std::cout << "   ~~~~~~~~~~ %= ~~~~~~~~~~" << std::endl;

        p1 %= p2;
        std::cout << "p1 %= p2 " << p1 << std::endl;
    } catch_and_print
}

void try_others_methods(Polynomial p){

}





int main(int argc, char* argv[]) {



    if (argc == 2 and argv[1][0]=='c'){
        try_valid_constructors();
        try_invalid_constructors();
    }
    else if (argc == 2){
        Polynomial f(argv[2]);
          
    }
    if (argc == 3){
        Polynomial f(argv[1]);
        Polynomial g(argv[2]);

        try_plus_minus(f, g);

        try_multiply(f, g);


        try_div(f, g);
        

        try_mod(f, g);

    }
}