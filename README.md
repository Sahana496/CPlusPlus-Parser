# C++ Parser

Developed a bottom-up LR(0) parser generator tool using C++.
Implemented a C++ parser with panic-mode error recovery using this tool and Lex. 

To compile and run the C++ parser - 

```
$ lex lex_file.l
$ g++ parser.cpp lex.yy.c -o parser
$ ./parser program
```

