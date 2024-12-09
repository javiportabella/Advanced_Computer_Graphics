#ifndef AREADIRSHADER_H
#define AREADIRSHADER_H

#include "shader.h"

class AreaDirShader : public Shader
{
public:
    AreaDirShader();
    AreaDirShader(Vector3D color_, Vector3D bgColor_);

    Vector3D computeColor(const Ray& r,
        const std::vector<Shape*>& objList,
        const std::vector<LightSource*>& lsList) const;
private:
    Vector3D color;

};

#endif // AREADIRSHADER_H