#if !defined(GHOST_H)

#include "ghost_platform.h"
#include "enums.h"

//
//
//

struct memory_arena
{
    memory_index Size;
    uint8 *Base;
    memory_index Used;
};

internal void
InitializeArena(memory_arena *Arena, memory_index Size, uint8 *Base)
{
    Arena->Size = Size;
    Arena->Base = Base;
    Arena->Used = 0;
}

#define PushStruct(Arena, type) (type *)PushSize_(Arena, sizeof(type))
#define PushArray(Arena, Count, type) (type *)PushSize_(Arena, (Count)*sizeof(type))
void *
PushSize_(memory_arena *Arena, memory_index Size)
{
    Assert((Arena->Used + Size) <= Arena->Size);
    void *Result = Arena->Base + Arena->Used;
    Arena->Used += Size;
    
    return(Result);
}

#include "ghost_math.h"
#include "ghost_intrinsics.h"

struct loaded_bitmap
{
    int32 Width;
    int32 Height;
    uint32 *Pixels;
};

struct entity
{
    loaded_bitmap BMP;
    v2 Pos;
    v2 ScreenPos;
};

/*struct character_bitmap
{    
    loaded_bitmap Body;
    
};*/

struct clock
{
    uint32 Hr;
    uint32 Min;
    uint32 Sec;
};

struct rgb
{
    real32 R;
    real32 G;
    real32 B;
};

struct hub
{
    v2 Pos;    
    uint64 ID;
    uint8 Char;
};

/* IMPORTANT NONDEMO
struct board
{
    uint32 *TileArray;
    uint32 DoorMask;
    
    uint32 ID;
    uint32 HubCount;
    
    v2 Pos;
    v2 MinLevel;
    v2 MaxLevel;

    hub *Hub;
};*/

/* IMPORTANT NONDEMO
struct level
{
    board Board[4];
    
    uint32 BoardCount;
    uint32 BoardMask;
    v2 BoardTileCount;
    v2 TileCount;
    
};
*/

 //IMPORTANT DEMO
struct room
{
    uint64 ID;    
    uint32 RectangleCount;
    v2 RectangleVertices[4][2];

    uint64 RoomsAway;
    uint64 DoorMask;
    uint32 AdjacentRoomCount;    
    
    uint32 HubCount;
    hub *Hub;  
};

//IMPORTANT DEMO
struct board
{
    uint32 *TileArray;    
//    uint32 ID;    
//    v2 Pos;
//    v2 MinLevel;
//    v2 MaxLevel;
};

 //IMPORTANT DEMO
struct level
{
    board *Board;
    uint32 BoardCount;
    v2 BoardDimensions;
    room *Rooms;
    uint32 RoomCount;    
    //v2 TileCount;    
};

struct dialogue
{
    loaded_bitmap BMP;
    uint32 TimeDuration;
};

struct object
{
    v2 ProperPos;
    v2 Pos;
    real32 Radius;
    rgb Color;
    loaded_bitmap BMP[2];    
    
    bool32 Burnable;
    bool32 Uncarryable;
    bool32 Possessed;    
    //bool32 Displaced;
    bool32 Broken;
    bool32 Equipped;
    uint32 AnimationTimer;
    loaded_bitmap ActionBMP[2];
    loaded_bitmap BrokenBMP;
};

struct event
{
    //clock Window;
    clock Timer;
    uint32 ID;
    uint32 Priority;
    uint32 Obj;
    uint32 Phase;       
    bool32 Complete;
    bool32 CouldComplete;
    //uint8 *SetPath;
};

struct npc
{
    loaded_bitmap CurrentBitmap;
    uint32 CurrentBitmapI;
    uint32 AnimationTimer;
    
    v2 Speed;
    v2 Pos;
    //v2 NextBoardDirection;
    v2 Dest;

    //ID of the current event
    event CurrentEvent;
    uint32 CurrentEventBufferI;
    event EventBuffer[5];
    uint32 EventsBuffered;

