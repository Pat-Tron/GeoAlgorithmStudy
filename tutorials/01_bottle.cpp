# include "01_bottle.h"

#include <Geom_TrimmedCurve.hxx>
#include <Geom_Plane.hxx>
#include <Geom_CylindricalSurface.hxx>
#include <Geom2d_Ellipse.hxx>
#include <Geom2d_TrimmedCurve.hxx>

#include <GC_MakeArcOfCircle.hxx>
#include <GC_MakeSegment.hxx>
#include <GCE2d_MakeSegment.hxx>

#include <TopoDS.hxx>
#include <TopoDS_Wire.hxx>
#include <TopExp_Explorer.hxx>
#include <gp_Ax1.hxx>

#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepFilletAPI_MakeFillet.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepOffsetAPI_MakeThickSolid.hxx>
#include <BRepLib.hxx>
#include <BRepOffsetAPI_ThruSections.hxx>

Bottle::Bottle(double height, double width, double thickness){
    // Unit: mm
    /*
     * This modeling requires four steps:
     * build the bottle's Profile
     * build the bottle's Body
     * build the Threading on the bottle's neck
     * build the result compound
     */

    /* Create bottle's bottom profile in negative Y direction.
     * ---1-----o-----5---
     *    |     |     |
     *    2     |     4
     *     \____3____/
     */
    gp_Pnt aPnt1(-width / 2., 0, 0);
    gp_Pnt aPnt2(-width / 2., -thickness / 4., 0);
    gp_Pnt aPnt3(0, -thickness / 2., 0);
    gp_Pnt aPnt4(width / 2., -thickness / 4., 0);
    gp_Pnt aPnt5(width / 2., 0, 0);

    Handle(Geom_TrimmedCurve) aArcOfCircle = GC_MakeArcOfCircle(aPnt2,aPnt3,aPnt4);
    Handle(Geom_TrimmedCurve) aSegment1    = GC_MakeSegment(aPnt1, aPnt2);
    Handle(Geom_TrimmedCurve) aSegment2    = GC_MakeSegment(aPnt4, aPnt5);

    // NOTE: GC method will raise an exception if construction has failed.
    //GC_MakeSegment mkSegment (aPnt1, aPnt2);
    //Handle(Geom_TrimmedCurve) aSegment;
    //if(mkSegment.IsDone()){
    //    aSegment = mkSegment.Value();
    //}
    //else {
    //    // handle error
    //}

    // Edges
    TopoDS_Edge anEdge1 = BRepBuilderAPI_MakeEdge(aSegment1);
    // Also: TopoDS_Edge anEdge1 = BRepBuilderAPI_MakeEdge(aPnt1, aPnt2);
    TopoDS_Edge anEdge2 = BRepBuilderAPI_MakeEdge(aArcOfCircle);
    TopoDS_Edge anEdge3 = BRepBuilderAPI_MakeEdge(aSegment2);

    // Wire
    TopoDS_Wire aWire = BRepBuilderAPI_MakeWire(anEdge1, anEdge2, anEdge3);

    // Rotation/Reflection axis: a unitary vector with positive direction
    gp_Ax1 xAxis = gp::OX();
    // or
    //gp_Pnt aOrigin(0, 0, 0);
    //gp_Dir xDir(1, 0, 0);
    //gp_Ax1 xAxis(aOrigin, xDir);

    // Transformation
    gp_Trsf aTrsf;
    aTrsf.SetMirror(xAxis);
    BRepBuilderAPI_Transform aBRepTrsf(aWire, aTrsf);
    TopoDS_Shape aMirroredShape = aBRepTrsf.Shape();
    TopoDS_Wire aMirroredWire = TopoDS::Wire(aMirroredShape);

    // Concatenate the two wires into a single wire.
    BRepBuilderAPI_MakeWire mkWire;
    mkWire.Add(aWire);
    mkWire.Add(aMirroredWire);
    TopoDS_Wire myWireProfile = mkWire.Wire();

    // Compute a face out of its wire.
    TopoDS_Face myFaceProfile = BRepBuilderAPI_MakeFace(myWireProfile);

    // Vector to prism the profile.
    gp_Vec aPrismVec(0, 0, height);

    // Prism.
    TopoDS_Shape myBody = BRepPrimAPI_MakePrism(myFaceProfile, aPrismVec);

    // Prepare making fillets.
    BRepFilletAPI_MakeFillet mkFillet(myBody);

    // Get all edges.
    TopExp_Explorer anEdgeExplorer(myBody, TopAbs_EDGE);
    while(anEdgeExplorer.More()){
        TopoDS_Edge anEdge = TopoDS::Edge(anEdgeExplorer.Current());
        mkFillet.Add(thickness / 12., anEdge);
        anEdgeExplorer.Next();
    }

    // Make fillets.
    myBody = mkFillet.Shape();


    // Adding the Neck.
    // Specify local coordinate of neck creating.
    gp_Ax2 neckAx2(gp_Pnt(0, 0, height), gp::DZ(), gp::DX());

    // Create cylinder.
    Standard_Real neckRadius = thickness / 4.;
    Standard_Real neckHeight = height / 10;
    BRepPrimAPI_MakeCylinder MKCylinder(neckAx2, neckRadius, neckHeight);
    TopoDS_Shape myNeck = MKCylinder.Shape();

    // Fuse.
    myBody = BRepAlgoAPI_Fuse(myBody, myNeck);

    // Find the highest plane which will be cut later.
    TopoDS_Face faceToRemove;
    Standard_Real zMax = -1;
    for(TopExp_Explorer aFaceExplorer(myBody, TopAbs_FACE) ; aFaceExplorer.More() ; aFaceExplorer.Next()){
        TopoDS_Face aFace = TopoDS::Face(aFaceExplorer.Current());
        Handle(Geom_Surface) aSurface = BRep_Tool::Surface(aFace);

        if(aSurface->DynamicType() == STANDARD_TYPE(Geom_Plane)){
            Handle(Geom_Plane) aPlane = Handle(Geom_Plane)::DownCast(aSurface);

            gp_Pnt aPnt = aPlane->Location();
            Standard_Real aZ = aPnt.Z();
            if(aZ > zMax){
                zMax = aZ;
                faceToRemove = aFace;
            }
        }
    }

    // Remove the highest plane.
    TopTools_ListOfShape facesToRemove;
    facesToRemove.Append(faceToRemove);

    //
    BRepOffsetAPI_MakeThickSolid aSolidMaker;
    aSolidMaker.MakeThickSolidByJoin(myBody, facesToRemove, -thickness / 50, 1.e-3);
    myBody = aSolidMaker.Shape();


    // Building the Threading
    // Create two cylindrical surfaces to clamp the threading
    Handle(Geom_CylindricalSurface) aCyl1 = new Geom_CylindricalSurface(neckAx2, neckRadius * 0.99);
    Handle(Geom_CylindricalSurface) aCyl2 = new Geom_CylindricalSurface(neckAx2, neckRadius * 1.05);

    // Local coordinate for threading
    gp_Pnt2d aPnt(2. * M_PI, neckHeight / 2.);
    gp_Dir2d aDir(2. * M_PI, neckHeight / 4.);
    gp_Ax2d anAx2d(aPnt, aDir);

    // Two ellipses
    Standard_Real aMajor = 2. * M_PI;
    Standard_Real aMinor = neckHeight / 10;
    Handle(Geom2d_Ellipse) anEllipse1 = new Geom2d_Ellipse(anAx2d, aMajor, aMinor);
    Handle(Geom2d_Ellipse) anEllipse2 = new Geom2d_Ellipse(anAx2d, aMajor, aMinor / 4.);

    // Make the upper halves of the two ellipses into parametric curves.
    Handle(Geom2d_TrimmedCurve) anArc1 = new Geom2d_TrimmedCurve(anEllipse1, 0, M_PI); // Why PI but 2*PI ?
    Handle(Geom2d_TrimmedCurve) anArc2 = new Geom2d_TrimmedCurve(anEllipse2, 0, M_PI);

    // Start point of arc1 and end point of arc2.
    gp_Pnt2d anEllipsePnt1 = anEllipse1->Value(0);
    gp_Pnt2d anEllipsePnt2 = anEllipse1->Value(M_PI);;
    // or: anEllipse1->D0(M_PI, anEllipsePnt2);

    // Segment with ends of the points made above.
    // The segment is a parametric line, which is not depend on any surfaces.
    Handle(Geom2d_TrimmedCurve) aSegment = GCE2d_MakeSegment(anEllipsePnt1, anEllipsePnt2);

    // Wire on each cylindrical surface is upper half of a ellipse.
    TopoDS_Edge anEdge1OnSurf1 = BRepBuilderAPI_MakeEdge(anArc1, aCyl1);
    TopoDS_Edge anEdge2OnSurf1 = BRepBuilderAPI_MakeEdge(aSegment, aCyl1);
    TopoDS_Edge anEdge1OnSurf2 = BRepBuilderAPI_MakeEdge(anArc2, aCyl2);
    TopoDS_Edge anEdge2OnSurf2 = BRepBuilderAPI_MakeEdge(aSegment, aCyl2);

    TopoDS_Wire threadingWire1 = BRepBuilderAPI_MakeWire(anEdge1OnSurf1, anEdge2OnSurf1);
    TopoDS_Wire threadingWire2 = BRepBuilderAPI_MakeWire(anEdge1OnSurf2, anEdge2OnSurf2);

    // Turn 2D wires into 3D wires.
    BRepLib::BuildCurves3d(threadingWire1);
    BRepLib::BuildCurves3d(threadingWire2);

    // Build loft(放样).
    BRepOffsetAPI_ThruSections aTool(Standard_True);
    aTool.AddWire(threadingWire1);
    aTool.AddWire(threadingWire2);
    aTool.CheckCompatibility(Standard_False);
    TopoDS_Shape myThreading = aTool.Shape();

    // Compound
    TopoDS_Compound aRes;
    BRep_Builder aBuilder;
    aBuilder.MakeCompound (aRes);
    aBuilder.Add (aRes, myBody);
    aBuilder.Add (aRes, myThreading);

    mainShapePtr = std::make_shared<TopoDS_Shape>(aRes);
}