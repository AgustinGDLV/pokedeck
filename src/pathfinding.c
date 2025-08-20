#include "global.h"
#include "event_object_movement.h"
#include "field_player_avatar.h"
#include "field_control_avatar.h"
#include "random.h"

/*
 * Enemy AI player-tracking uses a very simple, potentially
 * suboptimal algorithm meant to do as little work as possible.
 * If the player is within sight range (based on Manhattan
 * distance) and at the same elevation as the enemy, then:
 * 
 *   1.) Attempt to step towards the player along the direction
 *       the enemy is farthest from the player.
 *   2.) If an obstacle is in the way, try to walk along the
 *       other direction towards the player.
 *   3.) If an obstacle is in the way in that direction, too,
 *       walk opposite that direction.
 *   4.) If there are obstacles in all directions, revert to
 *       wandering.
 *
 * I had previously tested an implementation of A* pathfinding,
 * but considering that this is called every time the player takes
 * a step, this seems good enough.
 *  
*/

// Returns the absolute horizontal distance between two points.
static u32 GetHorizontalDistance(struct Coords16* p1, struct Coords16* p2)
{
    if (p1->x > p2->x)
        return p1->x - p2->x;
    else
        return p2->x - p1->x;
}

// Returns the absolute vertical distance between two points.
static u32 GetVerticalDistance(struct Coords16* p1, struct Coords16* p2)
{
    if (p1->y > p2->y)
        return p1->y - p2->y;
    else
        return p2->y - p1->y;
}

// Returns the Manhattan distance between two points.
static inline u32 GetManhattanDistance(struct Coords16* p1, struct Coords16* p2)
{
    return GetHorizontalDistance(p1, p2) + GetVerticalDistance(p1, p2);
}

// Returns whether an object event is within sight range of the player.
bool32 IsObjectEventInRangeOfPlayer(struct ObjectEvent* objectEvent)
{
    struct Coords16* objCoords = &objectEvent->currentCoords;
    struct Coords16* playerCoords = &gObjectEvents[gPlayerAvatar.objectEventId].currentCoords;

    return GetManhattanDistance(objCoords, playerCoords) <= objectEvent->trainerRange_berryTreeId;
}

// Returns whether an object event is cardinally adjacent to the player.
bool32 IsObjectEventAdjacentToPlayer(struct ObjectEvent* objectEvent)
{
    struct Coords16* objCoords = &objectEvent->currentCoords;
    struct Coords16* playerCoords = &gObjectEvents[gPlayerAvatar.objectEventId].currentCoords;

    // Avoid seeing across elevations.
    if (objectEvent->currentElevation != gObjectEvents[gPlayerAvatar.objectEventId].currentElevation)
        return FALSE;

    return GetManhattanDistance(objCoords, playerCoords) <= 1;
}

// Gets the greater direction of the vector from p1 to p2.
static u32 GetPrimaryVectorDirection(struct Coords16* p1, struct Coords16* p2)
{
    if (GetHorizontalDistance(p1, p2) > GetVerticalDistance(p1, p2))
    {
        if (p1->x < p2->x) return DIR_EAST;
        else return DIR_WEST;
    }
    else
    {
        if (p1->y > p2->y) return DIR_NORTH;
        else return DIR_SOUTH;
    }
}

// Gets the lesser direction of the vector from p1 to p2.
static u32 GetSecondaryVectorDirection(struct Coords16* p1, struct Coords16* p2)
{
    if (GetHorizontalDistance(p1, p2) > GetVerticalDistance(p1, p2))
    {
        if (p1->y > p2->y) return DIR_NORTH;
        else return DIR_SOUTH;
    }
    else
    {
        if (p1->x < p2->x) return DIR_EAST;
        else return DIR_WEST;
    }
}

// Returns the direction towards which to walk to path to the player.
u32 GetDirectionTowardsPlayer(struct ObjectEvent* objectEvent)
{
    u32 targetDir, altDir;
    struct Coords16 objCoords = objectEvent->currentCoords;
    struct Coords16 playerCoords = gObjectEvents[gPlayerAvatar.objectEventId].currentCoords;

    // Check elevation mismatch.
    if (objectEvent->currentElevation != PlayerGetElevation())
        return DIR_NONE;

    // Check sight range.
    if (!IsObjectEventInRangeOfPlayer(objectEvent))
        return DIR_NONE;

    // Check forwards direction.
    targetDir = GetPrimaryVectorDirection(&objCoords, &playerCoords);
    if (GetCollisionInDirection(objectEvent, targetDir) == COLLISION_NONE)
        return targetDir;

    // Check next best direction.
    altDir = GetSecondaryVectorDirection(&objCoords, &playerCoords);
    if (GetCollisionInDirection(objectEvent, altDir) == COLLISION_NONE)
        return altDir;

    // Check final direction.
    altDir = GetOppositeDirection(targetDir);
    if (GetCollisionInDirection(objectEvent, altDir) == COLLISION_NONE)
        return altDir;

    // Nothing to do :(
    return DIR_NONE;
}
