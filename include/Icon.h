#ifndef ICON_H
#define ICON_H
#include "Vector.h"
#include "defines.h"

#define ICO_MAX_FORCE 50.0f
#define ICO_MAX_SPEED 100.0f
#define ICO_FRICTION 0.001f

typedef enum
{
    ICO_ROCK,
    ICO_PAPER,
    ICO_SCISSORS
} IconType;

typedef struct 
{
    Vector m_vecPosition;
    Vector m_vecVelocity;
    Vector m_vecAcceleration;

    IconType m_iAlignment;
    BOOL m_bDead;
} Icon;

typedef enum
{
    R_AT,
    R_NO,
    R_FL,
} Relationship;

typedef struct
{
    Icon* icon;
    float dist2;
} DistanceMetric;

void ICO_Update(Icon* icon, float deltaTime, int windowWidth, int windowHeight, Icon* otherIcons, int otherIconsCount, BOOL infectionMode);
void ICO_FindNeighboringIcons(Icon* icon, Icon* otherIcons, DistanceMetric* distances, int iconSize);

Relationship ICO_Relationship(IconType thisType, IconType otherType);

#endif