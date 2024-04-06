#include "Polynomial.hpp"
#include <iostream>
std::ostream & operator<<(std::ostream & s, const Polynom_exception & p){
    
    switch(p.code){
        case codes::div_by_zero:
            s<<"Division by zero\n";
            break;
        case codes::dyn_mem_err:
            s<<"Error in memory allocation\n";
    }

    return s;
}
std::ostream & operator<<(std::ostream & s, const Polynom_exception_string & p){
    s << "Unsupported char <"<< p.ch << "> in contructor\n";
    return s;
}



bool Polynomial::is_digit(char s){
    int i = s-'0';
    return (i>=0) && (i<=9);
}

bool is_sup_char(char s){
    int i = s-'0';
    return ((i>=0) && (i<=9)) || (s == 'x')  || (s == '^') || (s=='+') || (s=='-')||(s=='\0');
}

int find_degree(char *s){
    int s_deg = 0;
    int cur_deg = 0;
    int min_deg = 0;
    int i = 0;
    enum state {begin, deg, end};
    state v = begin;
    while (s[i] != '\0'){

        if (is_sup_char(s[i]) == false){
            throw Polynom_exception_string(s[i]);
        }

        switch (v){
            case begin:

                if (s[i]=='^') v = deg;
                if (s[i] == 'x') min_deg = 1;
                ++i;
                break;
            
            case deg:{
                cur_deg = cur_deg*10 + s[i]-'0';
                ++i;
                if ((s[i] == '+') || (s[i]=='-')){
                    v=begin;
                    ++i;
                    if (cur_deg>s_deg) s_deg = cur_deg;
                    cur_deg = 0;
                }
                else
                    if (s[i]=='\0')
                        v = end;
                break;
            }
            case end:
                break;
        }
    }
    if (cur_deg>s_deg) s_deg=cur_deg;
    if (min_deg>s_deg) s_deg = min_deg;

    return s_deg;
}

Polynomial::Polynomial(char *s){
    degree = find_degree(s);
    coef = new double[degree+1];

    if (s[0] == '\0'){
        s[0] == 0;
        return;
    }

    int i = 0;
    int cur_coef = 0;
    int cur_deg = 0;
    int sign = 1;
    enum state {begin, deg, deg1, coefficient, coefficient1, fin, end};
    state v = begin;

    for (int j=0; j<degree+1; ++j){
        coef[j] = 0;
    }

    while (1){
        if (v == end) break;

        if (is_sup_char(s[i]) == false){
            delete[] coef;
            throw Polynom_exception_string(s[i]);
        }   

        switch(v){
            case begin:{
                cur_coef = 0;
                cur_deg = 0;
                sign = 1;

                if (s[i] == '-'){
                    sign = -1;
                    ++i;
                }

                if (s[i] == '+'){
                    ++i;
                }


                v=coefficient;
                break;
            }

            case coefficient:
                if (is_digit(s[i]) || s[i]=='x')
                    v = coefficient1;
                else{
                    delete[] coef;
                    throw Polynom_exception_string(s[i]);
                    }
                break;

            case coefficient1:

                if (is_digit(s[i])){
                    cur_coef = cur_coef*10 + s[i]-'0';
                    ++i;
                    break;
                }

                if (s[i] == 'x'){
                    if (cur_coef == 0)
                        cur_coef = 1;
                    v = deg;
                    ++i;
                    break;
                }

                if (s[i] == '+' || s[i] == '-'){
                    v = fin;
                    break;
                }

                if (s[i] == '\0'){
                    v=fin;
                    break;
                }
                else{
                    delete[] coef;
                    throw Polynom_exception_string(s[i]);
                }
                
            
            case deg:
                if (s[i] == '^'){
                    ++i;
                    v = deg1;
                }else{
                    cur_deg = 1;
                    v = fin;
                }
                break;
            

            case deg1:
                if (s[i] == '\0' || s[i] == '+' || s[i] == '-' || s[i] == ' ')
                    v = fin;
                else {
                    if (is_digit(s[i])){
                        cur_deg = cur_deg*10 + s[i]-'0';
                        ++i;
                    }
                    else{
                        delete[] coef;
                        throw Polynom_exception_string(s[i]);
                }

            case fin:
                while (s[i] == ' ')
                    ++i;
                coef[cur_deg] += sign * cur_coef;
                if (s[i] == '\0')
                    v=end;
                else
                    v=begin;
                break;
            
            case end:{
                break;
                }
            }
        }
    }
    chek_degree();
}

void Polynomial:: chek_degree(){
    if (coef[degree]!=0) return;

    int i = degree;
    while (coef[i]==0 && i>0)
        --i;
    degree = i;

    double* new_coef = new double[degree+1];

    for (int j=0; j<=degree; ++j)
        new_coef[j] = coef[j];

    delete[] coef;
    coef = new_coef;        
}   


