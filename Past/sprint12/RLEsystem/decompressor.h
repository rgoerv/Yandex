#pragma once

#include <string>

#include <fstream>
#include <array>

inline bool DecodeRLE(const std::string& src_name, const std::string& dst_name) {
    using namespace std;

    ifstream src(src_name, ios::in | ios::binary);

    if(!src){
        return false;
    }

    ofstream dst(dst_name, ios::out | ios::binary);

    do {
        unsigned char header = src.get();
        
        if (header == istream::traits_type::eof() || !src) {
            return true;
        }

        int block_type = (header & 1);
        const int data_size = (header >> 1) + 1;

        static const int BUFF_SIZE = 1024;
        std::array<char, BUFF_SIZE> buffer;

        if (block_type) {
            unsigned char serial = src.get();
            dst.write(string(data_size, serial).c_str(), data_size);
        } else {
            src.read(buffer.data(), data_size);
            dst.write(buffer.data(), src.gcount());
        }

    } while (src);

    return true;
}
