#include "brainfuck.h"
#include <string>
#include <vector>
#include <stdexcept>


Command::Command(char value) {
    this->value = value;
}

void Command::run(Interpreter &interpreter) {
    if (value == '>') {
        interpreter.data_pointer++;
    } else if (value == '<') {
        interpreter.data_pointer--;
    } else if (value == '+') {
        interpreter.data[interpreter.data_pointer]++;
    } else if (value == '-') {
        interpreter.data[interpreter.data_pointer]--;
    } else if (value == '.') {
        interpreter.output.push_back(interpreter.data[interpreter.data_pointer]);
    } else {
        throw std::invalid_argument("Invalid command value");
    }
}


Loop::Loop(std::vector<Runnable*> &instructions) {
    this->instructions = instructions;
}

void Loop::run(Interpreter &interpreter) {
    while (interpreter.data[interpreter.data_pointer] != 0) {
        for (unsigned int i = 0; i < instructions.size(); i++) {
            instructions[i]->run(interpreter);
        }
    }
}


std::vector<Runnable*> Parser::parse(std::string &source) {
    std::vector<char> tokens = tokenize(source);
    validate_correctness(tokens);

    return compile_program(tokens);
}


std::vector<char> Parser::tokenize(std::string &source) {
    std::vector<char> tokens;

    for (unsigned int i = 0; i < source.length(); i++) {
        if (source.at(i) == '+') {
            tokens.push_back('+');
        } else if (source.at(i) == '-') {
            tokens.push_back('-');
        } else if (source.at(i) == '<') {
            tokens.push_back('<');
        } else if (source.at(i) == '>') {
            tokens.push_back('>');
        } else if (source.at(i) == '.') {
            tokens.push_back('.');
        } else if (source.at(i) == '[') {
            tokens.push_back('[');
        } else if (source.at(i) == ']') {
            tokens.push_back(']');
        } else if ((source.at(i) == ' ') || (source.at(i) == '\n') || (source.at(i) == '\t')) {
            continue;
        } else {
            throw std::invalid_argument("Invalid symbol in program source");
        }
    }

    return tokens;
}

void Parser::validate_correctness(std::vector<char> &tokens) {
    std::vector<char> stack;

    for (unsigned int i = 0; i < tokens.size(); i++) {
        if (tokens[i] == '[') {
            stack.push_back('[');
        } else if (tokens[i] == ']') {
            if (stack.empty()) {
                throw std::invalid_argument("Brackets mismatch");
            } else {
                stack.pop_back();
            }
        }
    }

    if (stack.size() > 0) {
        throw std::invalid_argument("Brackets mismatch");
    }
}

std::vector<Runnable*> Parser::compile_program(std::vector<char> &tokens) {
    std::vector<Runnable*> instructions;

    unsigned int i = 0;
    while (i < tokens.size()) {
        if ((tokens[i] == '+')
            || (tokens[i] == '-')
            || (tokens[i] == '<')
            || (tokens[i] == '>')
            || (tokens[i] == '.')
        ) {
            Command *command = new Command(tokens[i]);
            instructions.push_back(command);
        } else if (tokens[i] == '[') {
            int cb_index = find_closing_bracket_index(tokens, i);
            std::vector<char> sub_tokens(tokens.begin() + i + 1, tokens.begin() + cb_index);
            std::vector<Runnable*> sub_instructions = compile_program(sub_tokens);
            Loop *loop = new Loop(sub_instructions);
            instructions.push_back(loop);
            i = cb_index;
        } else {
            throw std::logic_error("[Parser::compile_program()] This point shouldn't have been reached");
        }

        i++;
    }
    return instructions;
}

int Parser::find_closing_bracket_index(std::vector<char> &tokens, int opening_bracket_index) {
    std::vector<char> stack;

    for (unsigned int i = opening_bracket_index + 1; i < tokens.size(); i++) {
        if (tokens[i] == ']') {
            if (stack.empty()) {
                return i;
            } else {
                stack.pop_back();
            }
        } else if (tokens[i] == '[') {
            stack.push_back('[');
        }
    }

    throw std::logic_error("[Parser::find_closing_bracket_index()] This point shouldn't have been reached");
}


Interpreter::Interpreter() {
    std::vector<int> v(30000, 0);
    data = v;
}

std::vector<char> Interpreter::execute(std::vector<Runnable*> &instructions) {
    reset();

    for (unsigned int i = 0; i < instructions.size(); i++) {
        instructions[i]->run(*this);
    }

    return output;
}

void Interpreter::reset() {
    std::fill(data.begin(), data.end(), 0);
    data_pointer = 0;
    output.clear();
}
