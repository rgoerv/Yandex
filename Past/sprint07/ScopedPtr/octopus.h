#pragma once
#include <new> // Для исключения bad_alloc
#include <vector>
#include <iostream>

#include "ptrvector.h"
#include "scopedptr.h"

using std::size_t;
using std::ostream;
using std::cout;
using std::endl;

// Щупальце
class Tentacle {
public:
    explicit Tentacle(int id) noexcept
        : id_(id) {
    }

    int GetId() const noexcept {
        return id_;
    }

    Tentacle* GetLinkedTentacle() const noexcept {
        return linked_tentacle_;
    }
    void LinkTo(Tentacle& tentacle) noexcept {
        linked_tentacle_ = &tentacle;
    }
    void Unlink() noexcept {
        linked_tentacle_ = nullptr;
    }

private:
    int id_ = 0;
    Tentacle* linked_tentacle_ = nullptr;
};

// Осьминог
class Octopus {
public:
    Octopus()
            : Octopus(8) {
    }

    Octopus(const Octopus & octopus) {
        // PtrVector<Tentacle> octopus_;
        for(const auto & tentacle: octopus.tentacles_){
            // cout << "ориджинал тентакле линк ту: " << (tentacle->GetLinkedTentacle()) << endl;
            ScopedPtr<Tentacle> ptr(new Tentacle((tentacle->GetId())));
            ptr.GetRawPtr()->LinkTo(*tentacle->GetLinkedTentacle());
            //cout << "копи тентакле линк ту: " << (ptr.GetRawPtr()->GetLinkedTentacle()) << endl;
            tentacles_.push_back(ptr.GetRawPtr());
            // cout << "копи тентакле линк ту 2: " << (tentacles_.back()->GetLinkedTentacle()) << endl;
            ptr.Release();
        }
        // tentacles_ = octopus_.GetItems();
    }

    Octopus& operator=(const Octopus& rhs) {
        if (this != &rhs) {
            // Реализация операции присваивания с помощью идиомы Copy-and-swap.
            // Если исключение будет выброшено, то на текущий объект оно не повлияет.
            auto rhs_copy(rhs);

            // rhs_copy содержит копию правого аргумента.
            // Обмениваемся с ним данными.
            swap(this->tentacles_, rhs_copy.tentacles_);

            // Теперь текущий объект содержит копию правого аргумента,
            // а rhs_copy - прежнее состояние текущего объекта, которое при выходе
            // из блока будет разрушено.
        }
        return *this;
    }

    explicit Octopus(int num_tentacles) {
        Tentacle* t = nullptr;
        try {
            for (int i = 1; i <= num_tentacles; ++i) {
                t = new Tentacle(i);      // Может выбросить исключение bad_alloc
                tentacles_.push_back(t);  // Может выбросить исключение bad_alloc

                // Обнуляем указатель на щупальце, которое уже добавили в tentacles_,
                // чтобы не удалить его в обработчике catch повторно
                t = nullptr;
            }
        } catch (const std::bad_alloc&) {
            // Удаляем щупальца, которые успели попасть в контейнер tentacles_
            Cleanup();
            // Удаляем щупальце, которое создали, но не добавили в tentacles_
            delete t;
            // Конструктор не смог создать осьминога с восемью щупальцами,
            // поэтому выбрасываем исключение, чтобы сообщить вызывающему коду об ошибке
            // throw без параметров внутри catch выполняет ПЕРЕВЫБРОС пойманного исключения
            throw;
        }
    }

    ~Octopus() {
        // Осьминог владеет объектами в динамической памяти (щупальца),
        // которые должны быть удалены при его разрушении.
        // Деструктор - лучшее место, чтобы прибраться за собой.
        Cleanup();
    }

    // Добавляет новое щупальце с идентификатором,
    // равным (количество_щупалец + 1):
    // 1, 2, 3, ...
    // Возвращает ссылку на добавленное щупальце
    Tentacle& AddTentacle() {
        // cout << "адд ту " << tentacles_.size() << " тентаклес" << endl;
        int TentacleCount = GetTentacleCount();
        // cout << "До: " << TentacleCount << endl;
        ++TentacleCount;
        ScopedPtr<Tentacle> ptr(new Tentacle(TentacleCount));
        tentacles_.push_back(ptr.GetRawPtr());
        // cout << "афтер адд " << tentacles_.size() << " тентаклес" << endl;
        ptr.Release();
        // cout << "++ " << TentacleCount << endl << "last - 1 :" 
        //      << tentacles_.back()->GetId() << endl;
        return *tentacles_.back();
    }

    int GetTentacleCount() const noexcept {
        return static_cast<int>(tentacles_.size());
    }

    const Tentacle& GetTentacle(size_t index) const {
        return *tentacles_.at(index);
    }
    Tentacle& GetTentacle(size_t index) {
        return *tentacles_.at(index);
    }

private:
    void Cleanup() {
        // Удаляем щупальца осьминога из динамической памяти
        for (Tentacle* t : tentacles_) {
            delete t;
        }
        // Очищаем массив указателей на щупальца
        tentacles_.clear();
    }

    // Вектор хранит указатели на щупальца. Сами объекты щупалец находятся в куче
    std::vector<Tentacle*> tentacles_;
};