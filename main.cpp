#include <iostream>
#include "titsmall.h"

using namespace std;

int main()
{
    uint32_t eee = 5;
    TitSmall ts(1280, 720);
    for (int i = 0; i < 60*60; i ++) {
        ts.randomize(5.0 * i);
        ts.printHeader("TITSMALL PNM test");
        ts.printFrame(std::cout);
    }
    return 0;
}

