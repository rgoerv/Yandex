#include <cassert>
#include <iostream>
#include <memory>
#include <string>

using namespace std;

struct Cat {

    Cat(Cat&&) = default;
    Cat& operator=(Cat&&) = default;

    Cat(const string& name, int age)
        : name_(name)
        , age_(age)  //
    {
    }
    const string& GetName() const noexcept {
        return name_;
    }
    int GetAge() const noexcept {
        return age_;
    }
    ~Cat() {
    }
    void Speak() const {
        cout << "Meow!"s << endl;
    }

private:
    string name_;
    int age_;
};

// Функция создаёт двухлетних котов
unique_ptr<Cat> CreateCat(const string& name) {
    return make_unique<Cat>(name, 2);
}

class Witch {
public:

    Witch() = default;

    explicit Witch(const string& name)
        : name_(name) {
    }

    Witch(const Witch& obj)
    {
        try
        {
            Witch temp;
            temp.name_ = obj.GetName();
            temp.cat_ = move(make_unique<Cat>(obj.cat_.get()->GetName(), obj.cat_.get()->GetAge()));

            name_ = temp.GetName();
            cat_ = move(make_unique<Cat>(temp.cat_.get()->GetName(), temp.cat_.get()->GetAge()));
        }
        catch(...)
        {
            throw;
        }
        
    }

    Witch& operator=(Witch&& rhs)
    {

        name_ = rhs.GetName();
        cat_ = rhs.ReleaseCat();

        return *this;
    }

    Witch& operator=(const Witch& rhs)
    {
        try
        {
            Witch temp;
            temp.name_ = rhs.GetName();
            temp.cat_ = move(make_unique<Cat>(rhs.cat_.get()->GetName(), rhs.cat_.get()->GetAge()));

            name_ = temp.GetName();
            cat_ = temp.ReleaseCat();
        }
        catch(...)
        {
            throw;
        }        

        return *this;
    }

    Witch(Witch&& obj)
    {
        name_ = move(obj.GetName());
        cat_ = obj.ReleaseCat();
    }

    const string& GetName() const noexcept {
        return name_;
    }
    void SetCat(unique_ptr<Cat>&& cat) noexcept {
        cat_ = std::move(cat);
    }
    unique_ptr<Cat> ReleaseCat() noexcept {
        return std::move(cat_);
    }

private:
    string name_;
    unique_ptr<Cat> cat_;
};

void Test() {
    // Объекты Witch можно перемещать
    {
        Witch witch("Hermione"s);
        auto cat = CreateCat("Crookshanks"s);
        Cat* raw_cat = cat.get();
        assert(raw_cat);
        witch.SetCat(move(cat));

        Witch moved_witch(std::move(witch));
        auto released_cat = moved_witch.ReleaseCat();
        assert(released_cat.get() == raw_cat);  // Кот переместился от witch к moved_witch
    }

    // Можно использовать перемещающий оператор присваивания
    {
        Witch witch("Hermione"s);
        auto cat = CreateCat("Crookshanks"s);
        Cat* raw_cat = cat.get();
        witch.SetCat(move(cat));

        Witch witch2("Minerva McGonagall");
        witch2 = move(witch);
        auto released_cat = witch.ReleaseCat();
        assert(!released_cat);
        released_cat = witch2.ReleaseCat();
        assert(released_cat.get() == raw_cat);
    }

    // Можно копировать волшебниц
    {
        Witch witch("Hermione");
        auto cat = CreateCat("Crookshanks"s);
        witch.SetCat(move(cat));

        Witch witch_copy(witch);
        assert(!cat);
        cat = witch.ReleaseCat();
        assert(cat);  // У первой волшебницы кот никуда не делся

        auto cat_copy = witch_copy.ReleaseCat();
        assert(cat_copy != nullptr && cat_copy != cat);
        assert(cat_copy->GetName() == cat->GetName());  // Копия волшебницы содержит копию кота
    }

    // Работает копирующее присваивание волшебниц
    {
        Witch witch("Hermione"s);
        auto cat = CreateCat("Crookshanks"s);
        witch.SetCat(move(cat));

        Witch witch2("Minerva McGonagall"s);
        witch2 = witch;

        assert(!cat);
        cat = witch.ReleaseCat();
        assert(cat);  // У первой волшебницы кот никуда не делся

        auto cat_copy = witch2.ReleaseCat();
        assert(cat_copy != nullptr && cat_copy != cat);
        assert(cat_copy->GetName() == cat->GetName());  // При присваивании скопировался кот
    }
}

int main() {
    Test();
}