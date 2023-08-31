#pragma once

#include "common.h"
#include "formula.h"

#include <functional>
#include <string>
#include <memory>
#include <set>
#include <vector>
#include <cassert>

using namespace std::string_literals;

class Sheet;

class Cell : public CellInterface {
public:
    using CellPtr = Cell*;

    Cell(Sheet& sheet);
    ~Cell() = default;

    void Set(std::string text);
    void Clear();

    Value GetValue() const override;
    std::string GetText() const override;

    std::vector<Position> GetReferencedCells() const override;
    void CheckCircularDependency(std::set<CellPtr>& input_edges_);
    bool IsReferenced() const;
    void InvalidateCache();
private:

    class Impl {
    public:
        virtual ~Impl() = default;
        virtual Value GetValue() const = 0;
        virtual std::string GetText() const = 0;
        virtual std::vector<Position> GetReferencedCells() const = 0;
        virtual void InvalidateCache() = 0;
    };

    class EmptyImpl : public Impl {
    public:

        Value GetValue() const override;
        std::string GetText() const override;
        std::vector<Position> GetReferencedCells() const override;

        void InvalidateCache() override {}
    };

    class TextImpl : public Impl {
    public:
        TextImpl(std::string text)
            : text_(std::move(text))
        {
        }
        ~TextImpl() = default;

        Value GetValue() const;
        std::string GetText() const;
        std::vector<Position> GetReferencedCells() const;
        void InvalidateCache() override {}
    private:
        std::string text_;
    };

    class FormulaImpl : public Impl {
    public:
        FormulaImpl(SheetInterface& sheet_, std::string text)
            : sheet(sheet_)
            , formula_(ParseFormula(text.substr(1, text.npos)))
        {
        }

        ~FormulaImpl() = default;

        Value GetValue() const override;
        std::string GetText() const override;
        std::vector<Position> GetReferencedCells() const override;
        void InvalidateCache() override;
    private:
        SheetInterface& sheet;
        std::unique_ptr<FormulaInterface> formula_;
        mutable std::unique_ptr<Value> cache;
    };

    std::unique_ptr<Impl> impl_;
    Sheet& sheet;
    std::set<CellPtr> inputs_edges; // входные указатели, в которых находятся значения, нужные текущей ячейке
    std::set<CellPtr> outputs_edges; // выходные указатели, требующие значения текущей ячейки
};