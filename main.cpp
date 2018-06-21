#include <iostream>
#include "titsmall.h"

using namespace std;

int main()
{
    uint32_t eee = 5;
    TitSmall ts(5, 5);
    ts.printHeader("rrrrrrr");
    ts.printFrame(std::cout);
    return 0;
}

