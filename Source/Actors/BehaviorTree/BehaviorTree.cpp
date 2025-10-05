//
// Created by AI Assistant for Behavior Tree implementation
//

#include "BehaviorTree.h"
#include "../Enemy.h"

BehaviorTree::BehaviorTree() : mRoot(nullptr) {
}

BehaviorTree::~BehaviorTree() {
}

void BehaviorTree::SetRoot(std::shared_ptr<BehaviorTreeNode> root) {
    mRoot = root;
}

void BehaviorTree::Update(Enemy* enemy, float deltaTime) {
    if (mRoot) {
        mRoot->Execute(enemy, deltaTime);
    }
}

void BehaviorTree::Reset() {
    // A Behavior Tree não precisa de reset explícito como a FSM
    // Cada nó gerencia seu próprio estado interno
}
