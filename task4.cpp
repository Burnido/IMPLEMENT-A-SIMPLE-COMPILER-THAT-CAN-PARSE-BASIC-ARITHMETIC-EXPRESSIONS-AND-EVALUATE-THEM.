#include <iostream>
#include <string>
#include <cctype>
#include <stdexcept>

using namespace std;

class Parser {
    string expr;
    size_t pos;

public:
    Parser(const string& s) : expr(s), pos(0) {}

    double parse() {
        double result = parseExpression();
        skipWhitespace();
        if (pos < expr.length()) throw runtime_error("Unexpected characters at end");
        return result;
    }

private:
    void skipWhitespace() {
        while (pos < expr.length() && isspace(expr[pos])) pos++;
    }

    double parseExpression() {
        double result = parseTerm();
        while (true) {
            skipWhitespace();
            if (pos >= expr.length()) break;

            char op = expr[pos];
            if (op != '+' && op != '-') break;

            pos++;
            double value = parseTerm();
            if (op == '+') result += value;
            else result -= value;
        }
        return result;
    }

    double parseTerm() {
        double result = parseFactor();
        while (true) {
            skipWhitespace();
            if (pos >= expr.length()) break;

            char op = expr[pos];
            if (op != '*' && op != '/') break;

            pos++;
            double value = parseFactor();
            if (op == '*') result *= value;
            else {
                if (value == 0) throw runtime_error("Division by zero");
                result /= value;
            }
        }
        return result;
    }

    double parseFactor() {
        skipWhitespace();
        if (pos >= expr.length()) throw runtime_error("Unexpected end of input");

        char ch = expr[pos];

        if (ch == '(') {
            pos++;
            double result = parseExpression();
            skipWhitespace();
            if (expr[pos] != ')') throw runtime_error("Expected ')'");
            pos++;
            return result;
        }

        if (isdigit(ch) || ch == '.') {
            return parseNumber();
        }

        throw runtime_error(string("Unexpected character: ") + ch);
    }

    double parseNumber() {
        skipWhitespace();
        size_t start = pos;
        while (pos < expr.length() && (isdigit(expr[pos]) || expr[pos] == '.'))
            pos++;
        return stod(expr.substr(start, pos - start));
    }
};

int main() {
    cout << "Enter arithmetic expression (or 'exit' to quit):\n";

    string input;
    while (true) {
        cout << "> ";
        getline(cin, input);

        if (input == "exit") break;

        try {
            Parser parser(input);
            double result = parser.parse();
            cout << "Result: " << result << endl;
        } catch (exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }

    return 0;
}
