#include <vtkOBJReader.h>
#include <string>


#ifndef GEOALGORITHMSTUDY_DDGSTRUCTURE_H
#define GEOALGORITHMSTUDY_DDGSTRUCTURE_H


class DDGStructure {
public:
    explicit DDGStructure(const std::string &path) {
        vtkNew<vtkOBJReader> reader;
        reader->SetFileName(path.c_str());
        reader->Update();
        polyData = reader->GetOutput();
    }

    vtkSmartPointer<vtkPolyData> polyData;
};


#endif //GEOALGORITHMSTUDY_DDGSTRUCTURE_H
