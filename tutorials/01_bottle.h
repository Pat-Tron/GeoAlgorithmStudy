// STD
#include <memory>

// OCCT
#include <TopoDS_Shape.hxx>

#ifndef GEOALGORITHMSTUDY_01_BOTTLE_H
#define GEOALGORITHMSTUDY_01_BOTTLE_H

class Bottle {
    // https://dev.opencascade.org/doc/overview/html/occt__tutorial.html
public:
    Bottle(double height, double width, double thickness);
    std::shared_ptr<TopoDS_Shape> mainShapePtr;
};

#endif //GEOALGORITHMSTUDY_01_BOTTLE_H
