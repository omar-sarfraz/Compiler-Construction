#pragma once
#include <vector>
#include <string>
#include <sstream>
#include "Rule.hpp"

using namespace std;

class Production
{
private:
    string name;
    vector<Rule> rules;

public:
    Production() : name(""), rules(){};
    void add_rules(string rules);
    const string get_name();
    void set_name(string name);
    const vector<Rule> get_rules();
    friend ostream &operator<<(ostream &os, const Production &r);
};

void Production::add_rules(string rules)
{
    stringstream linestream(rules);
    string rule;
    while (getline(linestream, rule, '|'))
    {
        Rule r = Rule(rule);
        this->rules.push_back(r);
    }
}

const string Production::get_name()
{
    return this->name;
}

void Production::set_name(string name)
{
    this->name = name;
}
const vector<Rule> Production::get_rules()
{
    return this->rules;
}

ostream &operator<<(ostream &os, const Production &p)
{
    for (Rule r : p.rules)
    {
        os << p.name << " -> ";
        os << r << endl;
    }
    return os;
}