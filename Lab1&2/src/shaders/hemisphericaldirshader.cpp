#include "hemisphericaldirshader.h"
#include "../core/utils.h"
#include "../core/hemisphericalsampler.h"
#include <cmath>

HemisphericalDirShader::HemisphericalDirShader() :
    color(Vector3D(1, 0, 0))
{ }

HemisphericalDirShader::HemisphericalDirShader(Vector3D hitColor_, Vector3D bgColor_) :
    Shader(bgColor_), color(hitColor_)
{ }


Vector3D HemisphericalDirShader::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<LightSource*>& lsList) const {
    Intersection its;
    Vector3D hitColor = color;
    const double PI = 3.14159265358979323846;

    // Check if the ray intersects any object
    if (Utils::getClosestIntersection(r, objList, its)) {
        const Material& material = its.shape->getMaterial();  // Get the material of the intersected shape
        Vector3D n = its.normal;  // Normal at the intersection point
        Vector3D wo = (-r.d).normalized();  // Direction towards the camera (outgoing ray)
        Vector3D ambientTerm = Vector3D(0.2) * its.shape->getMaterial().getDiffuseReflectance();

        const int N = 256;  // Number of samples for Monte Carlo integration
        double probabilityofwi = 1.0 / (2.0 * PI);  // Probability density for uniform hemispherical sampling
        Vector3D directIllumination(0.0);  // Initialize direct illumination to zero
        HemisphericalSampler sampler;  // Create a sampler for hemisphere sampling

        if (material.isEmissive()) {
            return material.getEmissiveRadiance();
        }

        // Monte Carlo integration loop for N samples
        for (int j = 0; j < N; j++) {
            Vector3D wj = sampler.getSample(n);  // Get a random sample direction over the hemisphere
            Ray ray(its.itsPoint, wj, 0, Epsilon, 1e6);  // Cast a shadow ray in the sample direction
            int MaxDepth = 4;

            // If the recursion depth exceeds the max depth, return the accumulated color
            if (r.depth > MaxDepth) {
                return hitColor;
            }

            Intersection its2;  // Store the intersection information for the light

            // Check if the shadow ray intersects with any object
            if (Utils::getClosestIntersection(ray, objList, its2)) {
                const Material& lightMaterial = its2.shape->getMaterial();  // Get the material of the intersected shape

                // Check if the material is emissive
                if (lightMaterial.isEmissive()) {
                    Vector3D Le = lightMaterial.getEmissiveRadiance();  // Get the emissive radiance
                    Vector3D fr = material.getReflectance(n, wj, wo);  // Compute the BRDF at the intersection point
                    double cosTheta = std::max(dot(wj, n), 0.0);  // Cosine of the angle between the sample direction and the normal

                    // Accumulate the contribution of this light sample to the total illumination
                    directIllumination += (Le * fr * cosTheta) / probabilityofwi;
                }

                if (material.hasSpecular()) {

                    Vector3D reflectionDir = n * (2.0 * dot(n, wo)) - wo;
                    Ray reflectedRay(its.itsPoint, reflectionDir, r.depth + 1, Epsilon, 1e6); //escollim aquest numero perque és un número molt gran -> com posar infinit
                    Vector3D reflectedColor = computeColor(reflectedRay, objList, lsList);
                    return reflectedColor;
                }
            }
        }
        // Average the accumulated direct illumination over the number of samples

        hitColor = (directIllumination / N) + ambientTerm;
    }
    return hitColor;  // Return the computed color
}