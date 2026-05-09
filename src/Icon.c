
#include "Icon.h"
#include "Vector.h"
#include <math.h>
#include <stdio.h>

void ICO_Update(Icon *icon, float deltaTime, int windowWidth, int windowHeight, Icon* otherIcons, int otherIconsCount, BOOL infectionMode)
{
    int iconSize = windowWidth / 20.0f;
    float killRadius = iconSize / 2.0f * iconSize / 2.0f;
    // check neighbors

    if(icon->m_bDead) return;

    // FUCK you, MSVC!
    DistanceMetric distances[otherIconsCount];

    ICO_FindNeighboringIcons(icon, otherIcons, distances, otherIconsCount);

    DistanceMetric nearestPreyDistMetric = {NULL, INFINITY};

    // distances need to be sorted for attraction
    for(int i = 0; i < otherIconsCount; i++)
    {
        if(distances[i].icon == NULL) continue;
        if(distances[i].icon->m_bDead) continue;
        
        if(ICO_Relationship(icon->m_iAlignment, distances[i].icon->m_iAlignment) == R_AT)
        {
            if(distances[i].dist2 < nearestPreyDistMetric.dist2)
            {
                nearestPreyDistMetric = distances[i];
            }
        }
    }

    icon->m_vecAcceleration = g_vecZero;

    Vector thisPosition = icon->m_vecPosition;
    for(int i = 0; i < otherIconsCount; i++)
    {
        if(distances[i].icon == NULL) continue;
        Icon* other = distances[i].icon;
        float otherDistance = distances[i].dist2;
        // add to the net direction (attract if you can attack it, separate if it's attacking you)
        switch(ICO_Relationship(icon->m_iAlignment, other->m_iAlignment))
        {
            case R_NO:
                break;
            case R_AT:
                // kill it if it's in our killradius
                if(distances[i].dist2 < killRadius)
                {
                    if(infectionMode)
                    {
                        other->m_iAlignment = icon->m_iAlignment;
                    }
                    else {
                        other->m_bDead = TRUE;
                    }
                }
                break;
            case R_FL:
                // run away from it
                icon->m_vecAcceleration = VEC_Add(
                    icon->m_vecAcceleration, 
                    VEC_Mult(VEC_InvSquare(thisPosition, 
                                  other->m_vecPosition, 
                                  otherDistance), ICO_MAX_FORCE * 10));
                break;               
        }
    }

    if(nearestPreyDistMetric.icon != NULL)
    {
        Vector toPrey = VEC_Sub(nearestPreyDistMetric.icon->m_vecPosition, thisPosition);
        toPrey = VEC_Normalize(toPrey);
        toPrey = VEC_Mult(toPrey, ICO_MAX_FORCE);
        icon->m_vecAcceleration = VEC_Add(icon->m_vecAcceleration, toPrey);
    }
    // apply the deltatimes
    icon->m_vecAcceleration = VEC_Limit(icon->m_vecAcceleration, ICO_MAX_FORCE);
    icon->m_vecVelocity = VEC_Add(icon->m_vecVelocity, VEC_Mult(icon->m_vecAcceleration, deltaTime));
    icon->m_vecVelocity = VEC_Mult(VEC_Limit(icon->m_vecVelocity, ICO_MAX_SPEED), 1 - ICO_FRICTION);
    icon->m_vecPosition = VEC_Add(icon->m_vecPosition, VEC_Mult(icon->m_vecVelocity, deltaTime));

    // do some edge detection wraparound
    if(icon->m_vecPosition.x > windowWidth)
    {
        icon->m_vecPosition.x = 0;
    }
    else if(icon->m_vecPosition.x < 0)
    {
        icon->m_vecPosition.x = windowWidth;
    }

    if(icon->m_vecPosition.y > windowHeight)
    {
        icon->m_vecPosition.y = 0;
    }
    else if(icon->m_vecPosition.y < 0)
    {
        icon->m_vecPosition.y = windowHeight;
    }
}

Relationship ICO_Relationship(IconType thisType, IconType otherType)
{
    static const Relationship table[3][3] = 
    {                /*  ROCK   PAPER   SCISSORS */
        /*     Rock */ { R_NO,  R_FL,   R_AT},
        /*    Paper */ { R_AT,  R_NO,   R_FL},
        /* Scissors */ { R_FL,  R_AT,   R_NO}
    };

    return table[thisType][otherType];
}

void ICO_FindNeighboringIcons(Icon *icon, Icon* otherIcons, DistanceMetric *distances, int iconSize)
{
    for(int i = 0; i < iconSize; i++)
    {
        if(&otherIcons[i] == icon)
        {
            distances[i].icon = NULL;
            distances[i].dist2 = INFINITY;
            continue;
        }
        distances[i].icon = &otherIcons[i];
        distances[i].dist2 = VEC_Dist2(icon->m_vecPosition, otherIcons[i].m_vecPosition);
    
        if(distances[i].dist2 < 0.01f)
        {
            distances[i].dist2 = 0.01f;
        }
    }
}