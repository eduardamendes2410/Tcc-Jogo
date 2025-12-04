//
// Created by AI Assistant for Behavior Tree implementation
//

#ifndef ENEMY_A_BEHAVIOR_TREE_H
#define ENEMY_A_BEHAVIOR_TREE_H

#include "BehaviorTree.h"
#include <memory>

class Enemy;

class EnemyABehaviorTree {
public:
    // Cria uma Behavior Tree inteligente e complexa para o Inimigo A
    static std::shared_ptr<BehaviorTree> CreateSimpleFleeBehaviorTree();
};

#endif //ENEMY_A_BEHAVIOR_TREE_H
