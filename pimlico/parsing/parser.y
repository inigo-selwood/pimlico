%code requires {

    #include "../grammar/grammar.hpp"

    extern Root *root;

    extern Root *parse();

    int yylex(void);
    void yyerror(const char *);

}

%union {
    std::vector<Rule> *rule_list;
    Rule *rule;

    Term *term;

    std::string *string;
    int integer;
    char character;
}

%type <rule_list> RULE_LIST
%type <rule> RULE

%type <term> SEQUENCE
%type <term> CHOICE
%type <term> PREDICATED_TERM
%type <term> INSTANCED_TERM
%type <term> TERM
%type <term> CONSTANT
%type <term> RANGE
%type <term> REFERENCE

%token <string> _IDENTIFIER
%token <string> _CONSTANT
%token <integer> _INTEGER
%token <character> _CHARACTER

%token _NEWLINE
%token '[' ']' '(' ')' '{' '}' '+' '*' '?' ':' '-' '&' '!' '|'

%start ROOT

%%

ROOT
    : _NEWLINE RULE_LIST {
        root = new Root(*$2);
        delete $2;
        $2 = nullptr;
    }
    | RULE_LIST {
        root = new Root(*$1);
        delete $1;
        $1 = nullptr;
    }
    | RULE_LIST _NEWLINE {
        root = new Root(*$1);
        delete $1;
        $1 = nullptr;
    }
    | _NEWLINE RULE_LIST _NEWLINE {
        root = new Root(*$2);
        delete $2;
        $2 = nullptr;
    }

RULE_LIST
    : RULE {
        std::vector<Rule> *rules = new std::vector<Rule>();

        if($1) {
            rules->push_back(*$1);
            delete $1;
            $1 = nullptr;
        }

        $$ = rules;
    }
    | RULE_LIST _NEWLINE RULE {
        if($3) {
            $1->push_back(*$3);
            delete $3;
            $3 = nullptr;
        }

        $$ = $1;
    }

RULE
    : _IDENTIFIER ':' SEQUENCE {
        std::string name(*$1);
        delete $1;
        $1 = nullptr;

        Term sequence(*$3);
        delete $3;
        $3 = nullptr;

        $$ = new Rule(name, sequence);
    }
    | _NEWLINE {
        $$ = nullptr;
    }

SEQUENCE
    : CHOICE {
        std::vector<Term> value;
        value.push_back(*$1);
        delete $1;
        $1 = nullptr;

        Term *sequence = new Term();
        sequence->type = Term::Type::SEQUENCE;
        sequence->value = value;

        $$ = sequence;
    }
    | SEQUENCE CHOICE {
        std::vector<Term> &value = std::get<std::vector<Term>>($1->value);
        value.push_back(*$2);
        delete $2;
        $2 = nullptr;

        $$ = $1;
    }

CHOICE
    : PREDICATED_TERM {
        std::vector<Term> value;
        value.push_back(*$1);
        delete $1;
        $1 = nullptr;

        Term *choice = new Term();
        choice->type = Term::Type::CHOICE;
        choice->value = value;

        $$ = choice;
    }
    | CHOICE '|' PREDICATED_TERM {
        std::vector<Term> &value = std::get<std::vector<Term>>($1->value);
        value.push_back(*$3);
        delete $3;
        $3 = nullptr;

        $$ = $1;
    }

PREDICATED_TERM
    : INSTANCED_TERM
    | '&' INSTANCED_TERM {
        $2->predicate = Term::Predicate::AND;
        $$ = $2;
    }
    | '!' INSTANCED_TERM {
        $2->predicate = Term::Predicate::NOT;
        $$ = $2;
    }

INSTANCED_TERM
    : TERM '+' {
        $1->bounds = Term::Bounds({1, -1});
        $$ = $1;
    }
    | TERM '*' {
        $1->bounds = Term::Bounds({0, -1});
        $$ = $1;
    }
    | TERM '?' {
        $1->bounds = Term::Bounds({0, 1});
        $$ = $1;
    }
    | TERM '{' _INTEGER '}'                 {
        $1->bounds = Term::Bounds({$3, $3});
        $$ = $1;
    }
    | TERM '{' _INTEGER ':' '}'             {
        $1->bounds = Term::Bounds({$3, -1});
        $$ = $1;
    }
    | TERM '{' _INTEGER ':' _INTEGER '}'    {
        $1->bounds = Term::Bounds({$3, $5});
        $$ = $1;
    }
    | TERM '{' ':' _INTEGER '}'             {
        $1->bounds = Term::Bounds({-1, $4});
        $$ = $1;
    }
    | TERM

TERM
    : CONSTANT
    | RANGE
    | REFERENCE
    | '(' SEQUENCE ')' { $$ = $2; }

CONSTANT : _CONSTANT {

        // Create term
        Term *constant = new Term();
        constant->type = Term::Type::CONSTANT;
        constant->value = std::string(*$1);

        // Delete constant
        delete $1;
        $1 = nullptr;

        $$ = constant;
    }

RANGE
    : '[' _CHARACTER '-' _CHARACTER ']' {

        // Create term
        Term *range = new Term();
        range->type = Term::Type::RANGE;
        range->value = std::array<char, 2>({$2, $4});

        $$ = range;
    }

REFERENCE : _IDENTIFIER {

        // Create term
        Term *reference = new Term();
        reference->type = Term::Type::REFERENCE;
        reference->value = std::string(*$1);

        // Delete identifier
        delete $1;
        $1 = nullptr;

        $$ = reference;
    }

%%

Root *root;

Root *parse() {
    root = nullptr;
    yyparse();
    return root;
}
