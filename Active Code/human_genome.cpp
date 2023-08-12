#include <cassert>
#include <cstddef>
#include <cstdint>
#include <string>
#include <array>
#include <algorithm>

#include <iostream>

using namespace std;

struct Nucleotide {
    char symbol;
    size_t position;
    int chromosome_num;
    int gene_num;
    bool is_marked;
    char service_info;
};

const static int N = 4;
const std::array<char, N> Letters = {'A', 'T', 'G', 'C'};

struct CompactNucleotide {
    uint64_t position:32;
    uint64_t gene_num:15;
    uint64_t is_marked:1;
    uint64_t service_info:8;
    uint64_t chromosome_num:6;
    uint64_t symbol:2;
};

CompactNucleotide Compress(const Nucleotide& n) {
    CompactNucleotide compact_nucleotide;

    compact_nucleotide.position = n.position;
    compact_nucleotide.gene_num = n.gene_num;
    compact_nucleotide.is_marked = n.is_marked;
    compact_nucleotide.service_info = n.service_info;
    compact_nucleotide.chromosome_num = n.chromosome_num;
    compact_nucleotide.symbol = std::distance(Letters.begin(), std::find(Letters.begin(), Letters.end(), n.symbol));

    return compact_nucleotide;
}

Nucleotide Decompress(const CompactNucleotide& cn) {
    Nucleotide nucleotide;
    nucleotide.position = cn.position;
    nucleotide.gene_num = cn.gene_num;
    nucleotide.is_marked = cn.is_marked;
    nucleotide.service_info = cn.service_info;
    nucleotide.chromosome_num = cn.chromosome_num;
    nucleotide.symbol = Letters.at(cn.symbol);

    return nucleotide;
}

// static_assert(sizeof(CompactNucleotide) <= 8, "Your CompactNucleotide is not compact enough");
// static_assert(alignof(CompactNucleotide) == 4, "Don't use '#pragma pack'!");

bool operator==(const Nucleotide& lhs, const Nucleotide& rhs) {
    return (lhs.symbol == rhs.symbol) && (lhs.position == rhs.position) && (lhs.chromosome_num == rhs.chromosome_num)
        && (lhs.gene_num == rhs.gene_num) && (lhs.is_marked == rhs.is_marked) && (lhs.service_info == rhs.service_info);
}
void TestSize() {
    assert(sizeof(CompactNucleotide) <= 8);
}
void TestCompressDecompress() {
    Nucleotide source;
    source.symbol = 'T';
    source.position = 1'000'000'000;
    source.chromosome_num = 48;
    source.gene_num = 1'000;
    source.is_marked = true;
    source.service_info = '!';

    CompactNucleotide compressedSource = Compress(source);
    Nucleotide decompressedSource = Decompress(compressedSource);

    assert(source == decompressedSource);
}

int main() {
    TestSize();
    TestCompressDecompress();
}