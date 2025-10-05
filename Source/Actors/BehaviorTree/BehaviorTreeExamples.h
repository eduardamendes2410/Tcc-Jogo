//
// Created by AI Assistant for Behavior Tree implementation
// Exemplo de uso da Behavior Tree para diferentes tipos de inimigos
//

#ifndef BEHAVIOR_TREE_EXAMPLES_H
#define BEHAVIOR_TREE_EXAMPLES_H

#include "EnemyBehaviorTree.h"
#include <memory>

class BehaviorTreeExamples {
public:
    // Exemplo 1: Inimigo básico com comportamento simples
    static std::shared_ptr<BehaviorTree> CreateBasicEnemyTree();
    
    // Exemplo 2: Inimigo agressivo que sempre persegue
    static std::shared_ptr<BehaviorTree> CreateAggressiveEnemyTree();
    
    // Exemplo 3: Inimigo defensivo que foge quando ferido
    static std::shared_ptr<BehaviorTree> CreateDefensiveEnemyTree();
    
    // Exemplo 4: Inimigo inteligente com múltiplas estratégias
    static std::shared_ptr<BehaviorTree> CreateSmartEnemyTree();
    
    // Exemplo 5: Inimigo suicida que ataca sem medo
    static std::shared_ptr<BehaviorTree> CreateSuicidalEnemyTree();
};

#endif //BEHAVIOR_TREE_EXAMPLES_H
