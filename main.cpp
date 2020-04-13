#include <iostream>
#include <string>
#include <vector>
#include "brainfuck.h"

using namespace std;

int main()
{
    string source = "++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.";

    Parser parser;
    vector<Runnable*> instructions = parser.parse(source);

    Interpreter interpreter;
    vector<char> output = interpreter.execute(instructions);

    for (unsigned int i = 0; i < output.size(); i++) {
        cout << output[i];
    }
    cout << endl;


    return 0;
}
