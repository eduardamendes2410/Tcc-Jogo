//
// Created by AI Assistant for Behavior Tree implementation
//

#ifndef ACTION_NODES_H
#define ACTION_NODES_H

#include "BehaviorTreeNode.h"
#include "../../Math.h"

// Nó de ação: Patrulha entre dois pontos
class PatrolActionNode : public BehaviorTreeNode {
public:
    PatrolActionNode(float patrolDistance = 100.0f);
    NodeResult Execute(Enemy* enemy, float deltaTime) override;
    void OnStart(Enemy* enemy) override;
    void OnEnd(Enemy* enemy) override;

private:
    Vector2 mStartPoint; //come~co do ponto
    float mPatrolDistance; //fim do ponto que ele fica patrulhando
    int mCurrentDirection; //alternar a direcao
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
    float mAttackDuration; //QUANTO TEMPO DURA O ATAQUE
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
    int mHealRate = 1;
    int mMaxHealth = 8;
};

// Nó de ação: Fica parado (idle)
class IdleActionNode : public BehaviorTreeNode {
public:
    NodeResult Execute(Enemy* enemy, float deltaTime) override;
    void OnStart(Enemy* enemy) override;
};

// Nó de ação: Recupera vida gradualmente
class HealOverTimeNode : public BehaviorTreeNode {
public:
    HealOverTimeNode(float healRatePerSecond = 0.5f, float maxHealth = 8.0f)
        : mHealRate(healRatePerSecond), mMaxHealth(maxHealth) {}

    NodeResult Execute(Enemy* enemy, float deltaTime) override;

private:
    float mHealRate;   // quantidade de vida recuperada por segundo
    float mMaxHealth;  // vida máxima do inimigo
};

// Novo nó de ação
class RepositionBehindLeaderNode : public BehaviorTreeNode {
public:
    RepositionBehindLeaderNode(float followDistance);
protected:
    NodeResult Execute(Enemy* enemy, float deltaTime) override;
private:
    float mFollowDistance;
};

// Novo nó de condição
class HasLeaderAndIsWeakNode : public BehaviorTreeNode {
public:
    HasLeaderAndIsWeakNode(float healthThreshold);
protected:
    NodeResult Execute(Enemy* enemy,  float deltaTime) override;
private:
    float mHealthThreshold;
};

#endif //ACTION_NODES_H
