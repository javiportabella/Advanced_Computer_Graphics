#ifndef MIRROR_H
#define MIRROR_H

#include "../core/vector3d.h"
#include "material.h"

class Mirror : public Material
{
public:
    Mirror() {}

    Vector3D getReflectance(const Vector3D& n, const Vector3D& wo, const Vector3D& wi) const override {
        return Vector3D(1.0); // El color reflexat és exactament el que hi ha al seu entorn.
    }

    bool hasSpecular() const override { return true; }
    bool hasTransmission() const override { return false; } // No és translucid si és un mirall
    bool hasDiffuseOrGlossy() const override { return false; }
    bool isEmissive() const override { return false; }
};

#endif // MIRROR_H
