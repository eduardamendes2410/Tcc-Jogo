//
// Created by eduar on 25/06/2025.
//

#include "Enemy.h"

#include <iostream>

#include "Projectile.h"
#include "Block.h"
#include "../Game.h"
#include "Punk.h"
#include "../Components/RigidBodyComponent.h"
#include "FSM/State.h"
#include "../Components/DrawComponents/DrawAnimatedComponent.h"
#include "../Components/ColliderComponents/AABBColliderComponent.h"
#include "FSM/Patrol.h"
#include "../Components/DrawComponents/DrawRectangleComponent.h"
#include "BehaviorTree/EnemyBT.h"
#include "BehaviorTree/Selector.h"
#include "BehaviorTree/Sequence.h"

// Inclui os estados que vamos usar

const float INIMIGO_DEATH_TIME = 0.35f;

Enemy::Enemy(Game* game, Punk* punk, int type, AIType aiType)
    : Actor(game)
    , mPunk(punk)
    ,mAiType(aiType)
    , mVelocidade(75.0f) // Velocidade um pouco menor que a do player
    , mIsDying(false)
    , mDeathTimer(0.0f)
    ,mType(type)
    ,mMaxHP(3)
   ,mHP(3)
    ,mTimeSinceLastSeen(999.0f)

{
    // Configura os componentes, assim como no Punk
    mRigidBodyComponent = new RigidBodyComponent(this, 1.0f, 2.0f, false);
    mColliderComponent = new AABBColliderComponent(this, 0, 0, 32, 32, ColliderLayer::Enemy);
    if (mType == 0) {
        //inimigoA
        // Use os assets do seu inimigo aqui
        mDrawComponent = new DrawAnimatedComponent(this,
                                                  "../Assets/Sprites/Enemy/minion.png",
                                                   "../Assets/Sprites/Enemy/texture.json");

        // // Adiciona as animações que o inimigo terá
         mDrawComponent->AddAnimation("idle", {0,1,2,3,4});
         mDrawComponent->AddAnimation("run", {5,6,7,8,9,10,11,12});
         mDrawComponent->AddAnimation("death", {15,16,17,18,19});
         mDrawComponent->AddAnimation("damaged", {13,14});
        //
         mDrawComponent->SetAnimation("idle");
         mDrawComponent->SetAnimFPS(10.0f);
         mDrawComponent->SetPivot(Vector2(0.5f, 0.5f));
    }
    else {
        //inimigoB
        mDrawComponent = new DrawAnimatedComponent(this,
                                                   "../Assets/Sprites/EnemyB/hot_wheels.png",
                                                   "../Assets/Sprites/EnemyB/texture.json");
        mDrawComponent->AddAnimation("sleep", {1});
        mDrawComponent->AddAnimation("idle", {2,3,4,5});
        mDrawComponent->AddAnimation("run", {6,7,8,9,10,11,12,13});
        mDrawComponent->AddAnimation("attack", {14,15,16,17});
        mDrawComponent->AddAnimation("damaged", {18,19});
        mDrawComponent->AddAnimation("death", {20,21,22,23});


        mDrawComponent->SetAnimation("run");
        mDrawComponent->SetAnimFPS(10.0f);
        mDrawComponent->SetPivot(Vector2(0.5f, 0.5f));
    }

    mHudBase = new Actor(mGame);
    float barWidth = 50.0f;
    float barHeight = 6.0f;

    // Barra vermelha (vida cheia no início)
    mDrawHudLife = new DrawRectangleComponent(mHudBase, Vector2(barWidth, barHeight), Vector3(1.0f, 0.0f, 0.0f), 201);


    // --- A INICIALIZAÇÃO DA FSM ACONTECE AQUI! ---
    // O inimigo começa no estado "Patrulhando"
    //mEstadoAtual = std::make_unique<Patrol>(); SE DER PAU VOLTO AQUI

    if (mAiType == AIType::BehaviorTree) {
        SetupBehaviorTree();
    } else { // AIType::FSM
        SetupFSM();
    }
}

Enemy::~Enemy() {
    if (mHudBase)
        mHudBase->SetState(ActorState::Destroy);
}


