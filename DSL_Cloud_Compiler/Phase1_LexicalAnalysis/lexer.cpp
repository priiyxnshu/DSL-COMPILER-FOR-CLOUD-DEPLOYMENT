#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <cctype>

using namespace std;

// DSL keywords
unordered_set<string> keywords = {
    "service", "image", "replicas", "port"
};

// Check if token is number
bool isNumber(const string& token) {
    for (char c : token) {
        if (!isdigit(c)) return false;
    }
    return !token.empty();
}

// Check if character is symbol
bool isSymbol(char c) {
    return c == '{' || c == '}';
}

int main() {
    ifstream file("sample_input.dsl");

    if (!file.is_open()) {
        cout << "Error: Could not open sample_input.dsl\n";
        return 1;
    }

    cout << "Lexical Analysis Output:\n\n";

    string line;
    int lineNumber = 1;

    while (getline(file, line)) {
        string token;
        stringstream ss(line);

        while (ss >> token) {

            // Handle symbols attached to tokens (e.g., web{)
            if (token.size() > 1 && isSymbol(token.back())) {
                string word = token.substr(0, token.size() - 1);
                char symbol = token.back();

                if (!word.empty()) {
                    if (keywords.count(word))
                        cout << "[Line " << lineNumber << "] " << word << " -> KEYWORD\n";
                    else if (isNumber(word))
                        cout << "[Line " << lineNumber << "] " << word << " -> NUMBER\n";
                    else
                        cout << "[Line " << lineNumber << "] " << word << " -> IDENTIFIER\n";
                }

                cout << "[Line " << lineNumber << "] " << symbol << " -> SYMBOL\n";
            }
            else if (token.size() == 1 && isSymbol(token[0])) {
                cout << "[Line " << lineNumber << "] " << token << " -> SYMBOL\n";
            }
            else if (keywords.count(token)) {
                cout << "[Line " << lineNumber << "] " << token << " -> KEYWORD\n";
            }
            else if (isNumber(token)) {
                cout << "[Line " << lineNumber << "] " << token << " -> NUMBER\n";
            }
            else {
                cout << "[Line " << lineNumber << "] " << token << " -> IDENTIFIER\n";
            }
        }

        lineNumber++;
    }

    file.close();
    return 0;
}