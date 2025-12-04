//
// Created by AI Assistant for Behavior Tree implementation
//

#include "ActionNodes.h"
#include "../Enemy.h"
#include "../Punk.h"
#include "../../Components/RigidBodyComponent.h"
#include "../../Components/DrawComponents/DrawAnimatedComponent.h"
#include "../../Math.h"

float MAX_HEALTH = 8.0f;

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
        enemy->SetRotation(Math::Pi); //vira pro outro lado
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

// ------------------------------------------------------------------------------------------------------------

// ChaseActionNode Implementation
void ChaseActionNode::OnStart(Enemy* enemy) {
    enemy->GetDrawComponent()->SetAnimation("run");
}

NodeResult ChaseActionNode::Execute(Enemy* enemy, float deltaTime) {
    if (!enemy || !enemy->GetPunk()) {
        return NodeResult::Failure;
    }
    //quando retorna sucess?
    enemy->MoveTowardsPlayer();
    return NodeResult::Running;
}

void ChaseActionNode::OnEnd(Enemy* enemy) {
    RigidBodyComponent* rb = enemy->GetRigidBody();
    if (rb) {
        rb->SetVelocity(Vector2::Zero);
    }
}

// ------------------------------------------------------------------------------------------------------------

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

// cRIAR UM ATAQUE QUE ELE VAI ATIRAR MAIS VEZES OU MAIS RAPIDO E SAIR CORRENDO

// ------------------------------------------------------------------------------------------------------------

// FleeActionNode Implementation - FUGA
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
    float distance = direction.Length();

    RigidBodyComponent* rb = enemy->GetRigidBody();
    if (!rb) return NodeResult::Failure;

    // Se já estiver longe o bastante, cura e finaliza
    if (distance > 300.0f) {
        rb->SetVelocity(Vector2(0.0f, 0.0f));
        return NodeResult::Success;
    }

    direction.Normalize();
    
    // Vira o sprite para a direção do movimento (CORRIGIDO)
    if (direction.x > 0.0f) {
        enemy->SetRotation(0.0f); // Virado para a direita
    } else if (direction.x < 0.0f) {
        enemy->SetRotation(Math::Pi); // Virado para a esquerda (180 graus)
    }

    // Aplica velocidade de fuga MAIS RÁPIDA usando ApplyForce
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
// ------------------------------------------------------------------------------------------------------------


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

// ------------------------------------------------------------------------------------------------------------

// Nó de ação: Recupera vida gradualmente
NodeResult HealOverTimeNode::Execute(Enemy* enemy, float deltaTime) {
    NodeResult result = NodeResult::Running;

    if (!enemy) {
        result = NodeResult::Failure;
        //SDL_Log("HealOverTimeNode result %d", result);
        return result;
    }

    // Verifica distância até o jogador
    if (enemy->GetPunk()) {
        Vector2 enemyPos = enemy->GetPosition();
        Vector2 playerPos = enemy->GetPunk()->GetPosition();
        Vector2 diff = playerPos - enemyPos;
        float distance = diff.Length();

        // Se o jogador estiver muito perto, interrompe a cura
        if (distance < 80.0f) {
            //SDL_Log("Jogador se aproximou! Interrompendo cura (distância: %f)", distance);
            return NodeResult::Failure;
        }
    }

    static float healTimer = 0.0f;
    healTimer += deltaTime;

    if (healTimer >= 0.2f) {
        healTimer = 0.0f;
        if (enemy->GetHP() < MAX_HEALTH) {
            enemy->IncrementHP(mHealRate);
        }
    }

    // Continua até HP igual a 6 para nao ficar impossivel
    if (enemy->GetHP() >= 6) {
        result = NodeResult::Success;
        return result;
    }
        return result;
    }

// --- Implementação dos novos nós ---

RepositionBehindLeaderNode::RepositionBehindLeaderNode(float followDistance)
    : mFollowDistance(followDistance) {}

NodeResult RepositionBehindLeaderNode::Execute(Enemy* enemy, float deltaTime) {
    Enemy* leader = enemy->GetLeader();
    Punk* player = enemy->GetPunk();

    if (!leader || !player || leader->GetState() == ActorState::Destroy) {
        return NodeResult::Failure; // Se não há líder ou jogador, esta ação falha.
    }

    const float maxLeaderDistance = 600.0f;

    float distanceToLeader = Vector2::Distance(enemy->GetPosition(), leader->GetPosition());
    if (distanceToLeader > maxLeaderDistance) {
        // Se o líder estiver muito longe, não segue
        return NodeResult::Failure;
    }

    // Calcula a posição "atrás" do líder, longe do jogador
    Vector2 leaderPos = leader->GetPosition();
    Vector2 playerPos = player->GetPosition();

    Vector2 directionFromPlayerToLeader = Vector2::Normalize(leaderPos - playerPos);
    Vector2 targetPos = leaderPos + (directionFromPlayerToLeader * mFollowDistance);

    // Se já está perto o suficiente do ponto, sucesso!
    if (Vector2::Distance(enemy->GetPosition(), targetPos) < 20.0f) {
        return NodeResult::Success;
    }

    // Move em direção ao ponto de recuo
    Vector2 moveDirection = Vector2::Normalize(targetPos - enemy->GetPosition());
    enemy->GetRigidBody()->SetVelocity(moveDirection * enemy->GetVelocidade() * 1.2f);

    float angle = Math::Atan2(moveDirection.y, moveDirection.x);
    enemy->SetRotation(angle);

    return NodeResult::Running;
}

HasLeaderAndIsWeakNode::HasLeaderAndIsWeakNode(float healthThreshold)
    : mHealthThreshold(healthThreshold) {}

NodeResult HasLeaderAndIsWeakNode::Execute(Enemy* enemy,  float deltaTime) {
    if (enemy->GetLeader() && enemy->GetLeader()->GetState() != ActorState::Destroy) {
        if (enemy->GetHP() <= mHealthThreshold)
        return NodeResult::Success;
    }
    return NodeResult::Failure;
}
