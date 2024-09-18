// OCCT
#include <BRepBuilderAPI_MakeShape.hxx>
#include <BRepPrimAPI_MakeBox.hxx>

// VTK Viewer
#include "VTKViewer.h"

// STD
#include <vector>
#include <memory>

// Tutorials
#include "tutorials/01_bottle.h"

int main(int argc, char** argv)
{
    unsigned whichTest = 0;
    std::vector<std::shared_ptr<BRepBuilderAPI_MakeShape>> geoPrims;

    switch (whichTest) {
        case 0: {
            geoPrims.push_back(std::make_shared<BRepPrimAPI_MakeBox>(
                    BRepPrimAPI_MakeBox(10, 10, 20)));
            break;
        }
        case 1: {
            Bottle(70., 50., 30.);
            break;
        }
        default: break;
    }

    VTKViewer myViewer(1200, 600, "Bottle");
    myViewer.AddShapes(geoPrims);
    myViewer.Start();

    return EXIT_SUCCESS;
}
