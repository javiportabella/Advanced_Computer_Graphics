#include "arealightsource.h"

AreaLightSource::AreaLightSource(Square* areaLightsource_) :
    myAreaLightsource(areaLightsource_)
{ }



Vector3D AreaLightSource::getIntensity() const
{
    return myAreaLightsource->getMaterial().getEmissiveRadiance();
}


Vector3D AreaLightSource::sampleLightPosition()   const
{
    
    //FILL(...)
    // 
    //New Randam Pos inside Area Lightsource
    return Vector3D(0.0);
}

Vector3D AreaLightSource::generateRandomPoint() const {
    double r1 = (double)rand() / RAND_MAX;
    double r2 = (double)rand() / RAND_MAX;

    // Generate random point in local coordinates (assuming the light source is a rectangle)
    Vector3D randomPoint = myAreaLightsource->corner + myAreaLightsource->v1 * r1 + myAreaLightsource->v2 * r2;

    // Transform the point to world space if necessary
    return randomPoint;
}