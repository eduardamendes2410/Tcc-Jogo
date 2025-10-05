//
// Created by AI Assistant for Behavior Tree implementation
//

#ifndef ENEMY_BEHAVIOR_TREE_H
#define ENEMY_BEHAVIOR_TREE_H

#include "BehaviorTree.h"
#include <memory>

class Enemy;

// Classe que constrói e configura a Behavior Tree específica para inimigos
class EnemyBehaviorTree {
public:
    // Constrói uma Behavior Tree básica (similar à FSM atual)
    static std::shared_ptr<BehaviorTree> CreateBasicBehaviorTree();
    
    // Constrói uma Behavior Tree avançada com comportamentos mais inteligentes
    static std::shared_ptr<BehaviorTree> CreateAdvancedBehaviorTree();
    
    // Constrói uma Behavior Tree para inimigos com pouca vida (comportamento de fuga)
    static std::shared_ptr<BehaviorTree> CreateLowHealthBehaviorTree();
    
    // Constrói uma Behavior Tree para diferentes tipos de inimigos
    static std::shared_ptr<BehaviorTree> CreateBehaviorTreeForType(int enemyType);

private:
    // Métodos auxiliares para construir partes específicas da árvore
    static std::shared_ptr<BehaviorTreeNode> CreateCombatSequence();
    static std::shared_ptr<BehaviorTreeNode> CreatePatrolSequence();
    static std::shared_ptr<BehaviorTreeNode> CreateFleeSequence();
};

#endif //ENEMY_BEHAVIOR_TREE_H
