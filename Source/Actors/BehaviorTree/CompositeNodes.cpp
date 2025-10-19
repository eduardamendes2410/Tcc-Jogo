//
// Created by AI Assistant for Behavior Tree implementation
//

#include "CompositeNodes.h"
#include "../Enemy.h"

// SelectorNode Implementation
NodeResult SelectorNode::Execute(Enemy* enemy, float deltaTime) {
    for (auto& child : mChildren) {
        NodeResult result = child->Execute(enemy, deltaTime);
        if (result == NodeResult::Success || result == NodeResult::Running) {
            return result;
        }
    }
    return NodeResult::Failure;
}

// SequenceNode Implementation
NodeResult SequenceNode::Execute(Enemy* enemy, float deltaTime) {
    for (size_t i = mRunningChildIndex; i < mChildren.size(); ++i) {
        NodeResult result = mChildren[i]->Execute(enemy, deltaTime);

        if (result == NodeResult::Failure) {
            // Se falhar, reseta o estado e retorna Failure.
            mRunningChildIndex = 0;
            return NodeResult::Failure;
        }

        if (result == NodeResult::Running) {
            // Se estiver rodando, salva o índice e retorna Running.
            mRunningChildIndex = i;
            return NodeResult::Running;
        }

        // Se o resultado for Success, o loop continua para o próximo filho (i++)
    }

    // Se todos os filhos tiveram sucesso, reseta o estado e retorna Success.
    mRunningChildIndex = 0;
    return NodeResult::Success;
}

// ParallelNode Implementation
NodeResult ParallelNode::Execute(Enemy* enemy, float deltaTime) {
    bool allSuccess = true;
    bool anyRunning = false;
    
    for (auto& child : mChildren) {
        NodeResult result = child->Execute(enemy, deltaTime);
        if (result == NodeResult::Failure) {
            return NodeResult::Failure;
        }
        if (result == NodeResult::Running) {
            anyRunning = true;
        }
        if (result != NodeResult::Success) {
            allSuccess = false;
        }
    }
    
    if (anyRunning) {
        return NodeResult::Running;
    }
    
    return allSuccess ? NodeResult::Success : NodeResult::Failure;
}

// InverterNode Implementation
NodeResult InverterNode::Execute(Enemy* enemy, float deltaTime) {
    if (mChildren.empty()) {
        return NodeResult::Failure;
    }
    
    NodeResult result = mChildren[0]->Execute(enemy, deltaTime);
    
    switch (result) {
        case NodeResult::Success:
            return NodeResult::Failure;
        case NodeResult::Failure:
            return NodeResult::Success;
        case NodeResult::Running:
            return NodeResult::Running;
        default:
            return NodeResult::Failure;
    }
}
