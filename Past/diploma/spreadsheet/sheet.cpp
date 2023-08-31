#include "sheet.h"

#include "cell.h"
#include "common.h"

#include <string>
#include <iostream>
#include <memory>

using namespace std::literals;

std::ostream &operator<<(std::ostream& output, const CellInterface::Value& value)
{
    std::visit(
        [&](const auto &x)
        {
            output << x;
        },
        value);
    return output;
}

Sheet::~Sheet() {}

void Sheet::SetCell(Position pos, std::string text) 
{
    if (!pos.IsValid()) {
        throw InvalidPositionException(""s);
    }
    if(static_cast<int>(sheet_.size()) < pos.row + 1) {
        sheet_.resize(pos.row + 1);
    }
    if(static_cast<int>(sheet_[pos.row].size()) < pos.col + 1) {
        sheet_[pos.row].resize(pos.col + 1);
    }
    if(sheet_[pos.row][pos.col] == nullptr) {
        sheet_[pos.row][pos.col] = std::make_unique<Cell>(*this);
    }
    sheet_[pos.row][pos.col]->Set(std::move(text));
}

const CellInterface* Sheet::GetCell(Position pos) const 
{
    if (!pos.IsValid()) {
        throw InvalidPositionException(""s);
    }
    if(pos.row >= static_cast<int>(sheet_.size()) || 
        pos.col >= static_cast<int>(sheet_[pos.row].size())) {
        return nullptr;
    }
    return sheet_[pos.row][pos.col].get();
}

CellInterface* Sheet::GetCell(Position pos) 
{
    if (!pos.IsValid()) {
        throw InvalidPositionException(""s);
    }
    if(pos.row >= static_cast<int>(sheet_.size()) || 
        pos.col >= static_cast<int>(sheet_[pos.row].size())) {
        return nullptr;
    }
    return sheet_[pos.row][pos.col].get();
}

const Cell* Sheet::Get_Cell(Position pos) const {
    if (!pos.IsValid()) {
        throw InvalidPositionException(""s);
    }
    if(pos.row >= static_cast<int>(sheet_.size()) || 
        pos.col >= static_cast<int>(sheet_[pos.row].size())) {
        return nullptr;
    }
    return sheet_[pos.row][pos.col].get(); 
}

Cell* Sheet::Get_Cell(Position pos) {
    if (!pos.IsValid()) {
        throw InvalidPositionException(""s);
    }
    if(pos.row >= static_cast<int>(sheet_.size()) || 
        pos.col >= static_cast<int>(sheet_[pos.row].size())) {
        return nullptr;
    }
    return sheet_[pos.row][pos.col].get();
}

void Sheet::ClearCell(Position pos) 
{
    if (!pos.IsValid())  {
        throw InvalidPositionException(""s);
    }
    if(pos.row >= static_cast<int>(sheet_.size()) || 
        pos.col >= static_cast<int>(sheet_[pos.row].size())) {
        return;
    }
    if (sheet_[pos.row][pos.col] != nullptr) {
        sheet_[pos.row][pos.col].reset(nullptr);
    }
}

Size Sheet::GetPrintableSize() const 
{
    Size size;
    for (auto row = 0; row < static_cast<int>(sheet_.size()); ++row) {
        for (auto col = 0; col < static_cast<int>(sheet_[row].size()); ++col) {
            if (sheet_[row][col] != nullptr && !sheet_[row][col]->GetText().empty()) {
                if(size.rows < row + 1) {
                    size.rows = row + 1;
                }
                if (size.cols < col + 1) {
                    size.cols = col + 1;
                }
            }
        }
    }
    return size;
}

void Sheet::PrintValues(std::ostream &output) const 
{
    Size size = GetPrintableSize();
    for (auto row = 0; row < size.rows; ++row) {
        for (auto col = 0; col < size.cols; ++col) {
            if(col > 0) {
                output << '\t';
            }
            if (col < static_cast<int>(sheet_[row].size()) && sheet_[row][col] != nullptr) {
                output << sheet_[row][col]->GetValue();
            }
        }
        output << '\n';
    }
}

void Sheet::PrintTexts(std::ostream &output) const 
{
    Size size = GetPrintableSize();
    for (auto row = 0; row < size.rows; ++row) {
        for (auto col = 0; col < size.cols; ++col) {
            if(col > 0) {
                output << '\t';
            }
            if (col < static_cast<int>(sheet_[row].size()) && sheet_[row][col] != nullptr) {
                output << sheet_[row][col]->GetText();
            }
        }
        output << '\n';
    }
}

std::unique_ptr<SheetInterface> CreateSheet() {
    return std::make_unique<Sheet>();
}