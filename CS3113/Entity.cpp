#include "Entity.h"

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
  mTextureType{ animationAtlas.empty() ? SINGLE : ATLAS }, 
  mSpriteSheetDimensions{ spriteSheetDimensions },
  mAnimationAtlas{ animationAtlas },
  mAnimationIndices{ animationAtlas.empty() ? std::vector<int>{} : animationAtlas.at(RIGHT) }, 
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
{}


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
    if (mDirection == LEFT){
        moveLeft();
    }
    else{
        moveRight();
    }
}

void Entity::AIFollow(Entity *target)
{
    switch (mAIState)
    {
    case IDLE:
        if (Vector2Distance(mPosition, target->getPosition()) < 250.0f) 
            mAIState = WALKING;
        break;

    case WALKING:
        // Depending on where the player is in respect to their x-position
        // Change direction of the enemy
        if (mPosition.x > target->getPosition().x) moveLeft();
        else                                       moveRight();
    
    default:
        break;
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
    switch (mAIType) {
        case WANDERER: AIWander(); break;
        case FOLLOWER: AIFollow(target); break;
        case FLYER: AIFly(target); break;
        default: break;
    }
}

void Entity::update(float deltaTime, Entity *player, Map *map, 
    Entity *collidableEntities, int collisionCheckCount)
{
    if (mEntityStatus == INACTIVE) return;
    
    if (mEntityType == NPC) AIActivate(player);

    resetColliderFlags();

    if (mEntityType == NPC && mAIType == FLYER) mVelocity.y = mMovement.y * mSpeed;
    if (mEntityType == PLAYER && mMovement.x == 0.0f) mVelocity.x = 0.0f;
    mVelocity.x = mMovement.x * mSpeed;

    mVelocity.x += mAcceleration.x * deltaTime;
    mVelocity.y += mAcceleration.y * deltaTime;

    // ––––– JUMPING ––––– //
    if (mIsJumping)
    {
        // STEP 1: Immediately return the flag to its original false state
        mIsJumping = false;
        
        // STEP 2: The player now acquires an upward velocity
        mVelocity.y -= mJumpingPower;
    }

    mPosition.y += mVelocity.y * deltaTime;
    checkCollisionY(collidableEntities, collisionCheckCount);
    checkCollisionY(map);
    

    mPosition.x += mVelocity.x * deltaTime;
    checkCollisionX(collidableEntities, collisionCheckCount);
    checkCollisionX(map);

    if (mEntityType == NPC && map != nullptr)
    {
        if (mPosition.x < map->getLeftBoundary())
        {
            mPosition.x = map->getLeftBoundary();
            mMovement.x = 1.0f;   // push right
        }

        if (mPosition.x > map->getRightBoundary())
        {
            mPosition.x = map->getRightBoundary();
            mMovement.x = -1.0f;  // push left
        }
    }

    if (mTextureType == ATLAS && GetLength(mMovement) != 0 && mIsCollidingBottom) 
        animate(deltaTime);

    if (mEntityType == PLAYER) mMovement.x = 0.0f;
}


void Entity::render()
{
    if(mEntityStatus == INACTIVE) return;

    Rectangle textureArea;

    switch (mTextureType)
    {
        case SINGLE:
            // Whole texture (UV coordinates)
            textureArea = {
                // top-left corner
                0.0f, 0.0f,

                // bottom-right corner (of texture)
                static_cast<float>(mTexture.width),
                static_cast<float>(mTexture.height)
            };
            break;
        case ATLAS:
            textureArea = getUVRectangle(
                &mTexture, 
                mAnimationIndices[mCurrentFrameIndex], 
                mSpriteSheetDimensions.x, 
                mSpriteSheetDimensions.y
            );
        
        default: break;
    }

    // Destination rectangle – centred on gPosition
    Rectangle destinationArea = {
        mPosition.x,
        mPosition.y,
        static_cast<float>(mScale.x),
        static_cast<float>(mScale.y)
    };

    // Origin inside the source texture (centre of the texture)
    Vector2 originOffset = {
        static_cast<float>(mScale.x) / 2.0f,
        static_cast<float>(mScale.y) / 2.0f
    };

    // Render the texture on screen
    DrawTexturePro(
        mTexture, 
        textureArea, destinationArea, originOffset,
        mAngle, WHITE
    );

    // displayCollider();
}

void Entity::displayCollider() 
{
    // draw the collision box
    Rectangle colliderBox = {
        mPosition.x - mColliderDimensions.x / 2.0f,  
        mPosition.y - mColliderDimensions.y / 2.0f,  
        mColliderDimensions.x,                        
        mColliderDimensions.y                        
    };

    DrawRectangleLines(
        colliderBox.x,      // Top-left X
        colliderBox.y,      // Top-left Y
        colliderBox.width,  // Width
        colliderBox.height, // Height
        GREEN               // Color
    );
}