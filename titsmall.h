#ifndef SMALLTIT_H
#define SMALLTIT_H

#include <cstdint>

#include <stdio.h>
#include <cstdint>
#include <iostream>

class TitSmall {
    uint32_t wid_, hei_;

    const uint32_t dataSize_;
    const char* data_;

public:

    TitSmall(uint32_t w, uint32_t h) : dataSize_(w * h * 3), data_(new char[w * h * 3]), wid_(w), hei_(h) {


    }

    void printFrame(std::ostream& out) {
        out.write(data_, dataSize_);
        //std::cout << "e/n";
    }

    void printHeader(const char* msg) {
        printf("P6\n#%s\n%u %u\n255\n", msg, wid_, hei_);
    }

    void setPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b) {

    }

};

#endif // SMALLTIT_H
