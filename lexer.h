#pragma once

#include <string>
#include <vector>

enum Token {
    tok_eof = -1,
    tok_comment = -2,
    
    // Keywords
    tok_bool = -3,
    tok_numero = -4,
    tok_cadena = -5,
    tok_nada = -6,
    tok_funcion = -7,
    tok_cierto = -8,
    tok_falso = -9,
    tok_si = -10,
    tok_sino = -11,
    tok_por = -12,
    tok_en = -13,
    tok_mientras = -14,
    tok_parar = -15,
    tok_continuar = -16,
    tok_regresar = -17,

    // Variables/Data Types
    tok_identifier = -18,
    tok_number = -19,
    tok_string = -20,

    // Multi-character operators
    tok_increment = -21,
    tok_decrement = -22,
    tok_equal = -23,
    tok_not_equal = -24,
    tok_less_equal = -25,
    tok_greater_equal = -26,
    tok_and = -27,
    tok_or = -28,
    tok_add_assign = -29,
    tok_sub_assign = -30,
    tok_mul_assign = -31,
    tok_div_assign = -32,
    tok_mod_assign = -33,
    tok_ellipsis = -34,
    tok_arrow = -35
};

extern unsigned int line;
extern char *cur_pos;
extern std::string identifier;
extern double number;
extern std::string str;

std::vector<char> readFile(const char *);
void debug();
int scan();
int scanIdentifier();
int scanNumber();
int scanString();
int scanComment();
void printError(const std::string);