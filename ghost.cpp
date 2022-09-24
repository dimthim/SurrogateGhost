// IncFix 2/06/19

#include "ghost.h"


//000-
internal void
GameOutputSound(game_state *GameState, game_sound_output_buffer *SoundBuffer, int ToneHz)
{

    int16 ToneVolume = 3000;
    int WavePeriod = SoundBuffer->SamplesPerSecond/ToneHz;

    int16 *SampleOut = SoundBuffer->Samples;
    for(int SampleIndex = 0;
        SampleIndex < SoundBuffer->SampleCount;
        ++SampleIndex)
    {
        // TODO(casey): Draw this out for people
#if 0
        real32 SineValue = sinf(GameState->tSine);
        int16 SampleValue = (int16)(SineValue * ToneVolume);
#else
        int16 SampleValue = 0;
#endif
        *SampleOut++ = SampleValue;
        *SampleOut++ = SampleValue;

#if 0
        GameState->tSine += 2.0f*Pi32*1.0f/(real32)WavePeriod;
        if(GameState->tSine > 2.0f*Pi32)
        {
            GameState->tSine -= 2.0f*Pi32;
        }
#endif
    }
}

//001
internal void
DrawWorldRectangle(game_offscreen_buffer *Buffer, v2 vMin, v2 vMax, rgb RGB)
{    
    int32 MinX = RoundReal32ToInt32(vMin.X);
    int32 MinY = RoundReal32ToInt32(vMin.Y);
    int32 MaxX = RoundReal32ToInt32(vMax.X);
    int32 MaxY = RoundReal32ToInt32(vMax.Y);
/*
    int32 MinX = (int32)(vMin.X);
    int32 MinY = (int32)(vMin.Y);
    int32 MaxX = (int32)(vMax.X);
    int32 MaxY = (int32)(vMax.Y);
*/

    if(MinX < 0)
    {
        MinX = 0;
    }

    if(MinY < 0)
    {
        MinY = 0;
    }

    if(MaxX > Buffer->Width - 500)
    {
        MaxX = Buffer->Width - 500;
    }

    if(MaxY > Buffer->Height)
    {
        MaxY = Buffer->Height;
    }

    uint32 Color = ((RoundReal32ToUInt32(RGB.R * 255.0f) << 16) |
                    (RoundReal32ToUInt32(RGB.G * 255.0f) << 8) |
                    (RoundReal32ToUInt32(RGB.B * 255.0f) << 0));

    uint8 *Row = ((uint8 *)Buffer->Memory +
                  MinX*Buffer->BytesPerPixel +
                  MinY*Buffer->Pitch);
    for(int Y = MinY;
        Y < MaxY;
        ++Y)
    {
        uint32 *Pixel = (uint32 *)Row;
        for(int X = MinX;
            X < MaxX;
            ++X)
        {            
            *Pixel++ = Color;
        }
        
        Row += Buffer->Pitch;
    }
}


//001
internal void
DrawRectangle(game_offscreen_buffer *Buffer, v2 vMin, v2 vMax, rgb RGB)
{    
    int32 MinX = RoundReal32ToInt32(vMin.X);
    int32 MinY = RoundReal32ToInt32(vMin.Y);
    int32 MaxX = RoundReal32ToInt32(vMax.X);
    int32 MaxY = RoundReal32ToInt32(vMax.Y);
/*
    int32 MinX = (int32)(vMin.X);
    int32 MinY = (int32)(vMin.Y);
    int32 MaxX = (int32)(vMax.X);
    int32 MaxY = (int32)(vMax.Y);
*/

    if(MinX < 0)
    {
        MinX = 0;
    }

    if(MinY < 0)
    {
        MinY = 0;
    }

    if(MaxX > Buffer->Width)
    {
        MaxX = Buffer->Width;
    }

    if(MaxY > Buffer->Height)
    {
        MaxY = Buffer->Height;
    }

    uint32 Color = ((RoundReal32ToUInt32(RGB.R * 255.0f) << 16) |
                    (RoundReal32ToUInt32(RGB.G * 255.0f) << 8) |
                    (RoundReal32ToUInt32(RGB.B * 255.0f) << 0));

    uint8 *Row = ((uint8 *)Buffer->Memory +
                  MinX*Buffer->BytesPerPixel +
                  MinY*Buffer->Pitch);
    for(int Y = MinY;
        Y < MaxY;
        ++Y)
    {
        uint32 *Pixel = (uint32 *)Row;
        for(int X = MinX;
            X < MaxX;
            ++X)
        {            
            *Pixel++ = Color;
        }
        
        Row += Buffer->Pitch;
    }
}

//002
internal void
DrawWorldBitmap(game_offscreen_buffer *Buffer, loaded_bitmap *Bitmap,
           real32 MinPosX, real32 MinPosY)       
{

    uint32 BytesPerRow = Bitmap->Width*3;
    //uint32 PaddingPerRow = 0;
        
    while(BytesPerRow % 4 != 0)
    {
        //  ++PaddingPerRow;
        ++BytesPerRow;
    }
    
    int32 MinX = RoundReal32ToInt32(MinPosX);
    int32 MinY = RoundReal32ToInt32(MinPosY);    
    int32 MaxX = MinX + Bitmap->Width;
    int32 MaxY = MinY + Bitmap->Height;

    int32 SourceOffsetX = 0;
    if(MinX < 0)
    {
        SourceOffsetX = -MinX;
        MinX = 0;
    }

    int32 SourceOffsetY = 0;
    if(MinY < 0)
    {
        SourceOffsetY = -MinY;
        MinY = 0;
    }

    if(MaxX > Buffer->Width - 500)
    {
        MaxX = Buffer->Width - 500;
    }

    if(MaxY > Buffer->Height)
    {
        MaxY = Buffer->Height;
    }

    
    uint8 *SourceRow = (uint8 *)Bitmap->Pixels + (BytesPerRow)*(Bitmap->Height - 1 - SourceOffsetY);
    uint8 *DestRow =
        (uint8 *)Buffer->Memory +
        MinX*Buffer->BytesPerPixel +
        MinY*Buffer->Pitch;

    uint32 *Dest = (uint32 *)DestRow;
    uint8 *Source = SourceRow + (3*SourceOffsetX);
    
    for(int32 Y = MinY; Y < MaxY; ++Y)
    {
        
        Dest = (uint32 *)DestRow;
        Source = SourceRow + (3*SourceOffsetX); 

        for(int32 X = MinX; X < MaxX; ++X)
        {

            //TODO:(Fixed:6/30/3019)  6/10/19: Has that throws exception for walking off of the top of bmp's pixel address
            if((*(uint32 *)Source & 0x00ffffff) != 0x123456)
            {                
                *Dest = *(uint32 *)Source & 0x00ffffff;                
            }
            
            ++Dest;
            Source += 3;
        }

        DestRow += Buffer->Pitch;
        SourceRow -= (BytesPerRow);
        
    }
}

//002
internal void
DrawBitmap(game_offscreen_buffer *Buffer, loaded_bitmap *Bitmap,
           real32 MinPosX, real32 MinPosY)       
{

    uint32 BytesPerRow = Bitmap->Width*3;
    //uint32 PaddingPerRow = 0;
        
    while(BytesPerRow % 4 != 0)
    {
        //  ++PaddingPerRow;
        ++BytesPerRow;
    }
    
    int32 MinX = RoundReal32ToInt32(MinPosX);
    int32 MinY = RoundReal32ToInt32(MinPosY);    
    int32 MaxX = MinX + Bitmap->Width;
    int32 MaxY = MinY + Bitmap->Height;

    int32 SourceOffsetX = 0;
    if(MinX < 0)
    {
        SourceOffsetX = -MinX;
        MinX = 0;
    }

    int32 SourceOffsetY = 0;
    if(MinY < 0)
    {
        SourceOffsetY = -MinY;
        MinY = 0;
    }

    if(MaxX > Buffer->Width)
    {
        MaxX = Buffer->Width;
    }

    if(MaxY > Buffer->Height)
    {
        MaxY = Buffer->Height;
    }

    
    uint8 *SourceRow = (uint8 *)Bitmap->Pixels + (BytesPerRow)*(Bitmap->Height - 1 - SourceOffsetY);
    uint8 *DestRow =
        (uint8 *)Buffer->Memory +
        MinX*Buffer->BytesPerPixel +
        MinY*Buffer->Pitch;

    uint32 *Dest = (uint32 *)DestRow;
    uint8 *Source = SourceRow + (3*SourceOffsetX);
    
    for(int32 Y = MinY; Y < MaxY; ++Y)
    {
        
        Dest = (uint32 *)DestRow;
        Source = SourceRow + (3*SourceOffsetX); 

        for(int32 X = MinX; X < MaxX; ++X)
        {

            //TODO:(Fixed:6/30/3019)  6/10/19: Has that throws exception for walking off of the top of bmp's pixel address
            if((*(uint32 *)Source & 0x00ffffff) != 0x123456)
            {                
                *Dest = *(uint32 *)Source & 0x00ffffff;                
            }
            
            ++Dest;
            Source += 3;
        }

        DestRow += Buffer->Pitch;
        SourceRow -= (BytesPerRow);
        
    }
}

//003
#pragma pack(push, 1)
struct bitmap_header
{
    uint16 FileType;
    uint32 FileSize;
    uint16 Reserved1;
    uint16 Reserved2;
    uint32 BitmapOffset;
    uint32 Size;
    int32 Width;
    int32 Height;
    uint16 Planes;
    uint16 BitsPerPixel;
    uint32 Compression;
    uint32 SizeOfBitmap;
    int32 HorzResolution;
    int32 VertResolution;
    uint32 ColorsUsed;
    uint32 ColorsImportant;

};
#pragma pack(pop)

//IMPORTANT: We need a way to render bitmaps in the proper place and size onscreen.

//Need to read the file.
//Need to be able to find .bmp file's width, height, and the address of its pixels
   //From it's header information.
//We also need to be able to figure out how the RGB and possibly the A bytes are encoded
   //in each pixel (i.e. find out what order they are in and whether there even is an A
   //to worry about.
//004
internal loaded_bitmap
DEBUGLoadBMP(thread_context *Thread, debug_platform_read_entire_file *ReadEntireFile, char *FileName)
{
    loaded_bitmap Result = {};
    
    debug_read_file_result ReadResult = ReadEntireFile(Thread, FileName);    
    if(ReadResult.ContentsSize != 0)
    { 
        bitmap_header *Header = (bitmap_header *)ReadResult.Contents;
        uint32 *Pixels = (uint32 *)((uint8 *)ReadResult.Contents + Header->BitmapOffset);
        Result.Pixels = Pixels;
        Result.Width = Header->Width;
        Result.Height = Header->Height;

        
        //NOTE:Joel Commented this out: Assert(Header->Compression == 3);
        //because Aseprite has 24-bit pixels and uses 0 for compression
        Assert(Header->Compression == 0);
        
        // NOTE(casey): If you are using this generically for some reason,
        // please remember that BMP files CAN GO IN EITHER DIRECTION and
        // the height will be negative for top-down.
        // (Also, there can be compression, etc., etc... DON'T think this
        // is complete BMP loading code because it isn't!!)

        //We have the Pixels now. Now we want to basically put those pixels into a variable,
        // so that they can be loaded pixel by pixel from the variable to the screen. 
        //We know that they are 24 bits per pixel. We know their address starts at Pixels.
        //We know that the pixels start at lower left corner of bmp image and go up row by row.
        

    }
 
    return(Result);
}

internal bool32
DEBUGSaveGameState(thread_context *Thread, debug_platform_write_entire_file *WriteEntireFile, game_memory *Memory)
{
    bool32 Result = WriteEntireFile(0, "test/GameState.txt", sizeof(game_state), Memory->PermanentStorage);
    return(Result);
//#define DEBUG_PLATFORM_WRITE_ENTIRE_FILE(name) bool32 name(thread_context *Thread, char *Filename, uint32 MemorySize, void *Memory)
    //  typedef DEBUG_PLATFORM_WRITE_ENTIRE_FILE(debug_platform_write_entire_file);

}

//DEBUGLoadGameState(0, GameState, Memory->DEBUGPlatformWriteEntireFile, "../../ghost/data/GameState.txt"); 
internal game_state
DEBUGLoadGameState(thread_context *Thread, debug_platform_read_entire_file *ReadEntireFile, debug_platform_free_file_memory *FreeFile, char *FileName)
{
    game_state SaveData = {};
    
    debug_read_file_result ReadResult = ReadEntireFile(Thread, FileName);
    if(ReadResult.ContentsSize != 0)
    {
        SaveData = *(game_state *)ReadResult.Contents;
        FreeFile(0, ReadResult.Contents);
    }
    else
    {
        Assert(0);
    }

    return(SaveData);
        
}


inline void
CopyString(uint8 *Source, uint8 *Dest)
{
    uint32 DestI = 0;
    uint8 *SourcePtr = Source;
    
    while(*SourcePtr != 0 /*'@'*/)
    {
        Dest[DestI++] = *SourcePtr++;                        
    }

    //TODO: Don't use a literal for the string limiter dewd
    
    while(DestI < 20)
    {
        Dest[DestI++] = 0;
    }
}

//TODO: Debug this function
inline bool32
DropObj(v2 CharacterPos, uint32 FacingDirection, uint32 ObjID, object *Objects, uint32 *TileMap) 
{    
    v2 TilePos = CharacterPos;
    bool32 DropIt = false;    
    uint32 TileID = 0;
    v2 SecondTryPos = {};
    bool32 NS = false;
    bool32 EW = false;

    switch(FacingDirection)
    {
        case N :        
        case S :
        {
            
            if((TilePos.X - (uint32)TilePos.X) >= 0.5f && (TileMap[((uint32)TilePos.Y * 45) + (uint32)TilePos.X+1] == FLOOR_TILE))
            {
                SecondTryPos.X = (uint32)TilePos.X + 1.1f;
                NS = true;
            }
            else if(TileMap[((uint32)TilePos.Y * 45) + (uint32)TilePos.X-1] == FLOOR_TILE)
            {
                SecondTryPos.X = (uint32)TilePos.X - 0.1f;
                NS = true;
            }
        }break;
        case E :
        case W :
        {
            
            if((TilePos.Y - (uint32)TilePos.Y) >= 0.5f && TileMap[((uint32)(TilePos.Y+1) * 45) + (uint32)TilePos.X] == FLOOR_TILE)
            {
                SecondTryPos.Y = (uint32)TilePos.Y + 1.1f;
                EW = true;
            }
            else if(TileMap[((uint32)(TilePos.Y-1) * 45) + (uint32)TilePos.X] == FLOOR_TILE)
            {
                SecondTryPos.Y = (uint32)TilePos.Y - 0.1f;
                EW = true;
            }
        }break;
    };
        
    for(real32 Offset = 1.2f; Offset > -1.2f; Offset -= .25f)
    {
        
        //NOTE: Not the most efficient thing to have the switch inside of the loop, but it's a short loop.
        switch(FacingDirection)
        {
            case N : TilePos.Y = CharacterPos.Y + Offset + 0.3f; break;
            case S : TilePos.Y = CharacterPos.Y - Offset + 0.3f; break;
            case E : TilePos.X = CharacterPos.X + Offset + 0.3f; break;
            case W : TilePos.X = CharacterPos.X - Offset - 0.3f; break;
        };

        //TODO: shouldn't have that literal 45 in there.
        // Replace it with TileMap.width variable.
        TileID = TileMap[((uint32)TilePos.Y * 45) + (uint32)TilePos.X];
    
        if(TileID==HUB_TILE || TileID==CHAIR_TILE || TileID==TABLE_TILE || TileID==HOLE_TILE || TileID==FLOOR_TILE)
        {
            DropIt = true;
            break;
        }
        else if(NS || EW)
        {
            
            if(NS)
            {
                SecondTryPos.Y = TilePos.Y;            
            }        
            else
            {
                SecondTryPos.X = TilePos.X;            
            }
            TileID = TileMap[((uint32)SecondTryPos.Y * 45) + (uint32)SecondTryPos.X];

            if(TileID==HUB_TILE || TileID==CHAIR_TILE || TileID==TABLE_TILE || TileID==HOLE_TILE || TileID==FLOOR_TILE)
            {
                DropIt = true;
                TilePos = SecondTryPos;
                break;
            }
        }    
    }

    if(DropIt)
    {
        Objects[ObjID].Pos = TilePos;
        Objects[ObjID].Possessed = false;
    }
    
    return(DropIt);    
}