Polynomial::Polynomial(const Polynomial &p) {
    degree = p.degree;
    coef = new double[degree + 1];
    for (int i = 0; i < degree + 1; ++i)
        coef[i] = p.coef[i];
}

Polynomial Polynomial::operator=(const Polynomial &p){
    degree = p.degree;
    delete[] coef;
    coef = new double[degree + 1];
    for (int i = 0; i < degree + 1; ++i)
        coef[i] = p.coef[i];
    return *this;
}

Polynomial::Polynomial(){
    degree = 0;
    coef = new double[1];
    coef[0] = 0;
}

Polynomial Polynomial::operator+(const Polynomial &p) {
    int max_degree = (degree > p.degree) ? degree : p.degree;
    Polynomial result;
    result.degree = max_degree;
    delete[] result.coef;
    result.coef = new double[max_degree + 1];

    // Инициализация коэффициентов результата
    for (int i = 0; i <= max_degree; ++i) {
        int coeff1 = (i <= degree) ? coef[i] : 0;
        int coeff2 = (i <= p.degree) ? p.coef[i] : 0;
        result.coef[i] = coeff1 + coeff2;
    }
    result.chek_degree();
    return result; // Возвращаем ссылку на текущий объект
}
Polynomial Polynomial::operator-(const Polynomial &p) {
    int max_degree = (degree > p.degree) ? degree : p.degree;
    Polynomial result;
    result.degree = max_degree;
    delete[] result.coef;
    result.coef = new double[max_degree + 1];

    // Инициализация коэффициентов результата
    for (int i = 0; i <= max_degree; ++i) {
        int coeff1 = (i <= degree) ? coef[i] : 0;
        int coeff2 = (i <= p.degree) ? p.coef[i] : 0;
        result.coef[i] = coeff1 - coeff2;
    }
    result.chek_degree();
    return result; // Возвращаем ссылку на текущий объект
}
Polynomial Polynomial::operator*(const Polynomial &p){
    Polynomial result;
    result.degree = degree + p.degree;
    delete[] result.coef;
    result.coef = new double[result.degree + 1];
    for (int i=0; i<=result.degree; ++i) result.coef[i] = 0;

    for (int i=0; i<=degree; ++i){
        for (int j=0; j<=p.degree; ++j)
            result.coef[i+j] += coef[i] * p.coef[j];
    }
    return result;
}

Polynomial Polynomial::operator/(const Polynomial &p){
    Polynomial quotient;
    Polynomial t(*this);
    int res_deg = t.degree - p.degree;

    if (p.degree == 0 && p.coef[0] == 0) {
        throw Polynom_exception(codes::div_by_zero);
    }

    if (res_deg < 0) 
        return quotient;


    quotient.degree = res_deg;
    delete[] quotient.coef;

    quotient.coef = new double[quotient.degree + 1];
    for (int i = 0; i <= quotient.degree; ++i) {
        quotient.coef[i] = 0;
    }

    for (int i = t.degree; i>= p.degree;--i){
        double k = t.coef[i] / p.coef[p.degree];
        quotient.coef[i - p.degree] = k;
        for (int j=0; j<= p.degree; ++j){
            t.coef[i-p.degree+j] -= p.coef[j] * k ;
        }
    }


    return quotient;
}

Polynomial Polynomial::operator%(const Polynomial &p) {
    Polynomial quotient;         

    if (p.degree == 0 && p.coef[0] == 0) {
        throw Polynom_exception(codes::div_by_zero);
    }

    if (p.degree > degree) {
        return *this; 
    }

    return *this - (*this/p) * p;

}

Polynomial& Polynomial::operator/=(const Polynomial &p){
    Polynomial quotient;
    Polynomial t(*this);
    int res_deg = t.degree - p.degree;

    if (p.degree == 0 && p.coef[0] == 0) {
        throw Polynom_exception(codes::div_by_zero);
    }

    if (res_deg < 0){
        this->degree = 0;
        delete[] this->coef;
        this->coef = new double[1];
        this->coef[0] = 0;
        return *this;
    }


    quotient.degree = res_deg;
    delete[] quotient.coef;

    quotient.coef = new double[quotient.degree + 1];
    for (int i = 0; i <= quotient.degree; ++i) {
        quotient.coef[i] = 0;
    }

    for (int i = t.degree; i>= p.degree;--i){

        double k = t.coef[i] / p.coef[p.degree];

        quotient.coef[i - p.degree] = k;

        for (int j=0; j<= p.degree; ++j){
            t.coef[i-p.degree+j] -= p.coef[j] * k ;
        }
    }

    delete[] this->coef;
    this->coef = quotient.coef;
    quotient.coef = nullptr;
    this->degree = quotient.degree;

    return *this;

}


