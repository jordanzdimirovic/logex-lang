:: Compile logex lexer
gcc -c -o out/loglex.o -Isrc/lexer/headers src/lexer/loglex.c

:: Compile logex generics
gcc -c -o out/loglex-generics.o -Isrc/lexer/headers src/lexer/loglex-generics.c

:: Compile main C code
gcc -c -o out/logexlang.o -Isrc/lexer/headers src/main/logexlang.c

:: Link and complete compilation
gcc out/* -o build/main.exe