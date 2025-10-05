#pragma once
#include "Node.h"
#include "../../Actors/Enemy.h" // Precisa conhecer a classe Enemy para interagir com ela

// --- CONDIÇÕES (Nós que retornam Success ou Failure) ---

class IsPlayerInAttackRange : public Node {
public:
    IsPlayerInAttackRange(Enemy* owner);
    NodeStatus Tick() override;
private:
    Enemy* mOwner;
};

class IsPlayerInSightRange : public Node {
public:
    IsPlayerInSightRange(Enemy* owner);
    NodeStatus Tick() override;
private:
    Enemy* mOwner;
};

class IsHealthLow : public Node {
public:
    IsHealthLow(Enemy* owner, float healthThreshold);
    NodeStatus Tick() override;
private:
    Enemy* mOwner;
    float mHealthThreshold;
};

class HasPlayerBeenSeenRecently : public Node {
public:
    HasPlayerBeenSeenRecently(Enemy* owner, float timeThreshold);
    NodeStatus Tick() override;
private:
    Enemy* mOwner;
    float mTimeThreshold;
};

// --- AÇÕES (Nós que executam algo no jogo) ---

class AttackPlayer : public Node {
public:
    AttackPlayer(Enemy* owner);
    NodeStatus Tick() override;
private:
    Enemy* mOwner;
};

class ChasePlayer : public Node {
public:
    ChasePlayer(Enemy* owner);
    NodeStatus Tick() override;
private:
    Enemy* mOwner;
};

class Flee : public Node {
public:
    Flee(Enemy* owner);
    NodeStatus Tick() override;
private:
    Enemy* mOwner;
};

class SearchForPlayer : public Node {
public:
    SearchForPlayer(Enemy* owner);
    NodeStatus Tick() override;
private:
    Enemy* mOwner;
};

class PatrolBT : public Node {
public:
    PatrolBT(Enemy* owner, float patrolDistance);
    NodeStatus Tick() override;
private:
    Enemy* mOwner;
    float mPatrolDistance;
    Vector2 mStartPoint;
    int mCurrentDirection = 1;
    bool mIsInitialized = false;
};