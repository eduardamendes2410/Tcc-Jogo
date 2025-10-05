//
// Created by eduar on 28/09/2025.
//

#ifndef COMPOSITENODE_H
#define COMPOSITENODE_H

#pragma once
#include "Node.h"
#include <vector>
#include <memory>

// Classe base para nós que têm múltiplos filhos (Selector, Sequence)
class CompositeNode : public Node {
public:
    CompositeNode() = default;
    void AddChild(std::unique_ptr<Node> child) {
        mChildren.push_back(std::move(child));
        //mChildren.push_back(child); //CHATGPT FEZ ASSIM
    }
protected:
    std::vector<std::unique_ptr<Node>> mChildren;
};



#endif //COMPOSITENODE_H
