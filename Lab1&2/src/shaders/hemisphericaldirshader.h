#ifndef HEMISPHERICALDIRSHADER_H
#define HEMISPHERICALDIRSHADER_H

#include "shader.h"

class HemisphericalDirShader : public Shader
{
public:
    HemisphericalDirShader();
    HemisphericalDirShader(Vector3D color_, Vector3D bgColor_);

    Vector3D computeColor(const Ray& r,
        const std::vector<Shape*>& objList,
        const std::vector<LightSource*>& lsList) const;
private:
    Vector3D color;

    //Vector3D getSample(const Vector3D& normal) const;  // private function that we only use in compute colorto set a random vector
};

#endif // HEMISPHERICALDIRSHADER_H