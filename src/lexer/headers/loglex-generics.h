// DEFINITIONS
#ifndef LOGLEXGENERICS
#define LOGLEXGENERICS

#define MAX_LEXEME_LENGTH 128
#define MAX_TOKENS_PER_LINE 128
#define N_KEYWORDS 6

// Define token types
typedef enum {
    FLOAT,
    INTEGER,
    STRING,
    BOOL_TRUE,
    BOOL_FALSE,
    ASSIGN,
    PLUS,
    MINUS,
    TIMES,
    DIVIDE,
    MODULO,
    EQ,
    IF,
    WHILE,
    FOR,
    FUNC_DEF,
    LPAREN,
    RPAREN,
    NAME,
    ILLEGAL,
    EOL
} TokenType;

// Define keywords
typedef struct {
    char* literal;
    TokenType type;
} Keyword;

extern const Keyword keywords[N_KEYWORDS];

// Struct to represent a token
typedef struct {
    TokenType type;
    char lexeme[MAX_LEXEME_LENGTH];
    int line;
    int position;
    int length;
} Token;

typedef struct {
    Token* tokens;
    int nTokens;
} LexResult;

typedef struct  {
    int line;
    int position;
    char message[128];
} LexError;

#endif