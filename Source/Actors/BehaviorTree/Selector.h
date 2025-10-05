//
// Created by eduar on 28/09/2025.
//

#ifndef SELECTOR_H
#define SELECTOR_H



#pragma once
#include "CompositeNode.h"

class Selector : public CompositeNode {
public:
    NodeStatus Tick() override {
        for (const auto& child : mChildren) {
            const NodeStatus status = child->Tick();
            if (status != NodeStatus::Failure) {
                return status; // Retorna sucesso ou em execução
            }
        }
        return NodeStatus::Failure; // Todos os filhos falharam
    }
};



#endif //SELECTOR_H