Polynomial& Polynomial::operator%=(const Polynomial& p){
    Polynomial quotient;         

    if (p.degree == 0 && p.coef[0] == 0) {
        throw Polynom_exception(codes::div_by_zero);
    }

    if (p.degree > degree) {
        return *this; 
    }
    *this = *this - (*this/p) * p;
    return *this;
}



Polynomial& Polynomial::operator+=(const Polynomial& p) {
    int max_degree = (degree > p.degree) ? degree : p.degree;

    if (max_degree > degree) {
        double* new_coef = new double[max_degree + 1];
        for (int i = 0; i <= degree; ++i) {
            new_coef[i] = coef[i];
        }
        for (int i = degree + 1; i <= max_degree; ++i) {
            new_coef[i] = 0;
        }
        delete[] coef;
        coef = new_coef;
        degree = max_degree;
    }

    // Добавляем коэффициенты в текущий полином
    for (int i = 0; i <= p.degree; ++i) {
        coef[i] += p.coef[i];
    }
    chek_degree();
    return *this; // Возвращаем ссылку на текущий объект
}

Polynomial& Polynomial::operator-=(const Polynomial& p) {
    int max_degree = (degree > p.degree) ? degree : p.degree;

    if (max_degree > degree) {
        double* new_coef = new double[max_degree + 1];
        for (int i = 0; i <= degree; ++i) {
            new_coef[i] = coef[i];
        }
        for (int i = degree + 1; i <= max_degree; ++i) {
            new_coef[i] = 0;
        }
        delete[] coef;
        coef = new_coef;
        degree = max_degree;
    }

    // Добавляем коэффициенты в текущий полином
    for (int i = 0; i <= p.degree; ++i) {
        coef[i] -= p.coef[i];
    }
    chek_degree();
    return *this; // Возвращаем ссылку на текущий объект
}



Polynomial& Polynomial::operator*=(const Polynomial &p){
    int new_degree = degree + p.degree;
    std::cout<<degree<<'\n';
    double *new_coef = new double[new_degree + 1];
    for (int i=0; i<=new_degree; ++i) new_coef[i] = 0;

    for (int i=0; i<=degree; ++i){
        for (int j=0; j<=p.degree; ++j)
            new_coef[i+j] += (coef[i] * p.coef[j]);

    }
    delete[] coef;
    coef = new_coef;
    degree = new_degree;
    return *this;
}

double Polynomial::operator()(double x)const{
    double res = 0;
    for (int i=degree; i>=0; --i){
        res = res*x + coef[i];
    }
    return res;
}

double Polynomial::def_integreal(double a, double b)const{
    double res_a = 0;
    double res_b = 0;
    for (int i = degree; i>=0; -- i){
        res_a = res_a*a + (coef[i]/(i+1));
        res_b = res_b*b + (coef[i]/(i+1));
    }
    res_a *= a;
    res_b *= b;
    return (res_b-res_a);
}
Polynomial Polynomial::indef_integral()const{
    Polynomial result;
    result.degree = degree + 1;
    delete[] result.coef;
    result.coef = new double[result.degree + 1];
    for (int i=0; i<degree+1; ++i)
        result.coef[i+1] = coef[i] / (i+1);
    result.coef[0] = 0;
    return result;
}

Polynomial Polynomial::differentiate()const{
    Polynomial result;

    if (degree == 0)
        return result;

    delete [] result.coef;
    result.degree = (degree - 1);
    result.coef = new double[result.degree + 1];
    int i=0;
    for (; i <= result.degree; ++i)
        result.coef[i] = coef[i+1]*(i+1);
    
    return result;
}

Polynomial::~Polynomial(){
    if (coef != nullptr){
        delete[] coef;
        coef = nullptr;
    }
}

std::istream& operator>>(std::istream& f, Polynomial &p){
    char s[100];
    f>> s;
    Polynomial r(s);
    p = r;

    return f;
}

std::ostream& operator<<(std::ostream& f, const Polynomial &p){

    enum sign {plus, minus, zero};
    sign v ;

    if (p.degree == 0 && p.coef[0] == 0){
        f<<0;
        return f;
}    

    if (p.coef[0] != 0)
        std::cout<<p.coef[0];
    
    for (int i=1; i<=p.degree; ++i){
        v = (p.coef[i] > 0) ? plus : ((p.coef[i]==0) ? zero : minus);
        switch(v){
            case plus:
                std::cout<<"+" << p.coef[i];
                break;
            case minus:
                std::cout << p.coef[i];
                break;
            case zero:
                continue;
        }
        std::cout<<"x^"<<i;
    }
    std::cout << " degree= " << p.degree <<'\n';
    return f;
}

