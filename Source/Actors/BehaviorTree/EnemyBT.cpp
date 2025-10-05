#include "EnemyBT.h"
#include "../../Actors/Punk.h"
#include "../../Components/DrawComponents/DrawAnimatedComponent.h"
#include "../../Components/RigidBodyComponent.h"
#include "../../Math.h"

// --- CONSTRUTORES ---
IsPlayerInAttackRange::IsPlayerInAttackRange(Enemy* owner) : mOwner(owner) {}
IsPlayerInSightRange::IsPlayerInSightRange(Enemy* owner) : mOwner(owner) {}
IsHealthLow::IsHealthLow(Enemy* owner, float healthThreshold) : mOwner(owner), mHealthThreshold(healthThreshold) {}
HasPlayerBeenSeenRecently::HasPlayerBeenSeenRecently(Enemy* owner, float timeThreshold) : mOwner(owner), mTimeThreshold(timeThreshold) {}
AttackPlayer::AttackPlayer(Enemy* owner) : mOwner(owner) {}
ChasePlayer::ChasePlayer(Enemy* owner) : mOwner(owner) {}
Flee::Flee(Enemy* owner) : mOwner(owner) {}
SearchForPlayer::SearchForPlayer(Enemy* owner) : mOwner(owner) {}
PatrolBT::PatrolBT(Enemy* owner, float patrolDistance) : mOwner(owner), mPatrolDistance(patrolDistance) {}


// --- IMPLEMENTAÇÃO DAS CONDIÇÕES ---

NodeStatus IsPlayerInAttackRange::Tick() {
    float attackRange = 120.0f;

    if (mOwner->GetPunk() && Vector2::Distance(mOwner->GetPosition(), mOwner->GetPunk()->GetPosition()) < attackRange) {
        return NodeStatus::Success;
    }
    return NodeStatus::Failure;
}

NodeStatus IsPlayerInSightRange::Tick() {
    float sightRange = 400.0f;
    if (mOwner->GetPunk() && Vector2::Distance(mOwner->GetPosition(), mOwner->GetPunk()->GetPosition()) < sightRange) {
        return NodeStatus::Success;
    }
    return NodeStatus::Failure;
}

NodeStatus IsHealthLow::Tick() {
    if (mOwner->GetHealth() <= mHealthThreshold) {
        return NodeStatus::Success;
    }
    return NodeStatus::Failure;
}

NodeStatus HasPlayerBeenSeenRecently::Tick() {
    if (mOwner->GetTimeSinceLastSeen() < mTimeThreshold) {
        return NodeStatus::Success;
    }
    return NodeStatus::Failure;
}

// --- IMPLEMENTAÇÃO DAS AÇÕES ---

NodeStatus AttackPlayer::Tick() {
    if (!mOwner || !mOwner->GetDrawComponent()) return NodeStatus::Failure;

    mOwner->GetDrawComponent()->SetAnimation("attack");
    mOwner->MoveTowardsPlayer();

    if(mOwner->GetPunk()) {
        mOwner->ShootAtPlayer(mOwner->GetPunk()->GetPosition(), mOwner->GetPunk()->GetComponent<AABBColliderComponent>());
    }
    return NodeStatus::Success;
}

NodeStatus ChasePlayer::Tick() {
    if (!mOwner || !mOwner->GetDrawComponent()) return NodeStatus::Failure;

    mOwner->GetDrawComponent()->SetAnimation("run");
    mOwner->MoveTowardsPlayer();
    return NodeStatus::Success;
}


NodeStatus Flee::Tick() {
    if(!mOwner->GetPunk()) return NodeStatus::Failure;

    mOwner->GetDrawComponent()->SetAnimation("run");

    Vector2 fleeDirection = mOwner->GetPosition() - mOwner->GetPunk()->GetPosition();
    // Verificação de segurança antes de normalizar
        if (fleeDirection.LengthSq() > 0.001f) {
            fleeDirection.Normalize();
        } else {
            // Se estiver na mesma posição, escolhe uma direção padrão
            fleeDirection = Vector2(1.0f, 0.0f);
        }


    RigidBodyComponent* rb = mOwner->GetRigidBody();
    if (rb) {
        float fleeSpeed = mOwner->GetVelocidade() * 1.5f; // Foge um pouco mais rápido
        rb->ApplyForce(fleeDirection * fleeSpeed);
    }
    return NodeStatus::Success;
}

NodeStatus SearchForPlayer::Tick() {
    mOwner->GetDrawComponent()->SetAnimation("run");
    Vector2 targetPos = mOwner->GetLastKnownPosition();
    Vector2 currentPos = mOwner->GetPosition();

    if (Vector2::Distance(currentPos, targetPos) > 20.0f) {
        Vector2 direction = targetPos - currentPos;
        // Verificação de segurança antes de normalizar
                if (direction.LengthSq() > 0.001f) {
                    direction.Normalize();
                } else {
                    return NodeStatus::Success; // Já está no destino
                }


        RigidBodyComponent* rb = mOwner->GetRigidBody();
        if (rb) {
            rb->ApplyForce(direction * mOwner->GetVelocidade());
        }
        return NodeStatus::Running; // Ainda não chegou, continua a ação
    } else {
        RigidBodyComponent* rb = mOwner->GetRigidBody();
        if (rb) {
            rb->SetVelocity(Vector2::Zero);
        }
        // Poderia adicionar uma animação de "procura" aqui
        return NodeStatus::Success; // Chegou ao destino, ação completa
    }
}

NodeStatus PatrolBT::Tick() {
    if (!mIsInitialized) {
        mStartPoint = mOwner->GetPosition();
        mIsInitialized = true;
    }

    mOwner->GetDrawComponent()->SetAnimation("run");

    float leftBoundary = mStartPoint.x - mPatrolDistance;
    float rightBoundary = mStartPoint.x + mPatrolDistance;
    Vector2 currentPos = mOwner->GetPosition();

    if (mCurrentDirection == 1 && currentPos.x >= rightBoundary) {
        mCurrentDirection = -1;
        mOwner->SetRotation(Math::Pi);
    } else if (mCurrentDirection == -1 && currentPos.x <= leftBoundary) {
        mCurrentDirection = 1;
        mOwner->SetRotation(0.0f);
    }

    RigidBodyComponent* rb = mOwner->GetRigidBody();
    if (rb) {
        float forwardSpeed = mOwner->GetVelocidade();
        rb->ApplyForce(Vector2(mCurrentDirection * forwardSpeed, 0.0f));
    }
    return NodeStatus::Success;
}