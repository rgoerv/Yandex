#pragma once

#include "identity_document.h"
#include <iostream>
#include <string>

using namespace std::string_view_literals;

class DrivingLicence : public IdentityDocument {
public:
    DrivingLicence() {
        std::cout << "DrivingLicence::Ctor()"sv << std::endl;
    }

    DrivingLicence(const DrivingLicence& other)
        : IdentityDocument(other) 
    {
        std::cout << "DrivingLicence::CCtor()"sv << std::endl;
    }

    ~DrivingLicence() {
        std::cout << "DrivingLicence::Dtor()"sv << std::endl;
    }

    void PrintID() const {
        std::cout << "DrivingLicence::PrintID() : "sv << GetID() << std::endl;
    }
};