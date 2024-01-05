#include "Rule.hpp"
#include "Production.hpp"
#include "CFG.hpp"
using namespace std;

int main()
{
    CFG cfg = CFG();
    cfg.parse_file("./2_cfg/cfg.txt");
    cout << cfg;
    return 0;
}