#include "purepathtracing.h"
#include "../core/utils.h"
#include "../lightsources/arealightsource.h"
#include "../core/hemisphericalsampler.h"


PurePathTracing::PurePathTracing() :
    color(Vector3D(1, 0, 0))
{ }

PurePathTracing::PurePathTracing(Vector3D hitColor_, Vector3D bgColor_) :
    Shader(bgColor_), color(hitColor_)
{ }


Vector3D PurePathTracing::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<LightSource*>& lsList) const {
    Intersection its;
    Vector3D hitColor(0.0);
    const double PI = 3.14159265358979323846;
    HemisphericalSampler sampler;
    const int MaxDepth = 4;

    // Intentar intersección con la escena
    if (Utils::getClosestIntersection(r, objList, its)) {
        const Material& material = its.shape->getMaterial();  // Material en el punto de intersección
        Vector3D n = its.normal;
        Vector3D wo = (-r.d).normalized();  // direccion hacia la camara

        // Luz emitida por el material
        if (material.isEmissive()) {
            hitColor = material.getEmissiveRadiance();
        }
        
        // Acumulación de Iluminación Directa
        Vector3D directIllumination(0.0);
        const int N = 50;  // Muestras para Monte Carlo

        for (const auto& lightSource : lsList) {
            const AreaLightSource* areaLight = dynamic_cast<const AreaLightSource*>(lightSource);

            Vector3D Le = areaLight->getIntensity();
            double lightArea = areaLight->getArea();

            for (int j = 0; j < N; j++) {
                Vector3D y = areaLight->generateRandomPoint();  // Punto aleatorio en la luz
                Vector3D wi = (y - its.itsPoint).normalized();
                double distanceSquared = pow((y - its.itsPoint).length(), 2);

                // Verificación de sombra
                Ray shadowRay(its.itsPoint, wi, 0, Epsilon, sqrt(distanceSquared) - Epsilon);
                if (Utils::hasIntersection(shadowRay, objList)) continue;

                double G = std::max(dot(n, wi), 0.0) * std::max(dot(areaLight->getNormal(), -wi), 0.0) / distanceSquared;
                Vector3D fr = material.getReflectance(n, wi, wo);

                double probabilityDensity = 1.0 / lightArea;
                directIllumination += (Le * fr * G) / probabilityDensity;
            }
        }

        // Normalizar el promedio de la iluminación directa
        directIllumination /= N;
        hitColor += directIllumination;

        if (r.depth < MaxDepth) {
            Vector3D wj = sampler.getSample(n);
            Ray newR(its.itsPoint, wj, r.depth + 1, Epsilon, 1e6);
            Vector3D indirectLight = computeColor(newR, objList, lsList);
            Vector3D fr = material.getReflectance(n, wj, wo);
            double cosTheta = std::max(0.0, dot(n, wj));
            double pdf = 1.0 / (2.0 * PI);
            hitColor += (indirectLight * fr * cosTheta) / pdf;
        }
    }
    return hitColor;
}