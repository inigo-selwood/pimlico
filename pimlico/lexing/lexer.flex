%option noyywrap

%{

    extern "C" int fileno(FILE *stream);

    #include "../parsing/parser.tab.hpp"

    bool in_range = false;

%}

COMMENT     #.*[\n]
SPACE       [ \t\r]
WHITESPACE  {COMMENT}|{SPACE}

%%

{COMMENT} {}
{SPACE} {}

({SPACE}*([\n]|{COMMENT}))+ { return _NEWLINE; }

"[" {
        in_range = true;
        return '[';
    }
"]" {
        in_range = false;
        return ']';
    }
"(" { return '('; }
")" { return ')'; }
"{" { return '{'; }
"}" { return '}'; }

"+" { return '+'; }
"*" { return '*'; }
"?" { return '?'; }
":" { return ':'; }
"-" { return '-'; }
"&" { return '&'; }
"!" { return '!'; }
"|" { return '|'; }

[0-9]+ {
    const std::string text(yytext);
    if(in_range && text.length() == 1) {
        yylval.character = text[0];
        return _CHARACTER;
    }
    else {
        yylval.integer = std::stoi(text);
        return _INTEGER;
    }
}

[a-zA-Z0-9_]+ {
    const std::string text(yytext);
    if(in_range && text.length() == 1) {
        yylval.character = text[0];
        return _CHARACTER;
    }
    else {
        yylval.string = new std::string(text);
        return _IDENTIFIER;
    }
}

'[ -~]+' {
    const std::string text(yytext);
    if(in_range && text.length() == 1) {
        yylval.character = text[0];
        return _CHARACTER;
    }
    else {
        yylval.string = new std::string(text.substr(1, text.length() - 2));
        return _CONSTANT;
    }
}

[ -~] {
    yylval.character = yytext[0];
    return _CHARACTER;
}

%%

void yyerror(const char *error) {
    std::cerr << "syntax error: " << error << '\n';
    exit(1);
}
