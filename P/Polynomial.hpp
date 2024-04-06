#include <iostream>

enum class codes
{
	dyn_mem_err,
	invalid_string,
	unknown,
    div_by_zero
};

class Polynom_exception{
    codes code;
public:
    Polynom_exception(codes c = codes::unknown) { code = c;}
	friend std::ostream & operator<< (std::ostream & s, const Polynom_exception & e);
};

class Polynom_exception_string: public Polynom_exception{
    char ch;
public:
    Polynom_exception_string(char c): Polynom_exception(codes::invalid_string){ ch = c;};
    friend std::ostream & operator<< (std::ostream & s, const Polynom_exception_string& e);
};


class Polynomial{
    int degree;
    double *coef = nullptr;
    void chek_degree();
    friend int find_degree(char*);
    bool is_digit(char s);
    friend bool is_sup_char(char s);
public:

    Polynomial(char *s);
    Polynomial(const Polynomial & p);
    Polynomial();
    
    Polynomial operator+ (const Polynomial &p);
    Polynomial operator=(const Polynomial &p);
    Polynomial operator-(const Polynomial &p);
    Polynomial operator*(const Polynomial &p);
    Polynomial operator/(const Polynomial &p);
    Polynomial operator%(const Polynomial &p);
    
    Polynomial& operator+=(const Polynomial &p);
    Polynomial& operator-=(const Polynomial &p);
    Polynomial& operator*=(const Polynomial &p);
    Polynomial& operator/=(const Polynomial &p);
    Polynomial& operator%=(const Polynomial &p);
    
    double operator() (double x) const;

    double def_integreal(double a, double b) const;
    Polynomial indef_integral() const;
    Polynomial differentiate() const;
    friend std::ostream& operator<<(std::ostream& f, const Polynomial &p);
    friend std::istream& operator>>(std::istream& f, Polynomial &p);
    

    ~Polynomial();
};