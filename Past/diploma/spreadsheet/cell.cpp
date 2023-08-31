#include "cell.h"
#include "sheet.h"

#include <string>
#include <memory>
#include <set>
#include <cassert>
#include <iostream>

using namespace std::string_literals;

/*Cell::Cell*/

Cell::Cell(Sheet& sheet_)
    : impl_(std::make_unique<EmptyImpl>())
    , sheet(sheet_)
    {
    }

void Cell::CheckCircularDependency(std::set<CellPtr>& input_edges_) {
    
    std::set<CellPtr> output_edges_;
    std::vector<CellPtr> queue;

    queue.push_back(this); // this = start node

    while (!queue.empty()) {
        auto* node = queue.back();
        queue.pop_back();
        output_edges_.insert(node);

        if (input_edges_.find(node) == input_edges_.end()) { 
            for (auto* edge : node->outputs_edges) { 
                if (output_edges_.find(edge) == output_edges_.end()) {
                    queue.push_back(edge); //
                }
            }
        } else {
            throw CircularDependencyException(""s);
        }
    }
}

void Cell::Set(std::string text) {
    std::unique_ptr<Impl> temp;
    if (text.empty()) {
        temp = std::make_unique<EmptyImpl>();
    } else if (text[0] == FORMULA_SIGN && text.size() > 1) {
        temp = std::make_unique<FormulaImpl>(sheet, std::move(text));
    } else {
        temp = std::make_unique<TextImpl>(std::move(text));
    }

    const auto ref_cells = temp->GetReferencedCells();

    if (!ref_cells.empty()) {
        std::set<CellPtr> input_edges_;
        for (const auto& position : ref_cells) {
            input_edges_.insert(reinterpret_cast<Cell*>(sheet.GetCell(position)));
        }
        CheckCircularDependency(input_edges_);
    }
    impl_ = std::move(temp);

    // меняем старые ребра графа на новые
    for (auto* edge : inputs_edges) {
        edge->outputs_edges.erase(this);
    }
    inputs_edges.clear();

    // вставляем новые
    for (const auto& position : impl_->GetReferencedCells()) {
        auto* edge = reinterpret_cast<Cell*>(sheet.GetCell(position));
        if (!edge) {
            sheet.SetCell(position, "");
            edge = reinterpret_cast<Cell*>(sheet.GetCell(position));
        }
        inputs_edges.insert(edge);
        edge->outputs_edges.insert(this);
    }
    InvalidateCache();
}

void Cell::Clear() {
    impl_.reset(nullptr);
    impl_ = std::make_unique<EmptyImpl>();
}

Cell::Value Cell::GetValue() const {
    return impl_->GetValue();
}

std::string Cell::GetText() const {
    return impl_->GetText();
}

std::vector<Position> Cell::GetReferencedCells() const {
    return impl_->GetReferencedCells();
}

bool Cell::IsReferenced() const {
    return !outputs_edges.empty();
}

void Cell::InvalidateCache() {
    if(!impl_) {
        return;
    }
    impl_->InvalidateCache();
    for(const auto& cellptr : outputs_edges) {
        cellptr->InvalidateCache();
    }
}

/*Cell::EmptyImpl*/

CellInterface::Value Cell::EmptyImpl::GetValue() const {
    return ""s;
}

std::string Cell::EmptyImpl::GetText() const {
    return ""s;
}

std::vector<Position> Cell::EmptyImpl::GetReferencedCells() const {
    return std::vector<Position>();
}

CellInterface::Value Cell::TextImpl::GetValue() const {
    return text_[0] == ESCAPE_SIGN ? text_.substr(1, text_.npos) : text_;
}

std::string Cell::TextImpl::GetText() const {
    return text_;
}

std::vector<Position> Cell::TextImpl::GetReferencedCells() const {
    return std::vector<Position>();
}

/*Cell::FormulaImpl*/

CellInterface::Value Cell::FormulaImpl::GetValue() const {
    if (cache) {
        return *cache;
    } else {
        auto result = formula_->Evaluate(Cell::FormulaImpl::sheet);
        if (std::holds_alternative<double>(result)) {
            cache = std::move(std::make_unique<Value>(std::get<double>(result)));
        } else {
            cache = std::move(std::make_unique<Value>(std::get<FormulaError>(result)));
        }
        return *cache;
    }
}

std::string Cell::FormulaImpl::GetText() const {
    assert(formula_);
    return FORMULA_SIGN + formula_->GetExpression();
}

std::vector<Position> Cell::FormulaImpl::GetReferencedCells() const {
    assert(formula_);
    return formula_->GetReferencedCells();
}

void Cell::FormulaImpl::InvalidateCache() {
    cache.reset(nullptr);
}

/*_*/