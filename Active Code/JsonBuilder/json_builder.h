#pragma once
#include "json.h"

#include <vector>
#include <string>


namespace json {

class Builder {

    Builder() {

    }

    void Key(std::string key) {

        const auto top = nodes_stack_.back()->GetValue();


        
        if (!nodes_stack_.back()->IsDict() || !nodes_stack_.back()) {
            throw;
        }
    }



private:
    json::Node root_;
    std::vector<json::Node*> nodes_stack_;
};

}


