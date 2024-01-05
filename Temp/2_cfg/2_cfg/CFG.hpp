#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cerrno>
#include <cstring>
#include "Production.hpp"

using namespace std;

class CFG
{
private:
    vector<Production> productions;

public:
    CFG() : productions(){};
    void parse_file(string file_path);
    const vector<Production> get_productions();
    friend ostream &operator<<(ostream &os, const CFG &cfg);
};

void CFG::parse_file(string file_path)
{
    productions.clear();
    ifstream read_file(file_path);
    if (!read_file.is_open())
    {
        cerr << "Error while opening file: " << file_path << endl;
        cerr << strerror(errno) << endl;
        read_file.close();
        exit(-1);
    }
    string current_line = "";
    while (getline(read_file, current_line))
    {
        Production temp_production = Production();
        string token = "";
        stringstream linestream(current_line);
        getline(linestream, token, ':');
        temp_production.set_name(token);
        getline(linestream, token);
        temp_production.add_rules(token);
        this->productions.push_back(temp_production);
    }
    read_file.close();
}

const vector<Production> CFG::get_productions()
{
    return this->productions;
}

ostream &operator<<(ostream &os, const CFG &cfg)
{
    for (Production p : cfg.productions)
    {
        os << p;
    }
    return os;
}