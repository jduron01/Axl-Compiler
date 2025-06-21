#pragma once

#include <string>

using std::string;

enum Token {
    tok_eof,
    
    // Keywords
    tok_bool,
    tok_numero,
    tok_cadena,
    tok_nada,
    tok_funcion,
    tok_cierto,
    tok_falso,
    tok_si,
    tok_sino,
    tok_por,
    tok_en,
    tok_mientras,
    tok_parar,
    tok_continuar,
    tok_regresar,

    // Variables/Data Types
    tok_identifier,
    tok_integer,
    tok_number,
    tok_string
};

static string input;
static double number;
static string str;

int getToken();
int identifyToken(string);