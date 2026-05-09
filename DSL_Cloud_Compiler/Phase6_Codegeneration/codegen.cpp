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

void parse() {

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

void generateDockerfile() {
    ofstream docker("Dockerfile");

    docker << "FROM " << svc.image << "\n";
    docker << "EXPOSE " << svc.port << "\n";

    docker.close();

    cout << "\nDockerfile generated.\n";
}

void generateKubernetesYAML() {
    ofstream yaml("deployment.yaml");

    yaml << "apiVersion: apps/v1\n";
    yaml << "kind: Deployment\n";
    yaml << "metadata:\n";
    yaml << "  name: " << svc.name << "\n";
    yaml << "spec:\n";
    yaml << "  replicas: " << svc.replicas << "\n";
    yaml << "  selector:\n";
    yaml << "    matchLabels:\n";
    yaml << "      app: " << svc.name << "\n";
    yaml << "  template:\n";
    yaml << "    metadata:\n";
    yaml << "      labels:\n";
    yaml << "        app: " << svc.name << "\n";
    yaml << "    spec:\n";
    yaml << "      containers:\n";
    yaml << "      - name: " << svc.name << "\n";
    yaml << "        image: " << svc.image << "\n";
    yaml << "        ports:\n";
    yaml << "        - containerPort: " << svc.port << "\n";

    yaml.close();

    cout << "Kubernetes YAML generated.\n";
}

void printSummary() {
    cout << "\nDeployment Summary:\n\n";
    cout << "Service: " << svc.name << endl;
    cout << "Image: " << svc.image << endl;
    cout << "Replicas: " << svc.replicas << endl;
    cout << "Port: " << svc.port << endl;
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

    parse();

    generateDockerfile();
    generateKubernetesYAML();
    printSummary();

    return 0;
}