void Enemy::OnUpdate(float deltaTime)
{

    if (mTakingDamage)
    {
        mDamageTimer -= deltaTime;
        if (mDamageTimer <= 0.0f)
        {
            mTakingDamage = false;

            if (mHP <= 0)
            {
                Kill();
            }
            else
            {
                mDrawComponent->SetAnimation("idle");
            }
        }
        return;
    }

    if (mFireCooldown > 0.0f)
        mFireCooldown -= deltaTime;

    if (mIsDying) {
        mDeathTimer -= deltaTime;
        if (mDeathTimer <= 0.0f) {
            SetState(ActorState::Destroy);
        }
        return;
    }


    // --- NEW: DUAL AI EXECUTION LOGIC ---
    if (mAiType == AIType::BehaviorTree) {
        // If it's a BT enemy, update its variables and Tick the tree
        mTimeSinceLastSeen += deltaTime;
        float sightRange = 400.0f;
        if (mPunk && Vector2::Distance(GetPosition(), mPunk->GetPosition()) < sightRange) {
            JustSawPlayer();
            SetLastKnownPosition(mPunk->GetPosition());
        }
        if (mBehaviorTree) {
            mBehaviorTree->Tick();
        }
    } else { // AIType::FSM
        // If it's an FSM enemy, update the current state as before
        if (mEstadoAtual) {
            mEstadoAtual->Update(this, deltaTime);
        }
    }

    //SE DER PAU VOLTO AQUI
    // if (mEstadoAtual) {
    //     mEstadoAtual->Update(this, deltaTime);
    // }

    if (!mIsDying) {
        Vector2 enemyPos = GetPosition();
        Vector2 hudOffset(0.0f, -20.0f);
        if (mHudBase)
            mHudBase->SetPosition(enemyPos + hudOffset);

        if (mDrawHudLife)
        {
            float hpPercent = static_cast<float>(mHP) / 3.0f;
            float fullWidth = 50.0f;
            mDrawHudLife->SetSize(Vector2(fullWidth * hpPercent, 6.0f));
        }
    }
}

void Enemy::SetupFSM() {
    mEstadoAtual = std::make_unique<Patrol>(150.0f); // Pass patrol distance
    if(mEstadoAtual) {
        mEstadoAtual->Enter(this);
    }
}

void Enemy::SetupBehaviorTree() {
    auto root = std::make_unique<Selector>();

    // Ramo 1: Fuga
    auto fleeSequence = std::make_unique<Sequence>();
    fleeSequence->AddChild(std::make_unique<IsHealthLow>(this, 1.0f)); // Flee at 1 HP
    fleeSequence->AddChild(std::make_unique<Flee>(this));

    // Ramo 2: Ataque
    auto attackSequence = std::make_unique<Sequence>();
    attackSequence->AddChild(std::make_unique<IsPlayerInAttackRange>(this));
    attackSequence->AddChild(std::make_unique<AttackPlayer>(this));

    // Ramo 3: Perseguição
    auto chaseSequence = std::make_unique<Sequence>();
    chaseSequence->AddChild(std::make_unique<IsPlayerInSightRange>(this));
    chaseSequence->AddChild(std::make_unique<ChasePlayer>(this));

    // Ramo 4: Procura
    auto searchSequence = std::make_unique<Sequence>();
    searchSequence->AddChild(std::make_unique<HasPlayerBeenSeenRecently>(this, 5.0f));
    searchSequence->AddChild(std::make_unique<SearchForPlayer>(this));

    // Ação Padrão: Patrulha
    auto patrolAction = std::make_unique<PatrolBT>(this, 150.0f);

    root->AddChild(std::move(fleeSequence));
    root->AddChild(std::move(attackSequence));
    root->AddChild(std::move(chaseSequence));
    root->AddChild(std::move(searchSequence));
    root->AddChild(std::move(patrolAction));

    mBehaviorTree = std::move(root);
}

void Enemy::TakeDamage()
{
    if (mIsDying || mTakingDamage) return;

    mHP--;

    if (mHP <= 0)
    {
        mTakingDamage = true;
        mDamageTimer = 0.2f;
        mDrawComponent->SetAnimation("damaged");
        mRigidBodyComponent->SetVelocity(Vector2::Zero);
    }
    else
    {
        mTakingDamage = true;
        mDamageTimer = 0.2f;
        mDrawComponent->SetAnimation("damaged");
        mRigidBodyComponent->SetVelocity(Vector2::Zero);
    }
}

