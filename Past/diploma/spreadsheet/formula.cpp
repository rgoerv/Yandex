#include "formula.h"

#include "FormulaAST.h"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <sstream>
#include <vector>

using namespace std::literals;

FormulaError::FormulaError(FormulaError::Category category)
    : category_(category)
    {
    }

FormulaError::Category FormulaError::GetCategory() const {
    return category_;
}

bool FormulaError::operator==(FormulaError rhs) const {
    return GetCategory() == rhs.GetCategory();
}

std::string_view FormulaError::ToString() const {
    switch (GetCategory())
    {   
        case FormulaError::Category::Ref : return "#REF!"sv;
        case FormulaError::Category::Div0 : return "#DIV/0!"sv;
        case FormulaError::Category::Value : return "#VALUE!"sv;
        default:
            return "";
    }
}

std::ostream& operator<<(std::ostream& output, FormulaError fe) {
    return output << fe.ToString();
}

namespace {
class Formula : public FormulaInterface {
public:
    explicit Formula(std::string expression) 
    try : ast_(ParseFormulaAST(expression)) {}
    catch (...) {
        throw FormulaException(""s);
    }

    Value Evaluate(const SheetInterface& sheet) const override {
        try {
            auto func = [&sheet](Position position)
            {   
                if(!position.IsValid()) {
                    throw FormulaError(FormulaError::Category::Ref);
                }
                
                auto cell = sheet.GetCell(position);
                if(!cell) {
                    return .0;
                }
                auto value = cell->GetValue();
                if(std::holds_alternative<FormulaError>(value)) {
                    throw std::get<FormulaError>(value);
                } else if(std::holds_alternative<double>(value)) {
                    return std::get<double>(value);
                }

                auto str = std::get<std::string>(value);
                if(str.empty()) {
                    return .0;
                }
                double result = .0;
                size_t symbols_converted = 0;
                try {
                    result = std::stod(str, &symbols_converted);
                    if(symbols_converted < str.length()) {
                        throw FormulaError(FormulaError::Category::Value);
                    }
                } catch (...) {
                    throw FormulaError(FormulaError::Category::Value);
                }
                return result;
            };
            return ast_.Execute(func);
        } catch(const FormulaError& exc) {
            return FormulaError(exc.GetCategory());
        }
    }

    std::string GetExpression() const override {
        std::stringstream ss;
        ast_.PrintFormula(ss);
        return ss.str();
    }

    std::vector<Position> GetReferencedCells() const override {
        std::set<Position> ref_cells;
        std::for_each(ast_.GetCells().begin(), ast_.GetCells().end(),  
            [&ref_cells](const Position& position) {
                position.IsValid() && (ref_cells.count(position) < 1) ? (void)ref_cells.insert(position) : void();
            });
        return std::vector<Position>(ref_cells.begin(), ref_cells.end());
    }

private:
    FormulaAST ast_;
};
}  // namespace

std::unique_ptr<FormulaInterface> ParseFormula(std::string expression) {
    return std::make_unique<Formula>(std::move(expression));
}