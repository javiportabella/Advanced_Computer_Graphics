#include "areadirshader.h"
#include "../core/utils.h"
#include "../lightsources/arealightsource.h"


AreaDirShader::AreaDirShader() :
    color(Vector3D(1, 0, 0))
{ }

AreaDirShader::AreaDirShader(Vector3D hitColor_, Vector3D bgColor_) :
    Shader(bgColor_), color(hitColor_)
{ }


Vector3D AreaDirShader::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<LightSource*>& lsList) const {
    Intersection its;
    Vector3D hitColor = color;

    if (Utils::getClosestIntersection(r, objList, its)) {
        const Material& material = its.shape->getMaterial();  // Material del punto con interseccion
        Vector3D n = its.normal;
        Vector3D wo = (-r.d).normalized();  // direccion hacia la camara
        Vector3D ambientTerm = Vector3D(0.2) * its.shape->getMaterial().getDiffuseReflectance();

        const int N = 256;  // Nº para Monte Carlo
        Vector3D directIllumination(0.0);

        if (material.isEmissive()) {
            return material.getEmissiveRadiance();
        }

        for (const auto& lightSource : lsList) {
            const AreaLightSource* areaLight = dynamic_cast<const AreaLightSource*>(lightSource);

            Vector3D Le = areaLight->getIntensity();
            double lightArea = areaLight->getArea();


            for (int j = 0; j < N; j++) {

                Vector3D y = areaLight->generateRandomPoint();  // We take a random point on the light source
                Vector3D wi = (y - its.itsPoint).normalized();
                double distanceSquared = pow((y - its.itsPoint).length(), 2);

                Ray shadowRay(its.itsPoint, wi, 0, Epsilon, sqrt(distanceSquared) - Epsilon);  // Ray from lightsource point to itsPoint
                if (Utils::hasIntersection(shadowRay, objList)) continue;

                double G = std::max(dot(n, wi), 0.0) * std::max(dot(areaLight->getNormal(), -wi), 0.0) / distanceSquared;   // Calculamos G(x, y)

                Vector3D fr = material.getReflectance(n, wi, wo);    //BRDF en el punto de interseccion

                double probabilityDensity = 1.0 / lightArea;  // Probability of choosing this sample
                directIllumination += (Le * fr * G) / probabilityDensity;

                if (material.hasSpecular()) {

                    Vector3D reflectionDir = n * (2.0 * dot(n, wo)) - wo;
                    Ray reflectedRay(its.itsPoint, reflectionDir, r.depth + 1, Epsilon, 1e6); //escollim aquest numero perque és un número molt gran -> com posar infinit
                    Vector3D reflectedColor = computeColor(reflectedRay, objList, lsList);
                    return reflectedColor;
                }

            }
        }

        hitColor = (directIllumination / N) + ambientTerm;
    }

    return hitColor;
}
