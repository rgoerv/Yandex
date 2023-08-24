#include <iostream>
#include <string>

using namespace std::string_view_literals;

class MagicHat {
public:
    MagicHat()
    {
        MagicHat::SetVTable(this);
        std::cout << "MagicHat::Ctor()"sv << std::endl;
    }

    MagicHat(const MagicHat&)
    {
        MagicHat::SetVTable(this);
        std::cout << "MagicHat::CCtor()"sv << std::endl;
    }

    MagicHat& operator=(const MagicHat&) = delete;

    ~MagicHat() {
        std::cout << "MagicHat::Dtor()"sv << std::endl;
    }
    
    static void SetVTable(MagicHat* obj) {
        //(new-type) expression
        *(MagicHat::Vtable**)obj = &MagicHat::VTABLE;
    }
    
    using DeleteFunction = void(*)(MagicHat*);
    using InsideFunction = void(*)(const MagicHat*);

    struct Vtable {
        DeleteFunction delete_this;
        const InsideFunction inside_this;
    };

    void WhatInside() const {
        GetVtable()->inside_this(this);
    }

    void Delete() {
        GetVtable()->delete_this(this);
    }

    const MagicHat::Vtable* GetVtable() const {
        return vptr_;
    }

    MagicHat::Vtable* GetVtable() {
        return vptr_;
    }

private:
    MagicHat::Vtable* vptr_ = nullptr;

    static MagicHat::Vtable VTABLE;

    static void Delete(MagicHat* obj) {
        //В этот момент тип объекта известен. Просто удаляем указатель.
        //Вызов delete запустит процесс вызовов деструкторов
        delete obj;
    }

    static void WhatInside(const MagicHat*) {
        std::cout << "Nothing!"sv << std::endl;
    }
};

MagicHat::Vtable MagicHat::VTABLE = { MagicHat::Delete, MagicHat::WhatInside };

class HatWithRabbits {
public:
    HatWithRabbits()
        : magic_hat_()
    {
        HatWithRabbits::SetVTable(this);
        std::cout << "HatWithRabbits::Ctor()"sv << std::endl;
    }

    HatWithRabbits(const HatWithRabbits& other)
        : magic_hat_(other.magic_hat_)
    {
        HatWithRabbits::SetVTable(this);
        std::cout << "HatWithRabbits::CCtor()"sv << std::endl;
    }

    HatWithRabbits& operator=(const HatWithRabbits&) = delete;

    ~HatWithRabbits() {
        std::cout << "HatWithRabbits::Dtor()"sv << std::endl;
        MagicHat::SetVTable((MagicHat*)this);
    }

    static void SetVTable(HatWithRabbits* obj) {
        *(HatWithRabbits::Vtable**)obj = &HatWithRabbits::VTABLE;
    }
    
    using InsideFunction = void(*)(const HatWithRabbits*);
    using DeleteFunction = void(*)(HatWithRabbits*);

    struct Vtable {
        const DeleteFunction delete_this;
        const InsideFunction inside_this;
    };

    void Delete() {
        GetVtable()->delete_this(this);
    }

    void WhatInside() const {
        GetVtable()->inside_this(this);
    }

    operator MagicHat() {
        return {magic_hat_};
    }

    const Vtable* GetVtable() const {
        return (const HatWithRabbits::Vtable*)magic_hat_.GetVtable();
    }

    Vtable* GetVtable() {
        return (HatWithRabbits::Vtable*)magic_hat_.GetVtable();
    }

private:
    MagicHat magic_hat_;

    static HatWithRabbits::Vtable VTABLE;

    static void Delete(HatWithRabbits* obj) {
        delete obj;
    }

    static void WhatInside(const HatWithRabbits*) {
        std::cout << "Rabbit!"sv << std::endl;
    }
};

HatWithRabbits::Vtable HatWithRabbits::VTABLE = { HatWithRabbits::Delete,  HatWithRabbits::WhatInside };

void WhatInsideHat(MagicHat* hat) {
    hat->WhatInside();
}

int main() {
    MagicHat* hat = (MagicHat*)(new HatWithRabbits());
    WhatInsideHat(hat);
    hat->Delete();
    // MagicHat::WhatInside();
}
