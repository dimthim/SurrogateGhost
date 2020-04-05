
#if !defined(ENUMS_H)

#define ENUMS_H

enum TILE_TYPES
{
    FLOOR_TILE,
    WALL_TILE,
    OUTER_WALL_TILE,
    DOOR_TILE,
    TABLE_TILE,
    CHAIR_TILE,
    HOLE_TILE,
    HUB_TILE,
    OUTER_DOOR_TILE,

    MAX_TILE_TYPE
};

enum NSEW
{
    N = 0,
    S,
    E,
    W,

    MAX_NSEW
};

enum WORDS
{
    WORDS_WRITE,    
    WORDS_DEAR_EVELYN,
    WORDS_DOUBT,
    WORDS_SHREDDER,
    WORDS_TRASH,
    WORDS_THERE,
    WORDS_WORK,
    WORDS_FOOD,
    //WORDS_NOPE,
    WORDS_TV,
    WORDS_YAWN,
    WORDS_SEARCH,
    WORDS_INSPIREDWRITE,
    MAX_WORDS,
};

enum APT_LAYERS
{
    MAIN_LAYER_A,
    MAIN_CHAIR,
    MAIN_WALL,
    MAIN_WALL_N,
    MAIN_DOORFRAME_S,
    MAIN_DOORFRAME_N,
    MAIN_PINGPONG_TABLE,
    MAIN_LAYER_Z,
    
    BED_LAYER_A,    
    BED_WALL,
    BED_CHAIR,
    BED_DOORFRAME,
    BED_LAYER_Z,
    
    BATH_LAYER_A,    
    BATH_TOILET,
    BATH_WALL,
    BATH_DOORFRAME,
    BATH_LAYER_Z,
    
    TV_KITCHEN_LAYER_A,    
    KITCHEN_WALL,
    KITCHEN_STOOL,
    KITCHEN_TABLE,
    TV_WALL,
    TV_COUCH_HORI,
    TV_COUCH_VERTI,
    TV_TABLE,
    TV_KITCHEN_LAYER_Z,

    MAINCLOSET_LAYER_A,
    MAINCLOSET_LAYER_Z,
    
    BEDCLOSET_LAYER_A,
    BEDCLOSET_WALL,
    BEDCLOSET_LAYER_Z,

    OUTDOORS_LAYER_A,
    OUTDOORS_MAILBOX,
    OUTDOORS_DUMPSTER,
    OUTDOORS_LAYER_Z,

    MAX_LAYERS,
    
};

enum ROOMS
{
    MAIN_ROOM,//0
    BED_ROOM,//1
    BATH_ROOM,//2
    TV_ROOM,//3
    KITCHEN_ROOM,//4
    CLOSET_MAIN_ROOM,//5
    CLOSET_BED_ROOM,//6
    OUTDOORS_ROOM,//7

    MAX_ROOMS
};

enum EVENTS
{
    MIN_EVENTS = 0,
    BATHROOM = 1,
    WRITE_LETTER = 2,
    DESTROY_LETTER = 3,
    WORK = 4,
    PING_PONG = 5,
    EAT = 6,
    TV = 7,
    SLEEP = 8,
    SEARCH = 9,
    GRAB_ITEM = 10,
    PONDER = 11,
    FAINT = 12,
    
    MAX_EVENTS
};

enum OBJECTS
{
    MIN_OBJECTS = 0,
        
    PEN,
    LIGHTER,
    BALL,
    REMOTE,    
    SHREDDER,
    PAPER,
    LETTER,
    TV_GUIDE,
    MAX_OBJECTS    
    
};

enum PRIORITIES
{
    NO_PRIORITY = 0,
    LOW_PRIORITY = 1,
    MEDIUM_PRIORITY = 5,
    HIGH_PRIORITY = 10,
    ULTIMATE_PRIORITY = 15
};

enum HUB_SKIP_BITFLAGS
{
    CAN_HUB_SKIP,
    HAVE_SAME_X,
    HAVE_SAME_Y,

    MAX_HUB_SKIP_BITFLAGS
    
};

#endif
