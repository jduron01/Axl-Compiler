#include <iostream>
#include "lexer.h"
#include "parser.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: axlc <file name>.axl" << std::endl;
        return 1;
    }

    std::vector<char> contents = readFile(argv[1]);
    cur_pos = contents.data();

    parse();

    return 0;
}

void parse() {
    next();

    while (type != tok_eof) {
        parseProgram();
    }

    std::cout << "Parsing finished." << std::endl;
}

void parseProgram() {
    switch (type) {
        case tok_funcion:
            expect(tok_funcion);
            parsePrototype();

            if (type == ';') {
                expect(';');
            } else {
                parseBody();
            }

            break;
        case tok_bool:
            expect(tok_bool);
            parseDeclaration();
            break;
        case tok_numero:
            expect(tok_numero);
            parseDeclaration();
            break;
        case tok_cadena:
            expect(tok_cadena);
            parseDeclaration();
            break;
        default:
            expect(tok_comment);
            break;
    }
}

void parsePrototype() {
    expect(tok_identifier);
    expect('(');

    while (type != ')') {
        switch (type) {
            case tok_bool:
                expect(tok_bool);
                break;
            case tok_numero:
                expect(tok_numero);
                break;
            case tok_cadena:
                expect(tok_cadena);
                break;
            case tok_nada:
                expect(tok_nada);
                break;
        }

        if (type == '*') {
            expect('*');
        } else if (type == '&') {
            expect('&');
        }

        expect(tok_identifier);

        if (type == ',') {
            expect(',');
        }
    }

    expect(')');
    expect(tok_arrow);

    switch (type) {
        case tok_bool:
            expect(tok_bool);
            break;
        case tok_numero:
            expect(tok_numero);
            break;
        case tok_cadena:
            expect(tok_cadena);
            break;
        case tok_nada:
            expect(tok_nada);
            break;
    }

    if (type == '*') {
        expect('*');
    } else if (type == '&') {
        expect('&');
    }
}

void parseBody() {
    expect('{');

    while (type != '}') {
        parseStatement();
    }

    expect('}');
}

void parseStatement() {
    switch (type) {
        case tok_si:
            expect(tok_si);
            parseIf();
            break;
        case tok_mientras:
            expect(tok_mientras);
            parseWhile();
            break;
        case tok_por:
            expect(tok_por);
            parseFor();
            break;
        case tok_parar:
            expect(tok_parar);
            expect(';');
            break;
        case tok_continuar:
            expect(tok_continuar);
            expect(';');
            break;
        case tok_regresar:
            expect(tok_regresar);
            parseReturn();
            break;
        case tok_bool:
            expect(tok_bool);
            parseDeclaration();
            break;
        case tok_numero:
            expect(tok_numero);
            parseDeclaration();
            break;
        case tok_cadena:
            expect(tok_cadena);
            parseDeclaration();
            break;
        case tok_identifier:
            expect(tok_identifier);
            parseCall();
            expect(';');
            break;
        default:
            while (type != ';') {
                parseExpression();
            }

            expect(';');
            break;
    }
}

void parseIf() {
    parseExpression();
    parseBody();

    while (type == tok_sino) {
        expect(tok_sino);

        if (type == tok_si) {
            expect(tok_si);
            parseExpression();
        }

        parseBody();
    }
}

void parseWhile() {
    parseExpression();
    parseBody();
}

void parseFor() {
    expect(tok_identifier);
    expect(tok_en);
    parseExpression();

    if (type == tok_ellipsis) {
        expect(tok_ellipsis);
        parseExpression();
    }

    parseBody();
}

void parseReturn() {
    if (type != ';') {
        parseExpression();
    }

    expect(';');
}

void parseDeclaration() {
    if (type == '*') {
        expect('*');
    } else if (type == '&') {
        expect('&');
    }

    expect(tok_identifier);

    if (type == '[') {
        expect('[');

        if (type != ']') {
            parseExpression();
        }

        expect(']');
    }

    while (type != ';') {
        if (type == '=') {
            expect('=');

            if (type == '[') {
                expect('[');

                while (type != ']') {
                    parseExpression();

                    if (type == ',') {
                        expect(',');
                    }
                }

                expect(']');
            } else {
                parseExpression();
            }
        }

        if (type == ',') {
            expect(',');

            if (type == '*') {
                expect('*');
            } else if (type == '&') {
                expect('&');
            }

            expect(tok_identifier);
        }
    }

    expect(';');
}

