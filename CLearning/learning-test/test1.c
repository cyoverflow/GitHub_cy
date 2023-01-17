#include <stdio.h>

typedef struct complex{
    double re, im;
} Complex;

Complex make_complex(double, double);
Complex add_complex(Complex *c1,Complex *c2);

int main() {
    Complex c1 = {1.0, 0.0};
    Complex c2 = {1.0, 0.0};
    Complex c3 = {1.0, 0.0};

    Complex c4 = make_complex(1.1, 2.0);

    Complex c5 = add_complex(&c1, &c2);
    printf("%.2lf\n%.2lf", c5.re,c5.im);
}

Complex add_complex(Complex *c1,Complex *c2) {
    Complex c5 = {c1->re + c2->re, c1->im + c2->im};
    return c5;
}

Complex make_complex(double re, double im) {
    Complex com = {re, im};
    return com;
}
