#include "phong.h"

#include <iostream>
#include <cmath>  //Posem això per poder utilitzar el pi
#include <numbers>

Phong::Phong()
{ }

Phong::Phong(Vector3D Kd_, Vector3D Ks_, float alpha_):
rho_d(Kd_), Ks(Ks_), alpha(alpha_){}


Vector3D Phong::getReflectance(const Vector3D& n, const Vector3D& wo,const Vector3D& wi) const {
    
    double dotProduct = dot(n, wi);

    Vector3D wr = n * (2 * dotProduct) - wi;

    double dotProduct2 = std::max(dot(wr, wo), 0.0);
    const double PI = 3.14159265358979323846;
 

    Vector3D fr = (rho_d / PI ) + Ks * (2*PI/(alpha+1)) * pow(dotProduct2, alpha);

    return fr;

};

double Phong::getIndexOfRefraction() const
{
    std::cout << "Warning! Calling \"Material::getIndexOfRefraction()\" for a non-transmissive material"
              << std::endl;

    return -1;
}


Vector3D Phong::getEmissiveRadiance() const
{
    return Vector3D(0.0);
}


Vector3D Phong::getDiffuseReflectance() const
{
    return rho_d;
}

