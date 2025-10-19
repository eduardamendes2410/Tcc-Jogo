//
// Created by AI Assistant for Behavior Tree implementation
//

#include "ConditionNodes.h"
#include "../Enemy.h"
#include "../Punk.h"
#include "../../Math.h"

// IsPlayerNearNode Implementation
IsPlayerNearNode::IsPlayerNearNode(float detectionRange) 
    : mDetectionRange(detectionRange) {
}

NodeResult IsPlayerNearNode::Execute(Enemy* enemy, float deltaTime) {
    if (!enemy || !enemy->GetPunk()) {
        return NodeResult::Failure;
    }
    
    Vector2 enemyPos = enemy->GetPosition();
    Vector2 playerPos = enemy->GetPunk()->GetPosition();
    Vector2 diff = playerPos - enemyPos;
    float distance = diff.Length();
    
    return (distance <= mDetectionRange) ? NodeResult::Success : NodeResult::Failure;
}

// IsPlayerInAttackRangeNode Implementation
IsPlayerInAttackRangeNode::IsPlayerInAttackRangeNode(float attackRange) 
    : mAttackRange(attackRange) {
}

NodeResult IsPlayerInAttackRangeNode::Execute(Enemy* enemy, float deltaTime) {
    if (!enemy || !enemy->GetPunk()) {
        return NodeResult::Failure;
    }
    
    Vector2 enemyPos = enemy->GetPosition();
    Vector2 playerPos = enemy->GetPunk()->GetPosition();
    Vector2 diff = playerPos - enemyPos;
    float distance = diff.Length();
    
    return (distance <= mAttackRange) ? NodeResult::Success : NodeResult::Failure;
}

// IsLowHealthNode Implementation
IsLowHealthNode::IsLowHealthNode(float healthThreshold) 
    : mHealthThreshold(healthThreshold) {
}

NodeResult IsLowHealthNode::Execute(Enemy* enemy, float deltaTime) {
    SDL_Log("IsLowHealthNode::Execute()");
    int currentHP = enemy->GetHP();
    NodeResult result =  (currentHP <= mHealthThreshold) ? NodeResult::Success : NodeResult::Failure;
    SDL_Log("IsLowHealthNode result = %d", result);
    return result;

}

// IsPlayerTooFarNode Implementation
IsPlayerTooFarNode::IsPlayerTooFarNode(float loseSightRange) 
    : mLoseSightRange(loseSightRange) {
}

NodeResult IsPlayerTooFarNode::Execute(Enemy* enemy, float deltaTime) {
    if (!enemy || !enemy->GetPunk()) {
        return NodeResult::Success; // Se não há jogador, considera "muito longe"
    }
    
    Vector2 enemyPos = enemy->GetPosition();
    Vector2 playerPos = enemy->GetPunk()->GetPosition();
    Vector2 diff = playerPos - enemyPos;
    float distance = diff.Length();
    
    return (distance > mLoseSightRange) ? NodeResult::Success : NodeResult::Failure;
}

// CanShootNode Implementation
NodeResult CanShootNode::Execute(Enemy* enemy, float deltaTime) {
    if (!enemy) {
        return NodeResult::Failure;
    }
    
    float fireCooldown = enemy->GetFireCooldown();
    return (fireCooldown <= 0.0f) ? NodeResult::Success : NodeResult::Failure;
}