extern "C" GAME_UPDATE_AND_RENDER(GameUpdateAndRender)
{
    Assert((&Input->Controllers[0].Terminator - &Input->Controllers[0].Buttons[0]) ==
           (ArrayCount(Input->Controllers[0].Buttons)));
    Assert(sizeof(game_state) <= Memory->PermanentStorageSize);
    game_state *GameState = (game_state *)Memory->PermanentStorage;
        
    if(!Memory->IsInitialized)
    {
                
        //(0, -1, -1, -1)
        //18x26
        GameState->PlayerSouthBitmap[0] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/South1.bmp"); 
        GameState->PlayerSouthBitmap[1] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/South2.bmp");
        GameState->PlayerSouthBitmap[2] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/South3.bmp");
        GameState->PlayerSouthBitmap[3] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/South4.bmp");
        GameState->PlayerSouthBitmap[4] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/South5.bmp");
        GameState->PlayerSouthBitmap[5] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/South6.bmp");
        GameState->PlayerSouthBitmap[6] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/South7.bmp");
        GameState->PlayerSouthBitmap[7] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/South8.bmp");

        //(0, -2, -2, -2)
        //20x27
        GameState->PlayerNorthBitmap[0] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/North1.bmp");
        GameState->PlayerNorthBitmap[1] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/North2.bmp");
        GameState->PlayerNorthBitmap[2] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/North3.bmp");
        GameState->PlayerNorthBitmap[3] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/North4.bmp");
        GameState->PlayerNorthBitmap[4] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/North5.bmp");
        GameState->PlayerNorthBitmap[5] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/North6.bmp");
        GameState->PlayerNorthBitmap[6] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/North7.bmp");
        GameState->PlayerNorthBitmap[7] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/North8.bmp");

        //(0, -2, -1, -2)
        //19x27
        GameState->PlayerEastBitmap[0] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/East1.bmp");
        GameState->PlayerEastBitmap[1] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/East2.bmp");
        GameState->PlayerEastBitmap[2] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/East3.bmp");
        GameState->PlayerEastBitmap[3] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/East4.bmp");
        GameState->PlayerEastBitmap[4] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/East5.bmp");
        GameState->PlayerEastBitmap[5] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/East6.bmp");
        GameState->PlayerEastBitmap[6] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/East7.bmp");
        GameState->PlayerEastBitmap[7] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/East8.bmp");

        //(1, 0, -3, -1)
        //20x24
        GameState->PlayerWestBitmap[0] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/West1.bmp");  
        GameState->PlayerWestBitmap[1] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/West2.bmp");
        GameState->PlayerWestBitmap[2] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/West3.bmp");
        GameState->PlayerWestBitmap[3] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/West4.bmp");
        GameState->PlayerWestBitmap[4] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/West5.bmp");
        GameState->PlayerWestBitmap[5] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/West6.bmp");
        GameState->PlayerWestBitmap[6] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/West7.bmp");
        GameState->PlayerWestBitmap[7] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/West8.bmp");

        
        //(0, -1, -1, -1)
        //18x26
        GameState->BobSouthBitmap[0] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/South_0.bmp"); 
        GameState->BobSouthBitmap[1] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/South_L1.bmp");
        GameState->BobSouthBitmap[2] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/South_L2.bmp");
        GameState->BobSouthBitmap[3] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/South_L1.bmp");
        GameState->BobSouthBitmap[4] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/South_0.bmp");
        GameState->BobSouthBitmap[5] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/South_R1.bmp");
        GameState->BobSouthBitmap[6] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/South_R2.bmp");
        GameState->BobSouthBitmap[7] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/South_R1.bmp");

        //(0, -2, -2, -2)
        //20x27
        GameState->BobNorthBitmap[0] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/North_0.bmp");
        GameState->BobNorthBitmap[1] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/North_L1.bmp");
        GameState->BobNorthBitmap[2] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/North_L2.bmp");
        GameState->BobNorthBitmap[3] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/North_L1.bmp");
        GameState->BobNorthBitmap[4] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/North_0.bmp");
        GameState->BobNorthBitmap[5] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/North_R1.bmp");
        GameState->BobNorthBitmap[6] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/North_R2.bmp");
        GameState->BobNorthBitmap[7] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/North_R1.bmp");

        //(0, -2, -1, -2)
        //19x27
        GameState->BobEastBitmap[0] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/East_0.bmp");
        GameState->BobEastBitmap[1] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/East_L1.bmp");
        GameState->BobEastBitmap[2] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/East_L2.bmp");
        GameState->BobEastBitmap[3] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/East_L1.bmp");
        GameState->BobEastBitmap[4] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/East_0.bmp");
        GameState->BobEastBitmap[5] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/East_R1.bmp");
        GameState->BobEastBitmap[6] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/East_R2.bmp");
        GameState->BobEastBitmap[7] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/East_R1.bmp");

        //(1, 0, -3, -1)
        //20x24
        GameState->BobWestBitmap[0] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/West_0.bmp");  
        GameState->BobWestBitmap[1] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/West_L1.bmp");
        GameState->BobWestBitmap[2] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/West_L2.bmp");
        GameState->BobWestBitmap[3] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/West_L1.bmp");
        GameState->BobWestBitmap[4] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/West_0.bmp");
        GameState->BobWestBitmap[5] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/West_R1.bmp");
        GameState->BobWestBitmap[6] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/West_R2.bmp");
        GameState->BobWestBitmap[7] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/West_R1.bmp");

        
        GameState->PlayerSpiritBitmap = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/Spirit_0.bmp");
        
        //BMP Tiles
        //GameState->FloorTileBitmap = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/FloorTile.bmp");

        //Apartment layers
        GameState->AptLayer[MAIN_LAYER_A].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/MainA.bmp");
        GameState->AptLayer[MAIN_LAYER_Z].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/MainZ.bmp");
        GameState->AptLayer[MAIN_WALL].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/MainWall.bmp");
        GameState->AptLayer[MAIN_WALL].Pos.Y = 31.9f;
        GameState->AptLayer[MAIN_WALL_N].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/MainWallN.bmp");
        GameState->AptLayer[MAIN_WALL_N].Pos.Y = 40.5f;

        GameState->AptLayer[MAIN_CHAIR].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/MainChair.bmp");
        GameState->AptLayer[MAIN_CHAIR].Pos.Y = 28.0f;
        GameState->AptLayer[MAIN_DOORFRAME_S].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/MainDoorFrameSouth.bmp");
        GameState->AptLayer[MAIN_DOORFRAME_S].Pos.Y = 16.0f;
        GameState->AptLayer[MAIN_DOORFRAME_N].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/MainDoorFrameNorth.bmp");
        GameState->AptLayer[MAIN_DOORFRAME_N].Pos.Y = 36.0f;
        GameState->AptLayer[MAIN_PINGPONG_TABLE].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/MainPingPongTable.bmp");
        GameState->AptLayer[MAIN_PINGPONG_TABLE].Pos.Y = 20.0f;
        
        GameState->AptLayer[BED_LAYER_A].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/BedA.bmp");
        GameState->AptLayer[BED_LAYER_Z].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/BedZ.bmp");
        GameState->AptLayer[BED_CHAIR].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/BedChair.bmp");
        GameState->AptLayer[BED_CHAIR].Pos.Y = 28.0f;
        GameState->AptLayer[BED_WALL].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/BedWall.bmp");
        GameState->AptLayer[BED_WALL].Pos.Y = 32.5f;
        GameState->AptLayer[BED_DOORFRAME].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/BedDoorFrame.bmp");
        GameState->AptLayer[BED_DOORFRAME].Pos.Y = 16.0f;
        
        GameState->AptLayer[BATH_LAYER_A].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/BathA.bmp");
        GameState->AptLayer[BATH_WALL].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/BathWall.bmp");
        GameState->AptLayer[BATH_WALL].Pos.Y = 40.5f;
        GameState->AptLayer[BATH_LAYER_Z].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/BathZ.bmp");
        GameState->AptLayer[BATH_TOILET].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/BathToilet.bmp");
        GameState->AptLayer[BATH_TOILET].Pos.Y = 36.0f;
        GameState->AptLayer[BATH_DOORFRAME].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/BathDoorFrame.bmp");
        GameState->AptLayer[BATH_DOORFRAME].Pos.Y = 36.0f;
            
        GameState->AptLayer[TV_KITCHEN_LAYER_A].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/TvKitchenA.bmp");
        GameState->AptLayer[TV_KITCHEN_LAYER_Z].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/TvKitchenZ.bmp");
        GameState->AptLayer[KITCHEN_WALL].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/KitchenWall.bmp");
        GameState->AptLayer[KITCHEN_WALL].Pos.Y = 10.5f;
        GameState->AptLayer[KITCHEN_STOOL].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/KitchenStool.bmp");
        GameState->AptLayer[KITCHEN_STOOL].Pos.Y = 2.9f;
        GameState->AptLayer[KITCHEN_TABLE].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/KitchenTable.bmp");
        GameState->AptLayer[KITCHEN_TABLE].Pos.Y = 2.9f;
        GameState->AptLayer[TV_WALL].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/TvWall.bmp");
        GameState->AptLayer[TV_WALL].Pos.Y = 13.5f;
        GameState->AptLayer[TV_COUCH_HORI].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/TvCouchHori.bmp");
        GameState->AptLayer[TV_COUCH_HORI].Pos.Y = 3.33f;
        GameState->AptLayer[TV_COUCH_VERTI].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/TvCouchVerti.bmp");
        GameState->AptLayer[TV_COUCH_VERTI].Pos.Y = 5.5f;
        GameState->AptLayer[TV_TABLE].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/TvTable.bmp");
        GameState->AptLayer[TV_TABLE].Pos.Y = 6.9f;
        
        GameState->AptLayer[MAINCLOSET_LAYER_A].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/MainClosetA.bmp");
        GameState->AptLayer[MAINCLOSET_LAYER_Z].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/MainClosetZ.bmp");
        
        GameState->AptLayer[BEDCLOSET_LAYER_A].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/BedClosetA.bmp");
        GameState->AptLayer[BEDCLOSET_WALL].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/BedClosetWall.bmp");
        GameState->AptLayer[BEDCLOSET_WALL].Pos.Y = 15.5f;
        GameState->AptLayer[BEDCLOSET_LAYER_Z].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/BedClosetZ.bmp");

        //TODO
        //Outdoors
        GameState->AptLayer[OUTDOORS_LAYER_A].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/OutdoorsA.bmp");
        GameState->AptLayer[OUTDOORS_MAILBOX].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/OutdoorsMailbox.bmp");
        GameState->AptLayer[OUTDOORS_MAILBOX].Pos.Y = 42.0f; 
        GameState->AptLayer[OUTDOORS_DUMPSTER].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/OutdoorsDumpster.bmp");
        GameState->AptLayer[OUTDOORS_DUMPSTER].Pos.Y = 45.5f;
        
        GameState->QuestionMark = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/QuestionMark.bmp");
        GameState->ExclamationMark = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/ExclamationMark.bmp");
        GameState->BobIcon = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/BobIcon.bmp");
        GameState->SpeechIcon = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/SpeechIcon.bmp");
        
        
        GameState->Words[WORDS_WRITE].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/WordsWrite.bmp");
        GameState->Words[WORDS_WRITE].TimeDuration = 300;
        GameState->Words[WORDS_INSPIREDWRITE].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/WordsInspiredWrite.bmp");
        GameState->Words[WORDS_INSPIREDWRITE].TimeDuration = 300;
        GameState->Words[WORDS_DEAR_EVELYN].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/WordsDear.bmp");
        GameState->Words[WORDS_DEAR_EVELYN].TimeDuration = 200;
        GameState->Words[WORDS_DOUBT].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/WordsDoubt.bmp");
        GameState->Words[WORDS_DOUBT].TimeDuration = 600;
        GameState->Words[WORDS_SHREDDER].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/WordsShredder.bmp");
        GameState->Words[WORDS_SHREDDER].TimeDuration = 250;
        GameState->Words[WORDS_TRASH].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/WordsTrash.bmp");
        GameState->Words[WORDS_TRASH].TimeDuration = 250;
        GameState->Words[WORDS_THERE].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/WordsThere.bmp");
        GameState->Words[WORDS_THERE].TimeDuration = 120;
        GameState->Words[WORDS_WORK].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/WordsWork.bmp");
        GameState->Words[WORDS_WORK].TimeDuration = 300;
        GameState->Words[WORDS_FOOD].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/WordsFood.bmp");
        GameState->Words[WORDS_FOOD].TimeDuration = 300;
        GameState->Words[WORDS_TV].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/WordsTv.bmp");
        GameState->Words[WORDS_TV].TimeDuration = 600;
        GameState->Words[WORDS_YAWN].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/WordsYawn.bmp");
        GameState->Words[WORDS_YAWN].TimeDuration = 120;
        GameState->Words[WORDS_SEARCH].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/WordsSearch.bmp");
        GameState->Words[WORDS_SEARCH].TimeDuration = 180;
        

        
        
        GameState->ClockDigits[0] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/ClockDigit_0.bmp");
        GameState->ClockDigits[1] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/ClockDigit_1.bmp");
        GameState->ClockDigits[2] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/ClockDigit_2.bmp");
        GameState->ClockDigits[3] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/ClockDigit_3.bmp");
        GameState->ClockDigits[4] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/ClockDigit_4.bmp");
        GameState->ClockDigits[5] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/ClockDigit_5.bmp");
        GameState->ClockDigits[6] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/ClockDigit_6.bmp");
        GameState->ClockDigits[7] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/ClockDigit_7.bmp");
        GameState->ClockDigits[8] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/ClockDigit_8.bmp");
        GameState->ClockDigits[9] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/ClockDigit_9.bmp");
        GameState->ClockDigits[10] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/ClockDigit_AM.bmp");
        GameState->ClockDigits[11] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/ClockDigit_PM.bmp");
        GameState->CurrentClockDigits[2] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/ClockDigit_Colon.bmp");

        GameState->Clock = {5,0,0};
        
                    
            if((GameState->Clock.Hr > 0 && GameState->Clock.Hr < 10) || (GameState->Clock.Hr > 12 && GameState->Clock.Hr < 22))
            {
                GameState->CurrentClockDigits[0] = GameState->ClockDigits[0];            
            }
            else
            {
                GameState->CurrentClockDigits[0] = GameState->ClockDigits[1];            
            }

        
            if(!GameState->Clock.Hr)
            {
                GameState->CurrentClockDigits[1] = GameState->ClockDigits[2];
            }
            else if(GameState->Clock.Hr < 13)
            {
                GameState->CurrentClockDigits[1] = GameState->ClockDigits[GameState->Clock.Hr % 10];
            }        
            else 
            {
                GameState->CurrentClockDigits[1] = GameState->ClockDigits[(GameState->Clock.Hr - 2) % 10];
            }

            
            if(GameState->Clock.Hr == 12 && GameState->Clock.Min < 5)
            {         
                GameState->CurrentClockDigits[5] = {};
            }
            else if(!GameState->Clock.Hr && GameState->Clock.Min < 5)
            {         
                GameState->CurrentClockDigits[0] = {};
            }        
            else if(GameState->Clock.Hr < 12)
            {            
                GameState->CurrentClockDigits[5] = GameState->ClockDigits[10];
            }
            else
            {
                GameState->CurrentClockDigits[5] = GameState->ClockDigits[11];
            }
            
            GameState->CurrentClockDigits[3] = GameState->ClockDigits[GameState->Clock.Min/10];
            GameState->CurrentClockDigits[4] = GameState->ClockDigits[GameState->Clock.Min%10];




        
        //MAIN ROOM
        GameState->Room[MAIN_ROOM].ID = MAIN_ROOM;
        GameState->Room[MAIN_ROOM].RectangleCount = 2;
        GameState->Room[MAIN_ROOM].RoomsAway =
            ((uint64)0 << MAIN_ROOM*8) |
            ((uint64)1 << 8*BED_ROOM) |
            ((uint64)1 << 8*BATH_ROOM) |
            ((uint64)1 << 8*TV_ROOM) |
            ((uint64)2 << 8*KITCHEN_ROOM) |
            ((uint64)2 << 8*CLOSET_BED_ROOM) |
            ((uint64)1 << 8*CLOSET_MAIN_ROOM) |
            ((uint64)1 << 8*OUTDOORS_ROOM);
        GameState->Room[MAIN_ROOM].HubCount = ArrayCount(GameState->MainRoomHubs);
        GameState->Room[MAIN_ROOM].Hub = GameState->MainRoomHubs;
        GameState->MainRoomHubs[0].ID = 0x7252301022;// @: A, D
        GameState->MainRoomHubs[1].ID = 0x7352311123;// A: @, B        
        GameState->MainRoomHubs[2].ID = 0x7351321223;// B: A, C, E
        GameState->MainRoomHubs[3].ID = 0x7452331324;// C: B, F
        GameState->MainRoomHubs[4].ID = 0x7151311121;// D: @, G, E
        GameState->MainRoomHubs[5].ID = 0x7250321222;// E: B, D, F                
        GameState->MainRoomHubs[6].ID = 0x7351331323;// F: B, C, E
        GameState->MainRoomHubs[7].ID = 0x7052321220;// G: D                
        GameState->Room[MAIN_ROOM].AdjacentRoomCount = 5;
        GameState->Room[MAIN_ROOM].DoorMask = 0x0f0f0f0f0f;
                
        //BED ROOM
        GameState->Room[BED_ROOM].ID = BED_ROOM;
        GameState->Room[BED_ROOM].RectangleCount = 1;
        GameState->Room[BED_ROOM].RoomsAway =
            ((uint64)1 << MAIN_ROOM*8) |
            ((uint64)0 << 8*BED_ROOM) |
            ((uint64)1 << 8*BATH_ROOM) |
            ((uint64)2 << 8*TV_ROOM) |
            ((uint64)3 << 8*KITCHEN_ROOM) |
            ((uint64)1 << 8*CLOSET_BED_ROOM) |
            ((uint64)2 << 8*CLOSET_MAIN_ROOM) |
            ((uint64)2 << 8*OUTDOORS_ROOM);
        GameState->Room[BED_ROOM].HubCount = ArrayCount(GameState->BedRoomHubs);
        GameState->Room[BED_ROOM].Hub = GameState->BedRoomHubs;
        GameState->BedRoomHubs[0].ID = 0x602602;
        GameState->BedRoomHubs[1].ID = 0x612501;
        GameState->BedRoomHubs[2].ID = 0x622600;
        GameState->BedRoomHubs[3].ID = 0x622402;
        GameState->BedRoomHubs[4].ID = 0x652105;
        GameState->BedRoomHubs[5].ID = 0x642204;
        GameState->BedRoomHubs[6].ID = 0x632303;
        GameState->BedRoomHubs[7].ID = 0x662006;
        GameState->BedRoomHubs[8].ID = 0x652105;
        GameState->Room[BED_ROOM].AdjacentRoomCount = 3;
        GameState->Room[BED_ROOM].DoorMask = 0x0f0f0f;
        
        //BATH_ROOM ROOM
        GameState->Room[BATH_ROOM].ID = BATH_ROOM;
        GameState->Room[BATH_ROOM].RectangleCount = 1;
        GameState->Room[BATH_ROOM].RoomsAway =
            ((uint64)1 << MAIN_ROOM*8) |
            ((uint64)1 << 8*BED_ROOM) |
            ((uint64)0 << 8*BATH_ROOM) |
            ((uint64)2 << 8*TV_ROOM) |
            ((uint64)3 << 8*KITCHEN_ROOM) |
            ((uint64)2 << 8*CLOSET_BED_ROOM) |
            ((uint64)2 << 8*CLOSET_MAIN_ROOM) |
            ((uint64)2 << 8*OUTDOORS_ROOM);
        GameState->Room[BATH_ROOM].HubCount = ArrayCount(GameState->BathRoomHubs);
        GameState->Room[BATH_ROOM].Hub = GameState->BathRoomHubs;
        GameState->BathRoomHubs[0].ID = 0x0510;
        GameState->BathRoomHubs[1].ID = 0x0411;
        GameState->BathRoomHubs[2].ID = 0x0312;
        GameState->BathRoomHubs[3].ID = 0x0213;
        GameState->BathRoomHubs[4].ID = 0x0114;
        GameState->BathRoomHubs[5].ID = 0x0015;
        GameState->Room[BATH_ROOM].AdjacentRoomCount = 2;
        GameState->Room[BATH_ROOM].DoorMask = 0x0f0f;
                        
        //TV ROOM
        GameState->Room[TV_ROOM].ID = TV_ROOM;
        GameState->Room[TV_ROOM].RectangleCount = 1;
        GameState->Room[TV_ROOM].RoomsAway =
            ((uint64)1 << MAIN_ROOM*8) |
            ((uint64)2 << 8*BED_ROOM) |
            ((uint64)2 << 8*BATH_ROOM) |
            ((uint64)0 << 8*TV_ROOM) |
            ((uint64)1 << 8*KITCHEN_ROOM) |
            ((uint64)3 << 8*CLOSET_BED_ROOM) |
            ((uint64)2 << 8*CLOSET_MAIN_ROOM) |
            ((uint64)2 << 8*OUTDOORS_ROOM);
        GameState->Room[TV_ROOM].HubCount = ArrayCount(GameState->TvRoomHubs);
        GameState->Room[TV_ROOM].Hub = GameState->TvRoomHubs;
        GameState->TvRoomHubs[0].ID = 0x4202;
        GameState->TvRoomHubs[1].ID = 0x4303;
        GameState->TvRoomHubs[2].ID = 0x4000;
        GameState->TvRoomHubs[3].ID = 0x4101;
        GameState->TvRoomHubs[4].ID = 0x4202;
        GameState->Room[TV_ROOM].AdjacentRoomCount = 2;
        GameState->Room[TV_ROOM].DoorMask = 0x0f0f;        
                        
        //KITCHEN ROOM
        GameState->Room[KITCHEN_ROOM].ID = KITCHEN_ROOM;
        GameState->Room[KITCHEN_ROOM].RectangleCount = 1;
        GameState->Room[KITCHEN_ROOM].RoomsAway =
            ((uint64)2 << MAIN_ROOM*8) |
            ((uint64)3 << 8*BED_ROOM) |
            ((uint64)3 << 8*BATH_ROOM) |
            ((uint64)1 << 8*TV_ROOM) |
            ((uint64)0 << 8*KITCHEN_ROOM) |
            ((uint64)4 << 8*CLOSET_BED_ROOM) |
            ((uint64)3 << 8*CLOSET_MAIN_ROOM) |
            ((uint64)3 << 8*OUTDOORS_ROOM);
        GameState->Room[KITCHEN_ROOM].HubCount = ArrayCount(GameState->KitchenRoomHubs);
        GameState->Room[KITCHEN_ROOM].Hub = GameState->KitchenRoomHubs;
        GameState->KitchenRoomHubs[0].ID = 0x33;
        GameState->KitchenRoomHubs[1].ID = 0x32;
        GameState->KitchenRoomHubs[2].ID = 0x31;
        GameState->KitchenRoomHubs[3].ID = 0x30;
        GameState->Room[KITCHEN_ROOM].AdjacentRoomCount = 1;
        GameState->Room[KITCHEN_ROOM].DoorMask = 0x0f;
                        
        //CLOSET BED ROOM
        GameState->Room[CLOSET_BED_ROOM].ID = CLOSET_BED_ROOM;
        GameState->Room[CLOSET_BED_ROOM].RectangleCount = 1;
        GameState->Room[CLOSET_BED_ROOM].RoomsAway =
            ((uint64)2 << MAIN_ROOM*8) |
            ((uint64)1 << 8*BED_ROOM) |
            ((uint64)2 << 8*BATH_ROOM) |
            ((uint64)3 << 8*TV_ROOM) |
            ((uint64)4 << 8*KITCHEN_ROOM) |
            ((uint64)0 << 8*CLOSET_BED_ROOM) |
            ((uint64)3 << 8*CLOSET_MAIN_ROOM) |
            ((uint64)3 << 8*OUTDOORS_ROOM);
        GameState->Room[CLOSET_BED_ROOM].HubCount = ArrayCount(GameState->ClosetBedRoomHubs);
        GameState->Room[CLOSET_BED_ROOM].Hub = GameState->ClosetBedRoomHubs;
        GameState->ClosetBedRoomHubs[0].ID = 0x10;
        GameState->ClosetBedRoomHubs[1].ID = 0x11;
        GameState->Room[CLOSET_BED_ROOM].AdjacentRoomCount = 1;
        GameState->Room[CLOSET_BED_ROOM].DoorMask = 0x0f;
        //GameState->Room[0].Hub = ;
                                
        //CLOSET MAIN ROOM
        GameState->Room[CLOSET_MAIN_ROOM].ID = CLOSET_MAIN_ROOM;
        GameState->Room[CLOSET_MAIN_ROOM].RectangleCount = 1;        
        GameState->Room[CLOSET_MAIN_ROOM].RoomsAway =
            ((uint64)1 << MAIN_ROOM*8) |
            ((uint64)2 << 8*BED_ROOM) |
            ((uint64)2 << 8*BATH_ROOM) |
            ((uint64)2 << 8*TV_ROOM) |
            ((uint64)3 << 8*KITCHEN_ROOM) |
            ((uint64)3 << 8*CLOSET_BED_ROOM) |
            ((uint64)0 << 8*CLOSET_MAIN_ROOM) |
            ((uint64)2 << 8*OUTDOORS_ROOM);
        GameState->Room[CLOSET_MAIN_ROOM].HubCount = ArrayCount(GameState->ClosetMainRoomHubs);
        GameState->Room[CLOSET_MAIN_ROOM].Hub = GameState->ClosetMainRoomHubs;
        GameState->ClosetMainRoomHubs[0].ID = 0x00;
        GameState->ClosetMainRoomHubs[1].ID = 0x01;
        GameState->Room[CLOSET_MAIN_ROOM].AdjacentRoomCount = 1;
        GameState->Room[CLOSET_MAIN_ROOM].DoorMask = 0x0f;

        
        //TODO
        //OUTDOORS_ROOM
        GameState->Room[OUTDOORS_ROOM].ID = OUTDOORS_ROOM;
        GameState->Room[OUTDOORS_ROOM].RectangleCount = 1;        
        GameState->Room[OUTDOORS_ROOM].RoomsAway =
            ((uint64)1 << MAIN_ROOM*8) |
            ((uint64)2 << 8*BED_ROOM) |
            ((uint64)2 << 8*BATH_ROOM) |
            ((uint64)2 << 8*TV_ROOM) |
            ((uint64)3 << 8*KITCHEN_ROOM) |
            ((uint64)3 << 8*CLOSET_BED_ROOM) |
            ((uint64)2 << 8*CLOSET_MAIN_ROOM) |
            ((uint64)0 << 8*OUTDOORS_ROOM);
        GameState->Room[OUTDOORS_ROOM].HubCount = ArrayCount(GameState->OutdoorsRoomHubs);
        GameState->Room[OUTDOORS_ROOM].Hub = GameState->OutdoorsRoomHubs;
        GameState->OutdoorsRoomHubs[0].ID = 0x00;
        GameState->OutdoorsRoomHubs[1].ID = 0x01;
        GameState->Room[OUTDOORS_ROOM].AdjacentRoomCount = 1;
        GameState->Room[OUTDOORS_ROOM].DoorMask = 0x0f;

        
        
        //IMPORTANT: I swapped in L0_CollisionMap.bmp for Apt_CollisionMap.bmp
        GameState->ApartmentBitmap = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/Apt_CollisionMap.bmp");
        loaded_bitmap ApartmentHubAndRoomBitmap = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/Apt_HubAndRoomMap.bmp");

        uint32 OutdoorsRoomHubsI = 0;
        uint32 BathRoomHubsI = 0;
        uint32 MainRoomHubsI = 0;
        uint32 ClosetMainRoomHubsI = 0;
        uint32 ClosetBedRoomHubsI = 0;
        uint32 BedRoomHubsI = 0;
        uint32 KitchenRoomHubsI = 0;
        uint32 TvRoomHubsI = 0;               
        uint32 BathRoomDimsI = 0;
        uint32 MainRoomDimsI = 0;
        uint32 MainRoom2DimsI = 0;
        uint32 ClosetMainRoomDimsI = 0;
        uint32 ClosetBedRoomDimsI = 0;
        uint32 BedRoomDimsI = 0;
        uint32 KitchenRoomDimsI = 0;
        uint32 TvRoomDimsI = 0;
        uint32 OutdoorsRoomDimsI = 0;
        uint8 HubCharOffset = 64;
                
        for(uint32 BMPI = 0; BMPI < 2; ++BMPI)
        {
            loaded_bitmap TempBMP = (BMPI == 0) ? GameState->ApartmentBitmap : ApartmentHubAndRoomBitmap;
            uint64 BytesPerRow = TempBMP.Width*3;
            uint32 PaddingPerRow = 0;
        
            while(BytesPerRow % 4 != 0)
            {
                ++PaddingPerRow;
                ++BytesPerRow;
            }                        
            uint8 *ApartmentPixelsRow = (uint8 *)TempBMP.Pixels; //(TempBMP.Width*PaddingPerRow) + 3*(TempBMP.Width)*(TempBMP.Height - 1) - 3;                                                           
                                                            
            for(int32 AptY = 0; AptY < TempBMP.Height; ++AptY)
            {
                uint8 *AptPixel = ApartmentPixelsRow;            
            
                for(int32 AptX = 0; AptX < TempBMP.Width; ++AptX)
                {
                    // in mem:      BBGGRRXX
                    // in variable: XXRRGGBB
                    uint32 Pixel = *(uint32 *)AptPixel & 0x00ffffff;
                    
                    if(BMPI == 0)
                    {
                                                                                
                        if(Pixel == 0x833100)
                        {
                            GameState->AptTileMap[AptY][AptX] = TABLE_TILE;
                        }
                        else if(Pixel == 0x5b315b)
                        {
                            GameState->AptTileMap[AptY][AptX] = CHAIR_TILE;
                        }
                        else if(Pixel == 0xFFFF00)
                        {
                            GameState->AptTileMap[AptY][AptX] = DOOR_TILE;
                        }
                        else if(Pixel == 0x000000)
                        {
                            GameState->AptTileMap[AptY][AptX] = WALL_TILE;
                        }
                        else if(Pixel == 0x123456)
                        {
                            GameState->AptTileMap[AptY][AptX] = FLOOR_TILE;
                        }
                        else if(Pixel == 0xe60000)
                        {
                            GameState->AptTileMap[AptY][AptX] = HOLE_TILE;
                        }
                        else if(Pixel == 0xFF8584)
                        {
                            GameState->AptTileMap[AptY][AptX] = HUB_TILE;
                        }
                        else if(Pixel == 0x525252)
                        {
                            GameState->AptTileMap[AptY][AptX] = OUTER_WALL_TILE;
                        }
                        else if(Pixel == 0xff4200)
                        {
                            GameState->AptTileMap[AptY][AptX] = OUTER_DOOR_TILE;
                        }
                        else
                        {
                            Assert(0);
                        }
                    }
                    else // IF BMPI == 1
                    {
                        real32 RealAptX = (real32)AptX + 0.5f;
                        real32 RealAptY = (real32)AptY + 0.5f;
                        switch(Pixel)
                        {
                            case 0x123456 :
                            {
                                
                            }break;
                            case 0xffff00 :
                            {
                                GameState->BathRoomHubs[BathRoomHubsI].Pos.X = RealAptX;
                                GameState->BathRoomHubs[BathRoomHubsI].Pos.Y = RealAptY;
                                GameState->BathRoomHubs[BathRoomHubsI].Char = (uint8)BathRoomHubsI + HubCharOffset;
                                ++BathRoomHubsI;
                            }break;
                            case 0x000000 :
                            {
                                GameState->BedRoomHubs[BedRoomHubsI].Pos.X = RealAptX;
                                GameState->BedRoomHubs[BedRoomHubsI].Pos.Y = RealAptY;
                                GameState->BedRoomHubs[BedRoomHubsI].Char = (uint8)BedRoomHubsI + HubCharOffset;
                                ++BedRoomHubsI;
                            }break;
                            case 0xffffff :
                            {
                                GameState->MainRoomHubs[MainRoomHubsI].Pos.X = RealAptX;
                                GameState->MainRoomHubs[MainRoomHubsI].Pos.Y = RealAptY;
                                GameState->MainRoomHubs[MainRoomHubsI].Char = (uint8)MainRoomHubsI + HubCharOffset;
                                ++MainRoomHubsI;
                            }break;
                            case 0xff0000 :
                            {
                                GameState->TvRoomHubs[TvRoomHubsI].Pos.X = RealAptX;
                                GameState->TvRoomHubs[TvRoomHubsI].Pos.Y = RealAptY;
                                GameState->TvRoomHubs[TvRoomHubsI].Char = (uint8)TvRoomHubsI + HubCharOffset;
                                ++TvRoomHubsI;
                            }break;
                            case 0x00ff00 :
                            {
                                GameState->KitchenRoomHubs[KitchenRoomHubsI].Pos.X = RealAptX;
                                GameState->KitchenRoomHubs[KitchenRoomHubsI].Pos.Y = RealAptY;
                                GameState->KitchenRoomHubs[KitchenRoomHubsI].Char = (uint8)KitchenRoomHubsI + HubCharOffset;
                                ++KitchenRoomHubsI;
                            }break;
                            case 0xff00ff :
                            {
                                GameState->ClosetMainRoomHubs[ClosetMainRoomHubsI].Pos.X = RealAptX;
                                GameState->ClosetMainRoomHubs[ClosetMainRoomHubsI].Pos.Y = RealAptY;
                                GameState->ClosetMainRoomHubs[ClosetMainRoomHubsI].Char = (uint8)ClosetMainRoomHubsI + HubCharOffset;
                                ++ClosetMainRoomHubsI;
                            }break;
                            case 0x0000ff :
                            {
                                GameState->ClosetBedRoomHubs[ClosetBedRoomHubsI].Pos.X = RealAptX;
                                GameState->ClosetBedRoomHubs[ClosetBedRoomHubsI].Pos.Y = RealAptY;
                                GameState->ClosetBedRoomHubs[ClosetBedRoomHubsI].Char = (uint8)ClosetBedRoomHubsI + HubCharOffset;
                                ++ClosetBedRoomHubsI;
                            }break;

                            //TODO: Outdoors hubs initialization
                            case 0x031a28:
                            {
                                GameState->OutdoorsRoomHubs[OutdoorsRoomHubsI].Pos.X = RealAptX;
                                GameState->OutdoorsRoomHubs[OutdoorsRoomHubsI].Pos.Y = RealAptY;
                                GameState->OutdoorsRoomHubs[OutdoorsRoomHubsI].Char = (uint8)OutdoorsRoomHubsI + HubCharOffset;
                                ++OutdoorsRoomHubsI;                             
                            }break;
                            case 0x0c74b0:
                            {
                                GameState->Room[OUTDOORS_ROOM].RectangleVertices[0][OutdoorsRoomDimsI].X = RealAptX;
                                GameState->Room[OUTDOORS_ROOM].RectangleVertices[0][OutdoorsRoomDimsI].Y = RealAptY;
                                if(!OutdoorsRoomDimsI)
                                {
                                    GameState->AptLayer[OUTDOORS_LAYER_A].Pos.X = (real32)AptX;
                                }                                
                                else
                                {
                                    GameState->AptLayer[OUTDOORS_LAYER_A].Pos.Y = (real32)AptY+1;                                    
                                }
                                ++OutdoorsRoomDimsI;
                            }break;
                            
                            case 0x65a49b :
                            {
                                GameState->Room[MAIN_ROOM].RectangleVertices[0][MainRoomDimsI].X = RealAptX;
                                GameState->Room[MAIN_ROOM].RectangleVertices[0][MainRoomDimsI].Y = RealAptY;
                                if(!MainRoomDimsI)
                                {
                                    GameState->AptLayer[MAIN_LAYER_A].Pos.X = (real32)AptX;
                                    GameState->AptLayer[MAIN_LAYER_Z].Pos.X = (real32)AptX;
                                }                                
                                ++MainRoomDimsI;
                            }break;
                            case 0xff8584 :
                            {
                                GameState->Room[MAIN_ROOM].RectangleVertices[1][MainRoom2DimsI].X = RealAptX;
                                GameState->Room[MAIN_ROOM].RectangleVertices[1][MainRoom2DimsI].Y = RealAptY;

                                if(MainRoom2DimsI)
                                {
                                    GameState->AptLayer[MAIN_LAYER_A].Pos.Y = (real32)AptY + 5;
                                    GameState->AptLayer[MAIN_LAYER_Z].Pos.Y = (real32)AptY + 5;
                                }
                                ++MainRoom2DimsI;
                            }break;
                            case 0x833100 :
                            {
                                GameState->Room[BED_ROOM].RectangleVertices[0][BedRoomDimsI].X = RealAptX;
                                GameState->Room[BED_ROOM].RectangleVertices[0][BedRoomDimsI].Y = RealAptY;

                                if(!BedRoomDimsI)
                                {
                                    GameState->AptLayer[BED_LAYER_A].Pos.X = (real32)AptX;
                                    GameState->AptLayer[BED_LAYER_Z].Pos.X = (real32)AptX;
                                }
                                else
                                {
                                    GameState->AptLayer[BED_LAYER_A].Pos.Y = (real32)AptY + 5;
                                    GameState->AptLayer[BED_LAYER_Z].Pos.Y = (real32)AptY + 5;                                    
                                }                                
                                ++BedRoomDimsI;
                            }break;
                            case 0xffad63 :
                            {
                                GameState->Room[BATH_ROOM].RectangleVertices[0][BathRoomDimsI].X = RealAptX;
                                GameState->Room[BATH_ROOM].RectangleVertices[0][BathRoomDimsI].Y = RealAptY;

                                if(!BathRoomDimsI)
                                {
                                    GameState->AptLayer[BATH_LAYER_A].Pos.X = (real32)AptX;
                                    GameState->AptLayer[BATH_LAYER_Z].Pos.X = (real32)AptX;
                                }
                                else
                                {
                                    GameState->AptLayer[BATH_LAYER_A].Pos.Y = (real32)AptY + 5;
                                    GameState->AptLayer[BATH_LAYER_Z].Pos.Y = (real32)AptY + 5;                                    
                                }
                                ++BathRoomDimsI;
                            }break;
                            case 0x008300 :
                            {
                                GameState->Room[TV_ROOM].RectangleVertices[0][TvRoomDimsI].X = RealAptX;
                                GameState->Room[TV_ROOM].RectangleVertices[0][TvRoomDimsI].Y = RealAptY;
                                
                                if(TvRoomDimsI)
                                {
                                    GameState->AptLayer[TV_KITCHEN_LAYER_A].Pos.Y = (real32)AptY + 5;
                                    GameState->AptLayer[TV_KITCHEN_LAYER_Z].Pos.Y = (real32)AptY + 5;                                    
                                }
                                ++TvRoomDimsI;
                            }break;
                            case 0x7bff30 :
                            {
                                GameState->Room[KITCHEN_ROOM].RectangleVertices[0][KitchenRoomDimsI].X = RealAptX;
                                GameState->Room[KITCHEN_ROOM].RectangleVertices[0][KitchenRoomDimsI].Y = RealAptY;

                                if(!KitchenRoomDimsI)
                                {
                                    GameState->AptLayer[TV_KITCHEN_LAYER_A].Pos.X = (real32)AptX;
                                    GameState->AptLayer[TV_KITCHEN_LAYER_Z].Pos.X = (real32)AptX;
                                }
                                ++KitchenRoomDimsI;
                            }break;
                            case 0x943a3a :
                            {
                                GameState->Room[CLOSET_MAIN_ROOM].RectangleVertices[0][ClosetMainRoomDimsI].X = RealAptX;
                                GameState->Room[CLOSET_MAIN_ROOM].RectangleVertices[0][ClosetMainRoomDimsI].Y = RealAptY;

                                if(!ClosetMainRoomDimsI)
                                {
                                    GameState->AptLayer[MAINCLOSET_LAYER_A].Pos.X = (real32)AptX;
                                    GameState->AptLayer[MAINCLOSET_LAYER_Z].Pos.X = (real32)AptX;
                                }
                                else
                                {
                                    GameState->AptLayer[MAINCLOSET_LAYER_A].Pos.Y = (real32)AptY + 5;
                                    GameState->AptLayer[MAINCLOSET_LAYER_Z].Pos.Y = (real32)AptY + 5;                                    
                                }
                                ++ClosetMainRoomDimsI;
                            }break;
                            case 0xf7c4a5 :
                            {
                                GameState->Room[CLOSET_BED_ROOM].RectangleVertices[0][ClosetBedRoomDimsI].X = RealAptX;
                                GameState->Room[CLOSET_BED_ROOM].RectangleVertices[0][ClosetBedRoomDimsI].Y = RealAptY;

                                if(!ClosetBedRoomDimsI)
                                {
                                    GameState->AptLayer[BEDCLOSET_LAYER_A].Pos.X = (real32)AptX;
                                    GameState->AptLayer[BEDCLOSET_LAYER_Z].Pos.X = (real32)AptX;
                                }
                                else
                                {
                                    GameState->AptLayer[BEDCLOSET_LAYER_A].Pos.Y = (real32)AptY + 5;
                                    GameState->AptLayer[BEDCLOSET_LAYER_Z].Pos.Y = (real32)AptY + 5;                                    
                                }
                                ++ClosetBedRoomDimsI;
                            }break;
                            default :
                            {
                                Assert(0);
                            }break;
                        };                        
                    }
                    AptPixel += 3;
                }
                //ApartmentPixelsRow -= (PaddingPerRow + (TempBMP.Width*3));
                ApartmentPixelsRow += (PaddingPerRow + (TempBMP.Width*3));
            }
        }

        
        //TODO: Do I have an unnecessary amount of GameState-> preserved variables and pointers? Get rid of them if I do.
        //Initialize Board
        GameState->Board.TileArray = (uint32 *)GameState->AptTileMap;
        GameState->L0.Board = &GameState->Board;        
        
        GameState->L0.BoardCount = 1;
        //NOTE: Use the Dimensions to pass into functions that search the apt tile map arrays
        GameState->L0.BoardDimensions.X = (real32)GameState->ApartmentBitmap.Width;
        GameState->L0.BoardDimensions.Y = (real32)GameState->ApartmentBitmap.Height;
        GameState->L0.Rooms = (room *)GameState->Room;
        GameState->L0.RoomCount = MAX_ROOMS;
        

        //TODO make the coordinates Unreachable
        GameState->Obj[LETTER] = {{41.5f, 37.5f}, {41.5f, 37.5f}, 0.5f, {1.0f, 1.0f, 1.0f},
                                  {DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/Letter.bmp"),
                                   DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/Letter2.bmp")}, true, false, true};
        
        GameState->Obj[PEN] = {{8.5f, 31.01f}, {8.5f, 31.01f}, 0.5f, {0.0f, 0.0f, 1.0f},
                               {DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/Pencil.bmp"),
                                DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/Pencil2.bmp")}};
        GameState->Obj[LIGHTER] = {{17.5f, 14.5f}, {17.5f, 14.5f}, 0.5f, {255.0f, (79.0f/255.0f), (29.0f/255.0f)},
                                   {DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/Lighter.bmp"),
                                    DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/Lighter2.bmp")},
                                   false, false, false, false, false, 0,
                                   {DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/ActionLighter.bmp"),
                                    DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/ActionLighter2.bmp")}};
        //DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/BrokenLighter.bmp")
        GameState->Obj[BALL] = {{34.5f, 35.5f}, {34.5f, 35.5f}, 0.5f, {0.25f, 0.5f, 0.0f},
                                {DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/Ball.bmp"),
                                 DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/Ball2.bmp")}};
        GameState->Obj[REMOTE] = {{30.9f, 6.3f}, {30.9f, 6.3f}, 0.5f, {0.8f, 0.8f, 0.8f},
                                  {DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/Remote.bmp"),
                                   DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/Remote2.bmp")},
                                  false, false, false, false, false, 0,{},                                  
                                  DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/BrokenRemote.bmp")};
        GameState->Obj[SHREDDER] = {{37.0f, 31.0f}, {37.0f, 31.0f}, 0.0f, {(200.0f/255.0f), (200.0f/255.0f), (200.0f/255.0f)},
                                    {DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/Shredder.bmp"),
                                     DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/Shredder.bmp")}
                                    , false, true, false, false, false, 0,
                                    {DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/ActionShredder.bmp"),
                                     DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/ActionShredder2.bmp")},
                                    DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/BrokenShredder.bmp")};
        GameState->Obj[PAPER] = {{11.5f, 31.1f}, {11.5f, 31.1f}, 0.5f, {},
                                 {DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/Paper.bmp"),
                                  DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/Paper2.bmp")}, true};
        GameState->Obj[TV_GUIDE] = {{37.0f, 6.5f}, {37.0f, 6.5f}, 0.5f, {},
                                    {DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/TvGuide.bmp"),
                                     DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/TvGuide2.bmp")}, true};
        
        GameState->Channel[0] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/Channel0.bmp");
        GameState->Channel[1] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/Channel1.bmp");
        GameState->Channel[2] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/Channel2.bmp");
        GameState->Channel[3] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/Channel3.bmp");
        GameState->Channel[4] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/Channel4.bmp");
        GameState->Channel[5] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/Channel5.bmp");
        GameState->Channel[6] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/Channel6.bmp");
        GameState->Channel[7] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/Channel7.bmp");
        

        
        GameState->TvScreen[0] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/TvScreen0.bmp");
        GameState->TvScreen[1] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/TvScreen1.bmp");
        GameState->TvScreen[2] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/TvScreen2.bmp");
        GameState->TvScreen[3] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/TvScreen3.bmp");
        GameState->TvScreen[4] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/TvScreen4.bmp");
        GameState->TvScreen[5] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/TvScreen5.bmp");
        GameState->TvScreen[6] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/TvScreen6.bmp");
        GameState->TvScreen[7] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/TvScreen7.bmp");
        
        GameState->TvGuideBMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/TvGuideScreen.bmp");
        
        

        for(uint32 ii = 1; ii < MAX_OBJECTS; ++ii)
        {
            GameState->Bob.ObjPos[ii] = GameState->Obj[ii].ProperPos;
            //GameState->Obj[ii].Possessed = true;
        }
                
        GameState->Bob.CurrentEvent = {{3,45,0},SLEEP,MEDIUM_PRIORITY,0,0,0,1};
        GameState->Bob.EventBuffer[0] = {{3,45,0},SLEEP,MEDIUM_PRIORITY,0,0,0,1};
        GameState->Bob.CurrentEventBufferI = 0;
        GameState->Bob.EventsBuffered = 1;
        

        GameState->Bob.FalseHub[0].Char = 'a';
        GameState->Bob.FalseHub[1].Char = 'z';
        //GameState->Bob.Speed = v2{0.0f, -0.5f};
        GameState->Bob.Direction = N;
        GameState->Bob.CurrentPathI = 0;        
        //GameState->Bob.NextHub = GameState->L0.Rooms[MAIN_ROOM].Hub[0];
        //GameState->Bob.EmptyPathHub = GameState->Bob.NextHub;
        //GameState->Bob.PriorHub = GameState->L0.Rooms[MAIN_ROOM].Hub[4];
        GameState->Bob.Pos = v2{17.5f, 28.5};;
        //GameState->Bob.IsMoving = true;
        //GameState->Bob.DetectsPlayer = false;
        
        //TODO: Make this a member of the npc struct
        GameState->PathEmpty = true;        

        //TODO: make this 30 meters to pixel, not 32??
        GameState->MetersToPixels = 30; // (Sixty pixels to a meter)
        //6 dots per meter
        //5 pixels per dot
        GameState->PlayerPos = v2{23.6f, 48.1f};
        GameState->SpiritMode = true;
        GameState->CurrentPlayerBitmap = GameState->PlayerSouthBitmap[0];
        
        Memory->IsInitialized = true;
    }
    //real32 PlayerRadius = 18.0f/32.0f;         //24/60 = 4/10  24/30 = 8/10  4/10 = x/32
    real32 PlayerRadius = 0.999f;


    bool32 DoLoad = false;
    bool32 DoLoad1 = false;
    bool32 DoLoad2 = false;
    bool32 DoLoad3 = false;
    bool32 DoLoad4 = false;

    bool32 ButtonPressed = false;
    
    
    //00A
    v2 PlayerSpeed = {};
    bool32 PlayerIsMoving = false;
    bool32 CheckN = false;
    bool32 CheckS = false;
    bool32 CheckE = false;
    bool32 CheckW = false;
    
    v2 NewPlayerPos = GameState->PlayerPos;
    for(int ControllerIndex = 0;
        ControllerIndex < 1/*TODO:ArrayCount(Input->Controllers)*/;
        ++ControllerIndex)
    {
        game_controller_input *Controller = GetController(Input, ControllerIndex);
        game_controller_input OldController = GameState->OldController;

        if(Controller->IsAnalog)
        {
            // NOTE(casey): Use analog movement tuning
        }        
        else if(Controller->IsConnected)
        {


            
            
            
            if(Controller->Select.EndedDown && !OldController.Select.EndedDown)
            {
                DEBUGSaveGameState(0, Memory->DEBUGPlatformWriteEntireFile, Memory);
            }
            if(Controller->Start.EndedDown && !OldController.Start.EndedDown)
            {
                //esc key                    
                DoLoad = true;
            }
            else if(Controller->Load1.EndedDown && !OldController.Load1.EndedDown)
            {
                DoLoad = true;
                DoLoad1 = true;
            }
            else if(Controller->Load2.EndedDown && !OldController.Load2.EndedDown)
            {
                DoLoad = true;
                DoLoad2 = true;
            }
            else if(Controller->Load3.EndedDown && !OldController.Load3.EndedDown)
            {
                DoLoad = true;
                DoLoad3 = true;
            }
            else if(Controller->Load4.EndedDown && !OldController.Load4.EndedDown)
            {
                DoLoad = true;
                DoLoad4 = true;
            }             

            
            if(GameState->ObjMode)
            {
                if(Controller->ActionUp.EndedDown && !OldController.ActionUp.EndedDown)
                {
                    //If W is pressed, set the Object that you were possessing to NOT broken.
                    GameState->Obj[GameState->ObjMode].Broken = false;                    
                    GameState->ObjMode = MIN_OBJECTS;
                    GameState->SpiritMode = true;
                    
                }
                else
                {
                    if(Controller->ActionDown.EndedDown && !OldController.ActionDown.EndedDown)
                    {
                        //If S is pressed
                        
                        if(GameState->Obj[GameState->ObjMode].Broken)
                        {
                            GameState->Obj[GameState->ObjMode].Broken = false;
                        }
                        else
                        {
                            GameState->Obj[GameState->ObjMode].Broken  = true;
                        }
                    }
                    else if(Controller->ActionLeft.EndedDown && !OldController.ActionLeft.EndedDown)
                    {
                        //If A is pressed
                        //Actions happen                        
                        GameState->Obj[GameState->ObjMode].Broken = false;
                        
                                                
                        //TODO: fill this in.
                        switch(GameState->ObjMode)
                        {
                            case LETTER:
                            {
                                //Letter goes full screen
                            }break;
                            case LIGHTER:
                            {
                                //Check to see whether LETTER, (NPC) is within close proximity.
                                //If it is, burn it.                                
                                GameState->Obj[LIGHTER].AnimationTimer = 5;                                
                                for(uint32 OI = 1; OI < MAX_OBJECTS; ++OI)
                                {
                                    if(GameState->Obj[OI].Burnable && !GameState->Obj[OI].Possessed &&                                       
                                       (AbsDiff(GameState->Obj[OI].Pos.X, GameState->PlayerPos.X) < 1.9f) &&
                                       (AbsDiff(GameState->Obj[OI].Pos.Y, GameState->PlayerPos.Y) < 1.9f) &&
                                       SelectItemCheck(GameState->Obj[OI].Pos, GameState->PlayerPos, (uint32 *)GameState->AptTileMap))
                                    {
                                        GameState->Obj[OI].Possessed = true;
                                        GameState->Obj[OI].Pos = v2{41.5f, 37.5f};                                                       
                                    }                                    
                                }                                                                                                                                                                        
                            }break;                                                        
                            case SHREDDER:
                            {
                                GameState->Obj[SHREDDER].AnimationTimer = 30;
                                
                                for(uint32 OI = 1; OI < MAX_OBJECTS; ++OI)
                                {
                                    if(GameState->Obj[OI].Burnable && !GameState->Obj[OI].Possessed &&                                       
                                       (AbsDiff(GameState->Obj[OI].Pos.X, GameState->Obj[SHREDDER].Pos.X) < 1.9f) &&
                                       (AbsDiff(GameState->Obj[OI].Pos.Y, GameState->Obj[SHREDDER].Pos.Y) < 1.9f) &&
                                       SelectItemCheck(GameState->Obj[OI].Pos, GameState->Obj[SHREDDER].Pos, (uint32 *)GameState->AptTileMap))
                                    {
                                        GameState->Obj[OI].Possessed = true;
                                        GameState->Obj[OI].Pos = v2{41.5f, 37.5f};                                                       
                                    }
                                    
                                }
                            }break;
                            case TV_GUIDE:
                            {
                                
                                if(GameState->ShowTvGuide)
                                {
                                    GameState->ShowTvGuide = false;
                                }
                                else
                                {
                                    GameState->ShowTvGuide = true;
                                }
                                
                            }break;
                            case REMOTE:
                            {
                                if(GetRoom(&GameState->L0, GameState->PlayerPos) == TV_ROOM)
                                {
                                    GameState->ChannelTimer = 120;                                
                                    if(++GameState->ChannelI > 7)
                                    {
                                        GameState->ChannelI = 0;
                                    
                                    }
                                }              
                            }break;
                            /*case DOOR:
                              {
                                
                              }break;*/
                        };
                        
                    }

                    NewPlayerPos = GameState->Obj[GameState->ObjMode].Pos;
                }
                
                //NOTE: Would this variable be changed later down the line if the obj is possessed by a moving npc?
                                                
            }
            else
            {
                                                                      
                if(Controller->MoveUp.EndedDown)
                {
                    PlayerIsMoving = true;
                    CheckN = true;
                    GameState->FacingDirection = N;
                    PlayerSpeed.Y = 1.0f;
                }
                if(Controller->MoveDown.EndedDown)
                {
                    PlayerIsMoving = true;
                    CheckN = false;
                    CheckS = true;
                    GameState->FacingDirection = S;
                    PlayerSpeed.Y = -1.0f;
                }
                if(Controller->MoveLeft.EndedDown)
                {
                    PlayerIsMoving = true;
                    CheckW = true;
                    GameState->FacingDirection = W;
                    PlayerSpeed.X = -1.0f;
                }
                if(Controller->MoveRight.EndedDown)
                {
                    PlayerIsMoving = true;
                    CheckW = false;
                    CheckE = true;
                    GameState->FacingDirection = E;
                    PlayerSpeed.X = 1.0f;
                }            
                if(PlayerSpeed.X && PlayerSpeed.Y)
                {                        
                    PlayerSpeed.X *= 0.7071067811865475f;
                }
                    
                if(PlayerIsMoving)
                {
                    GameState->SelectedObjI = 0;
                }
                    
                real32 SpeedMultiplier = 6;
                if(GameState->SpiritMode &&
                   (GameState->AptTileMap[(int32)GameState->PlayerPos.Y][(int32)GameState->PlayerPos.X] == 1))
                {
                    SpeedMultiplier /= 2;
                }
                                    
                //TODO: Don't increment the coordinates just yet. You need to first check whether
                // you are bumping into an object at your current pace.
                NewPlayerPos = GameState->PlayerPos + (PlayerSpeed*SpeedMultiplier*Input->dtForFrame);   
                                                
                if(Controller->LeftShoulder.EndedDown && !OldController.LeftShoulder.EndedDown)
                {//if the scroll nearby objs button is pressed
                    
                    if(GameState->SelectedObjI)
                    {//and you were already scrolling them
                        
                        //Scroll to the next nearby obj
                        if(GameState->NearbyObj[++GameState->NearbyObjI] == 0)
                        {
                            GameState->NearbyObjI = 0;
                        }
                        
                        GameState->SelectedObjI = GameState->NearbyObj[GameState->NearbyObjI];
                    }
                    else
                    {//and you weren't already scrolling them

                        //Reset The count of Nearby Objects
                        GameState->NearbyObjCount = 0;
                        
                        bool32 ObjIsPresent = false;
                    
                        for(uint32 OI = MIN_OBJECTS+1; OI < MAX_OBJECTS; ++OI)
                        {//Loop through all of the objects in the level, adding all those within 2 feet to the uint32 array of nearby objects
                        
                            if(!GameState->Obj[OI].Possessed)
                            {
                                v2 OToPlayer = {};                            
                                OToPlayer.X = AbsDiff(GameState->PlayerPos.X, GameState->Obj[OI].Pos.X);
                                OToPlayer.Y = AbsDiff(GameState->PlayerPos.Y, GameState->Obj[OI].Pos.Y);
                            
                                if(OToPlayer.X < 2.3f && OToPlayer.Y < 2.3f &&
                                   //
                                   SelectItemCheck(GameState->PlayerPos, GameState->Obj[OI].Pos, (uint32 *)GameState->AptTileMap))
                                {
                                    
                                    if(!ObjIsPresent)
                                    {
                                        ObjIsPresent = true;
                                    }                                
                                    GameState->NearbyObj[GameState->NearbyObjCount] = OI;
                                    ++GameState->NearbyObjCount;                                    
                                }
                            } 
                        }                    
                        GameState->NearbyObj[GameState->NearbyObjCount] = 0;
                        GameState->NearbyObjI = 0;
                    
                        if(ObjIsPresent) 
                        {                            
                            GameState->SelectedObjI = GameState->NearbyObj[0];
                        }                        
                    }
                }

                if(Controller->RightShoulder.EndedDown && !OldController.RightShoulder.EndedDown && GameState->PlayerInventoryAmount)
                {//If the scroll inventory button is pressed and there are objects in the inventory

                    
                    if(++GameState->PlayerInventoryI > GameState->PlayerInventoryAmount)
                    {
                        GameState->PlayerInventoryI = 0;
                    }                    
                   
                }
                   

                if(GameState->SelectedObjI)
                {
                    if(Controller->ActionRight.EndedDown && !OldController.ActionRight.EndedDown && !GameState->SpiritMode)
                    {//...and the D key is hit, the selected obj is added to the player's inventory,
                        //and the player stops selecting objects

                        for(uint32 II = 0; II < ArrayCount(GameState->PlayerInventory); ++II)
                        {
                            if(!GameState->Obj[GameState->SelectedObjI].Uncarryable && GameState->PlayerInventory[II] == MIN_OBJECTS)
                            {
                                GameState->Obj[GameState->SelectedObjI].Possessed = true;
                                GameState->PlayerInventory[II] = GameState->SelectedObjI;
                                ++GameState->PlayerInventoryAmount;                                
                                GameState->SelectedObjI = 0;
                                break;
                            }
                        }                        
                        
                    }
                    else if(Controller->ActionUp.EndedDown && !OldController.ActionUp.EndedDown)
                    {//...Player goes into the object
                        //TODO drop all items in inventory
                        GameState->ObjMode = GameState->SelectedObjI;
                        NewPlayerPos = GameState->Obj[GameState->SelectedObjI].Pos;
                        GameState->SelectedObjI = 0;
                        for(uint32 II = 0; II < GameState->PlayerInventoryAmount; ++II)
                        {
                            
                            GameState->Obj[GameState->PlayerInventory[II]].Possessed = false;
                            GameState->Obj[GameState->PlayerInventory[II]].Pos = v2{(uint32)GameState->PlayerPos.X + 0.5f, (uint32)GameState->PlayerPos.Y + 0.5f};
                            GameState->Obj[GameState->PlayerInventory[II]].Broken = false;
                            //TODO am I even using this?
                            GameState->Obj[GameState->PlayerInventory[II]].Equipped = false;
                            GameState->Obj[GameState->PlayerInventory[II]].AnimationTimer = 0;
                            GameState->PlayerInventory[II] = 0;
                            
                            
                        }
                        GameState->PlayerInventoryAmount = 0;
                        GameState->PlayerInventoryI = 0;
                    }
                    else if(Controller->ActionLeft.EndedDown && !OldController.ActionLeft.EndedDown && !GameState->SpiritMode)
                    {//...Player causes the object to perform its action.

                        switch(GameState->SelectedObjI)
                        {
                            case LETTER:
                            {
                                //Letter goes full screen
                            }break;
                            case LIGHTER:
                            {
                                //Check to see whether LETTER, (NPC) is within close proximity.
                                //If it is, burn it.

                                GameState->Obj[LIGHTER].AnimationTimer = 5;
                                
                                for(uint32 OI = 1; OI < MAX_OBJECTS; ++OI)
                                {
                                    if(GameState->Obj[OI].Burnable && !GameState->Obj[OI].Possessed &&                                       
                                       (AbsDiff(GameState->Obj[OI].Pos.X, GameState->Obj[LIGHTER].Pos.X) < 1.9f) &&
                                       (AbsDiff(GameState->Obj[OI].Pos.Y, GameState->Obj[LIGHTER].Pos.Y) < 1.9f) &&
                                       SelectItemCheck(GameState->Obj[OI].Pos, GameState->Obj[LIGHTER].Pos, (uint32 *)GameState->AptTileMap))
                                    {
                                        GameState->Obj[OI].Possessed = true;
                                        GameState->Obj[OI].Pos = v2{41.5f, 37.5f};                                                       
                                    }
                                    
                                }
                                //Play lighter animation and possibly burning paper animation.
                                
                            }break;                            
                            case REMOTE:
                            {
                                if(GetRoom(&GameState->L0, GameState->PlayerPos) == TV_ROOM)
                                {
                                    GameState->ChannelTimer = 120;                                
                                    if(++GameState->ChannelI > 7)
                                    {
                                        GameState->ChannelI = 0;
                                    
                                    }
                                }
                            }break;
                            case SHREDDER:
                            {
                                GameState->Obj[SHREDDER].AnimationTimer = 30;
                                
                                for(uint32 OI = 1; OI < MAX_OBJECTS; ++OI)
                                {
                                    if(GameState->Obj[OI].Burnable && !GameState->Obj[OI].Possessed &&                                       
                                       (AbsDiff(GameState->Obj[OI].Pos.X, GameState->Obj[SHREDDER].Pos.X) < 1.9f) &&
                                       (AbsDiff(GameState->Obj[OI].Pos.Y, GameState->Obj[SHREDDER].Pos.Y) < 1.9f) &&
                                       SelectItemCheck(GameState->Obj[OI].Pos, GameState->Obj[SHREDDER].Pos, (uint32 *)GameState->AptTileMap))
                                    {
                                        GameState->Obj[OI].Possessed = true;
                                        GameState->Obj[OI].Pos = v2{41.5f, 37.5f};                                                       
                                    }
                                    
                                }
                            }break;
                            case TV_GUIDE:
                            {
                                
                                if(GameState->ShowTvGuide)
                                {
                                    GameState->ShowTvGuide = false;
                                }
                                else
                                {
                                    GameState->ShowTvGuide = true;
                                }
                                 
                            }break;
                            /*case DOOR:
                              {
                                
                              }break;*/
                        };
                    }
                }
                else if(GameState->PlayerInventory[GameState->PlayerInventoryI])
                {
                    if(Controller->ActionRight.EndedDown && !OldController.ActionRight.EndedDown && GameState->PlayerInventoryAmount && GameState->PlayerInventory[GameState->PlayerInventoryI])
                    {
                        bool32 ItDropped = false;

                        //ghost.cpp
                        ItDropped = DropObj(GameState->PlayerPos, GameState->FacingDirection, GameState->PlayerInventory[GameState->PlayerInventoryI], GameState->Obj, (uint32 *)GameState->AptTileMap);
                        if(!ItDropped)
                        {
                            Assert(0);
                            //Make noise?
                        }
                        else
                        {
                            GameState->PlayerInventory[GameState->PlayerInventoryI] = 0;
                            
                            if(GameState->PlayerInventoryI != GameState->PlayerInventoryAmount)
                            {
                                for(uint32 II = GameState->PlayerInventoryI+1; II < GameState->PlayerInventoryAmount; ++II)
                                {
                                    GameState->PlayerInventory[II-1] = GameState->PlayerInventory[II];
                                    GameState->PlayerInventory[II] = 0;
                                }
                                
                            }
                            
                            --GameState->PlayerInventoryAmount;                            
                            GameState->PlayerInventoryI = 0;
                            
                        }
                        
                    }
                    else if(Controller->ActionUp.EndedDown && !OldController.ActionUp.EndedDown)
                    {//...Player goes into the object
                        
                        GameState->ObjMode = GameState->PlayerInventory[GameState->PlayerInventoryI];
                        for(uint32 II = 0; II < GameState->PlayerInventoryAmount; ++II)
                        {
                            
                            GameState->Obj[GameState->PlayerInventory[II]].Possessed = false;
                            GameState->Obj[GameState->PlayerInventory[II]].Pos = v2{(uint32)GameState->PlayerPos.X + 0.5f, (uint32)GameState->PlayerPos.Y + 0.5f};
                            GameState->Obj[GameState->PlayerInventory[II]].Broken = false;
                            //TODO am I even using this?
                            GameState->Obj[GameState->PlayerInventory[II]].Equipped = false;
                            GameState->Obj[GameState->PlayerInventory[II]].AnimationTimer = 0;
                            GameState->PlayerInventory[II] = 0;
                            
                            
                        }
                        GameState->PlayerInventoryAmount = 0;
                        GameState->PlayerInventoryI = 0;
                        
                    }
                    else if(Controller->ActionLeft.EndedDown && !OldController.ActionLeft.EndedDown && !GameState->SpiritMode)
                    {//...Player causes the object to perform its action.

                        switch(GameState->PlayerInventory[GameState->PlayerInventoryI])
                        {
                            /*case LETTER:
                              {
                              //Letter goes full screen
                              }break;*/
                            case LIGHTER:
                            {
                                //Check to see whether LETTER, (NPC) is within close proximity.
                                //If it is, burn it.

                                GameState->Obj[LIGHTER].AnimationTimer = 5;
                                
                                for(uint32 OI = 1; OI < MAX_OBJECTS; ++OI)
                                {
                                    if(GameState->Obj[OI].Burnable && !GameState->Obj[OI].Possessed &&                                       
                                       (AbsDiff(GameState->Obj[OI].Pos.X, GameState->PlayerPos.X) < 1.9f) &&
                                       (AbsDiff(GameState->Obj[OI].Pos.Y, GameState->PlayerPos.Y) < 1.9f) &&
                                       SelectItemCheck(GameState->Obj[OI].Pos, GameState->PlayerPos, (uint32 *)GameState->AptTileMap))
                                    {
                                        GameState->Obj[OI].Possessed = true;
                                        GameState->Obj[OI].Pos = v2{41.5f, 37.5f};                                                       
                                    }
                                    
                                }
                                //Play lighter animation and possibly burning paper animation.                                
                            }break;                            
                            case REMOTE:
                            {
                                if(GetRoom(&GameState->L0, GameState->PlayerPos) == TV_ROOM)
                                {
                                    GameState->ChannelTimer = 120;                                
                                    if(++GameState->ChannelI > 7)
                                    {
                                        GameState->ChannelI = 0;
                                    
                                    }
                                }
                            }break;                            
                            case TV_GUIDE:
                            {
                                
                                if(GameState->ShowTvGuide)
                                {
                                    GameState->ShowTvGuide = false;
                                }
                                else
                                {
                                    GameState->ShowTvGuide = true;
                                }
                                                                
                            }break;
                            /*case DOOR:
                              {
                                
                              }break;*/
                        };
                    }                                            
                }

                if(Controller->Back.EndedDown && !OldController.Back.EndedDown)
                {
                    //TODO: add player radius in this calculation?
                    uint32 TileID = GameState->AptTileMap[(uint32)GameState->PlayerPos.Y][(uint32) GameState->PlayerPos.X];
                    if(!GameState->SpiritMode)
                    {
                        GameState->SpiritMode = true;
                        for(uint32 II = 0; II < GameState->PlayerInventoryAmount; ++II)
                        {
                            
                            GameState->Obj[GameState->PlayerInventory[II]].Possessed = false;
                            GameState->Obj[GameState->PlayerInventory[II]].Pos = v2{(uint32)GameState->PlayerPos.X + 0.5f, (uint32)GameState->PlayerPos.Y + 0.5f};
                            GameState->Obj[GameState->PlayerInventory[II]].Broken = false;
                            //TODO am I even using this?
                            GameState->Obj[GameState->PlayerInventory[II]].Equipped = false;
                            GameState->Obj[GameState->PlayerInventory[II]].AnimationTimer = 0;
                            GameState->PlayerInventory[II] = 0;
                            
                            
                        }
                        GameState->PlayerInventoryAmount = 0;
                        GameState->PlayerInventoryI = 0;
                    }
                    else if(TileID == FLOOR_TILE || TileID == HUB_TILE || TileID == DOOR_TILE)
                    {
                        GameState->SpiritMode = false;
                    }                        
                }                    
            }

            if(GameState->ShowTvGuide && !Controller->ActionLeft.EndedDown)
            {
                for(uint32 ButtI = 0; ButtI < ArrayCount(Input->Controllers[0].Buttons); ++ButtI)
                {
                    if(Controller->Buttons[ButtI].EndedDown)
                    {
                        GameState->ShowTvGuide = false;
                    }
                }
            }
            
            
            GameState->OldController = *Controller;
        }
    }
            



    if(GameState->DeathScreen ||       
       (!GameState->Obj[LETTER].Possessed &&
        (uint32)GameState->Obj[LETTER].Pos.Y == 42 &&
        ((uint32)GameState->Obj[LETTER].Pos.X == 27 ||
         (uint32)GameState->Obj[LETTER].Pos.X == 27)))
    {

        if(++GameState->ObjBlinkTimer > 254)
        {
            GameState->ObjBlinkTimer = 0;
        }
        real32 Gray = AbsDiff((real32)GameState->ObjBlinkTimer,127.0f)/254.0f;
        
        rgb Color = {Gray,Gray,Gray};
        
        DrawRectangle(Buffer,
                      v2{0.0f,0.0f},
                      v2{(real32)Buffer->Width, (real32)Buffer->Height},
                      Color);

        DrawBitmap(Buffer,
                   &GameState->QuestionMark,
                   (real32)Buffer->Width/2,
                   (real32)Buffer->Height/2);
      

    }
    else
    {
        
    
    //TODO: Functionalize the entire Collision code so you can pass in different TileIDs
    // to test for collision with (e.g. outer walls with value 2 vs. inner walls with value 1

    
    if(!GameState->ObjMode && PlayerIsMoving)
    {
            
        if(PlayerSpeed.X && PlayerSpeed.Y)
        {
                            
            v2 Point[5] = {};
            v2 CornerPos = {};
            bool32 IsColliding = false;
            bool32 NSColliding = false;
            bool32 EWColliding = false;
            bool32 HeadedE = false;
            
            for(uint32 PI = 0; PI < 5; ++PI)
            {
                Point[PI] = NewPlayerPos;
            }

            //IMPORTANT: Point[0] thru Point[2] are the N/S edge and corner,
            //Point[3] thru Point[4] are the E/W edge, 
                                
            if(CheckN)
            {
                Point[0].X -= PlayerRadius;
                Point[0].Y += PlayerRadius/2;
                Point[1].Y += PlayerRadius/2;
                Point[2].X += PlayerRadius;
                Point[2].Y += PlayerRadius/2;
                Point[4].Y -= PlayerRadius/2;
                    
                    
                if(CheckE)
                {//NE                                              
                    Point[3].X += PlayerRadius;                        
                    Point[4].X += PlayerRadius;
                    HeadedE = true;

                    if((NewPlayerPos.Y - (uint32)NewPlayerPos.Y) > (NewPlayerPos.X - (uint32)NewPlayerPos.X))
                    {
                        CornerPos.Y = NewPlayerPos.Y;
                        CornerPos.X = (uint32)NewPlayerPos.X - .005f;
                    }
                    else
                    {
                        CornerPos.X = NewPlayerPos.X;
                        CornerPos.Y = (uint32)NewPlayerPos.Y - .005f;
                    }
                }
                else
                {//NW                        
                    Point[3].X -= PlayerRadius;                        
                    Point[4].X -= PlayerRadius;

                    if((NewPlayerPos.Y - (uint32)NewPlayerPos.Y) > ((uint32)(NewPlayerPos.X+1) - NewPlayerPos.X))
                    {
                        CornerPos.Y = NewPlayerPos.Y;
                        CornerPos.X = ((uint32)NewPlayerPos.X + 1) + .005f;
                    }
                    else
                    {
                        CornerPos.X = NewPlayerPos.X;
                        CornerPos.Y = (uint32)NewPlayerPos.Y - .005f;
                    }
                }
            }
            else
            {
                Point[0].X -= PlayerRadius;
                Point[0].Y -= PlayerRadius/2;
                Point[1].Y -= PlayerRadius/2;
                Point[2].X += PlayerRadius;
                Point[2].Y -= PlayerRadius/2;
                Point[3].Y += PlayerRadius/2;
                    
                if(CheckE)
                {//SE
                    HeadedE = true;
                    Point[3].X += PlayerRadius;                        
                    Point[4].X += PlayerRadius;

                    if((((uint32)NewPlayerPos.Y + 1) - NewPlayerPos.Y) > (NewPlayerPos.X - (uint32)NewPlayerPos.X))
                    {
                        CornerPos.Y = NewPlayerPos.Y;
                        CornerPos.X = ((uint32)NewPlayerPos.X) - .005f;
                    }
                    else
                    {
                        CornerPos.X = NewPlayerPos.X;
                        CornerPos.Y = (uint32)(NewPlayerPos.Y+1) + .005f;
                    }
                }
                else
                {//SW
                    Point[3].X -= PlayerRadius;                        
                    Point[4].X -= PlayerRadius;

                    if((((uint32)NewPlayerPos.Y + 1) - NewPlayerPos.Y) > ((uint32)(NewPlayerPos.X + 1) - NewPlayerPos.X))
                    {
                        CornerPos.Y = NewPlayerPos.Y;
                        CornerPos.X = ((uint32)NewPlayerPos.X+1) + .005f;
                    }
                    else
                    {
                        CornerPos.X = NewPlayerPos.X;
                        CornerPos.Y = (uint32)(NewPlayerPos.Y+1) + .005f;
                    }
                }
            }
            
            if(!GameState->SpiritMode)
            {
                for(uint32 PI = 0; PI < 5; ++PI)
                {
                    if(IsObstacle(GameState->AptTileMap[(uint32)Point[PI].Y][(uint32)Point[PI].X]))
                    {                    
                        IsColliding = true;

                        if(HeadedE)
                        {                            
                            if(PI == 0 || PI == 1)
                            {
                                NSColliding = true;
                            }                        
                            else if(PI > 2)
                            {
                                EWColliding = true;
                            }
                        }                        
                        else
                        {
                            if(PI == 1 || PI == 2)
                            {
                                NSColliding = true;
                            }
                        
                            else if(PI > 2)
                            {
                                EWColliding = true;
                            }
                        }
                    }
                }
            }
            
            else
            {
                for(uint32 PI = 0; PI < 5; ++PI)
                {
                    if((GameState->AptTileMap[(uint32)Point[PI].Y][(uint32)Point[PI].X] == OUTER_WALL_TILE) ||
                       (GameState->AptTileMap[(uint32)Point[PI].Y][(uint32)Point[PI].X] == OUTER_DOOR_TILE))
                    {
                        IsColliding = true;
                        
                        if(HeadedE)
                        {                            
                            if(PI == 0 || PI == 1)
                            {
                                NSColliding = true;
                            }                        
                            else if(PI > 2)
                            {
                                EWColliding = true;
                            }
                        }                        
                        else
                        {
                            if(PI == 1 || PI == 2)
                            {
                                NSColliding = true;
                            }
                        
                            else if(PI > 2)
                            {
                                EWColliding = true;
                            }
                        }
                    }                
                }
            }
    
            
            if(IsColliding)
            {
                if(NSColliding && !EWColliding)
                {
                    GameState->PlayerPos.X = NewPlayerPos.X;                            
                }
                else if(EWColliding && !NSColliding)
                {
                    GameState->PlayerPos.Y = NewPlayerPos.Y;
                }
                else if(!EWColliding && !NSColliding)
                {
                    GameState->PlayerPos = CornerPos;                        
                }
                        
            }                
            else
            {
                GameState->PlayerPos = NewPlayerPos;
            }
                
        }
        else
        {
            bool32 IsColliding = false;
            v2 Point[3] = {};

            for(uint32 PI = 0; PI < 3; ++PI)
            {
                Point[PI] = NewPlayerPos;
            }
                    
                
            if(CheckN)
            {

                Point[0].X -= PlayerRadius;
                Point[0].Y += PlayerRadius/2;
                Point[1].Y += PlayerRadius/2;
                Point[2].X += PlayerRadius;
                Point[2].Y += PlayerRadius/2;
                    
            }
            else if(CheckS)
            {
                Point[0].X -= PlayerRadius;
                Point[0].Y -= PlayerRadius/2;
                Point[1].Y -= PlayerRadius/2;
                Point[2].X += PlayerRadius;
                Point[2].Y -= PlayerRadius/2;                    
            }
            else if(CheckE)
            {
                Point[0].X += PlayerRadius;
                Point[0].Y += PlayerRadius/2;
                Point[1].X += PlayerRadius;
                Point[2].X += PlayerRadius;
                Point[2].Y -= PlayerRadius/2;
            }
            else
            {
                Point[0].X -= PlayerRadius;
                Point[0].Y += PlayerRadius/2;
                Point[1].X -= PlayerRadius;
                Point[2].X -= PlayerRadius;
                Point[2].Y -= PlayerRadius/2;
            }

            if(!GameState->SpiritMode)
            {
                for(uint32 PI = 0; PI < 3; ++PI)
                {
                
                    if(IsObstacle(GameState->AptTileMap[(uint32)Point[PI].Y][(uint32)Point[PI].X]))
                    {
                        IsColliding = true;
                        break;
                    }   
                }
            }
            else if(GameState->SpiritMode)
            {
                for(uint32 PI = 0; PI < 3; ++PI)
                {
                    if((GameState->AptTileMap[(uint32)Point[PI].Y][(uint32)Point[PI].X] == OUTER_DOOR_TILE) ||
                       (GameState->AptTileMap[(uint32)Point[PI].Y][(uint32)Point[PI].X] == OUTER_WALL_TILE))
                    {
                        IsColliding = true;
                        break;
                    }
                }
            }                        
            if(!IsColliding)
            {
                GameState->PlayerPos = NewPlayerPos;
            }
        }
        
    }
    else
    {
        GameState->PlayerPos = NewPlayerPos;
    }


    //Check to see if events were triggered / had their priorites altered
    
    //Check to see which event has top priority and make that the current event
    
    //Check to see if NPC is arrived at NextEventDest (if not, keep walking bucko)
    //or if npc is done with an action or animation.
    
    //If so, check to see whether an(other) action is performed at that dest
    // and/or check to see whether this is a multidest/pathed event
    
    // If so, do action/ head to next dest.
    
    //When action is done, set flag. When Dest is arrived at, set flag,
    //When event is done (either completed or terminated by a time window), set flag and
    //removed that Event from the EventBuffer, and possibly reset the event data.
    
       
    //017 NPC Bob's schedule
    uint32 NewEventI = 0;
    uint32 NewEventID[10];
    bool32 OnTheDot = (GameState->Clock.Min == 0 && GameState->Clock.Sec == 0);    
    if(GameState->Clock.Hr == 6 && OnTheDot)
    {
        
        NewEventID[NewEventI++] = BATHROOM;
        GameState->Bob.GotNewEvent = true;
    }
    else if(GameState->Clock.Hr == 6 &&
            GameState->Clock.Min == 30 &&
            GameState->Clock.Sec == 0)
    {

        NewEventID[NewEventI++] = WRITE_LETTER;
        GameState->Bob.GotNewEvent = true;
    }
    else if(GameState->Clock.Hr == 8 &&
            GameState->Clock.Min == 30 &&
            GameState->Clock.Sec == 0)
    {
        NewEventID[NewEventI++] = WORK;
        GameState->Bob.GotNewEvent = true;
    }
    else if(GameState->Clock.Hr == 10 &&
            GameState->Clock.Min == 0 &&
            GameState->Clock.Sec == 0)
    {
        NewEventID[NewEventI++] = PING_PONG;
        GameState->Bob.GotNewEvent = true;
    }
    else if(GameState->Clock.Hr == 11 &&
            GameState->Clock.Min == 15 &&
            GameState->Clock.Sec == 0)
    {
        
        NewEventID[NewEventI++] = WRITE_LETTER;
        GameState->Bob.GotNewEvent = true;
    }
    else if(GameState->Clock.Hr == 13 &&
            GameState->Clock.Min == 00 &&
            GameState->Clock.Sec == 0)
    {
        NewEventID[NewEventI++] = WORK;
        GameState->Bob.GotNewEvent = true;
    }
    else if(GameState->Clock.Hr == 14 &&
            GameState->Clock.Min == 15 &&
            GameState->Clock.Sec == 0)
    {
                
        NewEventID[NewEventI++] = PING_PONG;
        GameState->Bob.GotNewEvent = true;
    }
    else if(GameState->Clock.Hr == 15 &&
            GameState->Clock.Min == 0 &&
            GameState->Clock.Sec == 0)
    {
        NewEventID[NewEventI++] = WRITE_LETTER;
        GameState->Bob.GotNewEvent = true;
    }
    else if(GameState->Clock.Hr == 16 &&
            GameState->Clock.Min == 30 &&
            GameState->Clock.Sec == 0)
    {
        NewEventID[NewEventI++] = EAT;
        GameState->Bob.GotNewEvent = true;
    }
    else if(GameState->Clock.Hr == 17 &&
            GameState->Clock.Min == 45 &&
            GameState->Clock.Sec == 0)
    {
        NewEventID[NewEventI++] = TV;
        GameState->Bob.GotNewEvent = true;
    }
    else if(GameState->Clock.Hr == 19 &&
            GameState->Clock.Min == 0 &&
            GameState->Clock.Sec == 0)
    {
        NewEventID[NewEventI++] = WRITE_LETTER;
        GameState->Bob.GotNewEvent = true;
    }
    else if(GameState->Clock.Hr == 21 &&
            GameState->Clock.Min == 15 &&
            GameState->Clock.Sec == 0)
    {
        NewEventID[NewEventI++] = PING_PONG;
        GameState->Bob.GotNewEvent = true;
    }
    else if(GameState->Clock.Hr == 22 &&
            GameState->Clock.Min == 30 &&
            GameState->Clock.Sec == 0)
    {
        NewEventID[NewEventI++] = TV;
        GameState->Bob.GotNewEvent = true;
    }
    else if(GameState->Clock.Hr == 0 &&
            GameState->Clock.Min == 0 &&
            GameState->Clock.Sec == 0)
    {
        NewEventID[NewEventI++] = SLEEP;
        GameState->Bob.GotNewEvent = true;
    }

   
    if(GameState->Bob.CurrentEvent.ID == TV && GameState->Bob.IsInspired)
    {
        for(uint32 BuffI = 0; BuffI < ArrayCount(GameState->Bob.EventBuffer); ++BuffI)
        {
            if(GameState->Bob.EventBuffer[BuffI].ID == WRITE_LETTER)
            {
                GameState->Bob.EventBuffer[BuffI].Complete = true;
            }
        }        
        NewEventID[NewEventI++] = WRITE_LETTER;
        GameState->Bob.GotNewEvent = true;
        GameState->Bob.CurrentEvent.Complete;
    }

    bool32 NewPhase = false;
    
    //NOTE:Look to see if CurrentEvent has been completed. If so, get a new currentevent
    event EmptyEvent = {};
    if(GameState->Bob.CurrentEvent.Complete)
    {                
        uint32 HighestEventPriority = 0;
        uint32 HighestBufferI = 0;
        //uint32 CompleteCurrentBufferI = 0;
        bool32 FoundNewCurrentEvent = false;
        for(uint32 BufferI = 0; BufferI < ArrayCount(GameState->Bob.EventBuffer); ++BufferI)
        {
            if(GameState->Bob.CurrentEvent.ID == GameState->Bob.EventBuffer[BufferI].ID)
            {
                
                GameState->Bob.EventBuffer[BufferI].Complete = true;
                //CompleteCurrentBufferI = BufferI;
            }            
            else if(!GameState->Bob.EventBuffer[BufferI].Complete &&
                    !GameState->Bob.EventBuffer[BufferI].CouldComplete &&
                    GameState->Bob.EventBuffer[BufferI].Priority > HighestEventPriority)                
            {
                HighestEventPriority = GameState->Bob.EventBuffer[BufferI].Priority;
                HighestBufferI = BufferI;
                FoundNewCurrentEvent = true;
            }                    
        }
     
        NewPhase = true;
        
        if(FoundNewCurrentEvent)
        {
            GameState->Bob.CurrentEventBufferI = HighestBufferI;
            GameState->Bob.CurrentEvent = GameState->Bob.EventBuffer[GameState->Bob.CurrentEventBufferI];
            
        }
        else
        {
            GameState->Bob.CurrentEvent = EmptyEvent;
        }
                
    }
    
    //NOTE:Drop any events from the event buffer which have (.Complete    
    for(uint32 BuffI = 0; BuffI < ArrayCount(GameState->Bob.EventBuffer); ++BuffI)
    {
        //TODO: Eventually I just want this to be a .Complete if gate, having handled
        // all of the other completeness indicators elsewhere. This looks so darn messy right
        // now!        
        if(GameState->Bob.EventBuffer[BuffI].Complete ||
           (GameState->Bob.EventBuffer[BuffI].CouldComplete && GameState->Bob.CurrentEventBufferI != BuffI))
        {            
            GameState->Bob.EventBuffer[BuffI] = EmptyEvent;
            --GameState->Bob.EventsBuffered;
        }
    }
             
    //016
    //Find facing direction    
    real32 SpeedMagnX = GameState->Bob.Speed.X;
    real32 SpeedMagnY = GameState->Bob.Speed.Y;

    v2 EyeOffset = {};
    if(!(SpeedMagnX == 0.0f) || !(SpeedMagnY == 0.0f))
    {
        if(SpeedMagnX < 0.0f)
        {
            SpeedMagnX *= -1;
        }
        if(SpeedMagnY < 0.0f)
        {
            SpeedMagnY *= -1;
        }
        if(SpeedMagnX >= SpeedMagnY)
        {
            if(GameState->Bob.Speed.X >= 0)
            {                
                GameState->Bob.Direction = E;
            }
            else
            {
                GameState->Bob.Direction = W;
            }
        }
        else
        {
            if(GameState->Bob.Speed.Y >= 0)
            {
                GameState->Bob.Direction = N;
            }
            else
            {
                GameState->Bob.Direction = S;
            }
        }
    }
    
    switch(GameState->Bob.Direction)
    {
        case N : EyeOffset.Y = PlayerRadius/2; break;
        case S : EyeOffset.Y = -PlayerRadius/2; break;
        case E : EyeOffset.X = PlayerRadius; break;
        case W : EyeOffset.X = -PlayerRadius; break;
        default : EyeOffset.X = 0; break;
    };
    
    //NPC Checks for Player

    real32 EyeWidth = 7.0f;
    real32 EyeDepth = 10.0f;    
    v2 BobEyesPos = {GameState->Bob.Pos.X + EyeOffset.X, GameState->Bob.Pos.Y + EyeOffset.Y};
    //TODO: Does Detects player need to be a persistant variable?
    GameState->Bob.DetectsPlayer = false;
    if(!GameState->SpiritMode && !GameState->ObjMode)
    {
        //ghost_path.h
        GameState->Bob.DetectsPlayer = EntityCheck(BobEyesPos,
                                                   EyeWidth, EyeDepth,
                                                   GameState->Bob.Direction,
                                                   GameState->PlayerPos,
                                                   v2{PlayerRadius-0.2f,PlayerRadius/2-0.2f},
                                                   (uint32 *)GameState->AptTileMap);
        
    }
    
    //NPC Checks for appropriate objects
    for(uint32 ObjI = 1; ObjI < ArrayCount(GameState->Obj); ++ObjI)
    {
        //If NPC sees an item in a new pos, update his knowledge
        if(GameState->Obj[ObjI].AnimationTimer)
        {
            --GameState->Obj[ObjI].AnimationTimer;
        }
        
        bool32 SeesObj = false;
        if(GameState->Obj[ObjI].Possessed)
        {
            if(GameState->Bob.Inventory[ObjI])
            {
                GameState->Obj[ObjI].Pos = GameState->Bob.Pos;
            }
            else
            {
                GameState->Obj[ObjI].Pos = GameState->PlayerPos;
            }
            continue;
        }
        
        if(((uint32)GameState->Bob.ObjPos[ObjI].X != (uint32)GameState->Obj[ObjI].Pos.X) ||
           ((uint32)GameState->Bob.ObjPos[ObjI].Y != (uint32)GameState->Obj[ObjI].Pos.Y))
        {
            //ghost_path.h
            if(ObjCheck(BobEyesPos,
                        EyeWidth,
                        EyeDepth,
                        GameState->Bob.Direction,
                        GameState->Obj[ObjI].Pos,
                        (uint32 *)GameState->AptTileMap))
            {

                SeesObj = true;
                GameState->Bob.ObjPos[ObjI] = GameState->Obj[ObjI].Pos;                
            }
        }        
        //If obj is one that you need for you current event, trigger GrabItem
        if(GameState->Bob.CurrentEvent.ID != GRAB_ITEM &&
           ObjI == GameState->Bob.CurrentEvent.Obj &&
           GameState->Bob.ObjNeeded[ObjI])
        {
            if(!SeesObj)
            {
                //ghost_path.h
                if(ObjCheck(BobEyesPos,
                            EyeWidth,
                            EyeDepth,
                            GameState->Bob.Direction,
                            GameState->Obj[ObjI].Pos,                               
                            (uint32 *)GameState->AptTileMap))
                {
                    GameState->Bob.ObjPos[ObjI] = GameState->Obj[ObjI].Pos;
                    NewEventID[NewEventI++] = GRAB_ITEM;
                    GameState->Bob.GotNewEvent = true;
                }
            }
            else
            {
                //Grab that dang Item
                NewEventID[NewEventI++] = GRAB_ITEM;
                GameState->Bob.GotNewEvent = true;
            }
        }
    }

    Assert(GameState->Bob.EventsBuffered < 6)
        //NOTE:Check and handle an empty event buffer
        if(GameState->Bob.EventsBuffered == 0 && !GameState->Bob.GotNewEvent)
        {
            if(GameState->Bob.ObjNeeded[BALL])
            {
                NewEventID[NewEventI++] = SEARCH;            
            }
            else
            {
                NewEventID[NewEventI++] = PING_PONG;
            }
            GameState->Bob.GotNewEvent = true;
        }
        
    if(GameState->Bob.GotNewEvent)
    {
        GameState->Bob.GotNewEvent = false;
        

        
        if(GameState->Bob.CurrentEvent.CouldComplete)
        {
            bool32 CompleteIt = true;
            for(uint32 NEII = 0; NEII < NewEventI; ++NEII)
            {
                if(GameState->Bob.CurrentEvent.ID == NewEventID[NEII])
                {
                    break;                    
                }
            }
            if(CompleteIt)
            {
                GameState->Bob.CurrentEvent = EmptyEvent;
                GameState->Bob.EventBuffer[GameState->Bob.CurrentEventBufferI] = EmptyEvent;
                --GameState->Bob.EventsBuffered;            
            }
        }

        bool32 IsDuplicate = false;
        for(uint32 NEI = 0; NEI < NewEventI; ++NEI)
        {
                                    
            //NOTE: Check to see that we don't risk
            // duplicate events in the event buffer
            for(uint32 EI = 0; EI < ArrayCount(GameState->Bob.EventBuffer); ++EI)
            {
                if(NewEventID[NEI] == GameState->Bob.EventBuffer[EI].ID)
                {                                        
                    
                    IsDuplicate = true;
                    break;
                }
            }
            if(IsDuplicate)
            {
                continue;
            }

                        
            event NewEvent = {};
            NewEvent.ID = NewEventID[NEI];
            switch(NewEventID[NEI])
            {
                
                case BATHROOM:
                case WORK:
                case EAT:
                case TV:
                case SLEEP:
                {
                    NewEvent.Priority = MEDIUM_PRIORITY;
                }break;
                                              
                case PING_PONG:
                {
                    NewEvent.Priority = LOW_PRIORITY;
                }
                break;
 
                case WRITE_LETTER:
                {
                    NewEvent.Priority = HIGH_PRIORITY;
                }
                break;                                               

                case SEARCH:
                {
                    //?
                    //Priority should be calculated based on the priority of the item.
                    //You will only be searching due to a prior event, due to the CURRENT
                    // EVENT in fact, so make it [equal?/greaterthan?] the currentevent
                    // priority                    
                    NewEvent.Priority = LOW_PRIORITY + 1;
                    NewEvent.Obj = BALL;                    
                }
                break;
                
                case GRAB_ITEM:
                {
                    NewEvent.CouldComplete = true;
                    NewEvent.Priority = GameState->Bob.CurrentEvent.Priority + 2;
                    NewEvent.Obj = GameState->Bob.CurrentEvent.Obj;
                }break;
/*
  case PONDER:
  {
  NewEvent.Priority = GameState->Bob.CurrentEvent.Priority;
  }
  break;
*/
                case FAINT:
                {
                    NewEvent.Priority = 20;
                }
                break;
                default:
                {
                    Assert(0);                    
                }
                break;
            };
            
            uint32 BufferSize = ArrayCount(GameState->Bob.EventBuffer);
            if(GameState->Bob.EventsBuffered == BufferSize)
            {
                uint32 LowestEventPriority = NewEvent.Priority;
                uint32 LowestBufferI = 100;
                bool32 SwapEvents = false;
                for(uint32 BufferI = 0; BufferI < BufferSize; ++BufferI)
                {
                    if(GameState->Bob.EventBuffer[BufferI].Priority < LowestEventPriority)
                    {
                        LowestEventPriority = GameState->Bob.EventBuffer[BufferI].Priority;
                        LowestBufferI = BufferI;
                        SwapEvents = true;
                    }                    
                }
                if(SwapEvents)
                {
                    GameState->Bob.EventBuffer[LowestBufferI] = NewEvent;
                }
                
            }
            else//IF EVENTBUFFER ISN'T FULL YET
            {
                for(uint32 BufferI = 0; BufferI < BufferSize; ++BufferI)
                {
                    if(GameState->Bob.EventBuffer[BufferI].Priority == 0)
                    {
                        GameState->Bob.EventBuffer[BufferI] = NewEvent;
                        ++GameState->Bob.EventsBuffered;
                        break;
                    }                    
                }
            }                
        }
        
        uint32 HighestEventPriority = GameState->Bob.CurrentEvent.Priority;
        uint32 HighestBufferI = 0;
        bool32 NewCurrentEvent = false;
        for(uint32 BufferI = 0; BufferI < ArrayCount(GameState->Bob.EventBuffer); ++BufferI)
        {
            if(GameState->Bob.EventBuffer[BufferI].Priority > HighestEventPriority)               
            {
                HighestEventPriority = GameState->Bob.EventBuffer[BufferI].Priority;
                HighestBufferI = BufferI;
                NewCurrentEvent = true;
            }                    
        }
        if(NewCurrentEvent)
        {
            if(GameState->Bob.CurrentEvent.ID != MIN_EVENTS)
            {
                GameState->Bob.EventBuffer[GameState->Bob.CurrentEventBufferI] = GameState->Bob.CurrentEvent;
            }
            
            GameState->Bob.CurrentEventBufferI = HighestBufferI;            
            GameState->Bob.CurrentEvent = GameState->Bob.EventBuffer[HighestBufferI];
            NewPhase = true;
        }             
    }

   
    clock EmptyTimer = {};
    switch(GameState->Bob.CurrentEvent.ID)
    {
        
        case WRITE_LETTER:
        {
            if(GameState->Bob.CurrentEvent.Phase == 0)
            {
                
                if(GameState->Bob.Inventory[PEN])
                {
                    // ...and the pen was there
                    NewPhase = true;
                    ++GameState->Bob.CurrentEvent.Phase;
                    GameState->Bob.CurrentEvent.Obj = MIN_OBJECTS;
                    GameState->Bob.ObjNeeded[PEN] = false;
                }
                else                
                {
                    if(NewPhase)
                    {
                        (GameState->Bob.IsInspired)?GameState->CurrentWords = GameState->Words[WORDS_INSPIREDWRITE]:GameState->CurrentWords = GameState->Words[WORDS_WRITE];
                        GameState->Bob.CurrentEvent.Obj = PEN;
                        GameState->Bob.ObjNeeded[PEN] = true;
                        GameState->Bob.AtDest = false;
                        GameState->Bob.GotNewDest = true;
                            
                        if(GameState->AptTileMap[(uint32)GameState->Bob.ObjPos[PEN].Y][(uint32)GameState->Bob.ObjPos[PEN].X] == TABLE_TILE ||
                           GameState->AptTileMap[(uint32)GameState->Bob.ObjPos[PEN].Y][(uint32)GameState->Bob.ObjPos[PEN].X] == HOLE_TILE)
                        {
                            GameState->Bob.Dest = AdjustDest(GameState->Bob.ObjPos[PEN], (uint32 *)GameState->AptTileMap);
                        }
                        else
                        {
                            GameState->Bob.Dest = GameState->Bob.ObjPos[PEN];
                        }                           
                    }                                                                
                    //If you arrive at the pen's last known place...                    
                    else if(GameState->Bob.AtDest)
                    {                                
                        // ...and the pen wasn't there
                        GameState->Bob.IsConfused = 100;
                        GameState->Bob.IsInspired = false;
                        GameState->Bob.CurrentEvent.Complete = true;
                        GameState->Bob.CurrentEvent.Obj = MIN_OBJECTS;
                        
                    }
                }                                                            
            }
        
            if(GameState->Bob.CurrentEvent.Phase == 1)
            {
            
                //Make the writing desk your new destination
                v2 WritingDeskPos = {10.0f, 29.45f};            
                if(NewPhase)
                {
                    GameState->Bob.AtDest = false;
                    GameState->Bob.GotNewDest = true;                
                    GameState->Bob.Dest = WritingDeskPos;            
                }
                else if(GameState->Bob.AtDest)
                {

                    GameState->Bob.Direction = N;
                    //Once you make it to the writing desk, sit and write for a 30 minutes                
                    if(++GameState->Bob.CurrentEvent.Timer.Sec == 60)
                    {
                        if(GameState->Bob.CurrentEvent.Timer.Min == 1)
                        {
                            GameState->CurrentWords = GameState->Words[WORDS_DEAR_EVELYN];
                        }
                        GameState->Bob.CurrentEvent.Timer.Sec = 0;
                        if(++GameState->Bob.CurrentEvent.Timer.Min == 30)
                        {
                            GameState->Bob.CurrentEvent.Timer = EmptyTimer;
                        
                            //Once done writing, Place pen back on the desk from your inventory
                            GameState->Obj[PEN].Possessed = false;
                            GameState->Bob.Inventory[PEN] = false;
                            GameState->Obj[PEN].Pos = GameState->Obj[PEN].ProperPos;
                            GameState->Bob.ObjPos[PEN] = GameState->Obj[PEN].Pos;
                        
                            //Add letter to Bob's inventory

                            for(uint32 PI = 0; PI < GameState->PlayerInventoryAmount; ++PI)
                            {
                                if(GameState->PlayerInventory[PI] == LETTER)
                                {                                    
                                    GameState->PlayerInventory[PI] = 0;                            
                                    if(PI != GameState->PlayerInventoryAmount)
                                    {
                                        for(uint32 II = GameState->PlayerInventoryI+1; II < GameState->PlayerInventoryAmount; ++II)
                                        {
                                            GameState->PlayerInventory[II-1] = GameState->PlayerInventory[II];
                                            GameState->PlayerInventory[II] = 0;
                                        }                                
                                    }                            
                                    --GameState->PlayerInventoryAmount;                            
                                    GameState->PlayerInventoryI = 0;
                                    break;
                                }
                            }
                            GameState->Bob.Inventory[LETTER] = true;
                            GameState->Obj[LETTER].Possessed = true;

                            //Phase 1 complete
                            NewPhase = true;
                            ++GameState->Bob.CurrentEvent.Phase;
                        }
                    }
                    //TODO: letter writing animation
                }                        
            }
        
            if(GameState->Bob.CurrentEvent.Phase == 2)
            {
                
                GameState->Bob.Direction = S;
                //Play self doubt animation and dialogue for X amount of seconds
                //When played through phase 2 is complete
                if(!GameState->Bob.CurrentEvent.Timer.Min && !GameState->Bob.CurrentEvent.Timer.Sec && !GameState->Bob.IsInspired)
                {                                                                
                    GameState->CurrentWords = GameState->Words[WORDS_DOUBT];                    
                }
                    
                if(++GameState->Bob.CurrentEvent.Timer.Sec == 60)
                {
                    GameState->Bob.CurrentEvent.Timer.Sec = 0;
                    if(++GameState->Bob.CurrentEvent.Timer.Min == 5)
                    {                        
                        //Phase 1 complete
                        GameState->Bob.CurrentEvent.Timer = EmptyTimer;
                        ++GameState->Bob.CurrentEvent.Phase;
                        NewPhase = true;
                    }
                }                             
            }
        
            //Assuming that Bob wasn't inspired by his television previous to this event.
            if(GameState->Bob.CurrentEvent.Phase == 3)
            {
                //Search for lighter in your inventory first.
                
                if(GameState->Bob.IsInspired)
                {                    
                    if(NewPhase)
                    {
                        GameState->Bob.AtDest = false;
                        GameState->Bob.GotNewDest = true;                
                        GameState->Bob.Dest = v2{26.0f,42.0f};            
                    }
                    else if(GameState->Bob.AtDest)
                    {
                        GameState->Bob.IsInspired = false;
                        GameState->Bob.Direction = E;
                        GameState->Obj[LETTER].Pos = v2{27.5f, 42.5f};
                        GameState->Bob.ObjPos[LETTER] = v2{27.5f, 42.5f};
                        GameState->Obj[LETTER].Possessed = false;                
                        GameState->Bob.Inventory[LETTER] = false;
                        GameState->Bob.CurrentEvent.Complete = true;
                    }
                }
                else
                {
                    if(GameState->Bob.Inventory[LIGHTER])
                    {
                        GameState->Bob.Direction = S;
                        GameState->Bob.CurrentEvent.Obj = MIN_OBJECTS;
                        GameState->Bob.ObjNeeded[LIGHTER] = false;
                    
                        /*if(GameState->Obj[LIGHTER].Broken)
                          {
                          GameState->Bob.IsConfused = 100;                    
                          GameState->Bob.CurrentEvent.Timer = EmptyTimer;
                          GameState->Obj[LIGHTER].Possessed = false;
                          GameState->Obj[LIGHTER].Pos = GameState->Bob.Pos;
                          GameState->Bob.ObjPos[LIGHTER] = GameState->Obj[LIGHTER].Pos;
                          GameState->Bob.Inventory[LIGHTER] = false;
                            
                          NewPhase = true;
                          ++GameState->Bob.CurrentEvent.Phase;
                          }
                          else*/
                    

                        GameState->Bob.CurrentEvent.CouldComplete = true;
                        GameState->Bob.Inventory[LETTER] = false;
                        if(++GameState->Bob.CurrentEvent.Timer.Sec == 60)
                        {
                            if(GameState->Bob.CurrentEvent.Timer.Min == 0)
                            {
                                GameState->Obj[LIGHTER].AnimationTimer = 20; 
                            }
                            GameState->Bob.CurrentEvent.Timer.Sec = 0;
                            if(++GameState->Bob.CurrentEvent.Timer.Min == 5)
                            {
                                GameState->Bob.CurrentEvent.Timer = EmptyTimer;
                            
                                GameState->Obj[LIGHTER].Possessed = false;
                                GameState->Obj[LIGHTER].Pos = GameState->Bob.Pos;
                                GameState->Bob.ObjPos[LIGHTER] = GameState->Obj[LIGHTER].Pos;
                                GameState->Bob.Inventory[LIGHTER] = false;

                        
                                GameState->Bob.CurrentEvent.Complete = true;
                            }
                        }
                   
                    }
                    else                
                    {                                    
                        if(NewPhase)
                        {
                            GameState->Bob.CurrentEvent.Obj = LIGHTER;                                                
                            GameState->Bob.ObjNeeded[LIGHTER] = true;
                            GameState->Bob.AtDest = false;
                            GameState->Bob.GotNewDest = true;
                        
                            if(GameState->AptTileMap[(uint32)GameState->Bob.ObjPos[LIGHTER].Y][(uint32)GameState->Bob.ObjPos[LIGHTER].X] == TABLE_TILE ||
                               GameState->AptTileMap[(uint32)GameState->Bob.ObjPos[LIGHTER].Y][(uint32)GameState->Bob.ObjPos[LIGHTER].X] == HOLE_TILE)
                            {
                                GameState->Bob.Dest = AdjustDest(GameState->Bob.ObjPos[LIGHTER], (uint32 *)GameState->AptTileMap);
                            }
                            else
                            {
                                GameState->Bob.Dest = GameState->Bob.ObjPos[LIGHTER];
                            }
                    
                        }
                        else if(GameState->Bob.AtDest)
                        {
                            GameState->Bob.IsConfused = 100;
                            GameState->Bob.CurrentEvent.Obj = MIN_OBJECTS;
                            GameState->Bob.CurrentEvent.Timer = EmptyTimer;
                            NewPhase = true;
                            ++GameState->Bob.CurrentEvent.Phase;
                        }
                    }
                }
                                                                
            }
        
            if(GameState->Bob.CurrentEvent.Phase == 4)
            {                
                //Go to the shredder's pos with the letter in your inventory.
                if(NewPhase)
                {
                    GameState->CurrentWords = GameState->Words[WORDS_SHREDDER];
                    GameState->Bob.GotNewDest = true;
                    GameState->Bob.AtDest = false;
                    GameState->Bob.Dest = v2{GameState->Obj[SHREDDER].Pos.X, GameState->Obj[SHREDDER].Pos.Y - 1.1f};

                }
                else if(GameState->Bob.AtDest)
                {
                    if(GameState->Obj[SHREDDER].Broken)
                    {
                        //Play flummoxed animation
                        GameState->Bob.IsConfused = 100;
                        if(++GameState->Bob.CurrentEvent.Timer.Sec == 60)
                        {
                            GameState->Bob.CurrentEvent.Timer.Sec = 0;
                            if(++GameState->Bob.CurrentEvent.Timer.Min == 3)
                            {
                                GameState->Bob.CurrentEvent.Timer = EmptyTimer;
                            
                                //Phase complete                            
                                ++GameState->Bob.CurrentEvent.Phase;
                                NewPhase = true;
                            }
                        }
                    }
                    else
                    {
                        GameState->Bob.CurrentEvent.CouldComplete = true;
                        GameState->Bob.Inventory[LETTER] = false;
                        //Play shredding animation
                        if(++GameState->Bob.CurrentEvent.Timer.Sec == 60)
                        {
                            if(GameState->Bob.CurrentEvent.Timer.Min == 0)
                            {
                                GameState->Obj[SHREDDER].AnimationTimer = 100; 
                            }
 
                            GameState->Bob.CurrentEvent.Timer.Sec = 0;
                            if(++GameState->Bob.CurrentEvent.Timer.Min == 10)
                            {                        
                                GameState->Bob.CurrentEvent.Timer = EmptyTimer;
                            
                                GameState->Bob.CurrentEvent.Complete = true;
                            }
                        }
                    }
                }
            }
            /*if(GameState->Bob.CurrentEvent.Phase == 5)
              {
              //Same as phase 4 but with stove.
              }*/

            if(GameState->Bob.CurrentEvent.Phase == 5)
            {
                //Head over to trash outside.
                v2 TrashCanPos = {23.0f, 45.8f};
                if(NewPhase)
                {
                    GameState->CurrentWords = GameState->Words[WORDS_TRASH];
                    GameState->Bob.GotNewDest = true;
                    GameState->Bob.AtDest = false;
                    GameState->Bob.Dest = TrashCanPos;
                }
                else if(GameState->Bob.AtDest)
                {
                    GameState->Bob.CurrentEvent.CouldComplete = true;
                    //Transfer LETTER from inventory to the trashcan.
                    GameState->Bob.Direction = W;
                    if(++GameState->Bob.CurrentEvent.Timer.Sec == 60)
                    {
                        GameState->Bob.CurrentEvent.Timer.Sec = 0;
                        if(++GameState->Bob.CurrentEvent.Timer.Min == 10)
                        {                        
                            GameState->Bob.CurrentEvent.Timer = EmptyTimer;
                            GameState->Obj[LETTER].Pos = v2{TrashCanPos.X - 1.5f, TrashCanPos.Y};
                            GameState->Bob.ObjPos[LETTER] = v2{TrashCanPos.X - 1.5f, TrashCanPos.Y};
                            GameState->Obj[LETTER].Possessed = false;                
                            GameState->Bob.Inventory[LETTER] = false;

                            GameState->Bob.CurrentEvent.Complete = true;
                        }
                    }
                    //Event Complete.
                }            
            }    
        }break;
        
        case BATHROOM:
        {
            
            if(GameState->Bob.CurrentEvent.Phase == 0)
            {
                //Go to the bathroom
                GameState->Bob.CurrentEvent.CouldComplete = true;
                v2 BathroomPos = {4.0f, 37.9f};
                if(NewPhase)
                {
                    
                    GameState->Bob.Dest = BathroomPos;
                    GameState->Bob.GotNewDest = true;
                }
                else if(GameState->Bob.AtDest)
                {
                    GameState->Bob.Direction = N;
                    //TODO: Bathroom activities (custom paths/animations) w/ timing
                }            
            }                   
        }break;

        case WORK:
        {
            GameState->Bob.CurrentEvent.CouldComplete = true;
            if(GameState->Bob.CurrentEvent.Phase == 0)
            {
                //Go to the computer
                v2 WorkPos = {40.35f, 29.5f};
                if(NewPhase)
                {
                    GameState->Bob.CurrentEvent.CouldComplete = true;
                    GameState->CurrentWords = GameState->Words[WORDS_WORK];
                    GameState->Bob.Dest = WorkPos;
                    GameState->Bob.GotNewDest = true;
                }
                else if(GameState->Bob.AtDest)
                {                    
                    GameState->Bob.Direction = N;
                    //TODO: typing activities (custom paths/animations) w/ timing
                }            
            }
        }break;

        case PING_PONG:
        {
            if(GameState->Bob.CurrentEvent.Phase == 0)
            {
                                
                bool32 HasBall = GameState->Bob.Inventory[BALL];                
                if(HasBall)
                {
                    
                    NewPhase = true;
                    ++GameState->Bob.CurrentEvent.Phase;
                    GameState->Bob.CurrentEvent.Obj = MIN_OBJECTS;
                    GameState->Bob.ObjNeeded[BALL] = false;
                }
                else if(NewPhase)
                {
                    GameState->Bob.CurrentEvent.Obj = BALL;
                    GameState->Bob.ObjNeeded[BALL] = true;
                    GameState->Bob.GotNewDest = true;
                    GameState->Bob.AtDest = false;
                        
                    if(GameState->AptTileMap[(uint32)GameState->Bob.ObjPos[BALL].Y][(uint32)GameState->Bob.ObjPos[BALL].X] == TABLE_TILE ||
                       GameState->AptTileMap[(uint32)GameState->Bob.ObjPos[LIGHTER].Y][(uint32)GameState->Bob.ObjPos[LIGHTER].X] == HOLE_TILE)
                    {
                        GameState->Bob.Dest = AdjustDest(GameState->Bob.ObjPos[BALL], (uint32 *)GameState->AptTileMap);
                    }
                    else
                    {
                        GameState->Bob.Dest = GameState->Bob.ObjPos[BALL];
                    }
                    
                    GameState->Bob.GotNewDest = true;
                }                                    
                else if(GameState->Bob.AtDest)
                {
                    GameState->Bob.IsConfused = 100;
                    GameState->Bob.CurrentEvent.Obj = MIN_OBJECTS;
                    GameState->Bob.CurrentEvent.Complete = true;
                }                                                                        
            }

            if(GameState->Bob.CurrentEvent.Phase == 1)
            {
                //Go to ping pong table and play
                v2 PingPongTablePos = v2{29.7f, 18.6f};
                if(NewPhase)
                {
                    GameState->Bob.Dest = PingPongTablePos;
                    GameState->Bob.GotNewDest = true;
                }                                    
                else if(GameState->Bob.AtDest)
                {

                    
                    GameState->Bob.Direction = E;
                    //TODO: PingPonging animations with timing and shtuff
                    //TEMP measures below.

                    if(GameState->Bob.Inventory[BALL])
                    {
                        GameState->Bob.CurrentEvent.CouldComplete = true;
                        GameState->Obj[BALL].Pos = v2{PingPongTablePos.X + 1.5f, PingPongTablePos.Y};
                        GameState->Obj[BALL].Possessed = false;
                        GameState->Bob.ObjPos[BALL] = GameState->Obj[BALL].Pos;
                        GameState->Bob.Inventory[BALL] = false;                    
                    }
                    else if(!GameState->Obj[BALL].Possessed)
                    {
                        GameState->Obj[BALL].Pos = v2{PingPongTablePos.X + 1.7f, PingPongTablePos.Y - AbsDiff((real32)GameState->Clock.Sec, 30.0f)*0.02f};
                        GameState->Bob.ObjPos[BALL] = GameState->Obj[BALL].Pos;
                    }
                    
                }            
            }
        }break;

        case EAT:
        {
            v2 KitchenPos = {4.92f, 2.5f};            
            if(NewPhase)
            {                
                GameState->CurrentWords = GameState->Words[WORDS_FOOD];
                GameState->Bob.Dest = KitchenPos;
                GameState->Bob.GotNewDest = true;
            }
            else if(GameState->Bob.AtDest)
            {
                GameState->Bob.Direction = E;
                GameState->Bob.CurrentEvent.CouldComplete = true;
                //TODO: Kitchen animations with timing and shtuff
                if(++GameState->Bob.CurrentEvent.Timer.Sec == 60)
                {
                    GameState->Bob.CurrentEvent.Timer.Sec = 0;
                    if(++GameState->Bob.CurrentEvent.Timer.Min == 30)
                    {                        
                        GameState->Bob.CurrentEvent.Timer = EmptyTimer;
                            
                        GameState->Bob.CurrentEvent.Complete = true;
                    }
                }
            }
        }break;

        case TV:
        {
            if(GameState->Bob.CurrentEvent.Phase == 0)
            {

                if(GameState->Bob.Inventory[REMOTE])
                {
                    //TODO Try turning on the tv with remote, check to see if it is broken.
                    GameState->Bob.ObjNeeded[REMOTE] = false;
                    NewPhase = true;
                    ++GameState->Bob.CurrentEvent.Phase;   
                }
                else if(NewPhase)
                {
                    
                    GameState->CurrentWords = GameState->Words[WORDS_TV];    
                    GameState->Bob.CurrentEvent.Obj = REMOTE;
                    GameState->Bob.ObjNeeded[REMOTE] = true;
                    if(GameState->AptTileMap[(uint32)GameState->Bob.ObjPos[REMOTE].Y][(uint32)GameState->Bob.ObjPos[REMOTE].X] == TABLE_TILE ||
                       GameState->AptTileMap[(uint32)GameState->Bob.ObjPos[REMOTE].Y][(uint32)GameState->Bob.ObjPos[REMOTE].X] == HOLE_TILE)
                    {
                        GameState->Bob.Dest = AdjustDest(GameState->Bob.ObjPos[REMOTE], (uint32 *)GameState->AptTileMap);
                    }
                    else
                    {
                        GameState->Bob.Dest = GameState->Bob.ObjPos[REMOTE];
                    }
                    
                    GameState->Bob.GotNewDest = true;
                }
                else if(GameState->Bob.AtDest)
                {
                    //TODO:Turn on tv manually
                    GameState->Bob.IsConfused = 100;
                    ++GameState->Bob.CurrentEvent.Phase;
                    NewPhase = true;                     
                }
            }
        
            if(GameState->Bob.CurrentEvent.Phase == 1)
            {
                v2 TVPos = {32.1f, 2.5f};            
                if(NewPhase)
                {

                    GameState->Bob.Dest = TVPos;
                    GameState->Bob.GotNewDest = true;
                }
                else if(GameState->Bob.AtDest)
                {
                                        
                    GameState->Bob.Direction = N;
                    //TODO: TV animations with timing and shtuff
                    //Drop remote on sidetable
                                                                                
                    if(GameState->Bob.Inventory[REMOTE])
                    {
                        
                        if(!GameState->Obj[REMOTE].Broken)
                        {
                            GameState->ChannelI = (uint32)((GameState->PlayerPos.X - (uint32)GameState->PlayerPos.X)*10);
                            if(GameState->ChannelI == 6)
                            {
                                ++GameState->ChannelI;
                            }
                            
                            GameState->ChannelTimer = 120;
                            GameState->Bob.CurrentEvent.CouldComplete = true;
                        }
                        else
                        {
                            GameState->Bob.IsConfused = 100;
                        }
                        
                        GameState->Obj[REMOTE].Possessed = false;
                        GameState->Obj[REMOTE].Pos = GameState->Obj[REMOTE].ProperPos;
                        GameState->Bob.ObjPos[REMOTE] = GameState->Obj[REMOTE].Pos;
                        GameState->Bob.Inventory[REMOTE] = false;
                    }
                    else
                    {
                        if(GameState->ChannelI == 6)
                        {
                            if(++GameState->Bob.CurrentEvent.Timer.Sec == 60)
                            {
                                GameState->Bob.CurrentEvent.Timer.Sec = 0;
                                if(++GameState->Bob.CurrentEvent.Timer.Min == 15)
                                {                        
                                    GameState->Bob.CurrentEvent.Timer = EmptyTimer;
                                    GameState->Bob.IsInspired = true;
                                    GameState->Bob.CurrentEvent.Complete = true;
                                }
                            }
                        }
                        else
                        {
                            GameState->Bob.CurrentEvent.CouldComplete = true;
                        }
                    }
                }
            }                        
        }break;

        case SLEEP:
        {
            
            if(NewPhase)               
            {
                GameState->CurrentWords = GameState->Words[WORDS_YAWN];
                v2 BedPos = {17.5f, 28.5};
                GameState->Bob.Dest = BedPos;
                GameState->Bob.GotNewDest = true;
            }
            else if(GameState->Bob.AtDest)
            {
                if(++GameState->Bob.CurrentEvent.Timer.Sec == 60)
                {
                    GameState->Bob.CurrentEvent.Timer.Sec = 0;
                    if(++GameState->Bob.CurrentEvent.Timer.Min == 60)
                    {
                        GameState->Bob.CurrentEvent.Timer.Min = 0;

                        if(++GameState->Bob.CurrentEvent.Timer.Hr == 4)
                        {
                            GameState->Bob.CurrentEvent.CouldComplete = true;
                        }
                        else if(++GameState->Bob.CurrentEvent.Timer.Hr == 8)
                        {
                            GameState->Bob.CurrentEvent.Timer = EmptyTimer;                            
                            GameState->Bob.CurrentEvent.Complete = true;
                        }
                        
                    }
                }
                //TODO: Tuck-into-bed animations with timing and shtuff
            }
        }break;

        case SEARCH:
        {
            bool32 HasObj = GameState->Bob.Inventory[GameState->Bob.CurrentEvent.Obj];

            if(HasObj)
            {
                GameState->Bob.CurrentEvent.Complete = true;
            }
            else if(GameState->Bob.AtDest)
            {                
                GameState->CurrentWords = GameState->Words[WORDS_SEARCH];
                //TODO make this more random
                uint32 bi = GameState->Clock.Sec % GameState->L0.RoomCount;
                uint32 hi = GameState->Clock.Min % GameState->L0.Rooms[bi].HubCount;
                GameState->Bob.Dest = GameState->L0.Rooms[bi].Hub[hi].Pos;
                GameState->Bob.GotNewDest = true;
            }
        }break;

        case GRAB_ITEM:            
        {

            if(GameState->Bob.CurrentEvent.Obj)
            {
                object *GrabbyObj = &GameState->Obj[GameState->Bob.CurrentEvent.Obj];
                if(NewPhase)
                {
                    if(GameState->AptTileMap[(uint32)GrabbyObj->Pos.Y][(uint32)GrabbyObj->Pos.X] == TABLE_TILE ||
                       GameState->AptTileMap[(uint32)GrabbyObj->Pos.Y][(uint32)GrabbyObj->Pos.X] == HOLE_TILE)
                    {
                        GameState->Bob.Dest = AdjustDest(GrabbyObj->Pos, (uint32 *)GameState->AptTileMap);
                    }
                    else
                    {
                        GameState->Bob.Dest = GrabbyObj->Pos;                
                    }
            
                    GameState->Bob.GotNewDest = true;
                }
                else if(GameState->Bob.AtDest)
                {
                    v2 OToNPC = {};
                    OToNPC.X = AbsDiff(GameState->Bob.Pos.X, GrabbyObj->Pos.X);
                    OToNPC.Y = AbsDiff(GameState->Bob.Pos.Y, GrabbyObj->Pos.Y);
                    if(OToNPC.X < 2.0f && OToNPC.Y < 2.0f && !GrabbyObj->Possessed)
                    {
                        GameState->Bob.Inventory[GameState->Bob.CurrentEvent.Obj] = true;
                        GameState->Obj[GameState->Bob.CurrentEvent.Obj].Possessed = true;
                        GameState->Bob.ObjNeeded[GameState->Bob.CurrentEvent.Obj] = false;
                        GameState->Bob.CurrentEvent.Obj = MIN_OBJECTS;
                    }
                    else
                    {
                        //TODO: You need to fill in this code.
                        GameState->Bob.IsConfused = 100;
                        uint32 huhwhereditgo = 0;
                    }
                    GameState->Bob.CurrentEvent.Complete = true;            
                }
            }
            else
            {
                GameState->Bob.CurrentEvent.Complete = true;
            }
            
        }break;

        case FAINT:
        {
            int foo = 0;
        }break;

        default:
        {
            Assert(0);
        }
    };
    
    //If NPC is at the newest Destination
    if((!GameState->Bob.AtDest || GameState->Bob.GotNewDest) &&        
       (AbsDiff(GameState->Bob.Pos.X, GameState->Bob.Dest.X) < 0.04f) &&
       (AbsDiff(GameState->Bob.Pos.Y, GameState->Bob.Dest.Y) < 0.04f))
    {        
        /*if(GameState->Bob.NextHub.ID > 0)
          {
          GameState->Bob.PriorHub = GameState->Bob.NextHub;
          }*/
                
        GameState->Bob.AtDest = true;
        GameState->InterRoomChase = false;
        GameState->Bob.GotNewDest = false;
        GameState->Bob.IsMoving = false;

        //NOTE: 3/23/19
        GameState->PathEmpty = true;
        
        GameState->Bob.EmptyPathHub.Pos = GameState->Bob.Pos;
        GameState->Bob.EmptyPathHub.ID = 0;
        GameState->Bob.EmptyPathHub.Char = 0;
    }

    //Else if NPC Got a new destination this frame and wasn't already at the right location
    //bool32 WritePath = true;
    if(GameState->Bob.GotNewDest)
    {
        
        GameState->Bob.EmptyPathHub.Pos = GameState->Bob.Pos;
        GameState->Bob.EmptyPathHub.ID = 0;
        GameState->Bob.EmptyPathHub.Char = 0;        
        GameState->Bob.NextHub = GameState->Bob.EmptyPathHub;
        
        GameState->PathEmpty = true;        
        GameState->Bob.IsMoving = true;
        GameState->Bob.GotNewDest = false;
        GameState->Bob.AtDest = false;
        //GameState->Bob.SeesDest = false;
        GameState->InterRoomChase = false;        
    }

    //014

    if(GameState->Bob.IsMoving &&
       GameState->PathEmpty &&
       (AbsDiff(GameState->Bob.Pos.X, GameState->Bob.EmptyPathHub.Pos.X) < 0.04f) &&
       (AbsDiff(GameState->Bob.Pos.Y, GameState->Bob.EmptyPathHub.Pos.Y) < 0.04f))
    {
        uint64 OneMask = 0x1111111111111111;
        //We will need to transfer some of my pathing code into functions at this
        //point I suppose.
        uint32 DestRoomI = GetRoom(&GameState->L0, GameState->Bob.Dest);
        uint32 CurrentRoomI = GetRoom(&GameState->L0, GameState->Bob.Pos);

        room RoomA = GameState->L0.Rooms[CurrentRoomI];
        uint64 RoomZID = GameState->L0.Rooms[DestRoomI].ID;
        
        
        
        if(RoomZID != RoomA.ID)
        {
            
                            
            uint32 NearbyHubI = 0;
            hub NearbyHub = {};                
            real32 ThisInner = 0.0f;
            real32 NearbyInner = 200.0f;
            bool32 ValidHub = false;
            bool32 NeedFalseHub = false;
            uint32 NearbyHubWallCheckBF = 0;
                
            for(uint32 HubI = 0; HubI < RoomA.HubCount; ++HubI)
            {
                    
                ThisInner = LengthSq(v2{GameState->Bob.Pos.X - RoomA.Hub[HubI].Pos.X, GameState->Bob.Pos.Y - RoomA.Hub[HubI].Pos.Y});
                                
                
                                        
                if(ThisInner < NearbyInner)
                {

                    NearbyHubWallCheckBF =                
                        ClearPathCheckAndFalseHubAssign(GameState->Bob.Pos,
                                                        RoomA.Hub[HubI].Pos,
                                                        &GameState->Bob.FalseHub[0].Pos,
                                                        (uint32 *)GameState->AptTileMap);
                        
                    ValidHub = (NearbyHubWallCheckBF & (0x1 << CAN_HUB_SKIP)) ? true : false;
                    //Huh = (UnoHubSkipBF1 & (0xF << HAVE_SAME_X)) ? true : false;
                    //Huh = (UnoHubSkipBF1 & (0xF << HAVE_SAME_Y)) ? true : false;

                    if(ValidHub)
                    {
                        NearbyHub = RoomA.Hub[HubI];
                        NearbyInner = ThisInner;
                        NeedFalseHub = ((NearbyHubWallCheckBF & (0x1 << HAVE_SAME_X)) || (NearbyHubWallCheckBF & (0x1 << HAVE_SAME_Y))) ? false : true;
                    }
                }                
            }
                                               
            Assert(NearbyHub.Char != 0);

            uint8 TempPath[20] = {};
            uint32 TempPathI = 0;
            
            if(NeedFalseHub)
            {
                TempPath[TempPathI++] = GameState->Bob.FalseHub[0].Char;
            }
            
            TempPath[TempPathI++] = NearbyHub.Char;
            uint8 *TempPath2;

            
            
            
            GameState->InterRoomChase = true;
            //int32 RoomsAway = (RoomA.ID & (0xF << 4*(3 - DestRoomI))) >> 4*(3 - DestRoomI);
            int32 RoomsAway =
                (RoomA.RoomsAway & ((uint64)0xF << 8*(DestRoomI))) >> 8*DestRoomI;
            
            for(uint32 i = 0; i < GameState->L0.RoomCount; ++i)
            {
                if(i == CurrentRoomI)
                {
                    continue;
                }
                //if the dest room is one less room away from the potential room compared to the current room, and that potential room is only one room away from current room
                                        
                int32 iRoomsAway = (GameState->L0.Rooms[i].RoomsAway & ((uint64)0xF << (8*DestRoomI))) >> 8*DestRoomI;
                bool32 OneApart = ((GameState->L0.Rooms[i].RoomsAway & ((uint64)0xF << (8*RoomA.ID))) - (RoomA.RoomsAway & ((uint64)0xF << (8*RoomA.ID))) == (OneMask & ((uint64)0xF << (8*RoomA.ID))));
        
                if((iRoomsAway == (RoomsAway - 1)) && OneApart)                   
                {

                    uint64 NextDoorMask = 0;
                    for(uint32 PotentialDoorI = 0; PotentialDoorI < (uint64)RoomA.AdjacentRoomCount; ++PotentialDoorI)
                    {
                        if(((uint64)RoomA.Hub[0].ID & ((uint64)0xF0 << (8*PotentialDoorI))) ==
                           ((uint64)GameState->L0.Rooms[i].ID << (8*PotentialDoorI + 4)))
                        {
                            NextDoorMask = ((uint64)0xF << (8*(uint64)PotentialDoorI));
                            break;
                        }
                    }
                    Assert(NextDoorMask != 0);

                    
                    
                    TempPath2 = PathToExit(&RoomA, &NearbyHub, TempPath, TempPathI, NextDoorMask);
                    CopyString(TempPath2, GameState->Bob.CurrentPath);
                    GameState->Bob.CurrentPathI = 0;
                                        
                    if(*TempPath2)
                    {
                        GameState->PathEmpty = false;
                    }

                    bool32 FoundEPHub = false;

                    for(uint32 DoorI = 0; DoorI < GameState->L0.Rooms[i].AdjacentRoomCount; ++DoorI)
                    {
                        if((RoomA.ID << (8*DoorI + 4)) == ((uint64)GameState->L0.Rooms[i].Hub[0].ID & ((uint64)0xF0 << (8*DoorI))))
                        {                            
                            for(uint32 HubI = 0; HubI < GameState->L0.Rooms[i].HubCount; ++HubI)
                            {
                                if(((GameState->L0.Rooms[i].Hub[HubI].ID) & ((uint64)0xF << (8*(uint64)DoorI))) == 0)
                                {
                                    GameState->Bob.EmptyPathHub = GameState->L0.Rooms[i].Hub[HubI];
                                    FoundEPHub = true;
                                    break;
                                }
                            }                            
                        }
                        if(FoundEPHub)
                        {
                            break;
                        }
                    }
                    
                    break;
                }
            }            
        }
        else if(GameState->InterRoomChase)
        {
            //if board A and board Z are the same, but
            //Chasing was started on a different board.
            GameState->InterRoomChase = false;
            uint8 ReversedPath[20] = {};
            uint8 LastPath[20] = {};

                                        
            uint32 NearbyHubI = 0;
            hub NearbyHub = {};                
            real32 ThisInner = 0.0f;
            real32 NearbyInner = 400.0f;
            bool32 ValidHub = false;
            bool32 NeedFalseHub = false;
            uint32 NearbyHubWallCheckBF = 0;
                
            for(uint32 HubI = 0; HubI < RoomA.HubCount; ++HubI)
            {
                    
                ThisInner = LengthSq(v2{GameState->Bob.Dest.X - RoomA.Hub[HubI].Pos.X, GameState->Bob.Dest.Y - RoomA.Hub[HubI].Pos.Y});
                                
                
                                        
                if(ThisInner < NearbyInner)
                {

                    NearbyHubWallCheckBF =                
                        ClearPathCheckAndFalseHubAssign(RoomA.Hub[HubI].Pos,
                                                        GameState->Bob.Dest,
                                                        &GameState->Bob.FalseHub[1].Pos,
                                                        (uint32 *)GameState->AptTileMap);
                        
                    ValidHub = (NearbyHubWallCheckBF & (0x1 << CAN_HUB_SKIP)) ? true : false;

                    if(ValidHub)
                    {
                        NearbyHub = RoomA.Hub[HubI];
                        NearbyInner = ThisInner;
                        NeedFalseHub = ((NearbyHubWallCheckBF & (0x1 << HAVE_SAME_X)) || (NearbyHubWallCheckBF & (0x1 << HAVE_SAME_Y))) ? false : true;
                    }
                }                
            }


            Assert(NearbyHub.Char != 0);
            
            hub HubA = GameState->Bob.NextHub;
            hub HubZ = NearbyHub;


            //if Current NPC Hub is different from Dest Hub
            if(HubA.Char != HubZ.Char)
            {
                
                uint64 ProperDoorMask = 0;
                for(uint32 DoorI = 0; DoorI < RoomA.AdjacentRoomCount; ++DoorI)
                {
                    
                    if((HubA.ID & ((uint64)0xF << (8*DoorI))) == 0)
                    {
                        ProperDoorMask = ((uint64)0xF << (8*DoorI));
                        break;
                    }
                }
                
                uint8 *f34 = PathToExit(&RoomA, &HubZ, ReversedPath, 0, ProperDoorMask);
            
                uint32 EndOfArray = 0;
                while(ReversedPath[EndOfArray++] != 0)
                {               
                }
                --EndOfArray;
                uint32 BufferLimito = EndOfArray;
            
                for(uint32 TempI = 0; TempI < BufferLimito; ++TempI)
                {
                    LastPath[TempI] = ReversedPath[--EndOfArray];
                    
                }
                
                LastPath[BufferLimito] = HubZ.Char;

                if(NeedFalseHub)
                {
                    ++BufferLimito;
                    LastPath[BufferLimito] = GameState->Bob.FalseHub[1].Char;
                }
                CopyString(LastPath, GameState->Bob.CurrentPath);
                GameState->Bob.CurrentPathI = 0;
            }
            else
            {
                //TODO
                LastPath[0] = HubZ.Char;
                if(NeedFalseHub)
                {
                
                    LastPath[1] = GameState->Bob.FalseHub[1].Char;
                }
                CopyString(LastPath, GameState->Bob.CurrentPath);
                GameState->Bob.CurrentPathI = 0;
            }
            
            //If current hub is actually the closest hub               
            //GameState->Bob.EmptyPathHub.Pos = GameState->Bob.Dest;
            //GameState->Bob.EmptyPathHub.ID = 0;
            //GameState->Bob.EmptyPathHub.Char = '0';

            GameState->PathEmpty = false;
            
        }
        else
        {
            //uno
            //If chase was started when the current board was == to the dest board

            //v2 StartPos = GameState->Bob.Pos;
            //v2 EndPos = GameState->Bob.Dest;

            uint32 UnoHubSkipBF1 =                
                ClearPathCheckAndFalseHubAssign(GameState->Bob.Pos,
                                                GameState->Bob.Dest,
                                                &GameState->Bob.FalseHub[0].Pos,
                                                (uint32 *)GameState->AptTileMap);
            
            bool32 CanHubSkip = (UnoHubSkipBF1 & (1 << CAN_HUB_SKIP)) ? true : false;
            bool32 HaveSameX = (UnoHubSkipBF1 & (1 << HAVE_SAME_X)) ? true : false;
            bool32 HaveSameY = (UnoHubSkipBF1 & (1 << HAVE_SAME_Y)) ? true : false;
            
            if(CanHubSkip)
            {
                /*
                  GameState->Bob.EmptyPathHub.Pos = GameState->Bob.Dest;
                  GameState->Bob.EmptyPathHub.ID = 0;
                  GameState->Bob.EmptyPathHub.Char = '0';
                */
                if(HaveSameX || HaveSameY)
                {
                    GameState->PathEmpty = true;            
                }                
                else
                {                    
                    uint8 FalsePath[2] = {GameState->Bob.FalseHub[0].Char, 0};
                    CopyString(FalsePath, GameState->Bob.CurrentPath);
                    GameState->Bob.CurrentPathI = 0;                
                    GameState->PathEmpty = false;                    
                }
                
            }
            else
            {
                

                real32 NorthBorder = RoomA.RectangleVertices[0][1].Y;
                real32 SouthBorder = RoomA.RectangleVertices[0][0].Y;
                real32 EastBorder = RoomA.RectangleVertices[0][1].X;
                real32 WestBorder = RoomA.RectangleVertices[0][0].X;

                
                if(RoomA.RectangleCount > 1)
                {                    
                    if(RoomA.RectangleVertices[1][1].Y > NorthBorder)
                    {
                        NorthBorder = RoomA.RectangleVertices[1][1].Y;
                    }
                    if(RoomA.RectangleVertices[1][0].Y < SouthBorder)
                    {
                        SouthBorder = RoomA.RectangleVertices[1][0].Y;
                    }
                    if(RoomA.RectangleVertices[1][1].X > EastBorder)
                    {
                        EastBorder = RoomA.RectangleVertices[1][1].X;
                    }
                    if(RoomA.RectangleVertices[1][0].X < WestBorder)
                    {
                        WestBorder = RoomA.RectangleVertices[1][0].X;
                    }                    
                }
                
                
                uint32 NearbyHubI = 0;
                hub NearbyHub[2] = {};                
                real32 ThisInner = 0.0f;
                real32 Inner1 = 100.0f;
                real32 Inner2 = 101.0f;
                bool32 ValidHub = true;
                bool32 AdjacentDestHub = false;
                bool32 TwoNearbyHubs = false;
                uint32 NearbyHubWallCheckBF = 0;

                real32 ThisInnerDest = 0.0f;
                real32 ClosestInnerDest = 100.0f;
                bool32 ValidDestHub = true;
                uint32 DestHubWallCheck = 0;
                hub DestHub = {};
                bool32 AdjacentHub = false;
                bool32 NoFalseHub[2] = {};

                
                for(uint32 HubI = 0; HubI < RoomA.HubCount; ++HubI)
                {
                    
                    ThisInner = LengthSq(v2{GameState->Bob.Pos.X - RoomA.Hub[HubI].Pos.X, GameState->Bob.Pos.Y - RoomA.Hub[HubI].Pos.Y});
                    ThisInnerDest = LengthSq(v2{GameState->Bob.Dest.X - RoomA.Hub[HubI].Pos.X, GameState->Bob.Dest.Y - RoomA.Hub[HubI].Pos.Y});
                
                    // IncFix
                                        
                    if(ThisInner < Inner2)
                    {

                        NearbyHubWallCheckBF =                
                            ClearPathCheckAndFalseHubAssign(GameState->Bob.Pos,
                                                            RoomA.Hub[HubI].Pos,
                                                            &GameState->Bob.FalseHub[0].Pos,
                                                            (uint32 *)GameState->AptTileMap);
                        
                        ValidHub = (NearbyHubWallCheckBF & (0x1 << CAN_HUB_SKIP)) ? true : false;
                        
                        AdjacentHub = ((NearbyHubWallCheckBF & (0xF << HAVE_SAME_X)) || (NearbyHubWallCheckBF & (0xF << HAVE_SAME_Y))) ? true : false;
                        

                        if(ValidHub)
                        {
                            
                            if(ThisInner <= Inner1)
                            {
                                Inner2 = Inner1;
                                Inner1 = ThisInner;
                                NearbyHub[1] = NearbyHub[0];
                                NoFalseHub[1] = NoFalseHub[0];
                                NearbyHub[0] = RoomA.Hub[HubI];
                                NoFalseHub[0] = AdjacentHub;
                            }
                            else
                            {
                                Inner2 = ThisInner;
                                NearbyHub[1] = RoomA.Hub[HubI];
                                NoFalseHub[1] = AdjacentHub;
                            }

                            if(Inner2 < 99.0f)
                            {
                                TwoNearbyHubs = true;
                            }
                        }
                    }
                    
                    if(ThisInnerDest < ClosestInnerDest)
                    {
                        
                        DestHubWallCheck =
                            ClearPathCheckAndFalseHubAssign(RoomA.Hub[HubI].Pos,
                                                            GameState->Bob.Dest,
                                                            &GameState->Bob.FalseHub[1].Pos,
                                                            (uint32 *)GameState->AptTileMap);

                        ValidDestHub = (DestHubWallCheck & (0x1 << CAN_HUB_SKIP)) ? true : false;
                        //TODO: Need to add a 'z' false hub if not adjacent
                        AdjacentDestHub = ((DestHubWallCheck & (0x1 << HAVE_SAME_X)) || (DestHubWallCheck & (0x1 << HAVE_SAME_Y))) ? true : false;

                        if(ValidDestHub)
                        {
                            DestHub = RoomA.Hub[HubI];
                            ClosestInnerDest = ThisInnerDest;
                        }


                    }
                }
                                               
                Assert(NearbyHub[0].Char != 0);
                Assert(DestHub.Char != 0);

                //Check whether either of the NearbyHubs can reach the Dest. If not... pick the better hub based off of other checks.
                //
                //
                //

                //TODO: Could add more checks in here for which of the two hubs to go to.
                if(TwoNearbyHubs)
                {
                    real32 FirstHubToDest = LengthSq(v2{DestHub.Pos.X - NearbyHub[0].Pos.X, DestHub.Pos.Y - NearbyHub[0].Pos.Y});
                    real32 SecondHubToDest = LengthSq(v2{DestHub.Pos.X - NearbyHub[1].Pos.X, DestHub.Pos.Y - NearbyHub[1].Pos.Y});

                    NearbyHubI = (FirstHubToDest <= SecondHubToDest)?0:1;
                }
                else
                {
                    NearbyHubI = 0;
                }

                hub ThisHub = NearbyHub[NearbyHubI];
                hub BackTrackHub = {};
                
                uint8 PotentialPath[256] = {};
                uint32 PotentialI = 0;

                if(!NoFalseHub[NearbyHubI])
                {
                    PotentialPath[PotentialI] = GameState->Bob.FalseHub[0].Char;
                    ++PotentialI;
                }
                PotentialPath[PotentialI] = NearbyHub[NearbyHubI].Char;                

                //NOTE: This will be an array of bitflags, 0 meaning Tested, and 1 meaning untested. 
                uint32 TestedDirections[30] = {};
                uint32 TestedDI = 0;

                uint32 TileValue = 0;
                hub PossibleHub[4] = {};
                uint32 DirectionCameFrom = 100;
                bool32 ArrivedAtDestHub = false;
                

                //NOTE: this is the main loop
                while(!ArrivedAtDestHub)
                {

                    for(uint32 DI = 0; DI < MAX_NSEW; ++DI)
                    {
                        PossibleHub[DI].ID = 0;
                        PossibleHub[DI].Char = 0;
                        PossibleHub[DI].Pos.X = 0.0f;
                        PossibleHub[DI].Pos.Y = 0.0f;                        
                    }

                    if(DirectionCameFrom != N && !(TestedDirections[TestedDI] & (0xF << N)))
                    {
                        
                        //Test North Direction
                        for(real32 UnitStep = ThisHub.Pos.Y + 1; UnitStep < NorthBorder; ++UnitStep)
                        {
                            TileValue = GameState->AptTileMap[(uint32)UnitStep][(uint32)ThisHub.Pos.X];

                            if(TileValue == DOOR_TILE || IsObstacle(TileValue))
                            {
                                TestedDirections[TestedDI] += (1 << N);
                                break;
                            }
                            else if(TileValue == HUB_TILE)
                            {
                            
                                for(uint32 HI = 0; HI < RoomA.HubCount; ++HI)
                                {
                                    if(((uint32)RoomA.Hub[HI].Pos.X == (uint32)ThisHub.Pos.X) && ((uint32)RoomA.Hub[HI].Pos.Y == (uint32)UnitStep))
                                    {
                                        PossibleHub[N] = RoomA.Hub[HI];
                                        if(PossibleHub[N].Char == DestHub.Char)
                                        {
                                            ArrivedAtDestHub = true;                                            
                                        }
                                        break;
                                    }
                                }

                                if(PossibleHub[N].Char)
                                {
                                    break;
                                }
                            }                        
                        }
                    }

                    if(ArrivedAtDestHub)
                    {
                        break;
                    }
                    
                    if(DirectionCameFrom != S && !(TestedDirections[TestedDI] & (0xF << S)))
                    {
                        
                        //Test South Direction
                        for(real32 UnitStep = ThisHub.Pos.Y - 1; UnitStep > SouthBorder; --UnitStep)
                        {
                            TileValue = GameState->AptTileMap[(uint32)UnitStep][(uint32)ThisHub.Pos.X];

                            if(TileValue == DOOR_TILE || IsObstacle(TileValue))
                            {
                                TestedDirections[TestedDI] += (1 << S);
                                break;
                            }
                            else if(TileValue == HUB_TILE)
                            {
                            
                                for(uint32 HI = 0; HI < RoomA.HubCount; ++HI)
                                {
                               
                                    if(((uint32)RoomA.Hub[HI].Pos.X == (uint32)ThisHub.Pos.X) && ((uint32)RoomA.Hub[HI].Pos.Y == (uint32)UnitStep))
                                    {
                                        PossibleHub[S] = RoomA.Hub[HI];
                                        if(PossibleHub[S].Char == DestHub.Char)
                                        {
                                            ArrivedAtDestHub = true;
                                        }
                                        break;
                                    }
                                }

                                if(PossibleHub[S].Char)
                                {
                                    break;
                                }
                            }                        
                        }                        
                    }

                    if(ArrivedAtDestHub)
                    {
                        break;
                    }

                    if(DirectionCameFrom != E && !(TestedDirections[TestedDI] & (0xF << E)))
                    {
                        
                        //Test East Direction
                        for(real32 UnitStep = ThisHub.Pos.X + 1; UnitStep < EastBorder; ++UnitStep)
                        {
                            TileValue = GameState->AptTileMap[(uint32)ThisHub.Pos.Y][(uint32)UnitStep];

                            if(TileValue == DOOR_TILE || IsObstacle(TileValue))
                            {
                                TestedDirections[TestedDI] += (1 << E);
                                break;
                            }
                            else if(TileValue == HUB_TILE)
                            {
                            
                                for(uint32 HI = 0; HI < RoomA.HubCount; ++HI)
                                {
                               
                                    if(((uint32)RoomA.Hub[HI].Pos.Y == (uint32)ThisHub.Pos.Y) && ((uint32)RoomA.Hub[HI].Pos.X == (uint32)UnitStep))
                                    {
                                        PossibleHub[E] = RoomA.Hub[HI];
                                        if(PossibleHub[E].Char == DestHub.Char)
                                        {
                                            ArrivedAtDestHub = true;
                                        }
                                        break;
                                    }
                                }

                                if(PossibleHub[E].Char)
                                {
                                    break;
                                }
                            }                        
                        }
                    }

                    if(ArrivedAtDestHub)
                    {
                        break;
                    }
                    
                    if(DirectionCameFrom != W && !(TestedDirections[TestedDI] & (0xF << W)))
                    {
                        
                        //Test West Direction
                        for(real32 UnitStep = ThisHub.Pos.X - 1; UnitStep > WestBorder; --UnitStep)
                        {

                            TileValue = GameState->AptTileMap[(uint32)ThisHub.Pos.Y][(uint32)UnitStep];

                            if(TileValue == DOOR_TILE || IsObstacle(TileValue))
                            {
                                TestedDirections[TestedDI] += (1 << W);
                                break;
                            }
                            else if(TileValue == HUB_TILE)
                            {
                            
                                for(uint32 HI = 0; HI < RoomA.HubCount; ++HI)
                                {
                               
                                    if(((uint32)RoomA.Hub[HI].Pos.Y == (uint32)ThisHub.Pos.Y) && ((uint32)RoomA.Hub[HI].Pos.X == (uint32)UnitStep))
                                    {
                                        PossibleHub[W] = RoomA.Hub[HI];
                                        if(PossibleHub[W].Char == DestHub.Char)
                                        {
                                            ArrivedAtDestHub = true;
                                        }
                                        break;
                                    }
                                }

                                if(PossibleHub[W].Char)
                                {
                                    break;
                                }
                            }                        
                        }
                    }

                    if(ArrivedAtDestHub)
                    {
                        break;
                    }
                    
                    real32 ThisHubToDestHub = 0.0f;
                    real32 ClosestHubToDestHub = 100000.0f;
                    uint32 ChosenHubI = 0;
                    uint32 InvalidDirections = 0;

                    
                    for(uint32 DI = 0; DI < MAX_NSEW; ++DI)
                    {
                        
                        if(DI != DirectionCameFrom && PossibleHub[DI].Char)
                        {                            
                            ThisHubToDestHub = LengthSq(v2{DestHub.Pos.X - PossibleHub[DI].Pos.X, DestHub.Pos.Y - PossibleHub[DI].Pos.Y});

                            if(ThisHubToDestHub < ClosestHubToDestHub)
                            {
                                ClosestHubToDestHub = ThisHubToDestHub;
                                ChosenHubI = DI;                                
                            }                            
                        }
                        else
                        {
                            ++InvalidDirections;
                        }
                    }

                    if(InvalidDirections == 4)
                    {
                        ThisHub = BackTrackHub;
                        TestedDirections[TestedDI] = 0;
                        --TestedDI;

                        PotentialPath[PotentialI] = 0;
                        --PotentialI;

                        if(TestedDirections[TestedDI] == 0)
                        {
                            uint8 HuhThatWasntSupposedToHappen = 0;
                        }
                        //Decrement PathI
                    }
                    else 
                    {
                        TestedDirections[TestedDI] += (1 << ChosenHubI);
                        ++TestedDI;
                    
                        switch(ChosenHubI)
                        {
                            case N :
                            {
                                DirectionCameFrom = S;                            
                            }break;
                            case S :
                            {
                                DirectionCameFrom = N;
                            }break;
                            case E :
                            {
                                DirectionCameFrom = W;
                            }break;
                            case W :
                            {
                                DirectionCameFrom = E;
                            }break;  
                        };

                        BackTrackHub = ThisHub;
                        ThisHub = PossibleHub[ChosenHubI];
                        ++PotentialI;
                        PotentialPath[PotentialI] = ThisHub.Char;                                             
                    }
                }
                
                BackTrackHub = ThisHub;
                ThisHub = DestHub;
                
                //Now test to see if you can get there from BackTrack hub. If not, write the final char
               
                uint32 BackTrackBF =
                    ClearPathCheckAndFalseHubAssign(BackTrackHub.Pos,
                                                    GameState->Bob.Dest,
                                                    &GameState->Bob.FalseHub[1].Pos,
                                                    (uint32 *)GameState->AptTileMap);
                
                bool32 ValidBackTrackToDest = (BackTrackBF & (0x1 << CAN_HUB_SKIP)) ? true : false;
                bool32 BackTrackIsAdjacentToDest = ((BackTrackBF & (0x1 << HAVE_SAME_X)) || (DestHubWallCheck & (0xF << HAVE_SAME_Y))) ? true : false;
                        
                if(ValidBackTrackToDest)
                {                 
                    if(!BackTrackIsAdjacentToDest)
                    {
                        ++PotentialI;
                        PotentialPath[PotentialI] = GameState->Bob.FalseHub[1].Char;
                    }                                                    
                }
                else
                {
                    
                    //We've found the right path!?
                    ++PotentialI;
                    PotentialPath[PotentialI] = ThisHub.Char;                    
                }
                CopyString(PotentialPath, GameState->Bob.CurrentPath);
                GameState->Bob.CurrentPathI = 0;
                //GameState->Bob.EmptyPathHub.Pos = GameState->Bob.Dest;
                //GameState->Bob.EmptyPathHub.ID = 0;
                //GameState->Bob.EmptyPathHub.Char = '0';
                GameState->PathEmpty = false;
                
                
                
                //Nowbasically loop back to the beginning of uno, using the NearbyHub.Pos as the startpos and the dest as the end pos once more.
                //But that doesn't work?
                //TODO: I need to split ClearPathCheckandFalseHub into two or more functions. One is used for finding things adjacent,
                //the other used for finding things with a false hub. Or keep the current function as it is, but also make one just for finding
                //adjacent stuff (for hub pathfinding). Remember that if either one of the Hubs has a 0 in its ID, we can just use PathToDoor functionality.
                //IMPORTANT: Check iphone note for good idea!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11
                //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            }
        }
    }
    
    
    //00D
    // Bob's Pathing
    //If Bob is standing in the middle of the next hub    
    if(GameState->Bob.IsMoving &&
       (AbsDiff(GameState->Bob.Pos.X, GameState->Bob.NextHub.Pos.X) < 0.04f) &&
       (AbsDiff(GameState->Bob.Pos.Y, GameState->Bob.NextHub.Pos.Y) < 0.04f))
    {
        
        /*if(GameState->Bob.NextHub.ID > 0)
          {
          GameState->Bob.PriorHub = GameState->Bob.NextHub;
          }*/
        
        uint8 *CharPtr = &GameState->Bob.CurrentPath[GameState->Bob.CurrentPathI];
        if(*CharPtr == 0 || GameState->PathEmpty)
        {

            //Adjust the Dest to be a place that the npc can actually stand, not in the middle of a table.
            //Set EmptyPathHub equal to the new Dest

            GameState->PathEmpty = true;
            
            if(GameState->Bob.EmptyPathHub.Char != 0)
            {
                GameState->Bob.NextHub = GameState->Bob.EmptyPathHub;                
                GameState->Bob.EmptyPathHub.ID = 0;
                GameState->Bob.EmptyPathHub.Char = 0;
            }
            else
            {
                //NOTE: I believe this code was made unnecessary by AdjustDest
                /*v2 DirectionToDest = GetNPCSpeedVector(GameState->Bob.Pos, GameState->Bob.Dest);
                tile EndTile = {};
                v2 StepBackOne = {};
                uint32 WalkDirection = 0;
            
                if(AbsValue(DirectionToDest.X) >= AbsValue(DirectionToDest.Y))
                {
                    if(DirectionToDest.X < 0)
                    {
                        StepBackOne.X = 1.01f;
                        WalkDirection = W;

                        //ghost_path.h
                        EndTile = TipToe(GameState->Bob.Pos,
                                         GameState->Bob.Dest,
                                         W, false, (uint32 *)GameState->AptTileMap);
                    }
                    else
                    {
                        StepBackOne.X = -0.01f;
                        WalkDirection = E;
                        EndTile = TipToe(GameState->Bob.Pos,
                                         GameState->Bob.Dest,
                                         E, false, (uint32 *)GameState->AptTileMap);
                    }
                }
                else
                {
                    if(DirectionToDest.Y < 0)
                    {
                        StepBackOne.Y = 1.01f;
                        WalkDirection = S;
                        EndTile = TipToe(GameState->Bob.Pos,
                                         GameState->Bob.Dest,
                                         S, false, (uint32 *)GameState->AptTileMap);
                    }
                    else
                    {
                        StepBackOne.Y = -0.01f;
                        WalkDirection = N;
                        EndTile = TipToe(GameState->Bob.Pos,
                                         GameState->Bob.Dest,
                                         N, false, (uint32 *)GameState->AptTileMap);
                    }
                }


                if(EndTile.Value != MAX_TILE_TYPE)
                {
                    //There's something impeding us from our dest.
                    if(WalkDirection == N || WalkDirection == S)
                    {
                        GameState->Bob.Dest.Y = (uint32)GameState->Bob.Dest.Y + StepBackOne.Y;
                    }
                    else
                    {
                        GameState->Bob.Dest.X = (uint32)GameState->Bob.Dest.X + StepBackOne.X;
                    }
                    
                }            
                */
                GameState->Bob.EmptyPathHub.Pos = GameState->Bob.Dest;
                GameState->Bob.EmptyPathHub.Char = 0;
                GameState->Bob.NextHub = GameState->Bob.EmptyPathHub;
            }

                   
            
        }

        if(!GameState->PathEmpty)
        {
            ++GameState->Bob.CurrentPathI;
            if(*CharPtr == 'a')
            {
                GameState->Bob.NextHub = GameState->Bob.FalseHub[0];
            }
            else if(*CharPtr == 'z')
            {
                GameState->Bob.NextHub = GameState->Bob.FalseHub[1];
            }
            else
            {
                uint8 NextHubID = *CharPtr;
                uint32 RoomOfHub = GetRoom(&GameState->L0, GameState->Bob.Pos);             
                GameState->Bob.NextHub = GetHub(&GameState->L0.Rooms[RoomOfHub], NextHubID);
            }
        }

        Assert(GameState->Bob.NextHub.Pos.X > 0.9f && GameState->Bob.NextHub.Pos.Y > 0.9f);
        GameState->Bob.Speed = GetNPCSpeedVector(GameState->Bob.Pos, GameState->Bob.NextHub.Pos);
        GameState->Bob.Speed.X *= 2.0f;
        GameState->Bob.Speed.Y *= 2.0f;
    }
    
    if(!GameState->Bob.IsMoving)
    {
        GameState->Bob.Speed = v2{0.0f, 0.0f};
    }
    
    GameState->Bob.Pos = GameState->Bob.Pos + (GameState->Bob.Speed*Input->dtForFrame);

    if(GameState->Bob.Inventory[GameState->ObjMode])
    {
        GameState->Obj[GameState->ObjMode].Pos = GameState->Bob.Pos;
    }




    //IMPORTANT: Start of World Rendering
    /*
      rgb LavaColor = {0.3f, 0.0, 0.7f};
      DrawRectangle(Buffer,
      v2{Buffer->Width-500.0f,0.0f},
      v2{(real32)Buffer->Width, (real32)Buffer->Height},
      LavaColor);
    */

    real32 WorldScreenCenterX = 0.5f*(Buffer->Width - 500);
    real32 WorldScreenCenterY = 0.5f* Buffer->Height;
    real32 MenuScreenLeft = Buffer->Width - 500.0f;
    real32 WhiteMenuBorder = MenuScreenLeft + 20;

    rgb Black = {};
    //00E
    //Render Lava Tablecloth    
    DrawRectangle(Buffer,
                  v2{0.0f,0.0f},
                  v2{MenuScreenLeft, (real32)Buffer->Height},
                  Black);

    rgb White = {1.0f,1.0f,1.0f};
    DrawRectangle(Buffer,
                  v2{MenuScreenLeft, 0.0f},
                  v2{WhiteMenuBorder, (real32)Buffer->Height},
                  White);

    rgb DarkGray = {0.1f,0.1f,0.1f};
    DrawRectangle(Buffer,
                  v2{WhiteMenuBorder, 0.0f},
                  v2{(real32)Buffer->Width, (real32)Buffer->Height},
                  DarkGray);

    
    
    //IMPORTANT: Start of Menu Rendering

    //Render clock
        
    if(!GameState->Clock.Sec)
    {
        (GameState->ObjBlinkTimer)?(GameState->ObjBlinkTimer = 0):(GameState->ObjBlinkTimer = 1);
                  
        if(GameState->Clock.Min % 5 == 0)
        {
            GameState->CurrentClockDigits[4] = GameState->ClockDigits[GameState->Clock.Min%10];
            GameState->CurrentClockDigits[3] = GameState->ClockDigits[GameState->Clock.Min/10];
        }        
        if(!GameState->Clock.Min)
        {
            if((GameState->Clock.Hr > 9 && GameState->Clock.Hr < 13) || (GameState->Clock.Hr > 21) || !GameState->Clock.Hr)
            {
                GameState->CurrentClockDigits[0] = GameState->ClockDigits[1];
            }
            else
            {
                GameState->CurrentClockDigits[0] = GameState->ClockDigits[0];
            }

            if(!GameState->Clock.Hr)
            {
                GameState->CurrentClockDigits[1] = GameState->ClockDigits[2];
            }
            else if(GameState->Clock.Hr < 13)
            {
                GameState->CurrentClockDigits[1] = GameState->ClockDigits[GameState->Clock.Hr % 10];
            }            
            else 
            {
                GameState->CurrentClockDigits[1] = GameState->ClockDigits[(GameState->Clock.Hr - 2) % 10];
            }
        }
        
        if((GameState->Clock.Min < 5) && ((GameState->Clock.Hr == 12) || !GameState->Clock.Hr))
        {
            GameState->CurrentClockDigits[5] = {};
        }
        else if(GameState->Clock.Hr < 12)
        {
            GameState->CurrentClockDigits[5] = GameState->ClockDigits[10];
        }
        else
        {
            GameState->CurrentClockDigits[5] = GameState->ClockDigits[11];
        }
    }
        
    for(uint32 CDI = 0; CDI < 5; ++CDI)
    {                
        DrawBitmap(Buffer,
                   &GameState->CurrentClockDigits[CDI],
                   ((real32)WhiteMenuBorder) + 10.0f + (CDI * (GameState->CurrentClockDigits[CDI].Width+5)),
                   (real32)Buffer->Height/2 - 100);        
    }

    DrawBitmap(Buffer,
               &GameState->CurrentClockDigits[5],
               ((real32)WhiteMenuBorder) + 100.0f + (5 * (GameState->CurrentClockDigits[0].Width)),
               (real32)Buffer->Height/2 - 100);        
            

    if(GameState->CurrentWords.TimeDuration)
    {
        
        if(GameState->CurrentWords.TimeDuration%100 < 50)
        {
            DrawBitmap(Buffer,
                   &GameState->SpeechIcon,
                   (real32)WhiteMenuBorder + GameState->BobIcon.Width + 5.0f,
                   10.0f);
        }
        
        --GameState->CurrentWords.TimeDuration;
        DrawBitmap(Buffer,
                   &GameState->CurrentWords.BMP,
                   (real32)WhiteMenuBorder + 5.0f,
                   100.0f);

    }

    DrawBitmap(Buffer,
                   &GameState->BobIcon,
                   (real32)WhiteMenuBorder,
                   10.0f);

    //Render Approriate Apartment layers

    //Check which Apartment layers need to be shown based on the player's position
    //Calculate the appropriate Min and Max based on the BMP's size, offset by the player's position relative to 

    /*
      DrawBitmap(Buffer,
      &GameState->AptLayers,
      ScreenCenterX - GameState->PlayerPos.X*GameState->MetersToPixels,
      GameState->PlayerPos.Y*GameState->MetersToPixels + ScreenCenterY - GameState->AptLayers.Height);
    */

    // NOTE:  Player In Bedroom (Door closed)
    //FIRST (A)
    //Floor AND
    //North wall AND
    //North Furniture AND
    //Mask
    //LAST (Z)
    //Ceiling AND
    //(Door Frame)

    
    
    if(GameState->ShowTvGuide)
    {
        DrawWorldBitmap(Buffer,
                        &GameState->TvGuideBMP,
                        0.0f, 0.0f);
    }
    else
    {
        entity Entity[20] = {};
        uint32 EntityCount = 0;
    
        entity Layer[2] = {};    
        uint32 RoomID = GetRoom(&GameState->L0, GameState->PlayerPos);
    
        v2 LToPlayer = {};
    
        switch(RoomID)
        {
            case MAIN_ROOM:
            {
                //TODO: Check if Front door is open or closed

                Layer[0] = GameState->AptLayer[MAIN_LAYER_A];
                Layer[1] = GameState->AptLayer[MAIN_LAYER_Z];

                LToPlayer.X = (Layer[0].Pos.X - GameState->PlayerPos.X)*GameState->MetersToPixels;
                LToPlayer.Y = (GameState->PlayerPos.Y - Layer[0].Pos.Y)*GameState->MetersToPixels;
            
                Layer[0].ScreenPos.X = WorldScreenCenterX + LToPlayer.X;
                Layer[0].ScreenPos.Y = WorldScreenCenterY + LToPlayer.Y;
            
                Layer[1].ScreenPos.X = WorldScreenCenterX + LToPlayer.X;
                Layer[1].ScreenPos.Y = WorldScreenCenterY + LToPlayer.Y;

                Entity[EntityCount] = GameState->AptLayer[MAIN_WALL_N];
                Entity[EntityCount].ScreenPos.X = Layer[0].ScreenPos.X + 5*6;
                Entity[EntityCount].ScreenPos.Y = Layer[0].ScreenPos.Y + 5*11;
                ++EntityCount;
            
                Entity[EntityCount] = GameState->AptLayer[MAIN_DOORFRAME_N];
                Entity[EntityCount].ScreenPos.X = Layer[0].ScreenPos.X + 5*6;
                Entity[EntityCount].ScreenPos.Y = Layer[0].ScreenPos.Y + 5*33;
                ++EntityCount;
            
                Entity[EntityCount] = GameState->AptLayer[MAIN_WALL];
                Entity[EntityCount].ScreenPos.X = Layer[0].ScreenPos.X + 5*60;
                Entity[EntityCount].ScreenPos.Y = Layer[0].ScreenPos.Y + 5*59;
                ++EntityCount;
            
                Entity[EntityCount] = GameState->AptLayer[MAIN_CHAIR];
                Entity[EntityCount].ScreenPos.X = Layer[0].ScreenPos.X + 5*(115);
                Entity[EntityCount].ScreenPos.Y = Layer[0].ScreenPos.Y + 5*85;
                ++EntityCount;                
            
                Entity[EntityCount] = GameState->AptLayer[MAIN_PINGPONG_TABLE];
                Entity[EntityCount].ScreenPos.X = Layer[0].ScreenPos.X + 5*66;
                Entity[EntityCount].ScreenPos.Y = Layer[0].ScreenPos.Y + 5*138;
                ++EntityCount;
            
                Entity[EntityCount] = GameState->AptLayer[MAIN_DOORFRAME_S];
                Entity[EntityCount].ScreenPos.X = Layer[0].ScreenPos.X + 5*6;
                Entity[EntityCount].ScreenPos.Y = Layer[0].ScreenPos.Y + 5*153;
                ++EntityCount;
            
            }break;
            case BED_ROOM:
            {
                //TODO: Check if Door is open or closed
                Layer[0] = GameState->AptLayer[BED_LAYER_A];
                Layer[1] = GameState->AptLayer[BED_LAYER_Z];
                LToPlayer.X = (Layer[0].Pos.X - GameState->PlayerPos.X)*GameState->MetersToPixels;
                LToPlayer.Y = (GameState->PlayerPos.Y - Layer[0].Pos.Y)*GameState->MetersToPixels;
            
                Layer[0].ScreenPos.X = WorldScreenCenterX + LToPlayer.X;
                Layer[0].ScreenPos.Y = WorldScreenCenterY + LToPlayer.Y;
            
                Layer[1].ScreenPos.X = WorldScreenCenterX + LToPlayer.X;
                Layer[1].ScreenPos.Y = WorldScreenCenterY + LToPlayer.Y;

                for(uint32 LI = BED_LAYER_A + 1; LI < BED_LAYER_Z; ++LI)
                {
                    Entity[EntityCount] = GameState->AptLayer[LI];
                    Entity[EntityCount].ScreenPos = Layer[0].ScreenPos;
                    ++EntityCount;
                }
            }break;
            case BATH_ROOM:
            {
                Layer[0] = GameState->AptLayer[BATH_LAYER_A];
                Layer[1] = GameState->AptLayer[BATH_LAYER_Z];

                LToPlayer.X = (Layer[0].Pos.X - GameState->PlayerPos.X)*GameState->MetersToPixels;
                LToPlayer.Y = (GameState->PlayerPos.Y - Layer[0].Pos.Y)*GameState->MetersToPixels;
            
                Layer[0].ScreenPos.X = WorldScreenCenterX + LToPlayer.X;
                Layer[0].ScreenPos.Y = WorldScreenCenterY + LToPlayer.Y;
            
                Layer[1].ScreenPos.X = WorldScreenCenterX + LToPlayer.X;
                Layer[1].ScreenPos.Y = WorldScreenCenterY + LToPlayer.Y;

            
                Entity[EntityCount] = GameState->AptLayer[BATH_WALL];
                Entity[EntityCount].ScreenPos.X = Layer[0].ScreenPos.X;
                Entity[EntityCount].ScreenPos.Y = Layer[0].ScreenPos.Y;
                ++EntityCount;
            
                Entity[EntityCount] = GameState->AptLayer[BATH_DOORFRAME];
                Entity[EntityCount].ScreenPos.X = Layer[0].ScreenPos.X + 5*119;
                Entity[EntityCount].ScreenPos.Y = Layer[0].ScreenPos.Y + 5*33;
                ++EntityCount;
            
                Entity[EntityCount] = GameState->AptLayer[BATH_TOILET];
                Entity[EntityCount].ScreenPos.X = Layer[0].ScreenPos.X + 5*53;
                Entity[EntityCount].ScreenPos.Y = Layer[0].ScreenPos.Y + 5*56;
                ++EntityCount;
                        


/*            for(uint32 LI = BATH_LAYER_A + 1; LI < BATH_LAYER_Z; ++LI)
              {
              Entity[EntityCount] = GameState->AptLayer[LI];
              Entity[EntityCount].ScreenPos.X = Layer[0].ScreenPos.X + ;
              Entity[EntityCount].ScreenPos.Y = Layer[0].ScreenPos.Y + ;
              ++EntityCount;
              }*/
            }break;
            case KITCHEN_ROOM:
            case TV_ROOM:
            {
                Layer[0] = GameState->AptLayer[TV_KITCHEN_LAYER_A];
                Layer[1] = GameState->AptLayer[TV_KITCHEN_LAYER_Z];

                LToPlayer.X = (Layer[0].Pos.X - GameState->PlayerPos.X)*GameState->MetersToPixels;
                LToPlayer.Y = (GameState->PlayerPos.Y - Layer[0].Pos.Y)*GameState->MetersToPixels;
            
                Layer[0].ScreenPos.X = WorldScreenCenterX + LToPlayer.X;
                Layer[0].ScreenPos.Y = WorldScreenCenterY + LToPlayer.Y;
            
                Layer[1].ScreenPos.X = WorldScreenCenterX + LToPlayer.X;
                Layer[1].ScreenPos.Y = WorldScreenCenterY + LToPlayer.Y;

                        
                Entity[EntityCount] = GameState->AptLayer[TV_WALL];
                Entity[EntityCount].ScreenPos.X = Layer[0].ScreenPos.X + 5*156;
                Entity[EntityCount].ScreenPos.Y = Layer[0].ScreenPos.Y + 5*6;
                ++EntityCount;

                Entity[EntityCount].BMP = GameState->TvScreen[GameState->ChannelI];
                Entity[EntityCount].Pos.Y = GameState->AptLayer[TV_WALL].Pos.Y - 0.05f;
                Entity[EntityCount].ScreenPos.X = Entity[EntityCount - 1].ScreenPos.X + 5*(25);
                Entity[EntityCount].ScreenPos.Y = Entity[EntityCount - 1].ScreenPos.Y + 5*(5);
                ++EntityCount;
                
                if(GameState->ChannelTimer)
                {
                 
                    Entity[EntityCount].BMP = GameState->Channel[GameState->ChannelI];
                    Entity[EntityCount].Pos.Y = Entity[EntityCount - 1].Pos.Y - 0.5f;
                    Entity[EntityCount].ScreenPos.X = Entity[EntityCount - 1].ScreenPos.X + 5*(57);
                    Entity[EntityCount].ScreenPos.Y = Entity[EntityCount - 1].ScreenPos.Y + 5*(1);
                    ++EntityCount;
                }

                                    
                Entity[EntityCount] = GameState->AptLayer[KITCHEN_WALL];
                Entity[EntityCount].ScreenPos.X = Layer[0].ScreenPos.X + 5*6;
                Entity[EntityCount].ScreenPos.Y = Layer[0].ScreenPos.Y + 5*24;
                ++EntityCount;

                                    
                Entity[EntityCount] = GameState->AptLayer[TV_TABLE];
                Entity[EntityCount].ScreenPos.X = Layer[0].ScreenPos.X + 5*180;
                Entity[EntityCount].ScreenPos.Y = Layer[0].ScreenPos.Y + 5*63;
                ++EntityCount;

                                    
                Entity[EntityCount] = GameState->AptLayer[TV_COUCH_VERTI];
                Entity[EntityCount].ScreenPos.X = Layer[0].ScreenPos.X + 5*251;
                Entity[EntityCount].ScreenPos.Y = Layer[0].ScreenPos.Y + 5*69;
                ++EntityCount;

                                    
                Entity[EntityCount] = GameState->AptLayer[TV_COUCH_HORI];
                Entity[EntityCount].ScreenPos.X = Layer[0].ScreenPos.X + 5*179;
                Entity[EntityCount].ScreenPos.Y = Layer[0].ScreenPos.Y + 5*87;
                ++EntityCount;

                                    
                Entity[EntityCount] = GameState->AptLayer[KITCHEN_STOOL];
                Entity[EntityCount].ScreenPos.X = Layer[0].ScreenPos.X + 5*25;
                Entity[EntityCount].ScreenPos.Y = Layer[0].ScreenPos.Y + 5*88;
                ++EntityCount;

                                    
                Entity[EntityCount] = GameState->AptLayer[KITCHEN_TABLE];
                Entity[EntityCount].ScreenPos.X = Layer[0].ScreenPos.X + 5*36;
                Entity[EntityCount].ScreenPos.Y = Layer[0].ScreenPos.Y + 5*85;
                ++EntityCount;



            }break;        
            case CLOSET_MAIN_ROOM:
            {
                Layer[0] = GameState->AptLayer[MAINCLOSET_LAYER_A];
                Layer[1] = GameState->AptLayer[MAINCLOSET_LAYER_Z];

                LToPlayer.X = (Layer[0].Pos.X - GameState->PlayerPos.X)*GameState->MetersToPixels;
                LToPlayer.Y = (GameState->PlayerPos.Y - Layer[0].Pos.Y)*GameState->MetersToPixels;
            
                Layer[0].ScreenPos.X = WorldScreenCenterX + LToPlayer.X;
                Layer[0].ScreenPos.Y = WorldScreenCenterY + LToPlayer.Y;
            
                Layer[1].ScreenPos.X = WorldScreenCenterX + LToPlayer.X;
                Layer[1].ScreenPos.Y = WorldScreenCenterY + LToPlayer.Y;
            }
            break;
            case CLOSET_BED_ROOM:
            {
                Layer[0] = GameState->AptLayer[BEDCLOSET_LAYER_A];
                Layer[1] = GameState->AptLayer[BEDCLOSET_LAYER_Z];

                LToPlayer.X = (Layer[0].Pos.X - GameState->PlayerPos.X)*GameState->MetersToPixels;
                LToPlayer.Y = (GameState->PlayerPos.Y - Layer[0].Pos.Y)*GameState->MetersToPixels;
            
                Layer[0].ScreenPos.X = WorldScreenCenterX + LToPlayer.X;
                Layer[0].ScreenPos.Y = WorldScreenCenterY + LToPlayer.Y;
            
                Layer[1].ScreenPos.X = WorldScreenCenterX + LToPlayer.X;
                Layer[1].ScreenPos.Y = WorldScreenCenterY + LToPlayer.Y;

                for(uint32 LI = BEDCLOSET_LAYER_A + 1; LI < BEDCLOSET_LAYER_Z; ++LI)
                {
                    Entity[EntityCount] = GameState->AptLayer[LI];
                    Entity[EntityCount].ScreenPos = Layer[0].ScreenPos;
                    ++EntityCount;
                }
            }break;
            case OUTDOORS_ROOM:
            {
                Layer[0] = GameState->AptLayer[OUTDOORS_LAYER_A];
            

                LToPlayer.X = (Layer[0].Pos.X - GameState->PlayerPos.X)*GameState->MetersToPixels;
                LToPlayer.Y = (GameState->PlayerPos.Y - Layer[0].Pos.Y)*GameState->MetersToPixels;
            
                Layer[0].ScreenPos.X = WorldScreenCenterX + LToPlayer.X;
                Layer[0].ScreenPos.Y = WorldScreenCenterY + LToPlayer.Y;

                Layer[1] = GameState->AptLayer[OUTDOORS_LAYER_A];
                Layer[1].BMP.Height = 0;
                Layer[1].BMP.Width = 0;
                        
                for(uint32 LI = OUTDOORS_LAYER_A + 1; LI < OUTDOORS_LAYER_Z; ++LI)
                {
                    Entity[EntityCount] = GameState->AptLayer[LI];
                    Entity[EntityCount].ScreenPos = Layer[0].ScreenPos;
                    ++EntityCount;
                }
            }break;
        };


        DrawWorldBitmap(Buffer,
                        &Layer[0].BMP,               
                        Layer[0].ScreenPos.X,
                        Layer[0].ScreenPos.Y);
               
        
    

        //Sort the in-between furniture, items, and characters based on their Y coordinates
        //maybe TODO: Add walls as entities if you don't want to use the individual room mask. 

        //Position, BMP
    
        v2 SpriteDims = {GameState->PlayerSouthBitmap[0].Width/GameState->MetersToPixels,
                         GameState->PlayerSouthBitmap[0].Height/GameState->MetersToPixels};
    
        if(!GameState->ObjMode && !GameState->SpiritMode)
        {
        
            if(!PlayerIsMoving)
            {
                GameState->CurrentPlayerBitmapI = 0;
                GameState->AnimationTimer = 0;
            }
            else
            {
            
                if(GameState->AnimationTimer++ > 4)
                {
                    GameState->AnimationTimer = 0;
                
                    if(++GameState->CurrentPlayerBitmapI > 7)
                    {
                        GameState->CurrentPlayerBitmapI = 0;
                    }                
                }                        
            }
                    
            switch(GameState->FacingDirection)
            {
                case N: GameState->CurrentPlayerBitmap = GameState->PlayerNorthBitmap[GameState->CurrentPlayerBitmapI]; break;
                case S: GameState->CurrentPlayerBitmap = GameState->PlayerSouthBitmap[GameState->CurrentPlayerBitmapI]; break;
                case E: GameState->CurrentPlayerBitmap = GameState->PlayerEastBitmap[GameState->CurrentPlayerBitmapI]; break;
                case W: GameState->CurrentPlayerBitmap = GameState->PlayerWestBitmap[GameState->CurrentPlayerBitmapI]; break;
                default: Assert("Unknown player direction");
            };
        
            Entity[EntityCount].Pos = GameState->PlayerPos;        
            Entity[EntityCount].ScreenPos.X = WorldScreenCenterX - (SpriteDims.X/2)*GameState->MetersToPixels;
            Entity[EntityCount].ScreenPos.Y = WorldScreenCenterY - (SpriteDims.Y - /*TODO:MAGIC NUMBER*/1.0f)*GameState->MetersToPixels;
            Entity[EntityCount].BMP = GameState->CurrentPlayerBitmap;
            ++EntityCount;
        }
    
    
        
    
        //TODO: Art and code for rendering spirit mode
    

        //For all other entities, check whether their objects are within rendering range before adding them to the entity array.

        //NPCs
        //Items
        //Furniture
    
        v2 NPCCorner[4];
        //Sprite is 16x25 * 500%
    
        //real32 SpriteFootOffset = .2f;
        //TODO: This needs work, at least as far as the Y dimension is concerned I suppose.
        //Try using the values calculated in the DrawWorldBitmap calls for Sprites
        NPCCorner[0] = {GameState->Bob.Pos.X - SpriteDims.X/2, GameState->Bob.Pos.Y + SpriteDims.Y};
        NPCCorner[1] = {GameState->Bob.Pos.X + SpriteDims.X/2, GameState->Bob.Pos.Y + SpriteDims.Y};
        NPCCorner[2] = {GameState->Bob.Pos.X - SpriteDims.X/2, GameState->Bob.Pos.Y - SpriteDims.Y};
        NPCCorner[3] = {GameState->Bob.Pos.X + SpriteDims.X/2, GameState->Bob.Pos.Y - SpriteDims.Y};    
    
        v2 NPCToPlayer = {};
        bool32 RenderNPC = false;
        for(uint32 NPCCornerI = 0; NPCCornerI < ArrayCount(NPCCorner); ++NPCCornerI)
        {
            NPCToPlayer.X = AbsDiff(GameState->PlayerPos.X, NPCCorner[NPCCornerI].X);
            NPCToPlayer.Y = AbsDiff(GameState->PlayerPos.Y, NPCCorner[NPCCornerI].Y);
            if((NPCToPlayer.X < (WorldScreenCenterX/GameState->MetersToPixels)) &&
               (NPCToPlayer.Y < (WorldScreenCenterY/GameState->MetersToPixels)))
            {
                RenderNPC = true;
                NPCToPlayer.X = (GameState->Bob.Pos.X - GameState->PlayerPos.X)*GameState->MetersToPixels;
                NPCToPlayer.Y = (GameState->PlayerPos.Y - GameState->Bob.Pos.Y)*GameState->MetersToPixels;
                break;
            }
        }
    
        if(RenderNPC)
        {
            //TODO:
            /*if(GameState->AptTileMap[(uint32)GameState->Bob.Pos.Y][(uint32)GameState->Bob.Pos.X] == CHAIR_TILE)
              {
              //Chair has a specific pos and direction.
              //Tell the code that npc is directly north of that position,
              //And use the BMP that corresponds with the direction that that chair is facing.
              }*/
        
            // NOTE: DrawRectangle Code
            v2 NPCMin = {(WorldScreenCenterX + NPCToPlayer.X) - (PlayerRadius*GameState->MetersToPixels), (WorldScreenCenterY + NPCToPlayer.Y) - (PlayerRadius*GameState->MetersToPixels)};
            v2 NPCMax = {NPCMin.X + 2*PlayerRadius*GameState->MetersToPixels, (WorldScreenCenterY + NPCToPlayer.Y) + (PlayerRadius*GameState->MetersToPixels)};
            rgb NPCRGB = {0.25f, 1.0f, 0.25f};

        
            if(GameState->Bob.IsFrightened && GameState->Bob.DetectsPlayer)
            {
            
                if(--GameState->Bob.IsFrightened == 0)
                {
                    GameState->DeathScreen = true;
                }
            }        
            else if(GameState->Bob.DetectsPlayer)
            {
                //NPCRGB.R = 1.0f;
                GameState->Bob.IsFrightened = 25;            
            }
            else if(GameState->Bob.IsFrightened)
            {
                GameState->Bob.IsFrightened = 0;
            }
        
            /*
              else if(GameState->Bob.DetectsObj)
              {
              NPCRGB.R = 75.0f/255.0f;
              NPCRGB.G = 0.0f/255.0f;
              NPCRGB.B = 130.0f/255.0f;
              }
        
              DrawWorldRectangle(Buffer,
              NPCMin,
              NPCMax,
              NPCRGB);
            */
        
            if(!GameState->Bob.IsMoving)
            {
                GameState->Bob.CurrentBitmapI = 0;
                GameState->Bob.AnimationTimer = 0;
            }
            else
            {
            
                if(GameState->Bob.AnimationTimer++ > 4)
                {
                    GameState->Bob.AnimationTimer = 0;
                
                    if(++GameState->Bob.CurrentBitmapI > 7)
                    {
                        GameState->Bob.CurrentBitmapI = 0;
                    }                
                }                        
            }
                    
            switch(GameState->Bob.Direction)
            {
                case N: GameState->Bob.CurrentBitmap = GameState->BobNorthBitmap[GameState->Bob.CurrentBitmapI]; break;
                case S: GameState->Bob.CurrentBitmap = GameState->BobSouthBitmap[GameState->Bob.CurrentBitmapI]; break;
                case E: GameState->Bob.CurrentBitmap = GameState->BobEastBitmap[GameState->Bob.CurrentBitmapI]; break;
                case W: GameState->Bob.CurrentBitmap = GameState->BobWestBitmap[GameState->Bob.CurrentBitmapI]; break;
                default: Assert("Unknown player direction");
            };

            Entity[EntityCount].BMP = GameState->Bob.CurrentBitmap;
            Entity[EntityCount].Pos = GameState->Bob.Pos;
            //TODO: Fix this and the object Screen Pos code also. 
            Entity[EntityCount].ScreenPos.X = WorldScreenCenterX + NPCToPlayer.X - (SpriteDims.X/2*GameState->MetersToPixels);
            Entity[EntityCount].ScreenPos.Y = WorldScreenCenterY + NPCToPlayer.Y - ((SpriteDims.Y-/*TODO:MAGIC NUMBER*/1.0f)*GameState->MetersToPixels);                
            ++EntityCount;

            if(GameState->Bob.IsFrightened)
            {
                Entity[EntityCount].BMP = GameState->ExclamationMark;            
                Entity[EntityCount].Pos.Y = GameState->Bob.Pos.Y - .01f;
                //TODO: Fix this and the object Screen Pos code also. 
                Entity[EntityCount].ScreenPos.X = Entity[EntityCount - 1].ScreenPos.X + 33.0f;
                Entity[EntityCount].ScreenPos.Y = Entity[EntityCount-1].ScreenPos.Y - 50.0f;
                ++EntityCount;
            
            }
            else if(GameState->Bob.IsConfused)
            {
                Entity[EntityCount].BMP = GameState->QuestionMark;            
                Entity[EntityCount].Pos.Y = GameState->Bob.Pos.Y - .01f;
                //TODO: Fix this and the object Screen Pos code also. 
                Entity[EntityCount].ScreenPos.X = Entity[EntityCount - 1].ScreenPos.X + 30.0f;
                Entity[EntityCount].ScreenPos.Y = Entity[EntityCount-1].ScreenPos.Y - 40.0f;
                ++EntityCount;
                --GameState->Bob.IsConfused;
            }
        }

    
    
        //Render objects
        object *O = GameState->Obj;    
    
        uint32 BobInventorySpacer = 0;
    
        for(uint32 ObjectI = MIN_OBJECTS+1; ObjectI < MAX_OBJECTS; ++ObjectI)
        {
            ++O;
            if(O->Possessed)
            {
                if(GameState->Bob.Inventory[ObjectI])
                {
                    
                    loaded_bitmap SelectedBMP = O->BMP[0];
                    if(O->AnimationTimer)
                    {
                        //TODO: Any time that an object gets possessed, make the animation timer == 0
                        if(O->AnimationTimer % 2 == 0)
                        {
                            SelectedBMP = O->ActionBMP[0];
                        }
                        else
                        {
                            SelectedBMP = O->ActionBMP[1];
                        }                                                         
                    }
                    else if(O->Broken && O->BrokenBMP.Width)
                    {
                            SelectedBMP = O->BrokenBMP;
                    }
                    DrawBitmap(Buffer,
                               &SelectedBMP,
                               WhiteMenuBorder + GameState->BobIcon.Width + 5.0f + 40.0f + (45.0f*BobInventorySpacer++),
                               10.0f);                                                                           
                }    
                else
                {
                    for(uint32 II = 0; II < MAX_OBJECTS; ++II)
                    {
                        if(ObjectI == GameState->PlayerInventory[II])
                        {
                            if(GameState->PlayerInventory[GameState->PlayerInventoryI] == ObjectI)
                            {
                                loaded_bitmap SelectedBMP = O->BMP[1];
                                if(O->AnimationTimer)
                                {
                                    //TODO: Any time that an object gets possessed, make the animation timer == 0
                                    if(O->AnimationTimer % 2 == 0)
                                    {
                                        SelectedBMP = O->ActionBMP[0];
                                    }
                                    else
                                    {
                                        SelectedBMP = O->ActionBMP[1];
                                    }
                                    --O->AnimationTimer;                                        
                                }
                                DrawBitmap(Buffer,
                                           &SelectedBMP,
                                           WhiteMenuBorder + 20.0f + (50*II),
                                           Buffer->Height - 100.0f);                                   
                            }                            
                            else
                            {
                                DrawBitmap(Buffer,
                                           &O->BMP[0],
                                           WhiteMenuBorder + 20.0f + (50*II),
                                           Buffer->Height - 100.0f);                        
                            }                    
                        }
                    }
                }            
            }
            else if(ObjectI == GameState->ObjMode)
            {
            
                if(O->Broken && O->BrokenBMP.Width)
                {
                    Entity[EntityCount].BMP = O->BrokenBMP;
                }
                else if(O->AnimationTimer)
                {
                    //TODO: Any time that an object gets possessed, make the animation timer == 0
                    if(O->AnimationTimer % 2 == 0)
                    {
                        Entity[EntityCount].BMP = O->ActionBMP[0];
                    }
                    else
                    {
                        Entity[EntityCount].BMP = O->ActionBMP[1];
                    }
                    --O->AnimationTimer;                                        
                }
                else
                {
                    Entity[EntityCount].BMP = O->BMP[0];
                }
                Entity[EntityCount].Pos = GameState->PlayerPos;           
                Entity[EntityCount].ScreenPos.X = WorldScreenCenterX - (O->BMP[0].Width/2);
                Entity[EntityCount].ScreenPos.Y = WorldScreenCenterY - (O->BMP[0].Height/2);
                ++EntityCount;
            }
            else if(ObjectI != GameState->SelectedObjI && GameState->AptTileMap[(uint32)O->Pos.Y][(uint32)O->Pos.X] != HOLE_TILE)
            {            
                v2 OCorner[4];
                OCorner[0] = {O->Pos.X - O->BMP[0].Width/(2*GameState->MetersToPixels), O->Pos.Y - O->BMP[0].Height/(2*GameState->MetersToPixels)};
                OCorner[1] = {O->Pos.X + O->BMP[0].Width/(2*GameState->MetersToPixels), O->Pos.Y - O->BMP[0].Height/(2*GameState->MetersToPixels)};
                OCorner[2] = {O->Pos.X - O->BMP[0].Width/(2*GameState->MetersToPixels), O->Pos.Y + O->BMP[0].Height/(2*GameState->MetersToPixels)};
                OCorner[3] = {O->Pos.X + O->BMP[0].Width/(2*GameState->MetersToPixels), O->Pos.Y + O->BMP[0].Height/(2*GameState->MetersToPixels)};

                v2 OToPlayer = {};
                bool32 RenderObject = false;
                for(uint32 OI = 0; OI < ArrayCount(OCorner); ++OI)
                {
                    OToPlayer.X = AbsDiff(GameState->PlayerPos.X, OCorner[OI].X);
                    OToPlayer.Y = AbsDiff(GameState->PlayerPos.Y, OCorner[OI].Y);
                    if((OToPlayer.X < (WorldScreenCenterX/GameState->MetersToPixels)) &&
                       (OToPlayer.Y < (WorldScreenCenterY/GameState->MetersToPixels)))
                    {
                        RenderObject = true;
                        OToPlayer.X = (O->Pos.X - GameState->PlayerPos.X)*GameState->MetersToPixels;
                        OToPlayer.Y = (GameState->PlayerPos.Y - O->Pos.Y)*GameState->MetersToPixels;
                        break;
                    }
                }

                if(RenderObject)
                {
                    RenderObject = false;
                    Entity[EntityCount].Pos = O->Pos;
                    if(O->AnimationTimer)
                    {
                        //TODO: Any time that an object gets possessed, make the animation timer == 0
                        if(O->AnimationTimer % 2)
                        {
                            Entity[EntityCount].BMP = O->ActionBMP[0];
                        }
                        else
                        {
                            Entity[EntityCount].BMP = O->ActionBMP[1];
                        }
                        --O->AnimationTimer;                                        
                    }
                    else
                    {
                        Entity[EntityCount].BMP = O->BMP[0];
                    }
                
                    Entity[EntityCount].ScreenPos.X = WorldScreenCenterX + (OToPlayer.X - O->BMP[0].Width/2);
                    Entity[EntityCount].ScreenPos.Y = WorldScreenCenterY + (OToPlayer.Y - O->BMP[0].Height/2);
                    ++EntityCount;
                    /*       
                             v2 OMin = {(WorldScreenCenterX + OToPlayer.X) - (O->Radius*GameState->MetersToPixels), (WorldScreenCenterY + OToPlayer.Y) - (O->Radius*GameState->MetersToPixels)};
                             v2 OMax = {OMin.X + 2*O->Radius*GameState->MetersToPixels, (WorldScreenCenterY + OToPlayer.Y) + (O->Radius*GameState->MetersToPixels)};

                             rgb ObjColor = O->Color;            
                             if(ObjectI == GameState->SelectedObjI)
                             {
                
                             ObjColor.R = 0.0f;
                             ObjColor.G = 0.0f;
                             ObjColor.B = 0.0f;
                
                             rgb SelectColor = {1.0f, 1.0f, 1.0f};
                
                             v2 SMin = {(WorldScreenCenterX + OToPlayer.X) - ((O->Radius + 0.01f)*GameState->MetersToPixels),
                             (WorldScreenCenterY + OToPlayer.Y) - ((O->Radius + 0.01f)*GameState->MetersToPixels)};
                             v2 SMax = {OMin.X + 2*(O->Radius + 0.01f)*GameState->MetersToPixels,
                             (WorldScreenCenterY + OToPlayer.Y) + ((O->Radius + 0.01f)*GameState->MetersToPixels)};

                             DrawWorldRectangle(Buffer,
                             SMin,
                             SMax,
                             SelectColor);
                              
                             }
            
                             DrawWorldRectangle(Buffer,
                             OMin,
                             OMax,
                             ObjColor);
                    */
                }            
            }

        }

        //NOTE: The larger the Y coordinate, the further to the front of the array.

        //TODO: Render furniture.
        //NOTE: static bitmap layers don't need to be the full 1350x1350 size. just have them be from the top left corner of the apt to the desired image, etc.
        //Could even choose different corners, or even just render the image by itself (duh) and position it accordingly. Would require More work math and aseprite work, but less CPU.

        
        if(GameState->ChannelTimer)
        {
            --GameState->ChannelTimer;
        }
        
        if(EntityCount > 0)
        {
        
            entity TempEnt = {};
            uint32 EntsCompared = 0;
            int32 PEI = 0;
            uint32 ShuffleCount = 0;

            for(uint32 EI = 0; EI < EntityCount - 1; ++EI)
            {
                if(Entity[EI].Pos.Y < Entity[EI + 1].Pos.Y)
                {
                    TempEnt = Entity[EI];
                    Entity[EI] = Entity[EI + 1];
                    Entity[EI + 1] = TempEnt;

                    if(EI > 0)
                    {
                        PEI = (int32)EI - 1;
                        while(PEI > -1)
                        {

                            if(Entity[PEI].Pos.Y < Entity[PEI + 1].Pos.Y)
                            {

                                TempEnt = Entity[PEI];
                                Entity[PEI] = Entity[PEI + 1];
                                Entity[PEI + 1] = TempEnt;
                                /*
                                  if(PEI < (int32)EI - 1)
                                  {                                
                                  for(uint32 ShufflerI = EI - 1; ShufflerI >= (uint32)PEI; --ShufflerI)
                                  {
                                  Entity[ShufflerI + 1] = Entity[ShufflerI];  
                                  }
                                  Entity[PEI] = TempEnt;
                                  }
                                  else
                                  {
                                  Entity[EI] = Entity[EI - 1];
                                  Entity[EI - 1] = TempEnt;
                                  }
                                */
                            }
                            else
                            {
                                break;
                            }
                            --PEI;
                        }                    
                    }                
                }                 
            }
            for(uint32 EntI = 0; EntI < EntityCount; ++EntI)
            {
                
                DrawWorldBitmap(Buffer,
                                &Entity[EntI].BMP,
                                Entity[EntI].ScreenPos.X,
                                Entity[EntI].ScreenPos.Y);
            }
        }

    
        DrawWorldBitmap(Buffer,
                        &Layer[1].BMP,               
                        Layer[1].ScreenPos.X,
                        Layer[1].ScreenPos.Y);

        if(GameState->SpiritMode && !GameState->ObjMode)
        {
            DrawWorldBitmap(Buffer,
                            &GameState->PlayerSpiritBitmap,                   
                            WorldScreenCenterX - (GameState->PlayerSpiritBitmap.Width/2),
                            WorldScreenCenterY - (GameState->PlayerSpiritBitmap.Height/2));
        }

    
    
        if(GameState->SelectedObjI)
        {

            if(GameState->Obj[GameState->SelectedObjI].AnimationTimer)
            {
                DrawWorldBitmap(Buffer,
                                &GameState->Obj[GameState->SelectedObjI].ActionBMP[GameState->Obj[GameState->SelectedObjI].AnimationTimer%2],
                                WorldScreenCenterX - (GameState->Obj[GameState->SelectedObjI].BMP[GameState->ObjBlinkTimer].Width/2),
                                WorldScreenCenterY - (GameState->Obj[GameState->SelectedObjI].BMP[GameState->ObjBlinkTimer].Height/2));
                --GameState->Obj[GameState->SelectedObjI].AnimationTimer;
        
            }
            else
            {
                DrawWorldBitmap(Buffer,
                                &GameState->Obj[GameState->SelectedObjI].BMP[GameState->ObjBlinkTimer],
                                WorldScreenCenterX - (GameState->Obj[GameState->SelectedObjI].BMP[GameState->ObjBlinkTimer].Width/2),
                                WorldScreenCenterY - (GameState->Obj[GameState->SelectedObjI].BMP[GameState->ObjBlinkTimer].Height/2));
        
            }
                        
        }


    }
    
    
   
    //Render the items currently in you inventory
    //Allow highlighting and selecting an item to (equip||posess||drop||etc)

    //Render the icons of the npc's nearest you, facing the direction that they are facing.
    //Render their inventory's to the right of their icon.
    //Render their speech if they are speaking, (and have a speaking symbol appear next to their icon?)

    
    
/*NOTE: Rectangle Code

      //011
    //Render Player Model in proper location
    
    if(!GameState->ObjMode)
    {

        v2 PlayerMinXY = {ScreenCenterX -  PlayerRadius*GameState->MetersToPixels, ScreenCenterY - PlayerRadius*GameState->MetersToPixels};
        v2 PlayerMaxXY = {PlayerMinXY.X + 2*PlayerRadius*GameState->MetersToPixels, PlayerMinXY.Y + 2*PlayerRadius*GameState->MetersToPixels};
                                       
        rgb PlayerRGB = {0.25f, 1.0f, 1.0f};
        if(GameState->SpiritMode)
        {
            PlayerRGB.R = 1.0f;
            PlayerRGB.G = 1.0f;
            PlayerRGB.B = 1.0f;
        }
        
        DrawRectangle(Buffer,
                      PlayerMinXY,
                      PlayerMaxXY,
                      PlayerRGB);        
    }


    //00F
    //Render Current Tilemap
    for(int Row = -10; Row < 10; ++Row)
    {
        
        for(int Column = -20; Column < 20; ++Column)
        {            
            
            int32 PlayerRelativeTileX = (int32)GameState->PlayerPos.X + Column;
            int32 PlayerRelativeTileY = (int32)GameState->PlayerPos.Y + Row;
            //int32 BoardOfTile = GetBoard(&GameState->L0, PlayerRelativeBoard);

            if(
               PlayerRelativeTileX >= 0 &&
               PlayerRelativeTileX < GameState->L0.BoardDimensions.X &&
               PlayerRelativeTileY >= 0 &&
               PlayerRelativeTileY < GameState->L0.BoardDimensions.Y)
            {
                //uint32 TileValue = CurrentBoard.TileArray[(Row) * (int)GameState->L0.BoardTileCount.X + Column];
                //uint32 TileValue = GameState->L0.Board[BoardOfTile].TileArray[(((int32)PlayerRelativeBoard.Y % 9) * 16) + ((int32)PlayerRelativeBoard.X % 16)];
                //v2 Tired = {(int32)PlayerRelativeBoard.X % 16, (int32)PlayerRelativeBoard.Y % 9};
                //v2 MinXY = {(real32)Column, (real32)Row};                
                //MinXY.X = GameState->MetersToPixels;
                //v2 MinXY = {(real32)Column - (GameState->PlayerPos.X - (int32)GameState->PlayerPos.X), ((real32)Row)};
                //MinXY = MinXY * GameState->MetersToPixels;                
                uint32 TileValue = GameState->AptTileMap[PlayerRelativeTileY][PlayerRelativeTileX];
                rgb TileRGB = {};
                bool32 HaveBMP = false;
                loaded_bitmap TileBMP = {};
                switch(TileValue)
                {
                    case FLOOR_TILE:
                    {
                        TileRGB.R = 0x12/255.0f;
                        TileRGB.G = 0x34/255.0f;
                        TileRGB.B = 0x56/255.0f;

                        //HaveBMP = true;
                        //TileBMP = GameState->FloorTileBitmap;
                        
                    }break;
                    case WALL_TILE:
                    {
                        TileRGB.R = 0x00/255.0f;
                        TileRGB.G = 0x00/255.0f;
                        TileRGB.B = 0x00/255.0f;
                    }break;
                    case OUTER_WALL_TILE:
                    {
                        TileRGB.R = 0x52/255.0f;
                        TileRGB.G = 0x52/255.0f;
                        TileRGB.B = 0x52/255.0f;
                    }break;
                    case TABLE_TILE:
                    {
                        TileRGB.R = 0x83/255.0f;
                        TileRGB.G = 0x31/255.0f;
                        TileRGB.B = 0x00/255.0f;
                    }break;
                    case CHAIR_TILE:
                    {
                        TileRGB.R = 0x5b/255.0f;
                        TileRGB.G = 0x31/255.0f;
                        TileRGB.B = 0x5b/255.0f;
                    }break;
                    case DOOR_TILE:
                    {
                        TileRGB.R = 0xff/255.0f;
                        TileRGB.G = 0xff/255.0f;
                        TileRGB.B = 0x00/255.0f;
                    }break;
                    case HOLE_TILE:
                    {
                        TileRGB.R = 0xe6/255.0f;
                        TileRGB.G = 0x00/255.0f;
                        TileRGB.B = 0x00/255.0f;
                    }break;
                    case HUB_TILE:
                    {
                        TileRGB.R = 0xff/255.0f;
                        TileRGB.G = 0x85/255.0f;
                        TileRGB.B = 0x84/255.0f;
                    }break;
                    case OUTER_DOOR_TILE:
                    {
                        TileRGB.R = 0xff/255.0f;
                        TileRGB.G = 0x42/255.0f;
                        TileRGB.B = 0x00/255.0f;
                    }break;
                };
                
                
                v2 MinXY = {};
                MinXY.X = ScreenCenterX - ((GameState->PlayerPos.X - (int32)GameState->PlayerPos.X)*GameState->MetersToPixels) + (real32)Column*GameState->MetersToPixels;
                MinXY.Y = ScreenCenterY + ((GameState->PlayerPos.Y - (int32)GameState->PlayerPos.Y)*GameState->MetersToPixels) - (real32)Row*GameState->MetersToPixels;                
                MinXY.Y -= GameState->MetersToPixels;
                v2 MaxXY = {};
                MaxXY.X = MinXY.X + GameState->MetersToPixels;
                MaxXY.Y = MinXY.Y + GameState->MetersToPixels;
                
                DrawRectangle(Buffer,
                              MinXY,
                              MaxXY,
                              TileRGB);

                if(HaveBMP)
                {
                    
                    DrawBitmap(Buffer,
                               &TileBMP,
                               MinXY.X,
                               MinXY.Y);                               
                }
            }                
        }        
    }
*/

    
    //Render Apt Tilemap

    
#if 0
    for(int Row = 0; Row < GameState->ApartmentBitmap.Height; ++Row)
    {
        
        for(int Column = 0; Column < GameState->ApartmentBitmap.Width; ++Column)
        {            
            
                uint32 TileValue = GameState->AptTileMap[Row][Column];
                rgb TileColor = {};
                
                if(TileValue == FLOOR_TILE)
                {
                    
                }
                else if(TileValue == WALL_TILE)
                {
                    TileColor.R = 255/255.0f;
                    TileColor.G = 255/255.0f;
                    TileColor.B = 255/255.0f;
                }
                else if(TileValue == TABLE_TILE)
                {
                    TileColor.R = 131.0f/255;
                    TileColor.G = 41.0f/255;                    
                }
                else if(TileValue == CHAIR_TILE)
                {
                    TileColor.R = 91.0f/255;
                    TileColor.G = 49.0f/255;
                    TileColor.B = 91.0f/255;
                }
                else if(TileValue == HUB_TILE)
                {
                    TileColor.R = 133.0f/255;
                    TileColor.G = 133.0f/255;
                    TileColor.B = 133.0f/255;
                }
                else if(TileValue == DOOR_TILE)
                {
                    TileColor.R = 255.0f/255;
                    TileColor.G = 255.0f/255;                    
                }
                else if(TileValue == HOLE_TILE)
                {
                    TileColor.R = 255.0f/255;                    
                    TileColor.B = 255.0f/255;
                }
                
                v2 MinXY = {Column*10.0f, Row*10.0f};
                
                v2 MaxXY = {};
                MaxXY.X = MinXY.X + 10;
                MaxXY.Y = MinXY.Y + 10;                
                DrawRectangle(Buffer,
                              MinXY,
                              MaxXY,
                              TileColor);
            }                
    }
#endif

    
    //012
#if 0
#include <Windows.h>
#include <stdio.h>
    char MovementString[256];
    _snprintf_s(MovementString, sizeof(MovementString),
                "Board: %i // (%f, %f)\n",
                BoardIndex, NewPlayerPos.X, NewPlayerPos.Y);
    OutputDebugStringA(MovementString);
#endif


    
    //Update the clock
    if(++GameState->Clock.Sec > (1/Input->dtForFrame))
    {
        GameState->Clock.Sec = 0;
        if(++GameState->Clock.Min == 60)
        {
            GameState->Clock.Min = 0;
            if(++GameState->Clock.Hr == 24)
            {
                GameState->Clock.Hr = 0;
            }
        }        
    }
    }    
    if(DoLoad)
    {

        
        for(uint32 BI = 0; BI < ArrayCount(GameState->PlayerSouthBitmap); ++BI)
        {
            Memory->DEBUGPlatformFreeFileMemory(0, GameState->PlayerSouthBitmap[BI].Pixels);
            Memory->DEBUGPlatformFreeFileMemory(0, GameState->BobSouthBitmap[BI].Pixels);
        }
        
        for(uint32 BI = 0; BI < ArrayCount(GameState->PlayerNorthBitmap); ++BI)
        {
            Memory->DEBUGPlatformFreeFileMemory(0, GameState->PlayerNorthBitmap[BI].Pixels);
            Memory->DEBUGPlatformFreeFileMemory(0, GameState->BobNorthBitmap[BI].Pixels);
        }
        
        for(uint32 BI = 0; BI < ArrayCount(GameState->PlayerEastBitmap); ++BI)
        {
            Memory->DEBUGPlatformFreeFileMemory(0, GameState->PlayerEastBitmap[BI].Pixels);
            Memory->DEBUGPlatformFreeFileMemory(0, GameState->BobEastBitmap[BI].Pixels);
        }
        
        for(uint32 BI = 0; BI < ArrayCount(GameState->PlayerWestBitmap); ++BI)
        {
            Memory->DEBUGPlatformFreeFileMemory(0, GameState->PlayerWestBitmap[BI].Pixels);
            Memory->DEBUGPlatformFreeFileMemory(0, GameState->BobWestBitmap[BI].Pixels);
        }
    
        for(uint32 BI = 0; BI < ArrayCount(GameState->AptLayer); ++BI)
        {
            Memory->DEBUGPlatformFreeFileMemory(0, GameState->AptLayer[BI].BMP.Pixels);
        }
        
        for(uint32 BI = 0; BI < ArrayCount(GameState->Obj); ++BI)
        {
            Memory->DEBUGPlatformFreeFileMemory(0, GameState->Obj[BI].BMP[0].Pixels);
            Memory->DEBUGPlatformFreeFileMemory(0, GameState->Obj[BI].BMP[1].Pixels);
            Memory->DEBUGPlatformFreeFileMemory(0, GameState->Obj[BI].ActionBMP[0].Pixels);
            Memory->DEBUGPlatformFreeFileMemory(0, GameState->Obj[BI].ActionBMP[1].Pixels);
            Memory->DEBUGPlatformFreeFileMemory(0, GameState->Obj[BI].BrokenBMP.Pixels);
        }

        
        for(uint32 BI = 0; BI < ArrayCount(GameState->Channel); ++BI)
        {
            Memory->DEBUGPlatformFreeFileMemory(0, GameState->Channel[BI].Pixels);
        }
        for(uint32 BI = 0; BI < ArrayCount(GameState->TvScreen); ++BI)
        {
            Memory->DEBUGPlatformFreeFileMemory(0, GameState->TvScreen[BI].Pixels);
        }
        
        
        for(uint32 BI = 0; BI < ArrayCount(GameState->ClockDigits); ++BI)
        {
            Memory->DEBUGPlatformFreeFileMemory(0, GameState->ClockDigits[BI].Pixels);
        }
        for(uint32 BI = 0; BI < ArrayCount(GameState->CurrentClockDigits); ++BI)
        {
            Memory->DEBUGPlatformFreeFileMemory(0, GameState->CurrentClockDigits[BI].Pixels);
        }

        for(uint32 BI = 0; BI < ArrayCount(GameState->Words); ++BI)
        {
            Memory->DEBUGPlatformFreeFileMemory(0, GameState->Words[BI].BMP.Pixels);
        }

        for(uint32 BI = 0; BI < ArrayCount(GameState->Channel); ++BI)
        {
            Memory->DEBUGPlatformFreeFileMemory(0, GameState->Channel[BI].Pixels);
        }
        
        Memory->DEBUGPlatformFreeFileMemory(0, GameState->CurrentWords.BMP.Pixels);
        
        Memory->DEBUGPlatformFreeFileMemory(0, GameState->QuestionMark.Pixels);
        Memory->DEBUGPlatformFreeFileMemory(0, GameState->ExclamationMark.Pixels);
        Memory->DEBUGPlatformFreeFileMemory(0, GameState->BobIcon.Pixels);
        Memory->DEBUGPlatformFreeFileMemory(0, GameState->SpeechIcon.Pixels);
        Memory->DEBUGPlatformFreeFileMemory(0, GameState->TvGuideBMP.Pixels);

        
        Memory->DEBUGPlatformFreeFileMemory(0, GameState->PlayerSpiritBitmap.Pixels);
        Memory->DEBUGPlatformFreeFileMemory(0, GameState->ApartmentBitmap.Pixels);
        

        if(DoLoad1)
        {
            *GameState = DEBUGLoadGameState(0, Memory->DEBUGPlatformReadEntireFile, Memory->DEBUGPlatformFreeFileMemory, "test/GameState1.txt");
        }
        else if(DoLoad2)
        {
            *GameState = DEBUGLoadGameState(0, Memory->DEBUGPlatformReadEntireFile, Memory->DEBUGPlatformFreeFileMemory, "test/GameState2.txt");
        }
        else if(DoLoad3)
        {
            *GameState = DEBUGLoadGameState(0, Memory->DEBUGPlatformReadEntireFile, Memory->DEBUGPlatformFreeFileMemory, "test/GameState3.txt");
        }
        else if(DoLoad4)
        {
            *GameState = DEBUGLoadGameState(0, Memory->DEBUGPlatformReadEntireFile, Memory->DEBUGPlatformFreeFileMemory, "test/GameState4.txt");
        }
        else
        {
            *GameState = DEBUGLoadGameState(0, Memory->DEBUGPlatformReadEntireFile, Memory->DEBUGPlatformFreeFileMemory, "test/GameState.txt");
        }



                
        GameState->QuestionMark = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/QuestionMark.bmp");
        GameState->ExclamationMark = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/ExclamationMark.bmp");
        GameState->BobIcon = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/BobIcon.bmp");
        GameState->SpeechIcon = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/SpeechIcon.bmp");
                        
        GameState->Words[WORDS_WRITE].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/WordsWrite.bmp");
        GameState->Words[WORDS_INSPIREDWRITE].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/WordsInspiredWrite.bmp");
        GameState->Words[WORDS_DEAR_EVELYN].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/WordsDear.bmp");
        GameState->Words[WORDS_DOUBT].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/WordsDoubt.bmp");
        GameState->Words[WORDS_SHREDDER].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/WordsShredder.bmp");
        GameState->Words[WORDS_TRASH].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/WordsTrash.bmp");
        GameState->Words[WORDS_THERE].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/WordsThere.bmp");
        GameState->Words[WORDS_WORK].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/WordsWork.bmp");
        GameState->Words[WORDS_FOOD].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/WordsFood.bmp");
        GameState->Words[WORDS_TV].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/WordsTv.bmp");
        GameState->Words[WORDS_YAWN].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/WordsYawn.bmp");
        GameState->Words[WORDS_SEARCH].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/WordsSearch.bmp");


        GameState->CurrentClockDigits[2] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/ClockDigit_Colon.bmp");
        GameState->ClockDigits[0] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/ClockDigit_0.bmp");
        GameState->ClockDigits[1] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/ClockDigit_1.bmp");
        GameState->ClockDigits[2] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/ClockDigit_2.bmp");
        GameState->ClockDigits[3] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/ClockDigit_3.bmp");
        GameState->ClockDigits[4] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/ClockDigit_4.bmp");
        GameState->ClockDigits[5] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/ClockDigit_5.bmp");
        GameState->ClockDigits[6] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/ClockDigit_6.bmp");
        GameState->ClockDigits[7] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/ClockDigit_7.bmp");
        GameState->ClockDigits[8] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/ClockDigit_8.bmp");
        GameState->ClockDigits[9] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/ClockDigit_9.bmp");
        GameState->ClockDigits[10] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/ClockDigit_AM.bmp");
        GameState->ClockDigits[11] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/ClockDigit_PM.bmp");

        
        GameState->Channel[0] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/Channel0.bmp");
        GameState->Channel[1] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/Channel1.bmp");
        GameState->Channel[2] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/Channel2.bmp");
        GameState->Channel[3] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/Channel3.bmp");
        GameState->Channel[4] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/Channel4.bmp");
        GameState->Channel[5] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/Channel5.bmp");
        GameState->Channel[6] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/Channel6.bmp");
        GameState->Channel[7] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/Channel7.bmp");


        
        GameState->TvScreen[0] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/TvScreen0.bmp");
        GameState->TvScreen[1] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/TvScreen1.bmp");
        GameState->TvScreen[2] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/TvScreen2.bmp");
        GameState->TvScreen[3] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/TvScreen3.bmp");
        GameState->TvScreen[4] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/TvScreen4.bmp");
        GameState->TvScreen[5] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/TvScreen5.bmp");
        GameState->TvScreen[6] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/TvScreen6.bmp");
        GameState->TvScreen[7] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/TvScreen7.bmp");

        GameState->TvGuideBMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/TvGuideScreen.bmp");

        
       
        
                    
            if((GameState->Clock.Hr > 0 && GameState->Clock.Hr < 10) || (GameState->Clock.Hr > 12 && GameState->Clock.Hr < 22))
            {
                GameState->CurrentClockDigits[0] = GameState->ClockDigits[0];            
            }
            else
            {
                GameState->CurrentClockDigits[0] = GameState->ClockDigits[1];            
            }

        
            if(!GameState->Clock.Hr)
            {
                GameState->CurrentClockDigits[1] = GameState->ClockDigits[2];
            }
            else if(GameState->Clock.Hr < 13)
            {
                GameState->CurrentClockDigits[1] = GameState->ClockDigits[GameState->Clock.Hr % 10];
            }        
            else 
            {
                GameState->CurrentClockDigits[1] = GameState->ClockDigits[(GameState->Clock.Hr - 2) % 10];
            }
        
            GameState->CurrentClockDigits[3] = GameState->ClockDigits[GameState->Clock.Min/10];
            GameState->CurrentClockDigits[4] = GameState->ClockDigits[GameState->Clock.Min%10];

            
            if(GameState->Clock.Hr == 12 && GameState->Clock.Min < 5)
            {       
                GameState->CurrentClockDigits[5] = {};
            }
            else if(!GameState->Clock.Hr && GameState->Clock.Min < 5)
            {      
                GameState->CurrentClockDigits[5] = {};
            }
            else if(GameState->Clock.Hr < 12)
            {            
                GameState->CurrentClockDigits[5] = GameState->ClockDigits[10];
            }
            else
            {
                GameState->CurrentClockDigits[5] = GameState->ClockDigits[11];
            }
            

                            
        //(0, -1, -1, -1)
        //18x26
        GameState->PlayerSouthBitmap[0] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/South1.bmp"); 
        GameState->PlayerSouthBitmap[1] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/South2.bmp");
        GameState->PlayerSouthBitmap[2] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/South3.bmp");
        GameState->PlayerSouthBitmap[3] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/South4.bmp");
        GameState->PlayerSouthBitmap[4] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/South5.bmp");
        GameState->PlayerSouthBitmap[5] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/South6.bmp");
        GameState->PlayerSouthBitmap[6] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/South7.bmp");
        GameState->PlayerSouthBitmap[7] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/South8.bmp");

        //(0, -2, -2, -2)
        //20x27
        GameState->PlayerNorthBitmap[0] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/North1.bmp");
        GameState->PlayerNorthBitmap[1] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/North2.bmp");
        GameState->PlayerNorthBitmap[2] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/North3.bmp");
        GameState->PlayerNorthBitmap[3] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/North4.bmp");
        GameState->PlayerNorthBitmap[4] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/North5.bmp");
        GameState->PlayerNorthBitmap[5] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/North6.bmp");
        GameState->PlayerNorthBitmap[6] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/North7.bmp");
        GameState->PlayerNorthBitmap[7] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/North8.bmp");

        //(0, -2, -1, -2)
        //19x27
        GameState->PlayerEastBitmap[0] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/East1.bmp");
        GameState->PlayerEastBitmap[1] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/East2.bmp");
        GameState->PlayerEastBitmap[2] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/East3.bmp");
        GameState->PlayerEastBitmap[3] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/East4.bmp");
        GameState->PlayerEastBitmap[4] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/East5.bmp");
        GameState->PlayerEastBitmap[5] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/East6.bmp");
        GameState->PlayerEastBitmap[6] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/East7.bmp");
        GameState->PlayerEastBitmap[7] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/East8.bmp");

        //(1, 0, -3, -1)
        //20x24
        GameState->PlayerWestBitmap[0] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/West1.bmp");  
        GameState->PlayerWestBitmap[1] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/West2.bmp");
        GameState->PlayerWestBitmap[2] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/West3.bmp");
        GameState->PlayerWestBitmap[3] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/West4.bmp");
        GameState->PlayerWestBitmap[4] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/West5.bmp");
        GameState->PlayerWestBitmap[5] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/West6.bmp");
        GameState->PlayerWestBitmap[6] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/West7.bmp");
        GameState->PlayerWestBitmap[7] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/West8.bmp");

        
        //(0, -1, -1, -1)
        //18x26
        GameState->BobSouthBitmap[0] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/South_0.bmp"); 
        GameState->BobSouthBitmap[1] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/South_L1.bmp");
        GameState->BobSouthBitmap[2] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/South_L2.bmp");
        GameState->BobSouthBitmap[3] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/South_L1.bmp");
        GameState->BobSouthBitmap[4] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/South_0.bmp");
        GameState->BobSouthBitmap[5] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/South_R1.bmp");
        GameState->BobSouthBitmap[6] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/South_R2.bmp");
        GameState->BobSouthBitmap[7] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/South_R1.bmp");

        //(0, -2, -2, -2)
        //20x27
        GameState->BobNorthBitmap[0] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/North_0.bmp");
        GameState->BobNorthBitmap[1] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/North_L1.bmp");
        GameState->BobNorthBitmap[2] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/North_L2.bmp");
        GameState->BobNorthBitmap[3] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/North_L1.bmp");
        GameState->BobNorthBitmap[4] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/North_0.bmp");
        GameState->BobNorthBitmap[5] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/North_R1.bmp");
        GameState->BobNorthBitmap[6] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/North_R2.bmp");
        GameState->BobNorthBitmap[7] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/North_R1.bmp");

        //(0, -2, -1, -2)
        //19x27
        GameState->BobEastBitmap[0] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/East_0.bmp");
        GameState->BobEastBitmap[1] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/East_L1.bmp");
        GameState->BobEastBitmap[2] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/East_L2.bmp");
        GameState->BobEastBitmap[3] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/East_L1.bmp");
        GameState->BobEastBitmap[4] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/East_0.bmp");
        GameState->BobEastBitmap[5] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/East_R1.bmp");
        GameState->BobEastBitmap[6] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/East_R2.bmp");
        GameState->BobEastBitmap[7] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/East_R1.bmp");

        //(1, 0, -3, -1)
        //20x24
        GameState->BobWestBitmap[0] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/West_0.bmp");  
        GameState->BobWestBitmap[1] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/West_L1.bmp");
        GameState->BobWestBitmap[2] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/West_L2.bmp");
        GameState->BobWestBitmap[3] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/West_L1.bmp");
        GameState->BobWestBitmap[4] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/West_0.bmp");
        GameState->BobWestBitmap[5] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/West_R1.bmp");
        GameState->BobWestBitmap[6] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/West_R2.bmp");
        GameState->BobWestBitmap[7] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/West_R1.bmp");


        
        GameState->PlayerSpiritBitmap = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/Spirit_0.bmp");
        
        //BMP Tiles
        //GameState->FloorTileBitmap = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/FloorTile.bmp");

        //Apartment layers
        GameState->AptLayer[MAIN_LAYER_A].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/MainA.bmp");
        GameState->AptLayer[MAIN_LAYER_Z].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/MainZ.bmp");
        GameState->AptLayer[MAIN_WALL].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/MainWall.bmp");
        GameState->AptLayer[MAIN_WALL_N].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/MainWallN.bmp");
        GameState->AptLayer[MAIN_CHAIR].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/MainChair.bmp");        
        GameState->AptLayer[MAIN_DOORFRAME_S].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/MainDoorFrameSouth.bmp");        
        GameState->AptLayer[MAIN_DOORFRAME_N].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/MainDoorFrameNorth.bmp");        
        GameState->AptLayer[MAIN_PINGPONG_TABLE].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/MainPingPongTable.bmp");               
        GameState->AptLayer[BED_LAYER_A].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/BedA.bmp");
        GameState->AptLayer[BED_LAYER_Z].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/BedZ.bmp");
        GameState->AptLayer[BED_CHAIR].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/BedChair.bmp");        
        GameState->AptLayer[BED_WALL].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/BedWall.bmp");        
        GameState->AptLayer[BED_DOORFRAME].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/BedDoorFrame.bmp");                
        GameState->AptLayer[BATH_LAYER_A].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/BathA.bmp");
        GameState->AptLayer[BATH_LAYER_Z].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/BathZ.bmp");
        GameState->AptLayer[BATH_WALL].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/BathWall.bmp");
        GameState->AptLayer[BATH_TOILET].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/BathToilet.bmp");        
        GameState->AptLayer[BATH_DOORFRAME].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/BathDoorFrame.bmp");                    
        GameState->AptLayer[TV_KITCHEN_LAYER_A].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/TvKitchenA.bmp");
        GameState->AptLayer[TV_KITCHEN_LAYER_Z].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/TvKitchenZ.bmp");
        GameState->AptLayer[KITCHEN_WALL].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/KitchenWall.bmp");        
        GameState->AptLayer[KITCHEN_STOOL].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/KitchenStool.bmp");        
        GameState->AptLayer[KITCHEN_TABLE].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/KitchenTable.bmp");        
        GameState->AptLayer[TV_WALL].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/TvWall.bmp");        
        GameState->AptLayer[TV_COUCH_HORI].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/TvCouchHori.bmp");        
        GameState->AptLayer[TV_COUCH_VERTI].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/TvCouchVerti.bmp");        
        GameState->AptLayer[TV_TABLE].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/TvTable.bmp");                
        GameState->AptLayer[MAINCLOSET_LAYER_A].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/MainClosetA.bmp");
        GameState->AptLayer[MAINCLOSET_LAYER_Z].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/MainClosetZ.bmp");        
        GameState->AptLayer[BEDCLOSET_LAYER_A].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/BedClosetA.bmp");
        GameState->AptLayer[BEDCLOSET_WALL].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/BedClosetWall.bmp");        
        GameState->AptLayer[BEDCLOSET_LAYER_Z].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/BedClosetZ.bmp");
        
        GameState->AptLayer[OUTDOORS_LAYER_A].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/OutdoorsA.bmp");
        GameState->AptLayer[OUTDOORS_MAILBOX].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/OutdoorsMailbox.bmp");
        GameState->AptLayer[OUTDOORS_DUMPSTER].BMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/OutdoorsDumpster.bmp");


        GameState->ApartmentBitmap = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/Apt_CollisionMap.bmp");
        //loaded_bitmap ApartmentHubAndRoomBitmap = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/Apt_HubAndRoomMap.bmp");


                        
        GameState->Obj[LETTER].BMP[0] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/Letter.bmp");                                   
        GameState->Obj[PEN].BMP[0] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/Pencil.bmp");                               
        GameState->Obj[LIGHTER].BMP[0] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/Lighter.bmp");
        GameState->Obj[LIGHTER].ActionBMP[0] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/ActionLighter.bmp");                               
        GameState->Obj[BALL].BMP[0] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/Ball.bmp");                                
        GameState->Obj[REMOTE].BMP[0] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/Remote.bmp");                                  
        GameState->Obj[SHREDDER].BMP[0] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/Shredder.bmp");
        GameState->Obj[SHREDDER].ActionBMP[0] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/ActionShredder.bmp");        
        GameState->Obj[PAPER].BMP[0] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/Paper.bmp");
        GameState->Obj[TV_GUIDE].BMP[0] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/TvGuide.bmp");
                        
        GameState->Obj[LETTER].BMP[1] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/Letter2.bmp");                                   
        GameState->Obj[PEN].BMP[1] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/Pencil2.bmp");                               
        GameState->Obj[LIGHTER].BMP[1] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/Lighter2.bmp");
        GameState->Obj[LIGHTER].ActionBMP[1] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/ActionLighter2.bmp");
        GameState->Obj[BALL].BMP[1] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/Ball2.bmp");                                
        GameState->Obj[REMOTE].BMP[1] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/Remote2.bmp");                                  
        GameState->Obj[SHREDDER].BMP[1] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/Shredder.bmp");
        GameState->Obj[SHREDDER].ActionBMP[1] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/ActionShredder2.bmp");
        GameState->Obj[PAPER].BMP[1] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/Paper2.bmp");
        GameState->Obj[TV_GUIDE].BMP[1] = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/TvGuide2.bmp");

        GameState->Obj[SHREDDER].BrokenBMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/BrokenShredder.bmp");
        GameState->Obj[REMOTE].BrokenBMP = DEBUGLoadBMP(0, Memory->DEBUGPlatformReadEntireFile, "test/BrokenRemote.bmp");         
    }    
}

extern "C" GAME_GET_SOUND_SAMPLES(GameGetSoundSamples)
{
    game_state *GameState = (game_state *)Memory->PermanentStorage;
    GameOutputSound(GameState, SoundBuffer, 400);
}
                    
                    
        

