/**
* Author: Quang Nguyen
* Assignment: Rise of the AI
* Date due: 2025-11-08, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "Entity.h"

#include <cmath>

// Default constructor
Entity::Entity()
: mPosition{0.0f, 0.0f},
  mMovement{0.0f, 0.0f},
  mVelocity{0.0f, 0.0f},
  mAcceleration{0.0f, 0.0f},
  mScale{DEFAULT_SIZE, DEFAULT_SIZE},
  mColliderDimensions{DEFAULT_SIZE, DEFAULT_SIZE},
  mTexture{ 0 },
  mTextureType{ SINGLE },
  mSpriteSheetDimensions{0.0f, 0.0f},
  mAnimationAtlas{},
  mAnimationIndices{},
  mDirection{ RIGHT },
  mFrameSpeed{ DEFAULT_FRAME_SPEED },
  mCurrentFrameIndex{ 0 },
  mAnimationTime{ 0.0f },
  mIsJumping{ false },
  mJumpingPower{ 0.0f },
  mSpeed{ DEFAULT_SPEED },
  mAngle{ 0.0f },
  mIsCollidingTop{ false },
  mIsCollidingBottom{ false },
  mIsCollidingRight{ false },
  mIsCollidingLeft{ false },
  mEntityStatus{ ACTIVE },
  mEntityType{ NONE },
  mAIType{ WANDERER },
  mAIState{ IDLE }
{
}

// Simple texture (single) constructor
Entity::Entity(Vector2 position, Vector2 scale, const char *textureFilepath, EntityType entityType)
: mPosition{ position },
  mMovement{ 0.0f, 0.0f },
  mVelocity{ 0.0f, 0.0f },
  mAcceleration{ 0.0f, 0.0f },
  mScale{ scale },
  mColliderDimensions{ scale },
  mTexture{ LoadTexture(textureFilepath) },
  mTextureType{ SINGLE },
  mSpriteSheetDimensions{ 0.0f, 0.0f },
  mAnimationAtlas{},
  mAnimationIndices{},
  mDirection{ RIGHT },
  mFrameSpeed{ DEFAULT_FRAME_SPEED },
  mCurrentFrameIndex{ 0 },
  mAnimationTime{ 0.0f },
  mIsJumping{ false },
  mJumpingPower{ 0.0f },
  mSpeed{ DEFAULT_SPEED },
  mAngle{ 0.0f },
  mIsCollidingTop{ false },
  mIsCollidingBottom{ false },
  mIsCollidingRight{ false },
  mIsCollidingLeft{ false },
  mEntityStatus{ ACTIVE },
  mEntityType{ entityType },
  mAIType{ WANDERER },
  mAIState{ IDLE }
{
}

// Atlas constructor
Entity::Entity(Vector2 position, Vector2 scale, const char *textureFilepath, TextureType textureType,
Vector2 spriteSheetDimensions, std::map<Direction, std::vector<int>> animationAtlas, EntityType entityType)
: mPosition{ position },
  mMovement{ 0.0f, 0.0f },
  mVelocity{ 0.0f, 0.0f },
  mAcceleration{ 0.0f, 0.0f },
  mScale{ scale },
  mColliderDimensions{ scale },
  mTexture{ LoadTexture(textureFilepath) },
  mTextureType{ animationAtlas.empty() ? SINGLE : ATLAS },  // FIX: Only ATLAS if map has data
  mSpriteSheetDimensions{ spriteSheetDimensions },
  mAnimationAtlas{ animationAtlas },
  mAnimationIndices{ animationAtlas.empty() ? std::vector<int>{} : animationAtlas.at(RIGHT) },  // FIX: Check if empty
  mDirection{ RIGHT },
  mFrameSpeed{ DEFAULT_FRAME_SPEED },
  mCurrentFrameIndex{ 0 },
  mAnimationTime{ 0.0f },
  mIsJumping{ false },
  mJumpingPower{ 0.0f },
  mSpeed{ DEFAULT_SPEED },
  mAngle{ 0.0f },
  mIsCollidingTop{ false },
  mIsCollidingBottom{ false },
  mIsCollidingRight{ false },
  mIsCollidingLeft{ false },
  mEntityStatus{ ACTIVE },
  mEntityType{ entityType },
  mAIType{ WANDERER },
  mAIState{ IDLE }
{
}

// Move constructor: steal resources
Entity::Entity(Entity&& other) noexcept
{
    // move simple PODs
    *this = std::move(other); // forward to move assignment for brevity
}

// Move assignment: steal texture id and other resources, zero out source
Entity& Entity::operator=(Entity&& other) noexcept
{
    if (this == &other) return *this;

    // shallow/move copy PODs and complex members
    mPosition = other.mPosition;
    mMovement = other.mMovement;
    mVelocity = other.mVelocity;
    mAcceleration = other.mAcceleration;
    mScale = other.mScale;
    mColliderDimensions = other.mColliderDimensions;

    // steal texture handle (so only one destructor will UnloadTexture)
    mTexture = other.mTexture;
    other.mTexture = { 0, 0, 0, 0, 0 }; // clear source to avoid double-unload

    mTextureType = other.mTextureType;
    mSpriteSheetDimensions = other.mSpriteSheetDimensions;
    mAnimationAtlas = std::move(other.mAnimationAtlas);
    mAnimationIndices = std::move(other.mAnimationIndices);
    mDirection = other.mDirection;
    mFrameSpeed = other.mFrameSpeed;
    mCurrentFrameIndex = other.mCurrentFrameIndex;
    mAnimationTime = other.mAnimationTime;
    mIsJumping = other.mIsJumping;
    mJumpingPower = other.mJumpingPower;
    mSpeed = other.mSpeed;
    mAngle = other.mAngle;
    mIsCollidingTop = other.mIsCollidingTop;
    mIsCollidingBottom = other.mIsCollidingBottom;
    mIsCollidingRight = other.mIsCollidingRight;
    mIsCollidingLeft = other.mIsCollidingLeft;
    mEntityStatus = other.mEntityStatus;
    mEntityType = other.mEntityType;
    mAIType = other.mAIType;
    mAIState = other.mAIState;

    // clear other to a safe state
    other.mPosition = {0.0f, 0.0f};
    other.mMovement = {0.0f, 0.0f};
    other.mVelocity = {0.0f, 0.0f};
    other.mAcceleration = {0.0f, 0.0f};
    other.mScale = {0.0f, 0.0f};
    other.mColliderDimensions = {0.0f, 0.0f};
    other.mTextureType = SINGLE;
    other.mSpriteSheetDimensions = {0.0f, 0.0f};
    other.mAnimationAtlas.clear();
    other.mAnimationIndices.clear();
    other.mDirection = RIGHT;
    other.mFrameSpeed = DEFAULT_FRAME_SPEED;
    other.mCurrentFrameIndex = 0;
    other.mAnimationTime = 0.0f;
    other.mIsJumping = false;
    other.mJumpingPower = 0.0f;
    other.mSpeed = DEFAULT_SPEED;
    other.mAngle = 0.0f;
    other.mIsCollidingTop = other.mIsCollidingBottom = other.mIsCollidingRight = other.mIsCollidingLeft = false;
    other.mEntityStatus = INACTIVE;
    other.mEntityType = NONE;
    other.mAIType = WANDERER;
    other.mAIState = IDLE;

    return *this;
}

Entity::~Entity() {
    // Only unload if texture has a valid id
    if (mTexture.id != 0) {
        UnloadTexture(mTexture);
        mTexture.id = 0;
    }
}

// ---- collision, AI, update, render implementations ----

void Entity::checkCollisionY(Entity *collidableEntities, int collisionCheckCount) {
    for (int i = 0; i < collisionCheckCount; i++) {
        Entity *collidableEntity = &collidableEntities[i];

        // Skip self - prevents immediate self-collision.
        if (collidableEntity == this) continue;

        if (isColliding(collidableEntity)) {
            float yDistance = fabsf(mPosition.y - collidableEntity->mPosition.y);
            float yOverlap = fabsf(yDistance - (mColliderDimensions.y / 2.0f)
            - (collidableEntity->mColliderDimensions.y / 2.0f));

            if (mVelocity.y > 0.0f) {
                mPosition.y -= yOverlap;
                mVelocity.y = 0.0f;
                mIsCollidingBottom = true;
                if (collidableEntity->mEntityType == BLOCK)
                    collidableEntity->deactivate();
            } else if (mVelocity.y < 0.0f) {
                mPosition.y += yOverlap;
                mVelocity.y = 0.0f;
                mIsCollidingTop = true;
            }
        }
    }
}

void Entity::checkCollisionX(Entity *collidableEntities, int collisionCheckCount) {
    for (int i = 0; i < collisionCheckCount; i++) {
        Entity *collidableEntity = &collidableEntities[i];

        // Skip self - prevents immediate self-collision.
        if (collidableEntity == this) continue;

        if (isColliding(collidableEntity)) {
            float yDistance = fabsf(mPosition.y - collidableEntity->mPosition.y);
            float yOverlap = fabsf(yDistance - (mColliderDimensions.y / 2.0f)
            - (collidableEntity->mColliderDimensions.y / 2.0f));

            if (yOverlap < Y_COLLISION_THRESHOLD) continue;

            float xDistance = fabsf(mPosition.x - collidableEntity->mPosition.x);
            float xOverlap = fabsf(xDistance - (mColliderDimensions.x / 2.0f)
            - (collidableEntity->mColliderDimensions.x / 2.0f));

            if (mVelocity.x > 0.0f) {
                mPosition.x -= xOverlap;
                mVelocity.x = 0.0f;
                mIsCollidingRight = true;
            } else if (mVelocity.x < 0.0f) {
                mPosition.x += xOverlap;
                mVelocity.x = 0.0f;
                mIsCollidingLeft = true;
            }
        }
    }
}

void Entity::checkCollisionY(Map *map) {
    if (map == nullptr) return;

    Vector2 topCentreProbe = { mPosition.x, mPosition.y - (mColliderDimensions.y / 2.0f) };
    Vector2 topLeftProbe = { mPosition.x - (mColliderDimensions.x / 2.0f), mPosition.y - (mColliderDimensions.y / 2.0f) };
    Vector2 topRightProbe = { mPosition.x + (mColliderDimensions.x / 2.0f), mPosition.y - (mColliderDimensions.y / 2.0f) };

    Vector2 bottomCentreProbe= { mPosition.x, mPosition.y + (mColliderDimensions.y / 2.0f) };
    Vector2 bottomLeftProbe = { mPosition.x - (mColliderDimensions.x / 2.0f), mPosition.y + (mColliderDimensions.y / 2.0f) };
    Vector2 bottomRightProbe = { mPosition.x + (mColliderDimensions.x / 2.0f), mPosition.y + (mColliderDimensions.y / 2.0f) };

    float xOverlap = 0.0f;
    float yOverlap = 0.0f;

    if ((map->isSolidTileAt(topCentreProbe, &xOverlap, &yOverlap) ||
        map->isSolidTileAt(topLeftProbe, &xOverlap, &yOverlap) ||
        map->isSolidTileAt(topRightProbe, &xOverlap, &yOverlap)) &&
        mVelocity.y < 0.0f && xOverlap < map->getTileSize() * 0.4f) {
        mPosition.y += yOverlap * 1.01f;
        mVelocity.y = 0.0f;
        mIsCollidingTop = true;
    }

    if ((map->isSolidTileAt(bottomCentreProbe, &xOverlap, &yOverlap) ||
        map->isSolidTileAt(bottomLeftProbe, &xOverlap, &yOverlap) ||
        map->isSolidTileAt(bottomRightProbe, &xOverlap, &yOverlap)) &&
        mVelocity.y > 0.0f) {
        mPosition.y -= yOverlap * 1.01f;
        mVelocity.y = 0.0f;
        mIsCollidingBottom = true;
    }
}

void Entity::checkCollisionX(Map *map) {
    if (map == nullptr) return;

    Vector2 leftCentreProbe = { mPosition.x - (mColliderDimensions.x / 2.0f), mPosition.y };
    Vector2 leftTopProbe = { mPosition.x - (mColliderDimensions.x / 2.0f), mPosition.y - (mColliderDimensions.y / 2.0f) };
    Vector2 leftBottomProbe = { mPosition.x - (mColliderDimensions.x / 2.0f), mPosition.y + (mColliderDimensions.y / 2.0f) };

    Vector2 rightCentreProbe = { mPosition.x + (mColliderDimensions.x / 2.0f), mPosition.y };
    Vector2 rightTopProbe = { mPosition.x + (mColliderDimensions.x / 2.0f), mPosition.y - (mColliderDimensions.y / 2.0f) };
    Vector2 rightBottomProbe = { mPosition.x + (mColliderDimensions.x / 2.0f), mPosition.y + (mColliderDimensions.y / 2.0f) };

    float xOverlap = 0.0f;
    float yOverlap = 0.0f;

    if ((map->isSolidTileAt(rightCentreProbe, &xOverlap, &yOverlap) ||
        map->isSolidTileAt(rightTopProbe, &xOverlap, &yOverlap) ||
        map->isSolidTileAt(rightBottomProbe, &xOverlap, &yOverlap)) &&
        mVelocity.x > 0.0f && yOverlap >= 0.5f) {
        mPosition.x -= xOverlap * 1.01f;
        mVelocity.x = 0.0f;
        mIsCollidingRight = true;
    }

    if ((map->isSolidTileAt(leftCentreProbe, &xOverlap, &yOverlap) ||
        map->isSolidTileAt(leftTopProbe, &xOverlap, &yOverlap) ||
        map->isSolidTileAt(leftBottomProbe, &xOverlap, &yOverlap)) &&
        mVelocity.x < 0.0f && yOverlap >= 0.5f) {
        mPosition.x += xOverlap * 1.01f;
        mVelocity.x = 0.0f;
        mIsCollidingLeft = true;
    }
}

bool Entity::isColliding(Entity *other) const {
    float xDistance = fabsf(mPosition.x - other->mPosition.x);
    float yDistance = fabsf(mPosition.y - other->mPosition.y);

    float xCollisionThreshold = (mColliderDimensions.x / 2.0f) + (other->mColliderDimensions.x / 2.0f);
    float yCollisionThreshold = (mColliderDimensions.y / 2.0f) + (other->mColliderDimensions.y / 2.0f);

    return xDistance < xCollisionThreshold && yDistance < yCollisionThreshold;
}

void Entity::animate(float deltaTime) {
    if (mTextureType != ATLAS || mAnimationIndices.empty()) return;

    mAnimationTime += deltaTime;
    float frameTime = 1.0f / (float)mFrameSpeed;

    if (mAnimationTime >= frameTime) {
        mCurrentFrameIndex++;

        if (mCurrentFrameIndex >= (int)mAnimationIndices.size()) {
            mCurrentFrameIndex = 0;
        }

        mAnimationTime = 0.0f;
    }
}

void Entity::AIWander() {
    if (mAIState == IDLE) {
        int randomAction = GetRandomValue(0, 100);

        if (randomAction < 5) {
            mAIState = WALKING;

            int randomDirection = GetRandomValue(0, 1);
            if (randomDirection == 0) {
                mMovement.x = -1.0f;
                mDirection = LEFT;
            } else {
                mMovement.x = 1.0f;
                mDirection = RIGHT;
            }
        }
    } else if (mAIState == WALKING) {
        int randomStop = GetRandomValue(0, 100);

        if (randomStop < 3) {
            mAIState = IDLE;
            mMovement.x = 0.0f;
        }
    }
}

void Entity::AIFollow(Entity *target) {
    if (target == nullptr) return;

    float distance = fabsf(mPosition.x - target->mPosition.x);

    if (distance > 10.0f) {
        if (mPosition.x < target->mPosition.x) {
            mMovement.x = 1.0f;
            mDirection = RIGHT;
        } else {
            mMovement.x = -1.0f;
            mDirection = LEFT;
        }

        mAIState = FOLLOWING;
    } else {
        mMovement.x = 0.0f;
        mAIState = IDLE;
    }
}

void Entity::AIFly(Entity *target) {
    if (target == nullptr) return;

    float horizontalDistance = target->mPosition.x - mPosition.x;
    float verticalDistance = target->mPosition.y - mPosition.y;

    if (fabsf(horizontalDistance) > 30.0f) {
        if (horizontalDistance > 0.0f) {
            mMovement.x = 1.0f;
            mDirection = RIGHT;
        } else {
            mMovement.x = -1.0f;
            mDirection = LEFT;
        }
    } else {
        mMovement.x = 0.0f;
    }

    if (fabsf(verticalDistance) > 30.0f) {
        if (verticalDistance > 0.0f) {
            mMovement.y = 1.0f;
        } else {
            mMovement.y = -1.0f;
        }
    } else {
        mMovement.y = 0.0f;
    }
}

void Entity::AIActivate(Entity *target) {
    if (mEntityType != NPC) return;

    switch (mAIType) {
        case WANDERER: AIWander(); break;
        case FOLLOWER: AIFollow(target); break;
        case FLYER: AIFly(target); break;
        default: break;
    }
}

void Entity::update(float deltaTime, Entity *player, Map *map, Entity *collidableEntities, int collisionCheckCount) {
    if (!isActive()) return;

    // Reset collider flags at the start of update so they are visible to ProcessInput() next frame
    resetColliderFlags();

    // Apply gravity
    mVelocity.y += mAcceleration.y * deltaTime;

    if (mEntityType == NPC && player != nullptr) {
        AIActivate(player);
    }

    // NOW calculate velocity based on movement (which was set by ProcessInput for player, or AIActivate for NPC)
    if (mEntityType == PLAYER) {
        if (mMovement.x != 0.0f) mVelocity.x = mMovement.x * (float)mSpeed;
        else mVelocity.x = 0.0f;
    } else if (mEntityType == NPC) {
        // For regular enemies (WANDERER, FOLLOWER)
        if (mMovement.x != 0.0f) mVelocity.x = mMovement.x * (float)mSpeed;
        else mVelocity.x = 0.0f;

        // For FLYER, also handle Y velocity
        if (mAIType == FLYER) {
            if (mMovement.y != 0.0f) mVelocity.y = mMovement.y * (float)mSpeed;
            else mVelocity.y = 0.0f;
        }
    }

    // Update position with velocity
    mPosition.x += mVelocity.x * deltaTime;
    mPosition.y += mVelocity.y * deltaTime;

    // Check collisions with map
    checkCollisionX(map);
    if (collidableEntities != nullptr && collisionCheckCount > 0) checkCollisionX(collidableEntities, collisionCheckCount);

    checkCollisionY(map);
    if (collidableEntities != nullptr && collisionCheckCount > 0) checkCollisionY(collidableEntities, collisionCheckCount);

    // Handle jumping
    if (mIsJumping && mIsCollidingBottom && mEntityType == PLAYER) {
        mVelocity.y = -mJumpingPower;
        mIsJumping = false;
    }

    // Boundary wrapping for NPC entities (keeps enemies in bounds)
    if (mEntityType == NPC && map != nullptr) {
        if (mPosition.x < map->getLeftBoundary()) {
            mPosition.x = map->getLeftBoundary() + 50.0f;
            mMovement.x = 1.0f; // Force rightward
        }

        if (mPosition.x > map->getRightBoundary()) {
            mPosition.x = map->getRightBoundary() - 50.0f;
            mMovement.x = -1.0f; // Force leftward
        }
    }

    // Deactivate if fallen off map
    if (map != nullptr && mPosition.y > map->getBottomBoundary() + 100.0f) {
        deactivate();
    }

    animate(deltaTime);

    if (mEntityType == NPC) {
        resetMovement();
    } else if (mEntityType == PLAYER) {
        resetMovement();
    }
}

void Entity::render() {
    if (!isActive()) return;

    if (mTextureType == SINGLE) {
        DrawTexturePro(
            mTexture,
            { 0.0f, 0.0f, (float)mTexture.width, (float)mTexture.height },
            { mPosition.x, mPosition.y, mScale.x, mScale.y },
            { mScale.x / 2.0f, mScale.y / 2.0f },
            mAngle,
            WHITE
        );
    } else if (mTextureType == ATLAS && !mAnimationIndices.empty()) {
        Rectangle source = getUVRectangle(&mTexture, mAnimationIndices[mCurrentFrameIndex],
                                         (int)mSpriteSheetDimensions.y, (int)mSpriteSheetDimensions.x);

        if (mDirection == LEFT) source.width = -source.width;

        DrawTexturePro(
            mTexture,
            source,
            { mPosition.x, mPosition.y, mScale.x, mScale.y },
            { mScale.x / 2.0f, mScale.y / 2.0f },
            mAngle,
            WHITE
        );
    }
}

void Entity::displayCollider() {
    DrawRectangle(
        (int)(mPosition.x - mColliderDimensions.x / 2.0f),
        (int)(mPosition.y - mColliderDimensions.y / 2.0f),
        (int)mColliderDimensions.x,
        (int)mColliderDimensions.y,
        GREEN
    );
}