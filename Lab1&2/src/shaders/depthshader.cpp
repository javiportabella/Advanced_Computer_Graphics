#include "depthshader.h"

#include "../core/utils.h"

DepthShader::DepthShader() :
    color(Vector3D(1, 0, 0))
{ }

DepthShader::DepthShader(Vector3D hitColor_, double maxDist_, Vector3D bgColor_) :
    Shader(bgColor_), maxDist(maxDist_), color(hitColor_)
{ }

Vector3D DepthShader::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<LightSource*>& lsList) const

{
    Intersection its;

    /* if (Utils::hasIntersection(r, objList)){
        return color;
    }
    else{
        return bgColor;
    }*/


    if (Utils::getClosestIntersection(r, objList, its)) {
        double distance2camera = (r.o - its.itsPoint).length();

        double max_distance = 7.0;
        //argumentar pq max_distance es7. Si fos 1000, els colors ex. (0.0.5) es dividiria per 1000 i sortiria un valor tan petit q les tres esferes no tindrinen diferencia de shader. Es factor per normalitzar.

        double ci = std::max(1.0 - (distance2camera / max_distance), 0.0);

        return color * ci;

    }
    else {
        return bgColor;

    }
    //return color;
}
