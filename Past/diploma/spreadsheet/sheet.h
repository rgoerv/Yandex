#pragma once

#include "cell.h"
#include "common.h"

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <vector>

std::ostream& operator<<(std::ostream& output, const CellInterface::Value& value);

class Sheet : public SheetInterface {
public:
    ~Sheet();

    void SetCell(Position pos, std::string text) override;

    const CellInterface* GetCell(Position pos) const override;
    CellInterface* GetCell(Position pos) override;
    const Cell* Get_Cell(Position pos) const;
    Cell* Get_Cell(Position pos);

    void ClearCell(Position pos) override;

    Size GetPrintableSize() const override;

    void PrintValues(std::ostream& output) const override;
    void PrintTexts(std::ostream& output) const override;

private:
	std::vector<std::vector<std::unique_ptr<Cell>>> sheet_;
};