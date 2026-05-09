#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

vector<string> tokens;
int pos = 0;

// Flags to track fields
bool hasImage = false;
bool hasReplicas = false;
bool hasPort = false;

string currentToken() {
    if (pos < tokens.size()) return tokens[pos];
    return "";
}

void advance() {
    pos++;
}

void error(string msg) {
    cout << "Semantic Error: " << msg << endl;
    exit(1);
}

bool isNumber(string s) {
    for (char c : s) if (!isdigit(c)) return false;
    return !s.empty();
}

void semanticCheck() {
    if (currentToken() != "service") error("Missing 'service' keyword");
    advance();

    if (currentToken().empty()) error("Missing service name");
    advance();

    if (currentToken() != "{") error("Missing '{'");
    advance();

    while (currentToken() != "}") {

        if (currentToken() == "image") {
            hasImage = true;
            advance();
            if (currentToken().empty()) error("Missing image name");
            advance();
        }

        else if (currentToken() == "replicas") {
            hasReplicas = true;
            advance();

            if (!isNumber(currentToken()))
                error("replicas must be a number");

            int value = stoi(currentToken());
            if (value <= 0)
                error("replicas must be greater than 0");

            advance();
        }

        else if (currentToken() == "port") {
            hasPort = true;
            advance();

            if (!isNumber(currentToken()))
                error("port must be a number");

            int value = stoi(currentToken());
            if (value < 1 || value > 65535)
                error("port must be between 1 and 65535");

            advance();
        }

        else {
            error("Unknown field: " + currentToken());
        }
    }

    advance(); // skip }

    if (!hasImage) error("Missing image field");
    if (!hasReplicas) error("Missing replicas field");
    if (!hasPort) error("Missing port field");
}

int main() {
    string filename;
    cout << "Enter DSL file name: ";
    cin >> filename;

    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Could not open file\n";
        return 1;
    }

    string line, word;
    while (getline(file, line)) {
        stringstream ss(line);
        while (ss >> word) tokens.push_back(word);
    }

    semanticCheck();

    cout << "Semantic Analysis Passed\n";
    return 0;
}