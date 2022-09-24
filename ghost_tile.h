#if !defined(GHOST_TILE_H)


//0005
inline int32
GetRoom(level *Lvl, v2 Pos)
{
    int32 Result = -1;
    int32 PosX = TruncateReal32ToInt32(Pos.X);
    int32 PosY = TruncateReal32ToInt32(Pos.Y);
    for(uint32 RoomsIndex = 0; RoomsIndex < Lvl->RoomCount; ++RoomsIndex)
    {
        
        for(uint32 RectI = 0; RectI < Lvl->Rooms[RoomsIndex].RectangleCount; ++RectI)
        {
            
            if((PosX >= (int32)Lvl->Rooms[RoomsIndex].RectangleVertices[RectI][0].X) &&
               (PosY >= (int32)Lvl->Rooms[RoomsIndex].RectangleVertices[RectI][0].Y) &&
               (PosX <= (int32)Lvl->Rooms[RoomsIndex].RectangleVertices[RectI][1].X) &&
               (PosY <= (int32)Lvl->Rooms[RoomsIndex].RectangleVertices[RectI][1].Y))
            {
                Result = RoomsIndex;
                break;
            }
        }
                    
        if(Result != -1)
        {
            break;
        }
    }
    return(Result);
}

inline int32
GetBoard(level *Lvl, v2 Pos)
{
    int32 Result = 0;
    if(Pos.X < 0.0f || Pos.Y < 0.0f)
    {
        Result = -1;
    }        
    return(Result);
}


//006
inline int32
GetPointID(level *Lvl, v2 Pos)
{
    int32 Result;
    
    int32 X = TruncateReal32ToInt32(Pos.X) % TruncateReal32ToInt32(Lvl->BoardDimensions.X);
    int32 Y = TruncateReal32ToInt32(Pos.Y) % TruncateReal32ToInt32(Lvl->BoardDimensions.Y);
    
    //int32 BoardIndex = GetBoard(Lvl, Pos);
    int32 BoardIndex = 0;
    
    int32 TileValue = Lvl->Board[BoardIndex].TileArray[((Y) * 45) + X];
    
    Result = TileValue;
              
    // TileArray[] to TileArray[-16]?
    // Should be 0 to 143.
    // TileArray[y][x] == TileArray[y*xcount + x]
    // If uint32 TA[8][15], then TA[4][2] is the highest value in that array
    // AKA TA[4*3 + 2] = 14
    // AKA TA[8*16 + 15] =  
    // and you would access these tiles by saying 
    return(Result);
    }

inline bool32
IsObstacle(uint32 TileID)
{
    bool32 Result = false;
    
    if(TileID==OUTER_WALL_TILE || TileID==OUTER_DOOR_TILE || TileID==WALL_TILE || TileID==TABLE_TILE || /*TileID==CHAIR_TILE ||*/ TileID==HOLE_TILE)
    {
        Result = true;
    }

    return(Result);
}


struct tile_map_difference
{
    v2 dXY;
    real32 dZ;
};

struct tile
{
    v2 Pos;
    uint32 Value;
};

struct tile_map_position
{
    // NOTE(casey): These are fixed point tile locations.  The high
    // bits are the tile chunk index, and the low bits are the tile
    // index in the chunk.
    uint32 AbsTileX;
    uint32 AbsTileY;
    uint32 AbsTileZ;

    // NOTE(casey): These are the offsets from the tile center
    v2 Offset;
};

struct tile_chunk_position
{
    uint32 TileChunkX;
    uint32 TileChunkY;
    uint32 TileChunkZ;

    uint32 RelTileX;
    uint32 RelTileY;
};

struct tile_chunk
{
    // TODO(casey): Real structure for a tile!
    uint32 *Tiles;
};

struct tile_map
{
    uint32 ChunkShift;
    uint32 ChunkMask;
    uint32 ChunkDim;
    
    real32 TileSideInMeters;

    // TODO(casey): REAL sparseness so anywhere in the world can be
    // represented without the giant pointer array.
    uint32 TileChunkCountX;
    uint32 TileChunkCountY;
    uint32 TileChunkCountZ;
    tile_chunk *TileChunks;
};


#define HANDMADE_TILE_H
#endif
