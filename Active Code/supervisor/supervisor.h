#pragma once

#include "people.h"

/*
    Надзиратель за уровнем удовлетворённости.
    Способен наблюдать за состоянием человека
    Если уровень удовлетворённости человека опустится ниже минимального уровня, Надзиратель
    побуждает человека танцевать до тех пор, пока уровень уровень удовлетворённости
    не станет больше или равен максимальному значению
*/
class SatisfactionSupervisor : public PersonObserver {
public:
    // Конструктор принимает значение нижнего и верхнего уровня удовлетворённости
    SatisfactionSupervisor(int min_satisfaction, int max_satisfaction) 
        : min_satisfaction_(min_satisfaction)
        , max_satisfaction_(max_satisfaction)
        {
        }

    void OnSatisfactionChanged(Person& person, int old_value, int new_value) override {
        (void)old_value;
        if(new_value < min_satisfaction_) {
            for( ; person.GetSatisfaction() <= max_satisfaction_;) {
                person.ChangeDanceCount(+1);
                person.ChangeSatisfaction(+1);
            }
        }
    }

private:
    int min_satisfaction_;
    int max_satisfaction_;
};