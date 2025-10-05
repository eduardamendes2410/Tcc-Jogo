//
// Created by AI Assistant for Behavior Tree implementation
//

#ifndef BEHAVIOR_TREE_H
#define BEHAVIOR_TREE_H

#include "BehaviorTreeNode.h"
#include <memory>

class Enemy;

// Classe principal da Behavior Tree
class BehaviorTree {
public:
    BehaviorTree();
    ~BehaviorTree();
    
    // Define a raiz da árvore
    void SetRoot(std::shared_ptr<BehaviorTreeNode> root);
    
    // Executa a árvore de comportamento
    void Update(Enemy* enemy, float deltaTime);
    
    // Reinicia a árvore (útil quando o inimigo muda de estado)
    void Reset();

private:
    std::shared_ptr<BehaviorTreeNode> mRoot;
};

#endif //BEHAVIOR_TREE_H
