//
// Created by eduar on 25/06/2025.
//

#ifndef ENEMY_H
#define ENEMY_H
#include <memory>
#include "Actor.h"
#include "Punk.h"
#include "../Game.h"
#include "FSM/State.h"
#include "../Components/DrawComponents/DrawRectangleComponent.h"
#include "BehaviorTree/Node.h"

enum class AIType {
    FSM,
    BehaviorTree
};

class Enemy: public Actor {
public:
    Enemy(Game* game, Punk* player, int type, AIType aiType);
    ~Enemy();

    void OnUpdate(float deltaTime) override;
    void Kill() override;

    // --- Funções para a FSM ---
    void ChangeState(std::unique_ptr<State> newState);

    void Start();

    // --- Funções para a Lógica de Dano e Estado ---
    void TakeDamage();
    float GetHealth() const { return static_cast<float>(mHP); } // Adapta o mHP para a BT

    // --- Funções de Ação ---
    void ShootAtPlayer(Vector2 targetPos, AABBColliderComponent* targetComponent); //Shooting related
    void MoveTowardsPlayer();

    // --- Funções de Colisão ---
    void OnHorizontalCollision(const float minOverlap, AABBColliderComponent* other) override;
    void OnVerticalCollision(const float minOverlap, AABBColliderComponent* other) override;

    // Getters para os Estados
    Punk* GetPunk() { return mPunk; }
    RigidBodyComponent* GetRigidBody() { return mRigidBodyComponent; }
    DrawAnimatedComponent* GetDrawComponent() { return mDrawComponent; }
    float GetVelocidade() const { return mVelocidade; }

    // --- Funções Específicas da Behavior Tree ---
    void SetLastKnownPosition(const Vector2& pos) { mLastKnownPosition = pos; }
    const Vector2& GetLastKnownPosition() const { return mLastKnownPosition; }
    void JustSawPlayer() { mTimeSinceLastSeen = 0.0f; }
    float GetTimeSinceLastSeen() const { return mTimeSinceLastSeen; }

private:

    // Funções de inicialização privadas
    void SetupFSM();
    void SetupBehaviorTree();

    // Ponteiros para os componentes, igual ao Punk
    RigidBodyComponent* mRigidBodyComponent;
    AABBColliderComponent* mColliderComponent;
    DrawAnimatedComponent* mDrawComponent;

    // O cérebro da IA
    AIType mAiType;
    std::unique_ptr<State> mEstadoAtual;  // Usado APENAS se mAiType == FSM
    std::unique_ptr<Node> mBehaviorTree;

    // Dados específicos do inimigo
    Punk* mPunk; // Precisa de uma referência ao player para saber quem seguir/atacar
    float mVelocidade;
    bool mIsDying;
    float mDeathTimer;
    int mHP = 3;
    bool mTakingDamage = false;
    float mDamageTimer = 0.0f;
    int mType;
    bool mIsShooting; //Shooting related
    float mFireCooldown; //Shooting related

    Actor* mHudBase;
    DrawRectangleComponent* mDrawHudBackground;
    DrawRectangleComponent* mDrawHudLife;
    int mMaxHP;

    // --- Variáveis novas, usadas pela Behavior Tree ---
    Vector2 mLastKnownPosition;
    float mTimeSinceLastSeen;
};

#endif //ENEMY_H
