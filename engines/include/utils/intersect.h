#pragma once

#include "utils/geom3d.h"
#include "utils/res.h"

namespace e2 {

    bool intersectRayWithPlane(const Ray3d& ray, const Pla3d& plane, std::vector<Vec3d>& pointsOut) {
        Vec3d d = ray.direction();
        Vec3d n = plane.normal();
        double a = d.dot(n);
        
        if (angleZero(a)) { // Check if ray is parallel to plane
            return false;
        }
        
        Vec3d diff = plane.position() - ray.position();
        double t = diff.dot(n) / a;
        
        pointsOut.push_back(ray.position() + d * t);
        return true;
    }

    bool intersectCircleWithPlane(const Cir3d& circle, const Pla3d& plane, std::vector<Vec3d>& pointsOut) {
        Vec3d n = plane.normal();
        Vec3d circleNormal = circle.normal();
        double d = circleNormal.dot(n);
        
        if (angleZero(d)) { // Circle plane is parallel to the plane
            return false;
        }
        
        Vec3d diff = plane.position() - circle.center();
        double t = diff.dot(n) / d;
        
        Vec3d intersectionCenter = circle.center() + circleNormal * t;
        
        double distSq = (intersectionCenter - circle.center()).magnitudeSquared();
        double rSq = circle.radius() * circle.radius();
        
        if (distSq > rSq + RESNOR) {
            return false; // No intersection
        } 
        else if (angleZero(distSq - rSq)) {
            pointsOut.push_back(intersectionCenter); // Tangent intersection
            return true;
        } 
        else {
            double h = std::sqrt(rSq - distSq);
            Vec3d dir = n.cross(circleNormal).normalize();
            pointsOut.push_back(intersectionCenter + dir * h);
            pointsOut.push_back(intersectionCenter - dir * h);
            return true; // Two intersection points
        }
    }

    bool intersectCurveWithPlane(const Geom3d& curve, const Pla3d& plane, std::vector<Vec3d>& pointsOut) {

        Ray3d ray;
        Cir3d cir;
        if (curve.isLine(ray)) {
            return intersectRayWithPlane(ray, plane, pointsOut);
        }
        else if (curve.isCircle(cir)) {
            return intersectCircleWithPlane(cir, plane, pointsOut);
        }
        return false; // Unsupported curve type
    }

    // Computes the (interior) intersection points between a bounded curve and a plane
    bool intersectBoundedCurveWithPlane(const BoundedCurve& boundedCurve, const Pla3d& plane, std::vector<CVec>& pointsOut) {
        const Geom3d& curve = boundedCurve.curve();
        std::vector<Vec3d> allPoints;
        if (intersectCurveWithPlane(curve, plane, allPoints)) {
            for (const auto& point : allPoints) {
                double t = parameterize(curve, point);
                double periodicity;
                if (curve.isPeriodicCurve(periodicity)) {
                    t = pullIntoRange(t, boundedCurve.start().t(), periodicity);
                }
                if (boundedCurve.start().t() < t && t < boundedCurve.end().t() ) {
                    pointsOut.push_back(CVec(point, t));
                }
            }
            std::sort(pointsOut.begin(), pointsOut.end(), [](const CVec& a, const CVec& b) {
                return a.t() < b.t();
            });
        }
        return !pointsOut.empty();
    }
};


    
