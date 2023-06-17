#include "json_builder.h"


namespace json {

DictValueContext DictKeyContext::Value(Node::Value value)
{
    builder_.Value(value);
    return DictValueContext{ builder_ };
}

ArrayItemContext DictKeyContext::StartArray()
{
    builder_.StartArray();
    return ArrayItemContext{ builder_ };
}

DictItemContext DictKeyContext::StartDict()
{
    builder_.StartDict();
    return DictItemContext{ builder_ };
}

DictKeyContext DictValueContext::Key(std::string key)
{
    builder_.Key(std::move(key));
    return DictKeyContext{ builder_ };
}

Builder& DictValueContext::EndDict()
{
    return builder_.EndDict();
}

DictKeyContext DictItemContext::Key(std::string key)
{
    builder_.Key(std::move(key));
    return DictKeyContext{ builder_ };
}

Builder& DictItemContext::EndDict()
{ 
    return builder_.EndDict();
}

ArrayItemContext ArrayItemContext::Value(Node::Value value)
{
    builder_.Value(value);
    return ArrayItemContext{ builder_ };
}

ArrayItemContext ArrayItemContext::StartArray()
{
    builder_.StartArray();
    return ArrayItemContext{ builder_ };
}

DictItemContext ArrayItemContext::StartDict()
{
    builder_.StartDict();
    return DictItemContext{ builder_ };
}

Builder& ArrayItemContext::EndArray()
{
    return builder_.EndArray();
}

DictKeyContext Builder::Key(std::string key)
{
    if (nodes_stack_.empty())
    {
        throw std::logic_error("Dictionary don't created");
    }
    else if (!nodes_stack_.top()->IsDict())
    {
        throw std::logic_error("Key is assigned only to dictionary");
    }
    else
    {
        if (last_.IsString() &&
            nodes_stack_.top()->AsDict().count(last_.AsString()) != 0)
        { // key.key
            throw std::logic_error("Previous key has no value");
        }
        std::unique_ptr<Node> node(new Node(std::move(key)));

        nodes_stack_.top()->AsDict()[node->AsString()];
        last_ = *node;
    }

    return DictKeyContext{*this};
}

Builder &Builder::Value(Node::Value value)
{
    std::unique_ptr<Node> node(new Node(nullptr));

    // std::nullptr_t, Array, Dict, bool, int, double, std::string

    if (std::holds_alternative<Array>(value))
    {
        node.reset(new Node(std::get<Array>(value)));
    }
    else if (std::holds_alternative<Dict>(value))
    {
        node.reset(new Node(std::get<Dict>(value)));
    }
    else if (std::holds_alternative<bool>(value))
    {
        node.reset(new Node(std::get<bool>(value)));
    }
    else if (std::holds_alternative<int>(value))
    {
        node.reset(new Node(std::get<int>(value)));
    }
    else if (std::holds_alternative<double>(value))
    {
        node.reset(new Node(std::get<double>(value)));
    }
    else if (std::holds_alternative<std::string>(value))
    {
        node.reset(new Node(std::get<std::string>(std::move(value))));
    }

    if (nodes_stack_.empty() && root_.IsNull())
    {
        root_ = *node;
    }
    else if (nodes_stack_.empty() && !root_.IsNull())
    {
        throw std::logic_error("Assignment value is ambiguous");
    }
    else if (nodes_stack_.top()->IsArray())
    {
        nodes_stack_.top()->AsArray().push_back(*node);
    }
    else if (nodes_stack_.top()->IsDict() && last_.IsString())
    {
        nodes_stack_.top()->AsDict().at(last_.AsString()) = *node;
    }
    else
    {
        throw std::logic_error("Assignment value is ambiguous");
    }

    last_ = *node;
    return *this;
}

DictItemContext Builder::StartDict()
{
    Dict dictionary;
    std::unique_ptr<Node> node(new Node(dictionary));

    if (nodes_stack_.empty())
    {
        root_ = *node;
        nodes_stack_.push(&root_);
    }
    else
    {
        if (nodes_stack_.top()->IsArray())
        {
            nodes_stack_.top()->AsArray().push_back(*node);
            nodes_stack_.push(&nodes_stack_.top()->AsArray().back());
        }
        else if (nodes_stack_.top()->IsDict() && last_.IsString())
        {
            nodes_stack_.top()->AsDict().at(last_.AsString()) = *node;
            nodes_stack_.push(&nodes_stack_.top()->AsDict().at(last_.AsString()));
        }
        else
        {
            throw std::logic_error("StartDict is ambiguous");
        }
    }

    last_ = *node;
    return DictItemContext{*this};
}

ArrayItemContext Builder::StartArray()
{
    Array array;
    std::unique_ptr<Node> node(new Node(array));

    if (nodes_stack_.empty())
    {
        root_ = *node;
        nodes_stack_.push(&root_);
    }
    else
    {
        if (nodes_stack_.top()->IsArray())
        {
            nodes_stack_.top()->AsArray().push_back(*node);
            nodes_stack_.push(&nodes_stack_.top()->AsArray().back());
        }
        else if (nodes_stack_.top()->IsDict() && last_.IsString())
        {
            nodes_stack_.top()->AsDict().at(last_.AsString()) = *node;
            nodes_stack_.push(&nodes_stack_.top()->AsDict().at(last_.AsString()));
        }
        else
        {
            throw std::logic_error("StartArray is ambiguous");
        }
    }

    last_ = *node;
    return ArrayItemContext{*this};
}

Builder& Builder::EndDict()
{
    if (nodes_stack_.empty() || !nodes_stack_.top()->IsDict())
    {
        throw std::logic_error("StartDict is not found");
    }
    nodes_stack_.pop();
    return *this;
}

Builder& Builder::EndArray()
{
    if (nodes_stack_.empty() || !nodes_stack_.top()->IsArray())
    {
        throw std::logic_error("StartArray is not found");
    }
    nodes_stack_.pop();
    return *this;
}

json::Node Builder::Build()
{
    if (!nodes_stack_.empty())
    {
        throw std::logic_error("Json building is not complete");
    }
    else if (root_.IsNull() && nodes_stack_.empty())
    {
        throw std::logic_error("Json empty");
    }
    return root_;
}

} // namespace json