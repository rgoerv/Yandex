#pragma once

#include <string>

class Person;

// Наблюдатель за состоянием человека.
class PersonObserver {
public:
    // Этот метод вызывается, когда меняется состояние удовлетворённости человека
    virtual void OnSatisfactionChanged(Person& /*person*/, int /*old_value*/, int /*new_value*/) {
        // Реализация метода базового класса ничего не делает
    }

protected:
    // Класс PersonObserver не предназначен для удаления напрямую
    ~PersonObserver() = default;
};

/*
    Человек.
    При изменении уровня удовлетворённости уведомляет
    связанного с ним наблюдателя
*/
class Person {
public:
    Person(const std::string& name, int age) 
        : name_(name)
        , age_(age)
        {
        }

    int GetSatisfaction() const {
        return satisfaction_;
    }

    const std::string& GetName() const {
        return name_;
    }

    // «Привязывает» наблюдателя к человеку. Привязанный наблюдатель
    // уведомляется об изменении уровня удовлетворённости человека
    // Новый наблюдатель заменяет собой ранее привязанного
    // Если передать nullptr в качестве наблюдателя, это эквивалентно отсутствию наблюдателя
    void SetObserver(PersonObserver* observer) {
        observer_ = observer;
    }

    int GetAge() const {
        return age_;
    }

    // Увеличивает на 1 количество походов на танцы
    // Увеличивает удовлетворённость на 1
    virtual void Dance() {
        ++dance_count_;
        ++satisfaction_;
        if (observer_) {
            observer_->OnSatisfactionChanged(*this, satisfaction_ - 1, satisfaction_);
        }
    }

    int GetDanceCount() const {
        return dance_count_;
    }

    // Прожить день. Реализация в базовом классе ничего не делает
    virtual void LiveADay() {
    }

    PersonObserver* GetObserver() {
        return observer_;
    }

    virtual ~Person() {  
    }
    
protected:
    int satisfaction_ = 100;
    int dance_count_ = 0;
private:
    std::string name_;
    PersonObserver* observer_ = nullptr;
    int age_;
};

// Рабочий.
// День рабочего проходит за работой
class Worker : public Person {
public:
    Worker(const std::string& name, int age) 
        : Person(name, age)
        {
        }

    // Рабочий старше 30 лет и младше 40 за танец получает 2 единицы удовлетворённости вместо 1
    void Dance() override {
        if (const int age = GetAge(); age > 30 && age < 40) {
            satisfaction_+=2;
            ++dance_count_;
            if (GetObserver()) {
                GetObserver()->OnSatisfactionChanged(*this, satisfaction_ - 2, satisfaction_);
            }
        }
        else {
            Person::Dance();
        }
    }

    // День рабочего проходит за работой
    // Прожить день. 
    void LiveADay() override {
        Work();
    }
    // Увеличивает счётчик сделанной работы на 1, уменьшает удовлетворённость на 5
    void Work() {
        ++work_count_;
        satisfaction_ -= 5;
        if(GetObserver()) {
            GetObserver()->OnSatisfactionChanged(*this, satisfaction_ + 5, satisfaction_);
        }
    }

    // Возвращает значение счётчика сделанной работы
    int GetWorkDone() const {
        return work_count_;
    }
private:
    int work_count_ = 0;
};

// Студент.
// День студента проходит за учёбой
class Student : public Person {
public:
    Student(const std::string& name, int age) 
        : Person(name, age)
        {
        }

    // День студента проходит за учёбой
    void LiveADay() override {
        Study();
    }
    // Учёба увеличивает уровень знаний на 1, уменьшает уровень удовлетворённости на 3
    void Study() {
        ++knowledge_level_;
        satisfaction_ -= 3;
        if(GetObserver()) {
            GetObserver()->OnSatisfactionChanged(*this, satisfaction_ + 3, satisfaction_);
        }
    }

    // Возвращает уровень знаний
    int GetKnowledgeLevel() const {
        return knowledge_level_;
    }
private:
    int knowledge_level_ = 0;
};