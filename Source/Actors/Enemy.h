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
#include "BehaviorTree/BehaviorTree.h"
#include "../Components/DrawComponents/DrawRectangleComponent.h"

class Enemy: public Actor {
public:
    Enemy(Game* game, Punk* player, int type);
    ~Enemy();

    void OnUpdate(float deltaTime) override;
    void Kill() override;
    void ChangeState(std::unique_ptr<State> newState);

    void Start();
    void TakeDamage();

    void ShootAtPlayer(Vector2 targetPos, AABBColliderComponent* targetComponent); //Shooting related

    void OnHorizontalCollision(const float minOverlap, AABBColliderComponent* other) override;
    void OnVerticalCollision(const float minOverlap, AABBColliderComponent* other) override;

    // Getters para os Estados
    Punk* GetPunk() { return mPunk; }
    RigidBodyComponent* GetRigidBody() { return mRigidBodyComponent; }
    DrawAnimatedComponent* GetDrawComponent() { return mDrawComponent; }
    float GetVelocidade() const { return mVelocidade; }
    void MoveTowardsPlayer();
    
    // Getters para Behavior Tree
    int GetHP() const { return mHP; }
    float GetFireCooldown() const { return mFireCooldown; }
    void SetFireCooldown(float cooldown) { mFireCooldown = cooldown; }

private:
    // Ponteiros para os componentes, igual ao Punk
    RigidBodyComponent* mRigidBodyComponent;
    AABBColliderComponent* mColliderComponent;
    DrawAnimatedComponent* mDrawComponent;

    // O cérebro da FSM (mantido para compatibilidade)
    std::unique_ptr<State> mEstadoAtual;
    
    // O cérebro da Behavior Tree
    std::shared_ptr<BehaviorTree> mBehaviorTree;

    // Dados específicos do inimigo
    Punk* mPunk; // Precisa de uma referência ao player para saber quem seguir/atacar
    float mVelocidade;
    bool mIsDying;
    float mDeathTimer;
    int mHP = 3;
    bool mTakingDamage = false;
    float mDamageTimer = 0.0f;
    int mType;
    bool mIsShooting = false; //Shooting related
    float mFireCooldown = 0.0f; //Shooting related

    Actor* mHudBase;
    DrawRectangleComponent* mDrawHudBackground;
    DrawRectangleComponent* mDrawHudLife;
    int mMaxHP;

};

#endif //ENEMY_H
