#include "whittedintegrator.h"
#include "../materials/phong.h"
#include "../core/utils.h"

WhittedIntegrator::WhittedIntegrator() : Shader() { }

WhittedIntegrator::WhittedIntegrator(Vector3D bgColor_) : Shader(bgColor_) { }


Vector3D WhittedIntegrator::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<LightSource*>& lsList) const
{
    Intersection its;
    Vector3D color(0.0);

    if (Utils::getClosestIntersection(r, objList, its)) {

        const Material& material = its.shape->getMaterial();
        Vector3D n = its.normal;
        Vector3D wo = (-r.d).normalized();
        //Vector3D ambientTerm = Vector3D(0.3) * its.shape->getMaterial().getDiffuseReflectance();

        if (material.hasSpecular()) {
            
            Vector3D wr = n* (2.0 * dot(n, wo)) - wo;
            Ray reflectedRay(its.itsPoint, wr, r.depth + 1, Epsilon, 1e6); //escollim aquest numero perque és un número molt gran -> com posar infinit
            Vector3D reflectedColor = computeColor(reflectedRay, objList, lsList);
            return reflectedColor;
        }
        
        if (material.hasDiffuseOrGlossy()) {
            Vector3D ambientTerm = Vector3D(0.3) * material.getDiffuseReflectance();

            for (size_t lsIndex = 0; lsIndex < lsList.size(); lsIndex++) {

                Vector3D lightSourcePos = lsList[int(lsIndex)]->sampleLightPosition();
                Vector3D wi = (lightSourcePos - its.itsPoint).normalized();

                double distanceLight = (lightSourcePos - its.itsPoint).length();

                Vector3D L_si = lsList[int(lsIndex)]->getIntensity();
                Ray shadowRay = Ray(its.itsPoint, wi, 0, Epsilon, distanceLight);


                if (!Utils::hasIntersection(shadowRay, objList)) {
                    Vector3D fr = its.shape->getMaterial().getReflectance(n, wo, wi);
                    color += L_si * fr * std::max(dot(n, wi), 0.0);
                }
            }
            color += ambientTerm;
            return color;
        }

        if (material.hasTransmission()) {
            bool isEntering = int(dot(wo, n)) < 0; // Mirem si el raig esta entrant o sortint
            double eta_i = isEntering ? 1.0 : material.getIndexOfRefraction(); // Air -> Material
            double eta_t = isEntering ? material.getIndexOfRefraction() : 1.0; // Material -> Air
            double mu_t = eta_i / eta_t;

            Vector3D wi = (-wo).normalized();
            double cos_theta_i = dot(n, wi); 
            double radicand = (1.0 - mu_t * mu_t * (1.0 - cos_theta_i * cos_theta_i));

            if (radicand >= 0.0) {
                Vector3D refractedDir = wi * (-mu_t) + n * (mu_t * cos_theta_i - sqrt(radicand));
                refractedDir.normalized();

                Ray refractedRay(its. itsPoint, refractedDir, 0, Epsilon, 1e6);
                color += computeColor(refractedRay, objList, lsList);
            }
            else {
                Vector3D reflectedDir = wi - n * 2.0 * dot(wi, n);
                reflectedDir.normalized();

                Ray reflectedRay(its.itsPoint, reflectedDir, 0, Epsilon, 1e6);
                color += computeColor(reflectedRay, objList, lsList);
            }
        }

        return Vector3D(0.3) * material.getDiffuseReflectance();

    }
    
    return bgColor;

}
