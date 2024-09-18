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
    unsigned whichTest {0};
    if (argc == 1) {
        whichTest = 0;
    } else {
        whichTest = std::atoi(argv[1]);
    }
    auto a = BRepPrimAPI_MakeBox(10, 10, 20);
    std::vector<std::shared_ptr<TopoDS_Shape>> geoPrims;
    switch (whichTest) {
        case 0: {
            geoPrims.push_back(std::make_shared<TopoDS_Shape>(
                    BRepPrimAPI_MakeBox(10, 10, 20).Shape()));
            break;
        }
        case 1: {
            geoPrims.push_back(Bottle(70., 50., 22.).mainShapePtr);
            break;
        }
        default: break;
    }

    VTKViewer myViewer(1200, 600, "Bottle");
    myViewer.AddShapes(geoPrims);
    myViewer.Start();

    return EXIT_SUCCESS;
}
