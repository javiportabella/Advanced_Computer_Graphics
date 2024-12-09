#ifndef TRANSMISSIVE_H
#define TRANSMISSIVE_H

#include "../core/vector3d.h"
#include "material.h"

class Transmissive : public Material{

private:
    double refractiveIndex;

public:
    Transmissive(double refractiveIndex_): refractiveIndex(refractiveIndex_) {}

    Vector3D getReflectance(const Vector3D& normal, const Vector3D& wi, const Vector3D& wo) const override {
        return Vector3D(0.0); // there is no reflectance
    }

    double getIndexOfRefraction() const {
        return refractiveIndex;
    }

    bool hasSpecular() const override { return false; }
    bool hasTransmission() const override { return true; } // Ara si que és translucid
    bool hasDiffuseOrGlossy() const override { return false; }
    bool isEmissive() const override { return false; }
};

#endif // TRANSMISSIVE_H
