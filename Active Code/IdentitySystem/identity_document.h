#pragma once

#include <iostream>
#include <string>

using namespace std::string_view_literals;

class IdentityDocument {
private:
    using DeleteFunction = void(*)(IdentityDocument*);
    using PrintIDFunction = void(*)(const IdentityDocument*);

    struct Vtable {
        DeleteFunction delete_this;
        PrintIDFunction print_id_this;
    };
public:
    IdentityDocument()
        : unique_id_(++unique_id_count_) 
    {
        IdentityDocument::SetVTable(this);
        std::cout << "IdentityDocument::Ctor() : "sv << unique_id_ << std::endl;
    }

    ~IdentityDocument() {
        --unique_id_count_;
        std::cout << "IdentityDocument::Dtor() : "sv << unique_id_ << std::endl;
    }

    IdentityDocument(const IdentityDocument& other)
        : unique_id_(++unique_id_count_) 
    {
        IdentityDocument::SetVTable(this);
        std::cout << "IdentityDocument::CCtor() : "sv << unique_id_ << std::endl;
    }

    IdentityDocument& operator=(const IdentityDocument&) = delete;

    void Delete() {
        GetVtable()->delete_this(this);
    }

    void PrintID() const {
        GetVtable()->print_id_this(this);
    }

    static void PrintUniqueIDCount() {
        std::cout << "unique_id_count_ : "sv << unique_id_count_ << std::endl;
    }

    int GetID() const {
        return unique_id_;
    }

    static void SetVTable(IdentityDocument* obj) {
        //(*new-type) expression
        *(IdentityDocument::Vtable**)obj = &IdentityDocument::VTABLE;
    }

    const Vtable* GetVtable() const {
        return vptr_;
    }

    Vtable* GetVtable() {
        return vptr_;
    }

private:
    static void Delete(IdentityDocument* obj) {
        delete obj;
    }

    static void PrintID(const IdentityDocument* obj) {
        std::cout << "IdentityDocument::PrintID() : "sv << obj->unique_id_ << std::endl;
    }

private:
    IdentityDocument::Vtable* vptr_ = nullptr;
    static IdentityDocument::Vtable VTABLE;

    static int unique_id_count_;
    int unique_id_;
};

int IdentityDocument::unique_id_count_ = 0;

IdentityDocument::Vtable IdentityDocument::VTABLE = {
    IdentityDocument::Delete,
    IdentityDocument::PrintID
};