void parseCall() {
    expect('(');

    while (type != ')') {
        parseExpression();

        if (type == ',') {
            expect(',');
        }
    }

    expect(')');
}

void parseExpression() {
    parseAssignment();
}

void parseAssignment() {
    parseOr();

    switch (type) {
        case '=':
            expect('=');
            parseExpression();
            break;
        case tok_add_assign:
            expect(tok_add_assign);
            parseExpression();
            break;
        case tok_sub_assign:
            expect(tok_sub_assign);
            parseExpression();
            break;
        case tok_mul_assign:
            expect(tok_mul_assign);
            parseExpression();
            break;
        case tok_div_assign:
            expect(tok_div_assign);
            parseExpression();
            break;
        case tok_mod_assign:
            expect(tok_mod_assign);
            parseExpression();
            break;
    }
}

void parseOr() {
    parseAnd();

    while (type == tok_or) {
        expect(tok_or);
        parseAnd();
    }
}

void parseAnd() {
    parseEquality();

    while (type == tok_and) {
        expect(tok_and);
        parseEquality();
    }
}

void parseEquality() {
    parseRelational();

    while (type == tok_equal || type == tok_not_equal) {
        switch (type) {
            case tok_equal:
                expect(tok_equal);
                break;
            case tok_not_equal:
                expect(tok_not_equal);
                break;
        }

        parseRelational();
    }
}

void parseRelational() {
    parseAdditive();

    while (type == '<' || type == tok_less_equal ||
           type == '>' || type == tok_greater_equal) {
        switch (type) {
            case '<':
                expect('<');
                break;
            case tok_less_equal:
                expect(tok_less_equal);
                break;
            case '>':
                expect('>');
                break;
            case tok_greater_equal:
                expect(tok_greater_equal);
                break;
        }

        parseAdditive();
    }
}

void parseAdditive() {
    parseMultiplicative();

    while (type == '+' || type == '-') {
        switch (type) {
            case '+':
                expect('+');
                break;
            case '-':
                expect('-');
                break;
        }

        parseMultiplicative();
    }
}

void parseMultiplicative() {
    parseUnary();

    while (type == '*' || type == '/' || type == '%') {
        switch (type) {
            case '*':
                expect('*');
                break;
            case '/':
                expect('/');
                break;
            case '%':
                expect('%');
                break;
        }

        parseUnary();
    }
}

void parseUnary() {
    while (type == '!' || type == '-' || type == '*' || type == '&' ||
        type == tok_increment || type == tok_decrement) {
        switch (type) {
            case '!':
                expect('!');
                break;
            case '-':
                expect('-');
                break;
            case '*':
                expect('*');
                break;
            case '&':
                expect('&');
                break;
            case tok_increment:
                expect(tok_increment);
                break;
            case tok_decrement:
                expect(tok_decrement);
                break;
        }

    }

    parsePrimary();

    while (type == tok_increment || type == tok_decrement) {
        switch (type) {
            case tok_increment:
                expect(tok_increment);
                break;
            case tok_decrement:
                expect(tok_decrement);
                break;
        }
    }
}

void parsePrimary() {
    switch (type) {
        case '(':
            expect('(');
            parseExpression();
            expect(')');
            break;
        case tok_identifier:
            expect(tok_identifier);

            if (type == '(') {
                parseCall();
            } else if (type == '[') {
                expect('[');
                parseExpression();
                expect(']');
            }

            break;
        case tok_cierto:
            expect(tok_cierto);
            break;
        case tok_falso:
            expect(tok_falso);
            break;
        case tok_number:
            expect(tok_number);
            break;
        case tok_string:
            expect(tok_string);
            break;
    }
}

void expect(int match) {
    if (match != type) {
        printError("syntax error");
    }

    next();
}

void next() {
    type = scan();
}