//
// Created by AI Assistant for Behavior Tree implementation
//

#ifndef COMPOSITE_NODES_H
#define COMPOSITE_NODES_H

#include "BehaviorTreeNode.h"

// Nó Selector: Executa filhos até que um retorne Success
// Se todos falharem, retorna Failure
class SelectorNode : public BehaviorTreeNode {
public:
    NodeResult Execute(Enemy* enemy, float deltaTime) override;
};

// Nó Sequence: Executa filhos até que um retorne Failure
// Se todos tiverem sucesso, retorna Success
class SequenceNode : public BehaviorTreeNode {
public:
    NodeResult Execute(Enemy* enemy, float deltaTime) override;

private:
    int mRunningChildIndex = 0;
};

// Nó Parallel: Executa todos os filhos simultaneamente
// Retorna Success se todos tiverem sucesso, Failure se algum falhar
class ParallelNode : public BehaviorTreeNode {
public:
    NodeResult Execute(Enemy* enemy, float deltaTime) override;
};

// Nó Inverter: Inverte o resultado do filho
// Success vira Failure, Failure vira Success, Running continua Running
class InverterNode : public BehaviorTreeNode {
public:
    NodeResult Execute(Enemy* enemy, float deltaTime) override;
};

#endif //COMPOSITE_NODES_H
