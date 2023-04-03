#include <algorithm>
#include <cassert>
#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <iterator>

using namespace std;

// Объявляем Sentence<Token> для произвольного типа Token
// синонимом vector<Token>.
// Благодаря этому в качестве возвращаемого значения
// функции можно указать не малопонятный вектор векторов,
// а вектор предложений — vector<Sentence<Token>>.
template <typename Token>
using Sentence = vector<Token>;

template <typename TokenForwardIt>
TokenForwardIt FindSentenceEnd(TokenForwardIt tokens_begin, TokenForwardIt tokens_end) {
    const TokenForwardIt before_sentence_end
        = adjacent_find(tokens_begin, tokens_end, [](const auto& left_token, const auto& right_token) {
        return left_token.IsEndSentencePunctuation() && !right_token.IsEndSentencePunctuation();
            });
    return before_sentence_end == tokens_end ? tokens_end : next(before_sentence_end);
}

// Класс Token имеет метод bool IsEndSentencePunctuation() const
template <typename Token>
vector<Sentence<Token>> SplitIntoSentences(vector<Token> tokens) {
    if(tokens.empty()) { return {}; }

    vector<Sentence<Token>> result;

    typename Sentence<Token>::iterator s_begin = tokens.begin();
    typename Sentence<Token>::iterator s_end = tokens.begin();

    while(s_end != tokens.end()) {
        s_begin = s_end;
        s_end = std::move(FindSentenceEnd(s_begin, tokens.end()));

        result.push_back(std::move(Sentence<Token>{std::make_move_iterator(s_begin), std::make_move_iterator(s_end)}));
    }
    return result;
}

struct TestToken {
    string data;
    bool is_end_sentence_punctuation = false;

    bool IsEndSentencePunctuation() const {
        return is_end_sentence_punctuation;
    }
    bool operator==(const TestToken& other) const {
        return data == other.data && is_end_sentence_punctuation == other.is_end_sentence_punctuation;
    }
};

ostream& operator<<(ostream& stream, const TestToken& token) {
    return stream << token.data;
}

template <typename Token>
class NocopyToken {
public:
    NocopyToken(const NocopyToken& other) = delete;

    NocopyToken(const vector<Token>& elements) {
        data = vector<Token>{elements.begin(), elements.end() };
    }

    vector<Sentence<Token>>& Get() {
        return data;
    }
private:
    vector<Sentence<Token>> data;
};

// Тест содержит копирования объектов класса TestToken.
// Для проверки отсутствия копирований в функции SplitIntoSentences
// необходимо написать отдельный тест.
void TestSplitting() {
    assert(SplitIntoSentences(vector<TestToken>({ {"Split"s}, {"into"s}, {"sentences"s}, {"!"s} }))
        == vector<Sentence<TestToken>>({ {{"Split"s}, {"into"s}, {"sentences"s}, {"!"s}} }));

    assert(SplitIntoSentences(vector<TestToken>({ {"Split"s}, {"into"s}, {"sentences"s}, {"!"s, true} }))
        == vector<Sentence<TestToken>>({ {{"Split"s}, {"into"s}, {"sentences"s}, {"!"s, true}} }));

    assert(SplitIntoSentences(vector<TestToken>(
        { {"Split"s}, {"into"s}, {"sentences"s}, {"!"s, true}, {"!"s, true}, {"Without"s}, {"copies"s}, {"."s, true} }))
        == vector<Sentence<TestToken>>({
            {{"Split"s}, {"into"s}, {"sentences"s}, {"!"s, true}, {"!"s, true}},
            {{"Without"s}, {"copies"s}, {"."s, true}},
            }));

    // vector<TestToken> v1(
    //     { {"Split"s}, {"into"s}, {"sentences"s}, {"!"s, true}, {"!"s, true}, {"Without"s}, {"copies"s}, {"."s, true} });
    // vector<Sentence<TestToken>> v2({
    //     {{"Split"s}, {"into"s}, {"sentences"s}, {"!"s, true}, {"!"s, true}}, {{"Without"s}, {"copies"s}, {"."s, true}} });

    // NocopyToken<TestToken> obj1(v1);
    // NocopyToken obj1(v2);

    // assert(SplitIntoSentences(obj1.Get()) == v2);
}




int main() {
    TestSplitting();
}