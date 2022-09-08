#include "loglex.h"
#include <stdio.h>

int main(){
    
    LexResult res;
    LexError err;
    int status = generateTokens("when x = false", 1, &res, &err);
    if(status == 0){
        for(int i = 0; i < res.nTokens; i++){
            Token currToken = res.tokens[i];
            printf("'%s' -> Type: %d\n", currToken.lexeme, currToken.type);
        }
        
    } else {
        printf(err.message);
    }
    return 0;
}