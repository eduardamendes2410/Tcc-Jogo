//
// Created by AI Assistant for Behavior Tree implementation
//

#ifndef ACTION_NODES_H
#define ACTION_NODES_H

#include "BehaviorTreeNode.h"

// Nó de ação: Patrulha entre dois pontos
class PatrolActionNode : public BehaviorTreeNode {
public:
    PatrolActionNode(float patrolDistance = 100.0f);
    NodeResult Execute(Enemy* enemy, float deltaTime) override;
    void OnStart(Enemy* enemy) override;
    void OnEnd(Enemy* enemy) override;

private:
    Vector2 mStartPoint;
    float mPatrolDistance;
    int mCurrentDirection;
    bool mInitialized;
};

// Nó de ação: Persegue o jogador
class ChaseActionNode : public BehaviorTreeNode {
public:
    NodeResult Execute(Enemy* enemy, float deltaTime) override;
    void OnStart(Enemy* enemy) override;
    void OnEnd(Enemy* enemy) override;
};

// Nó de ação: Ataca o jogador
class AttackActionNode : public BehaviorTreeNode {
public:
    AttackActionNode(float attackDuration = 0.5f);
    NodeResult Execute(Enemy* enemy, float deltaTime) override;
    void OnStart(Enemy* enemy) override;
    void OnEnd(Enemy* enemy) override;

private:
    float mAttackDuration;
    float mTimer;
    bool mAttacking;
};

// Nó de ação: Foge do jogador (quando com pouca vida)
class FleeActionNode : public BehaviorTreeNode {
public:
    FleeActionNode(float fleeSpeed = 100.0f);
    NodeResult Execute(Enemy* enemy, float deltaTime) override;
    void OnStart(Enemy* enemy) override;
    void OnEnd(Enemy* enemy) override;

private:
    float mFleeSpeed;
};

// Nó de ação: Fica parado (idle)
class IdleActionNode : public BehaviorTreeNode {
public:
    NodeResult Execute(Enemy* enemy, float deltaTime) override;
    void OnStart(Enemy* enemy) override;
};

#endif //ACTION_NODES_H
