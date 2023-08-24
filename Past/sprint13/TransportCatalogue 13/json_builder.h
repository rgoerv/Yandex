#pragma once
#include "json.h"

#include <stack>
#include <string>
#include <variant>
#include <stdexcept>
#include <memory>

namespace json {

class Builder;

class DictKeyContext;
class DictValueContext;
class DictItemContext;

class ArrayItemContext;

class DictKeyContext {
public:
    DictKeyContext(Builder& builder)
        : builder_(builder)
    {
    }
    
    DictValueContext Value(Node::Value value);
    ArrayItemContext StartArray();
    DictItemContext StartDict();

private:
    Builder& builder_;
};

class DictValueContext {
public:
    DictValueContext(Builder& builder)
        : builder_(builder)
    {
    }
    
    DictKeyContext Key(std::string key);
    Builder& EndDict();

private:
    Builder& builder_;
};

class DictItemContext {
public:
    DictItemContext(Builder& builder)
        : builder_(builder)
    {
    }

    DictKeyContext Key(std::string key);
    Builder& EndDict();

private:
    Builder& builder_;
};

class ArrayItemContext {
public:
    ArrayItemContext(Builder& builder)
        : builder_(builder)
    {
    }

    ArrayItemContext Value(Node::Value value);
    ArrayItemContext StartArray();
    DictItemContext StartDict();
    Builder& EndArray();

private:
    Builder& builder_;
};

class Builder {
public:
    DictKeyContext Key(std::string key);
    Builder& Value(Node::Value value);
    DictItemContext StartDict();
    ArrayItemContext StartArray();
    Builder& EndDict();
    Builder& EndArray();
    json::Node Build();

private:
    json::Node root_;
    json::Node last_;
    std::stack<json::Node*> nodes_stack_;
};

} // namespace json