    v2 ObjPos[MAX_OBJECTS];
    //bool32 WrongObjPos[MAX_OBJECTS];
    bool32 Inventory[MAX_OBJECTS];
    bool32 ObjNeeded[MAX_OBJECTS];
    
    hub NextHub;
    hub PriorHub;
    hub EmptyPathHub;
    hub FalseHub[2];
           
    uint8 CurrentPath[20];
    uint32 CurrentPathI;
    uint32 Direction;
    uint32 DestName;
    
    bool32 IsMoving;
    bool32 IsInspired;
    
    bool32 DetectsPlayer;
    bool32 DetectsObj;
    bool32 DetectsDisplacedObj;
    
    bool32 GotNewDest;
    bool32 AtDest;
    bool32 GotNewEvent;

    uint32 IsConfused;
    uint32 IsFrightened;
};

struct game_state
{
    
    //IMPORTANT DEMO - APARTMENT SPECIFIC
    level L0;      

    room Room[MAX_ROOMS];
    //board Boards[6];
    board Board;
    hub BathRoomHubs[6];
    hub BedRoomHubs[9];
    hub ClosetBedRoomHubs[2];
    hub KitchenRoomHubs[4];    
    hub MainRoomHubs[8];
    hub ClosetMainRoomHubs[2];
    hub TvRoomHubs[5];
    hub OutdoorsRoomHubs[2];
    
    uint32 AptTileMap[50][45];
    object Obj[MAX_OBJECTS];
    npc Bob;
    uint32 BobFacingDirection;
    
    // END DEMO SPECIFIC
    
    //memory_arena WorldArena;
    
    clock Clock;    
    uint32 ObjBlinkTimer;
    uint32 AnimationTimer;
    
    uint32 CurrentPlayerBitmapI;
    
    loaded_bitmap ApartmentBitmap;    
    loaded_bitmap CurrentPlayerBitmap;


    loaded_bitmap BobSouthBitmap[8];
    loaded_bitmap BobNorthBitmap[8];
    loaded_bitmap BobEastBitmap[8];
    loaded_bitmap BobWestBitmap[8];
    
    loaded_bitmap PlayerSouthBitmap[8];
    loaded_bitmap PlayerNorthBitmap[8];
    loaded_bitmap PlayerEastBitmap[8];
    loaded_bitmap PlayerWestBitmap[8];
    loaded_bitmap PlayerSpiritBitmap;
    
    loaded_bitmap QuestionMark;
    loaded_bitmap ExclamationMark;
    loaded_bitmap BobIcon;
    loaded_bitmap SpeechIcon;

    loaded_bitmap Channel[9];
    uint32 ChannelTimer;
    uint32 ChannelI;
    
    bool32 ShowTvGuide;
    loaded_bitmap TvGuideBMP;
    loaded_bitmap TvScreen[9];
    
    
    loaded_bitmap ClockDigits[12];    
    loaded_bitmap CurrentClockDigits[6];
    entity AptLayer[MAX_LAYERS];

    dialogue CurrentWords;

    dialogue Words[MAX_WORDS];
    

    //loaded_bitmap FloorTileBitmap;    
    bool32 InterRoomChase;
    bool32 PathEmpty;
    bool32 SpiritMode;
    bool32 DeathScreen;
    uint32 ObjMode;

    uint32 PlayerInventory[MAX_OBJECTS];
    uint32 PlayerInventoryI;
    uint32 PlayerInventoryAmount;
    
    
    uint32 NearbyObj[MAX_OBJECTS];
    uint32 NearbyObjCount;
    uint32 NearbyObjI;
    uint32 SelectedObjI;
    
    game_controller_input OldController;
    
    real32 TilePixels;
    real32 TileMeters;
    real32 MetersToPixels;

    v2 PlayerPos;
//    uint32 ObjInventory[6];
    //   uint32 ObjAmount;
//    uint32 EquippedObj;
    
    
    uint32 FacingDirection;
};

#include "ghost_tile.h"
#include "ghost_path.h"


#define GHOST_H
#endif
