//
// Created by AI Assistant for Behavior Tree implementation
//

#ifndef ENEMY_A_BEHAVIOR_TREE_H
#define ENEMY_A_BEHAVIOR_TREE_H

#include "BehaviorTree.h"
#include <memory>

class Enemy;

// Behavior Tree especial para o Inimigo A que demonstra as vantagens da BT sobre FSM
class EnemyABehaviorTree {
public:
    // Cria uma Behavior Tree inteligente e complexa para o Inimigo A
    static std::shared_ptr<BehaviorTree> CreateIntelligentBehaviorTree();
    
    // Cria uma Behavior Tree com comportamento de fuga estratégica
    static std::shared_ptr<BehaviorTree> CreateTacticalBehaviorTree();
    
    // Cria uma Behavior Tree com múltiplas estratégias de combate
    static std::shared_ptr<BehaviorTree> CreateMultiStrategyBehaviorTree();

private:
    // Métodos auxiliares para construir partes específicas da árvore
    static std::shared_ptr<BehaviorTreeNode> CreateSmartFleeSequence();
    static std::shared_ptr<BehaviorTreeNode> CreateTacticalCombatSequence();
    static std::shared_ptr<BehaviorTreeNode> CreateAdaptivePatrolSequence();
};

#endif //ENEMY_A_BEHAVIOR_TREE_H
