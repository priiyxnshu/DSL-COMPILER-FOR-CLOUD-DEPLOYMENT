#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct Service {
    string name;
    string image;
    int replicas;
    int port;
};

vector<string> tokens;
int pos = 0;

Service svc;

string currentToken() {
    if (pos < tokens.size()) return tokens[pos];
    return "";
}

void advance() {
    pos++;
}

void error(string msg) {
    cout << "Error: " << msg << endl;
    exit(1);
}

bool isNumber(string s) {
    for (char c : s) if (!isdigit(c)) return false;
    return !s.empty();
}

void generateIntermediate() {

    if (currentToken() != "service") error("Missing service");
    advance();

    svc.name = currentToken();
    advance();

    if (currentToken() != "{") error("Missing {");
    advance();

    while (currentToken() != "}") {

        if (currentToken() == "image") {
            advance();
            svc.image = currentToken();
            advance();
        }

        else if (currentToken() == "replicas") {
            advance();
            if (!isNumber(currentToken())) error("Invalid replicas");
            svc.replicas = stoi(currentToken());
            advance();
        }

        else if (currentToken() == "port") {
            advance();
            if (!isNumber(currentToken())) error("Invalid port");
            svc.port = stoi(currentToken());
            advance();
        }

        else {
            error("Unknown field: " + currentToken());
        }
    }

    advance();
}

void optimize() {

    // Default values
    if (svc.replicas == 0) {
        svc.replicas = 1;
        cout << "[Optimization] Default replicas set to 1\n";
    }

    // Normalize image name
    transform(svc.image.begin(), svc.image.end(), svc.image.begin(), ::tolower);

    // Port safety check
    if (svc.port < 1024) {
        cout << "[Optimization Warning] Port < 1024 (system reserved)\n";
    }
}

void printOptimized() {

    cout << "\nOptimized Representation:\n\n";

    cout << "SERVICE: " << svc.name << endl;
    cout << "IMAGE: " << svc.image << endl;
    cout << "REPLICAS: " << svc.replicas << endl;
    cout << "PORT: " << svc.port << endl;
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

    generateIntermediate();
    optimize();
    printOptimized();

    return 0;
}