void Enemy::ChangeState(std::unique_ptr<State> novoEstado) {
    // 1. Chama o Exit do estado atual (se houver) para limpeza
    if (mEstadoAtual) {
        mEstadoAtual->Exit(this);
    }
    // 2. Troca o estado. O unique_ptr antigo é destruído automaticamente.
    mEstadoAtual = std::move(novoEstado);
    // 3. Chama o Enter do novo estado para inicialização
    if (mEstadoAtual) {
        mEstadoAtual->Enter(this);
    }
}

void Enemy::Start() {
    if (mEstadoAtual) {
        mEstadoAtual->Enter(this);
    }}

void Enemy::Kill() {
    if (mIsDying) return;

    mIsDying = true;
    mDeathTimer = INIMIGO_DEATH_TIME;
    mRigidBodyComponent->SetVelocity(Vector2::Zero); // Para de se mover
    mDrawComponent->SetAnimation("death");
}

void Enemy::ShootAtPlayer(Vector2 targetPos, AABBColliderComponent* targetColliderComponent) //OBS: target pos no caso do jogador é o mouse, é onde estamos mirando. Aqui seria a posição do jogador
{
    Vector2 playerCenterOffset = Vector2(targetColliderComponent->GetWidth() / 2, targetColliderComponent->GetHeight() / 2);
    Vector2 targetPosCenter = targetPos + playerCenterOffset;

    Vector2 center = mColliderComponent->GetMin() + Vector2(mColliderComponent->GetWidth() / 2, mColliderComponent->GetHeight() / 2);
    Vector2 direction = targetPosCenter - center;
    direction.Normalize();

    if (targetPosCenter.x > center.x)
        SetRotation(0.0f);
    else
        SetRotation(Math::Pi);

    Vector2 mouthOffset = Vector2(0.0f, 0.0f); //OBS: ajustável conforme sprite, vai ajustando para sair da boca do bicho
    Vector2 firePosition = center + mouthOffset;

    if (mFireCooldown <= 0.0f) {
        Projectile* projectile = new Projectile(mGame, ColliderLayer::EnemyProjectile, 2);
        projectile->SetPosition(firePosition);
        projectile->mPreviousPosition = firePosition;
        projectile->GetComponent<RigidBodyComponent>()->ApplyForce(direction * 2500.0f);
        mFireCooldown = .9f;
    }
}


void Enemy::MoveTowardsPlayer()
{
    Punk* punk = GetPunk();
    RigidBodyComponent* rb = GetRigidBody();

    // Se não houver punk ou corpo rígido, não faz nada
    if (!punk || !rb) {
        return;
    }

    // Pega a direção para o jogador
    Vector2 direction = punk->GetPosition() - GetPosition();
    direction.Normalize();

    // Vira o sprite para a direção do movimento
    if (direction.x > 0.0f) {
        SetRotation(0.0f);
    } else if (direction.x < 0.0f) {
        SetRotation(Math::Pi);
    }

    // Define a velocidade para se mover naquela direção
    float speed = GetVelocidade();
    rb->SetVelocity(direction * speed);
}


void Enemy::OnHorizontalCollision(const float minOverlap, AABBColliderComponent* other)
{
    if (mIsDying) return;

    if (other->GetLayer() == ColliderLayer::PlayerProjectile) {
        TakeDamage();

        other->GetOwner()->SetState(ActorState::Destroy);
    }
}

void Enemy::OnVerticalCollision(const float minOverlap, AABBColliderComponent* other)
{
    if (mIsDying) return;

    if (other->GetLayer() == ColliderLayer::PlayerProjectile) {
        TakeDamage();

        other->GetOwner()->SetState(ActorState::Destroy);
    }

    else if (other->GetLayer() == ColliderLayer::Bricks && minOverlap < 0) {
        Block *block = static_cast<Block*>(other->GetOwner());
        block->OnColision();
    }
}



