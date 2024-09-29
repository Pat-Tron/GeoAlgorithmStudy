#include <vtkOBJReader.h>
#include <TopoDS_Shape.hxx>
#include <RWObj_Provider.hxx>
#include <RWObj_ConfigurationNode.hxx>

#include <string>


#ifndef GEOALGORITHMSTUDY_DDGSTRUCTURE_H
#define GEOALGORITHMSTUDY_DDGSTRUCTURE_H


class DDGStructure {
public:
    explicit DDGStructure(const std::string &path) {
        Handle(RWObj_ConfigurationNode) conNode = new RWObj_ConfigurationNode();
        RWObj_Provider provider(conNode);
        TopoDS_Shape shape;
        if(provider.Read(path.c_str(), shape)) {
            shapePtr = std::make_shared<TopoDS_Shape>(shape);
        } else {
            return;
        }
    }

    std::shared_ptr<TopoDS_Shape> shapePtr = nullptr;
};


#endif //GEOALGORITHMSTUDY_DDGSTRUCTURE_H
