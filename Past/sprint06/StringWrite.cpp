#include <fstream>
#include <string>

#include "log_duration.h"

using namespace std;

int main() {
    // {
    //     LOG_DURATION("READING 50000 STRINGS ШТРИХ n");
    //     ofstream out_file("ballad.txt"s);
    //     for (int i = 0; i < 50000; ++i) {
    //         out_file << "С любимыми не расставайтесь\n"s;
    //     }
    // }
    
        LOG_DURATION("READING 50000 STRINGS ENDL");
        ofstream out_file("ballad.txt"s);
        for (int i = 0; i < 50000; ++i) {
            out_file << "С любимыми не расставайтесь"s << endl;
        }
    
    throw;
}