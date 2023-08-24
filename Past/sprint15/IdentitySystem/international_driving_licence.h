#pragma once

#include "identity_document.h"
#include "driving_licence.h"
#include <iostream>
#include <string>

using namespace std::string_view_literals;

class InternationalDrivingLicence {
private:
    using DeleteFunction = void(*)(InternationalDrivingLicence*);
    using PrintIdFunction = void(*)(const InternationalDrivingLicence*);
    
    struct Vtable {
        DeleteFunction delete_this;
        const PrintIdFunction print_id_this;
    };
public:
    InternationalDrivingLicence()
        : driving_licence_()
    {
        InternationalDrivingLicence::SetVTable(this);
        std::cout << "InternationalDrivingLicence::Ctor()"sv << std::endl;
    }

    InternationalDrivingLicence(const InternationalDrivingLicence& other)
        : driving_licence_(other.driving_licence_) 
    {
        InternationalDrivingLicence::SetVTable(this);
        std::cout << "InternationalDrivingLicence::CCtor()"sv << std::endl;
    }

    ~InternationalDrivingLicence() {
        std::cout << "InternationalDrivingLicence::Dtor()"sv << std::endl;
        DrivingLicence::SetVTable((DrivingLicence*)this);
    }

    void Delete() {
        GetVtable()->delete_this(this);
    }

    void PrintID() const {
        GetVtable()->print_id_this(this);
    }

    int GetID() const {
        return driving_licence_.GetID();
    }

    operator DrivingLicence() {
        return { driving_licence_ };
    }

    static void SetVTable(InternationalDrivingLicence* obj) {
        //(*new-type) expression
        *(InternationalDrivingLicence::Vtable**)obj = &InternationalDrivingLicence::VTABLE;
    }

    const Vtable* GetVtable() const {
        return (const InternationalDrivingLicence::Vtable*)driving_licence_.GetVtable();
    }

    Vtable* GetVtable() {
        return (InternationalDrivingLicence::Vtable*)driving_licence_.GetVtable();
    }

private:
    static void Delete(InternationalDrivingLicence* obj) {
        delete obj;
    }

    static void PrintID(const InternationalDrivingLicence* obj) {
        std::cout << "DrivingLicence::PrintID() : "sv << obj->GetID() << std::endl;
    }

private:
    DrivingLicence driving_licence_;
    static InternationalDrivingLicence::Vtable VTABLE;
};

InternationalDrivingLicence::Vtable InternationalDrivingLicence::VTABLE = {
    InternationalDrivingLicence::Delete,
    InternationalDrivingLicence::PrintID
};