#include <iostream>
#include <fstream>
#include <cstring>
#include <filesystem>
#include "lexer.h"

unsigned int line = 1;
char *cur_pos;
std::string identifier;
double number;
std::string str;

std::vector<char> readFile(const char *path) {
    const char *extension = strrchr(path, '.');

    if (!extension || strcmp(extension, ".axl") != 0) {
        std::cerr << "Error: file must end in '.axl'" << std::endl;
        exit(1);
    }

    std::ifstream file(path, std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Error: could not open file " << path << std::endl;
        exit(1);
    }

    uintmax_t size = std::filesystem::file_size(path);

    std::vector<char> buffer(size + 1);
    file.read(buffer.data(), size);
    buffer[size] = '\0';

    file.close();

    return buffer;
}

int scan() {
    while (*cur_pos == ' ' || *cur_pos == '\t' || *cur_pos == '\n' || *cur_pos == '\r') {
        if (*cur_pos == '\n') {
            line++;
        }

        cur_pos++;
    }

    if (isalpha(*cur_pos) || *cur_pos == '_') {
        return scanIdentifier();
    } else if (isdigit(*cur_pos)) {
        return scanNumber();
    }

    switch (*cur_pos) {
        case '/':
            if (*(cur_pos + 1) == '/') {
                return scanComment();
            } else if (*(cur_pos + 1) == '=') {
                cur_pos += 2;
                identifier = "/=";

                return tok_div_assign;
            }

            cur_pos++;
            return '/';
        case '+':
            if (*(cur_pos + 1) == '+') {
                cur_pos += 2;
                identifier = "++";

                return tok_increment;
            } else if (*(cur_pos + 1) == '=') {
                cur_pos += 2;
                identifier = "+=";

                return tok_add_assign;
            }

            cur_pos++;
            return '+';
        case '-':
            if (*(cur_pos + 1) == '-') {
                cur_pos += 2;
                identifier = "--";

                return tok_decrement;
            } else if (*(cur_pos + 1) == '=') {
                cur_pos += 2;
                identifier = "-=";

                return tok_sub_assign;
            } else if (*(cur_pos + 1) == '>') {
                cur_pos += 2;
                identifier = "->";

                return tok_arrow;
            }

            cur_pos++;
            return '-';
        case '*':
            if (*(cur_pos + 1) == '=') {
                cur_pos += 2;
                identifier = "*=";

                return tok_mul_assign;
            }

            cur_pos++;
            return '*';
        case '%':
            if (*(cur_pos + 1) == '=') {
                cur_pos += 2;
                identifier = "%=";

                return tok_mod_assign;
            }

            cur_pos++;
            return '%';
        case '=':
            if (*(cur_pos + 1) == '=') {
                cur_pos += 2;
                identifier = "==";

                return tok_equal;
            }

            cur_pos++;
            return '=';
        case '!':
            if (*(cur_pos + 1) == '=') {
                cur_pos += 2;
                identifier = "!=";

                return tok_not_equal;
            }

            cur_pos++;
            return '!';
        case '<':
            if (*(cur_pos + 1) == '=') {
                cur_pos += 2;
                identifier = "<=";

                return tok_less_equal;
            }

            cur_pos++;
            return '<';
        case '>':
            if (*(cur_pos + 1) == '=') {
                cur_pos += 2;
                identifier = ">=";

                return tok_greater_equal;
            }

            cur_pos++;
            return '>';
        case '&':
            if (*(cur_pos + 1) == '&') {
                cur_pos += 2;
                identifier = "&&";

                return tok_and;
            }

            cur_pos++;
            return '&';
        case '|':
            if (*(cur_pos + 1) == '|') {
                cur_pos += 2;
                identifier = "||";

                return tok_or;
            }
            
            cur_pos++;
            return '|';
        case '\'':
            return scanString();
        case '"':
            return scanString();
        case '.':
            if (*(cur_pos + 1) == '.' && *(cur_pos + 2) == '.') {
                cur_pos += 3;
                identifier = "...";

                return tok_ellipsis;
            }

            cur_pos++;
            return '.';
        case '\0':
            return tok_eof;
        default:
            char ch = *cur_pos;
            cur_pos++;
            return ch;
    }
}

int scanIdentifier() {
    identifier.clear();
    
    while (isalnum(*cur_pos) || *cur_pos == '_') {
        identifier += *cur_pos++;
    }

    if (identifier == "bool") {
        return tok_bool;
    } else if (identifier == "numero" || identifier == "número") {
        return tok_numero;
    } else if (identifier == "cadena") {
        return tok_cadena;
    } else if (identifier == "nada") {
        return tok_nada;
    } else if (identifier == "funcion" || identifier == "función") {
        return tok_funcion;
    } else if (identifier == "cierto") {
        return tok_cierto;
    } else if (identifier == "falso") {
        return tok_falso;
    } else if (identifier == "si") {
        return tok_si;
    } else if (identifier == "sino") {
        return tok_sino;
    } else if (identifier == "por") {
        return tok_por;
    } else if (identifier == "en") {
        return tok_en;
    } else if (identifier == "mientras") {
        return tok_mientras;
    } else if (identifier == "parar") {
        return tok_parar;
    } else if (identifier == "continuar") {
        return tok_continuar;
    } else if (identifier == "regresar") {
        return tok_regresar;
    }

    return tok_identifier;
}

int scanNumber() {
    std::string n;
    bool has_point = false;

    do {
        n += *cur_pos++;

        if (*cur_pos == '.' && *(cur_pos + 1) == '.' && *(cur_pos + 2) == '.') {
            number = std::stod(n);
            return tok_number;
        } else if (has_point && *cur_pos == '.') {
            printError("too many decimal points in number");
        } else if (*cur_pos == '.') {
            has_point = true;
        }
    } while (isdigit(*cur_pos) || *cur_pos == '.');

    number = std::stod(n);

    return tok_number;
}

int scanString() {
    str.clear();
    char delimiter = *cur_pos;

    if (delimiter != '"' && delimiter != '\'') {
        printError("string must start with ' or \"");
    }

    cur_pos++;

    while (*cur_pos != delimiter && *cur_pos != '\0') {
        if (*cur_pos == '\\' && *(cur_pos + 1) != '\0') {
            cur_pos++;

            switch (*cur_pos) {
                case 'n':
                    str += '\n';
                    break;
                case 't':
                    str += '\t';
                    break;
                case 'r':
                    str += '\r';
                    break;
                case '\\':
                    str += '\\';
                    break;
                case '"':
                    str += '"';
                    break;
                case '\'':
                    str += '\'';
                    break;
                default:
                    str += *cur_pos;
                    break;
            }
        } else {
            str += *cur_pos;
        }

        cur_pos++;
    }

    if (*cur_pos == delimiter) {
        cur_pos++;
    } else {
        printError("unterminated string literal");
    }

    return tok_string;
}

int scanComment() {
    cur_pos += 2;
    
    while (*cur_pos != '\0' && *cur_pos != '\n' && *cur_pos != '\r') {
        cur_pos++;
    }

    if (*cur_pos == '\n') {
        line++;
        cur_pos++;
    } else if (*cur_pos == '\r') {
        cur_pos++;

        if (*cur_pos == '\n') {
            cur_pos++;
        }

        line++;
    }

    return tok_comment;
}

void printError(const std::string &msg) {
    std::cerr << "Error at line " << line << ": " << msg << std::endl;
    exit(1);
}