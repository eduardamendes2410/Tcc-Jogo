//
// Created by eduar on 28/09/2025.
//

#ifndef SEQUENCE_H
#define SEQUENCE_H

#pragma once
#include "CompositeNode.h"

class Sequence : public CompositeNode {
public:
    NodeStatus Tick() override {
        for (const auto& child : mChildren) {
            const NodeStatus status = child->Tick();
            if (status != NodeStatus::Success) {
                return status; // Retorna falha ou em execução
            }
        }
        return NodeStatus::Success; // Todos os filhos tiveram sucesso
    }
};



#endif //SEQUENCE_H
