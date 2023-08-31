#include "common.h"

#include <cctype>
#include <sstream>
#include <tuple>
#include <regex>

const int LETTERS = 26;
const int PADDING = 64;
const int MAX_POSITION_LENGTH = 17;
const int MAX_POS_LETTER_COUNT = 3;

const Position Position::NONE = {-1, -1};

using namespace std::string_literals;

static std::regex reg(R"/(^([ABCDEFGHIJKLMNOPQRSTUVWXYZ]+)([0123456789]+)$)/");

bool Position::operator==(const Position rhs) const {
    return std::make_tuple(col, row) == std::make_tuple(rhs.col, rhs.row);
}

bool Position::operator<(const Position rhs) const {
    return std::make_tuple(col, row) < std::make_tuple(rhs.col, rhs.row);
}

bool Position::IsValid() const {
    return (col < MAX_COLS && row < MAX_ROWS) && (col >= 0 && row >= 0);
}

std::string Position::ToString() const {
    if(!IsValid()) {
        return ""s;
    }
    int colonum = col + 1;
    std::string str_col;
    while(colonum > 26) {
        int quotient = colonum / 26;
        int remainder = colonum % 26;
        colonum = remainder == 0 ? quotient - 1 : quotient;
        str_col.push_back(static_cast<char>(remainder == 0 ? PADDING + LETTERS : PADDING + remainder));
    }
    str_col.push_back(static_cast<char>(64 + colonum));

    std::reverse(str_col.begin(), str_col.end());
    return str_col + std::to_string(row + 1); 
}

Position Position::FromString(std::string_view str) {
    
    std::string str_pos(str);
    std::smatch m;
    if(!std::regex_match(str_pos, m, reg)) {
        return Position::NONE;
    }
    if(m[2].str()[0] == '0' || m[2].str().length() > MAX_POSITION_LENGTH || m[1].str().length() > MAX_POS_LETTER_COUNT) {
        return Position::NONE;
    }
    std::string_view symbols(m[1].str());

    int colonum = static_cast<unsigned int>(symbols[0] - 64);
    for(size_t idx = 1; idx < symbols.size(); ++idx) {
        colonum = (colonum * 26) + static_cast<unsigned int>(symbols[idx] - PADDING);
    }
    Position position;
    position.col = colonum - 1;
    position.row = std::stoi(m[2].str()) - 1;

    if(!position.IsValid()) {
        return Position::NONE;
    }
    return position;
}

bool Size::operator==(Size rhs) const {
    return cols == rhs.cols && rows == rhs.rows;
}