#include <iostream>
#include "lexer.h"

using std::cerr;
using std::endl;

int getToken() {
    static char cur_char = ' ';

    while (isspace(cur_char)) {
        cur_char = getchar();
    }

    if (isalpha(cur_char)) {
        input = cur_char;

        while (isalnum(cur_char = getchar())) {
            input += cur_char;
            cur_char = getchar();
        }

        return identifyToken(input);
    }
    
    if (isdigit(cur_char)) {
        string n = "";
        bool has_point = false;

        do {
            n += cur_char;
            cur_char = getchar();

            if (has_point && cur_char == '.') {
                cerr << "Too many decimal points in number." << endl;
                exit(1);
            }
            
            if (cur_char == '.') {
                has_point = true;
            }
        } while (isdigit(cur_char) || cur_char == '.');

        number = std::stod(n);
        
        return tok_number;
    }
    
    if (cur_char == '\"' || cur_char == '\'') {
        char quote = cur_char;

        str += cur_char;
        cur_char = getchar();

        while (cur_char != quote && cur_char != EOF && cur_char != '\n' && cur_char != '\r') {
            str += cur_char;
            cur_char = getchar();
        }

        if (cur_char == quote) {
            str += cur_char;
            cur_char = getchar();

            return tok_string;
        } else {
            cerr << "Missing closing quote." << endl;
            exit(1);
        }
    }
    
    if (cur_char == '/' && getchar() == '/') {
        do {
            cur_char = getchar();
        } while (cur_char != EOF && cur_char != '\n' && cur_char != '\r');

        if (cur_char != EOF) {
            return getToken();
        }
    }
    
    if (cur_char == EOF) {
        return tok_eof;
    }

    int this_char = cur_char;
    cur_char = getchar();

    return this_char;
}

int identifyToken(string s) {
    if (s == "bool") {
        return tok_bool;
    }

    if (s == "numero" || s == "número") {
        return tok_numero;
    }

    if (s == "cadena") {
        return tok_cadena;
    }

    if (s == "nada") {
        return tok_nada;
    }

    if (s == "funcion" || s == "función") {
        return tok_funcion;
    }

    if (s == "cierto") {
        return tok_cierto;
    }

    if (s == "falso") {
        return tok_falso;
    }

    if (s == "si") {
        return tok_si;
    }

    if (s == "sino") {
        return tok_sino;
    }

    if (s == "por") {
        return tok_por;
    }

    if (s == "en") {
        return tok_en;
    }

    if (s == "mientras") {
        return tok_mientras;
    }

    if (s == "parar") {
        return tok_parar;
    }

    if (s == "continuar") {
        return tok_continuar;
    }

    if (s == "regresar") {
        return tok_regresar;
    }

    return tok_identifier;
}