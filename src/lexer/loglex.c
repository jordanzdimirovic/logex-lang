// Lexer for LogexLang
#include "loglex.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

// extern const Keyword keywords[N_KEYWORDS];

void writeError(const char* msg, int line, int pos, LexError* out){
    (*out).line = line;
    (*out).position = pos;
    strcpy((*out).message, msg);
}

// === Lexing helpers ===

char isAlpha(char c){
    // Returns true if the character is alphabetic (A to z)
    return c >= 'A' && c <= 'z';
}

char isKeyword(char* target, Keyword* res){
    for(int i = 0; i < N_KEYWORDS; i++){
        printf("Checking if %s == %s\n", target, keywords[i].literal);
        if(strcmp(target, keywords[i].literal) == 0){
            memcpy(res, &(keywords[i]), sizeof(Keyword));
            return 1;
        }
    }
    
    return 0;

}

//

int generateTokens(const char* content, int lineNo, LexResult* resOut, LexError* errorOut){
    int currentPos = 0;
    int nTokens = 0;
    Token tokens[MAX_TOKENS_PER_LINE] = {{0}};
    while(content[currentPos] != '\0'){
        char currentChar = content[currentPos];
        printf("Current char is |%c|\n", currentChar);
        if(nTokens >= MAX_TOKENS_PER_LINE){
            writeError("Token overflow - too many tokens", lineNo, currentPos, errorOut);
            return 1;
        }

        else if(currentChar == ' '){
            currentPos += 1;
            continue;
        }

        else if(currentChar == '#'){
            break;
        }

        #pragma region  Operators
        else if(currentChar == '+'){
            tokens[nTokens].type = PLUS;
            tokens[nTokens].lexeme[0] = '+';
            tokens[nTokens].line = lineNo;
            tokens[nTokens].position = currentPos;
            tokens[nTokens].length = 1;
            nTokens++;

            currentPos += 1;
        } else if(currentChar == '-'){
            tokens[nTokens].type = MINUS;
            tokens[nTokens].lexeme[0] = '-';
            tokens[nTokens].line = lineNo;
            tokens[nTokens].position = currentPos;
            tokens[nTokens].length = 1;
            nTokens++;

            currentPos += 1;
        } else if(currentChar == '*'){
            tokens[nTokens].type = TIMES;
            tokens[nTokens].lexeme[0] = '*';
            tokens[nTokens].line = lineNo;
            tokens[nTokens].position = currentPos;
            tokens[nTokens].length = 1;
            nTokens++;

            currentPos += 1;
        } else if(currentChar == '/'){
            tokens[nTokens].type = DIVIDE;
            tokens[nTokens].lexeme[0] = '/';
            tokens[nTokens].line = lineNo;
            tokens[nTokens].position = currentPos;
            tokens[nTokens].length = 1;
            nTokens++;

            currentPos += 1;
        } else if(currentChar == '='){
            tokens[nTokens].type = EQ;
            tokens[nTokens].lexeme[0] = '=';
            tokens[nTokens].line = lineNo;
            tokens[nTokens].position = currentPos;
            tokens[nTokens].length = 1;
            nTokens++;

            currentPos += 1;
        } else if(currentChar == '('){ 
            tokens[nTokens].type = LPAREN;
            tokens[nTokens].lexeme[0] = '(';
            tokens[nTokens].line = lineNo;
            tokens[nTokens].position = currentPos;
            tokens[nTokens].length = 1;
            nTokens++;

            currentPos += 1;
        } else if(currentChar == ')'){ 
            tokens[nTokens].type = RPAREN;
            tokens[nTokens].lexeme[0] = ')';
            tokens[nTokens].line = lineNo;
            tokens[nTokens].position = currentPos;
            tokens[nTokens].length = 1;
            nTokens++;

            currentPos += 1;
        }
        #pragma endregion

        else if(currentChar == '"'){
            // Process string
            int strPointer = 1;
            while(content[currentPos + strPointer] != '"'){
                if(content[currentPos + strPointer] == '\0'){
                    writeError("Unexpected end of string", lineNo, currentPos, errorOut);
                    return 1;
                }
                // Add to lexeme
                tokens[nTokens].lexeme[strPointer - 1] = content[currentPos + strPointer];
                strPointer ++;
            }

            tokens[nTokens].type = STRING;
            tokens[nTokens].line = lineNo;
            tokens[nTokens].position = currentPos;
            tokens[nTokens].length = strPointer;
            
            // Set current char to next char in string
            currentPos += strPointer + 1;

            nTokens ++;
        } else if(isdigit(currentChar)){
            int dotCount = 0;
            int numPointer = 0;
            while(isdigit(content[currentPos + numPointer]) || content[currentPos + numPointer] == '.'){
                if(content[currentPos + numPointer] == '.'){
                    dotCount ++;
                }

                tokens[nTokens].lexeme[numPointer] = content[currentPos + numPointer];

                numPointer ++;
            }

            // If the last value was a dot
            if(tokens[nTokens].lexeme[numPointer - 1] == '.'){
                writeError("Float cannot finish with a dot", lineNo, currentPos, errorOut);
                return 1;
            }

            if(dotCount == 0){
                tokens[nTokens].type = INTEGER;
            } else if(dotCount == 1){
                tokens[nTokens].type = FLOAT;
            } else{
                writeError("Too many dots in float", lineNo, currentPos, errorOut);
                return 1;
            }

            tokens[nTokens].length = numPointer;
            tokens[nTokens].line = lineNo;
            tokens[nTokens].position = currentPos;

            currentPos += numPointer;

            nTokens++;

        } else if(isAlpha(currentChar)){
            // Name
            int namePointer = 0;
            char currentlyKeyword = 0;
            Keyword kw;
            while(
                isAlpha(content[currentPos + namePointer]) || 
                isdigit(content[currentPos + namePointer]) ||
                content[currentPos + namePointer] == '_'
            ){
                
                tokens[nTokens].lexeme[namePointer] = content[currentPos + namePointer];
                // Check if lexeme is a keyword
                currentlyKeyword = isKeyword(tokens[nTokens].lexeme, &kw);
                namePointer ++;

            }
            
            if(currentlyKeyword){
                // Set token type
                tokens[nTokens].type = kw.type;
            }

            else{
                // Set token type
                tokens[nTokens].type = NAME;
            }

            tokens[nTokens].length = namePointer;
            tokens[nTokens].line = lineNo;
            tokens[nTokens].position = currentPos;

            currentPos += namePointer;

            nTokens++;

        }

        


    }

    // Request sufficient heap memory
    Token* result = malloc(sizeof(Token) * nTokens);

    // Copy from stack to heap
    memcpy(result, tokens, sizeof(Token) * nTokens);

    LexResult res = {.tokens = result, .nTokens = nTokens};
    memcpy(resOut, &res, sizeof(LexResult));

    return 0;
}