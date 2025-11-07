#include "Map.h"
#include <cmath>
#include <iostream>

Map::Map(int mapColumns,
         int mapRows,
         unsigned int* levelData,
         const char* textureFilePath,
         float tileSize,
         int textureColumns,
         int textureRows,
         Vector2 origin)
: mMapColumns(mapColumns)
, mMapRows(mapRows)
, mLevelData(levelData)
, mTileSize(tileSize)
, mTextureColumns(textureColumns)
, mTextureRows(textureRows)
, mOrigin(origin)
, mLeftBoundary(0.0f)
, mRightBoundary(0.0f)
, mTopBoundary(0.0f)
, mBottomBoundary(0.0f)
{
    mTextureAtlas = LoadTexture(textureFilePath);
    if (mMapColumns <= 0 || mMapRows <= 0) {
        std::cerr << "Map::Map - invalid dimensions\n";
    }
    if (mLevelData == nullptr) {
        std::cerr << "Map::Map - warning: level data pointer is null\n";
    }
    build();
}

Map::~Map() {
    UnloadTexture(mTextureAtlas);
}

void Map::build()
{
    mLeftBoundary   = mOrigin.x - (mMapColumns * mTileSize) / 2.0f;
    mRightBoundary  = mOrigin.x + (mMapColumns * mTileSize) / 2.0f;
    mTopBoundary    = mOrigin.y - (mMapRows    * mTileSize) / 2.0f;
    mBottomBoundary = mOrigin.y + (mMapRows    * mTileSize) / 2.0f;

    mTextureAreas.clear();
    mTextureAreas.reserve(mTextureColumns * mTextureRows);

    const float cellW = static_cast<float>(mTextureAtlas.width)  / static_cast<float>(mTextureColumns);
    const float cellH = static_cast<float>(mTextureAtlas.height) / static_cast<float>(mTextureRows);

    for (int r = 0; r < mTextureRows;  r++) {
        for (int c = 0; c < mTextureColumns; c++) {
            Rectangle src{ c * cellW, r * cellH, cellW, cellH };
            mTextureAreas.push_back(src);
        }
    }
}

void Map::render()
{
    const int totalCells = mTextureColumns * mTextureRows;

    for (int row = 0; row < mMapRows; row++) {
        for (int col = 0; col < mMapColumns; col++) {

            const int idx = row * mMapColumns + col;
            unsigned int tile = mLevelData ? mLevelData[idx] : 0;

            if (tile == 0) { continue; }

            int local = static_cast<int>(tile) - 1;

            if (local < 0 || local >= totalCells) {
                std::cerr << "ERROR: Map::render - tile index " << tile
                          << " (local " << local << ") out of tileset bounds (cells="
                          << totalCells << ") at map pos (" << col << "," << row << ")\n";
                continue;
            }

            const float worldX = mLeftBoundary + (col + 0.5f) * mTileSize;
            const float worldY = mTopBoundary  + (row + 0.5f) * mTileSize;

            Rectangle dst{ worldX, worldY, mTileSize, mTileSize };

            DrawTexturePro(
                mTextureAtlas,
                mTextureAreas[local],
                dst,
                { mTileSize / 2.0f, mTileSize / 2.0f },
                0.0f,
                WHITE
            );
        }
    }
}

bool Map::isSolidTileAt(const Vector2& worldPoint, float* outPenX, float* outPenY) const
{
    const int col = static_cast<int>(std::floor((worldPoint.x - mLeftBoundary) / mTileSize));
    const int row = static_cast<int>(std::floor((worldPoint.y - mTopBoundary)  / mTileSize));

    if (col < 0 || col >= mMapColumns || row < 0 || row >= mMapRows) {
        if (outPenX) *outPenX = 0.0f;
        if (outPenY) *outPenY = 0.0f;
        return false;
    }

    const int idx = row * mMapColumns + col;
    const unsigned int tile = mLevelData ? mLevelData[idx] : 0;

    if (tile == 0) {
        if (outPenX) *outPenX = 0.0f;
        if (outPenY) *outPenY = 0.0f;
        return false;
    }

    const float tileCenterX = mLeftBoundary + (col + 0.5f) * mTileSize;
    const float tileCenterY = mTopBoundary  + (row + 0.5f) * mTileSize;

    const float dx = worldPoint.x - tileCenterX;
    const float dy = worldPoint.y - tileCenterY;

    if (outPenX) *outPenX = (mTileSize / 2.0f) - std::fabs(dx);
    if (outPenY) *outPenY = (mTileSize / 2.0f) - std::fabs(dy);

    return true;
}
