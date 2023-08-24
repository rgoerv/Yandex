#pragma once

#include "identity_document.h"
#include <iostream>
#include <string>

using namespace std::string_view_literals;

class DrivingLicence {
private:
    using DeleteFunction = void(*)(DrivingLicence*);
    using PrintIDFunction = void(*)(const DrivingLicence*);
    
    struct Vtable {
        DeleteFunction delete_this;
        const PrintIDFunction print_id_this;
    };
public:
    DrivingLicence()
        : identity_document_()
    {
        DrivingLicence::SetVTable(this);
        std::cout << "DrivingLicence::Ctor()"sv << std::endl;
    }

    DrivingLicence(const DrivingLicence& other)
        : identity_document_(other.identity_document_) 
    {
        DrivingLicence::SetVTable(this);
        std::cout << "DrivingLicence::CCtor()"sv << std::endl;
    }

    ~DrivingLicence() {
        std::cout << "DrivingLicence::Dtor()"sv << std::endl;
        IdentityDocument::SetVTable((IdentityDocument*)this);
    }

    void Delete() {
        GetVtable()->delete_this(this);
    }

    void PrintID() const {
        GetVtable()->print_id_this(this);
    }

    int GetID() const {
        return identity_document_.GetID();
    }

    operator IdentityDocument() {
        return { identity_document_ };
    }

    static void SetVTable(DrivingLicence* obj) {
        //(*new-type) expression
        *(DrivingLicence::Vtable**)obj = &DrivingLicence::VTABLE;
    }

    const Vtable* GetVtable() const {
        return (const DrivingLicence::Vtable*)identity_document_.GetVtable();
    }

    Vtable* GetVtable() {
        return (DrivingLicence::Vtable*)identity_document_.GetVtable();
    }

private:
    static void Delete(DrivingLicence* obj) {
        delete obj;
    }

    static void PrintID(const DrivingLicence* obj) {
        std::cout << "DrivingLicence::PrintID() : "sv << obj->GetID() << std::endl;
    }

private:
    IdentityDocument identity_document_;
    static DrivingLicence::Vtable VTABLE;
};

DrivingLicence::Vtable DrivingLicence::VTABLE = {
    DrivingLicence::Delete,
    DrivingLicence::PrintID
};