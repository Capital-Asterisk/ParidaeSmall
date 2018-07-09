#ifndef SMALLTIT_H
#define SMALLTIT_H

#include <cstdint>

#include <stdio.h>
#include <cstdint>
#include <iostream>
#include <random>


class TitSmall {
    uint32_t wid_, hei_;

    const uint32_t dataSize_;
    char* data_;

public:

    TitSmall(uint32_t w, uint32_t h) : dataSize_(w * h * 3), data_(new char[w * h * 3]), wid_(w), hei_(h) {

    }

    TitSmall(uint32_t w, uint32_t h, char* data) : dataSize_(w * h * 3), data_(data), wid_(w), hei_(h) {

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

    void randomize(double seed) {
        std::random_device dev;
        //std::mt19937 gen(dev());
        //std::bernoulli_distribution rd(0.5);
        //printf("E...\n");
        for (uint_fast32_t i = 0; i < dataSize_; i ++) {
            //data_[i] = (1 * rd(gen)) | (2 * rd(gen)) | (4 * rd(gen)) | (8 * rd(gen))
            //         | (16 * rd(gen)) | (32 * rd(gen)) | (64 * rd(gen)) | (128 * rd(gen));
            //printf("%u\n", uint8_t(data_[i]));
            data_[i] = uint8_t(dev());
        }

    }

};

#endif // SMALLTIT_H
