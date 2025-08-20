#ifndef GUARD_PATHFINDING_H
#define GUARD_PATHFINDING_H

bool32 IsObjectEventInRangeOfPlayer(struct ObjectEvent* objectEvent);
bool32 IsObjectEventAdjacentToPlayer(struct ObjectEvent* objectEvent);
u32 GetDirectionTowardsPlayer(struct ObjectEvent* objectEvent);

#endif
