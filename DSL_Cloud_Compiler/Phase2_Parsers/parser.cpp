#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

vector<string> tokens;
int pos = 0;

string currentToken() {
    if (pos < tokens.size()) return tokens[pos];
    return "";
}

void advance() {
    pos++;
}

void error(string msg) {
    cout << "Syntax Error: " << msg << endl;
    exit(1);
}

void expect(string expected) {
    if (currentToken() == expected) {
        advance();
    } else {
        error("Expected '" + expected + "', got '" + currentToken() + "'");
    }
}

bool isNumber(string s) {
    for (char c : s) if (!isdigit(c)) return false;
    return !s.empty();
}

void parseService() {
    expect("service");

    if (currentToken().empty()) error("Missing service name");
    advance(); // identifier

    expect("{");

    expect("image");
    advance(); // image name

    expect("replicas");
    if (!isNumber(currentToken())) error("replicas must be number");
    advance();

    expect("port");
    if (!isNumber(currentToken())) error("port must be number");
    advance();

    expect("}");
}

int main() {
    ifstream file("sample_input.dsl");

    if (!file.is_open()) {
        cout << "Could not open DSL file\n";
        return 1;
    }

    string line, word;
    while (getline(file, line)) {
        stringstream ss(line);
        while (ss >> word) tokens.push_back(word);
    }

    parseService();

    cout << "Syntax is valid\n";
    return 0;
}