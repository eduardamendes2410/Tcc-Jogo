//
// Created by AI Assistant for Behavior Tree implementation
//

#ifndef CONDITION_NODES_H
#define CONDITION_NODES_H

#include "BehaviorTreeNode.h"
#include "../../Math.h"

// Nó de condição: Verifica se o jogador está próximo
class IsPlayerNearNode : public BehaviorTreeNode {
public:
    IsPlayerNearNode(float detectionRange = 200.0f);
    NodeResult Execute(Enemy* enemy, float deltaTime) override;

private:
    float mDetectionRange;
};

// Nó de condição: Verifica se o jogador está no alcance de ataque
class IsPlayerInAttackRangeNode : public BehaviorTreeNode {
public:
    IsPlayerInAttackRangeNode(float attackRange = 120.0f);
    NodeResult Execute(Enemy* enemy, float deltaTime) override;

private:
    float mAttackRange;
};

// Nó de condição: Verifica se o inimigo tem pouca vida
class IsLowHealthNode : public BehaviorTreeNode {
public:
    IsLowHealthNode(float healthThreshold = 1.0f);
    NodeResult Execute(Enemy* enemy, float deltaTime) override;

private:
    float mHealthThreshold;
};

// Nó de condição: Verifica se o jogador está muito longe
class IsPlayerTooFarNode : public BehaviorTreeNode {
public:
    IsPlayerTooFarNode(float loseSightRange = 400.0f);
    NodeResult Execute(Enemy* enemy, float deltaTime) override;

private:
    float mLoseSightRange;
};

// Nó de condição: Verifica se o inimigo pode atirar (cooldown acabou)
class CanShootNode : public BehaviorTreeNode {
public:
    NodeResult Execute(Enemy* enemy, float deltaTime) override;
};

#endif //CONDITION_NODES_H
