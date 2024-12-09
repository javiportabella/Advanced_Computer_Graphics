#ifndef PUREPATHTRACING_H
#define PUREPATHTRACING_H

#include "shader.h"

class PurePathTracing : public Shader
{
public:
    PurePathTracing();
    PurePathTracing(Vector3D color_, Vector3D bgColor_);

    Vector3D computeColor(const Ray& r,
        const std::vector<Shape*>& objList,
        const std::vector<LightSource*>& lsList) const;
private:
    Vector3D color;

};

#endif // PUREPATHTRACING_H