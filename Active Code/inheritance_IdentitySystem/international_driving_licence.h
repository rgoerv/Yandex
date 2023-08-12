#pragma once

#include "driving_licence.h"
#include <iostream>
#include <string>

using namespace std::string_view_literals;

class InternationalDrivingLicence : public DrivingLicence {
public:
    InternationalDrivingLicence() {
        std::cout << "InternationalDrivingLicence::Ctor()"sv << std::endl;
    }

    InternationalDrivingLicence(const InternationalDrivingLicence& other)
        : DrivingLicence(other) 
    {
        std::cout << "InternationalDrivingLicence::CCtor()"sv << std::endl;
    }

    ~InternationalDrivingLicence() {
        std::cout << "InternationalDrivingLicence::Dtor()"sv << std::endl;
    }

    void PrintID() const {
        std::cout << "InternationalDrivingLicence::PrintID() : "sv << GetID() << std::endl;
    }
};