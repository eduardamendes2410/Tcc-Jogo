//
// Created by AI Assistant for Behavior Tree implementation
//

#include "ActionNodes.h"
#include "../Enemy.h"
#include "../Punk.h"
#include "../../Components/RigidBodyComponent.h"
#include "../../Components/DrawComponents/DrawAnimatedComponent.h"
#include "../../Math.h"

// PatrolActionNode Implementation
PatrolActionNode::PatrolActionNode(float patrolDistance) 
    : mPatrolDistance(patrolDistance), mCurrentDirection(1), mInitialized(false) {
}

void PatrolActionNode::OnStart(Enemy* enemy) {
    mStartPoint = enemy->GetPosition();
    mCurrentDirection = 1;
    mInitialized = true;
    enemy->GetDrawComponent()->SetAnimation("run");
    enemy->SetRotation(0.0f);
}

NodeResult PatrolActionNode::Execute(Enemy* enemy, float deltaTime) {
    if (!mInitialized) {
        OnStart(enemy);
    }
    
    // Define os limites da patrulha
    float leftBoundary = mStartPoint.x - mPatrolDistance;
    float rightBoundary = mStartPoint.x + mPatrolDistance;
    
    Vector2 currentPos = enemy->GetPosition();
    
    // Verifica se atingiu os limites e inverte a direção
    if (mCurrentDirection == 1 && currentPos.x >= rightBoundary) {
        mCurrentDirection = -1;
        enemy->SetRotation(Math::Pi);
    }
    else if (mCurrentDirection == -1 && currentPos.x <= leftBoundary) {
        mCurrentDirection = 1;
        enemy->SetRotation(0.0f);
    }
    
    // Aplica a força para mover o inimigo
    RigidBodyComponent* rb = enemy->GetRigidBody();
    if (rb) {
        float forwardSpeed = enemy->GetVelocidade();
        rb->ApplyForce(Vector2(mCurrentDirection * forwardSpeed, 0.0f));
    }
    
    return NodeResult::Running;
}

void PatrolActionNode::OnEnd(Enemy* enemy) {
    RigidBodyComponent* rb = enemy->GetRigidBody();
    if (rb) {
        rb->SetVelocity(Vector2(0.0f, rb->GetVelocity().y));
    }
}

// ChaseActionNode Implementation
void ChaseActionNode::OnStart(Enemy* enemy) {
    enemy->GetDrawComponent()->SetAnimation("run");
}

NodeResult ChaseActionNode::Execute(Enemy* enemy, float deltaTime) {
    if (!enemy || !enemy->GetPunk()) {
        return NodeResult::Failure;
    }
    
    enemy->MoveTowardsPlayer();
    return NodeResult::Running;
}

void ChaseActionNode::OnEnd(Enemy* enemy) {
    RigidBodyComponent* rb = enemy->GetRigidBody();
    if (rb) {
        rb->SetVelocity(Vector2::Zero);
    }
}

// AttackActionNode Implementation
AttackActionNode::AttackActionNode(float attackDuration) 
    : mAttackDuration(attackDuration), mTimer(0.0f), mAttacking(false) {
}

void AttackActionNode::OnStart(Enemy* enemy) {
    mTimer = mAttackDuration;
    mAttacking = true;
}

NodeResult AttackActionNode::Execute(Enemy* enemy, float deltaTime) {
    if (!enemy || !enemy->GetPunk()) {
        return NodeResult::Failure;
    }
    
    if (!mAttacking) {
        OnStart(enemy);
    }
    
    // Continua se movendo em direção ao jogador
    enemy->MoveTowardsPlayer();
    
    // Tenta atirar
    enemy->ShootAtPlayer(enemy->GetPunk()->GetPosition(), 
                        enemy->GetPunk()->GetComponent<AABBColliderComponent>());
    
    // Decrementa o cronômetro
    mTimer -= deltaTime;
    if (mTimer <= 0.0f) {
        mAttacking = false;
        return NodeResult::Success;
    }
    
    return NodeResult::Running;
}

void AttackActionNode::OnEnd(Enemy* enemy) {
    mAttacking = false;
}

// FleeActionNode Implementation
FleeActionNode::FleeActionNode(float fleeSpeed) : mFleeSpeed(fleeSpeed) {
}

void FleeActionNode::OnStart(Enemy* enemy) {
    if (enemy && enemy->GetDrawComponent()) {
        enemy->GetDrawComponent()->SetAnimation("run");
    }
}

NodeResult FleeActionNode::Execute(Enemy* enemy, float deltaTime) {
    if (!enemy || !enemy->GetPunk()) {
        return NodeResult::Failure;
    }
    
    // Calcula direção oposta ao jogador
    Vector2 enemyPos = enemy->GetPosition();
    Vector2 playerPos = enemy->GetPunk()->GetPosition();
    Vector2 direction = enemyPos - playerPos; // Direção oposta ao jogador
    direction.Normalize();
    
    // Vira o sprite para a direção do movimento (CORRIGIDO)
    if (direction.x > 0.0f) {
        enemy->SetRotation(0.0f); // Virado para a direita
    } else if (direction.x < 0.0f) {
        enemy->SetRotation(Math::Pi); // Virado para a esquerda (180 graus)
    }
    
    // Aplica velocidade de fuga MAIS RÁPIDA usando ApplyForce
    RigidBodyComponent* rb = enemy->GetRigidBody();
    if (rb) {
        // Usa ApplyForce para movimento mais fluido e rápido
        float fleeForce = mFleeSpeed * 2.0f; // Dobra a força de fuga
        rb->ApplyForce(direction * fleeForce);
    }
    
    return NodeResult::Running;
}

void FleeActionNode::OnEnd(Enemy* enemy) {
    RigidBodyComponent* rb = enemy->GetRigidBody();
    if (rb) {
        rb->SetVelocity(Vector2::Zero);
    }
}

// IdleActionNode Implementation
void IdleActionNode::OnStart(Enemy* enemy) {
    enemy->GetDrawComponent()->SetAnimation("idle");
}

NodeResult IdleActionNode::Execute(Enemy* enemy, float deltaTime) {
    // Para o inimigo
    RigidBodyComponent* rb = enemy->GetRigidBody();
    if (rb) {
        rb->SetVelocity(Vector2::Zero);
    }
    
    return NodeResult::Running;
}
