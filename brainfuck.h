#ifndef BRAINFUCK_H_INCLUDED
#define BRAINFUCK_H_INCLUDED

#include <string>
#include <vector>

class Interpreter;

class Runnable {
public:
    virtual void run(Interpreter &interpreter) = 0;
};

class Command : public Runnable {
public:
    Command(char value);

    void run(Interpreter &interpreter);
private:
    char value;
};

class Loop : public Runnable {
public:
    Loop(std::vector<Runnable*> &instructions);

    void run(Interpreter &interpreter);
private:
    std::vector<Runnable*> instructions;
};

class Parser {
public:
    std::vector<Runnable*> parse(std::string &source);
private:
    std::vector<char> tokenize(std::string &source);

    void validate_correctness(std::vector<char> &tokens);

    std::vector<Runnable*> compile_program(std::vector<char> &tokens);

    int find_closing_bracket_index(std::vector<char> &tokens, int opening_bracket_index);
};

class Interpreter {
public:
    std::vector<int> data;
    int data_pointer = 0;
    std::vector<char> output;

    Interpreter();

    std::vector<char> execute(std::vector<Runnable*> &instructions);
private:
    void reset();
};

#endif // BRAINFUCK_H_INCLUDED
