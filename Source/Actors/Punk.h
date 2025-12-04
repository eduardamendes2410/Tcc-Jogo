//
// Created by Lucas N. Ferreira on 08/09/23.
//

#pragma once
#include "Actor.h"
#include "PunkArm.h"
#include <SDL.h>

const float DEATH_TIMER = 0.71f; // seconds

class Punk : public Actor
{
public:
    explicit Punk(Game* game, float forwardSpeed = 1500.0f, float jumpSpeed = -750.0f);

    void OnProcessInput(const Uint8* keyState) override;
    void OnUpdate(float deltaTime) override;
    void OnHandleKeyPress(const int key, const bool isPressed) override;

    void OnHorizontalCollision(const float minOverlap, AABBColliderComponent* other) override;
    void OnVerticalCollision(const float minOverlap, AABBColliderComponent* other) override;

    void Kill() override;
    void Win(AABBColliderComponent *poleCollider);
    int Lives() { return mLives; }

    void FindKey();
    void FindHeart();
    void FindShotgun();

    void OnShoot(Vector2 &recoilForce);

    Vector2 GetCenter() const
    {
        return mColliderComponent->GetCenter();
    }   

    int GetAmmo();
    int GetMaxAmmo();

    std::string GetCurrentWeaponName();

    void StartDash();
    bool GetCollisionEnabled() {return mCollisionEnabled;}

private:
    static const int POLE_SLIDE_TIME = 1; // Time in seconds to slide down the pole

    void ManageAnimations();

    float mForwardSpeed;
    float mJumpSpeed;
    float mPoleSlideTimer;
    bool mIsRunning;
    bool mIsOnPole;
    bool mIsDying; float mDeathTimer;
    bool mFoundKey;

    int mLives = 5;
    float mInvincibilityTimer = 0.0f;

    class RigidBodyComponent* mRigidBodyComponent;
    class DrawAnimatedComponent* mDrawComponent;
    class AABBColliderComponent* mColliderComponent;

    class PunkArm* mArm;

    void MaintainInbound();
    void TakeDamage();

    bool mIsDashing = false;
    float mDashDuration = 0.2f;     // duração do dash (em segundos)
    float mDashTimer = 0.0f;        // controla o tempo de dash
    float mDashSpeed = 1200.0f;     // velocidade do dash
    bool mCollisionEnabled = true;  // controla colisões

    float mDashCooldown = 0.5f;      // tempo entre dashes
    float mDashCooldownTimer = 0.0f;  // controla o tempo restante

    Vector2 mLastMoveDir = Vector2(1.0f, 0.0f);
};