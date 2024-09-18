// OCCT
#include <Standard_Real.hxx>

#ifndef GEOALGORITHMSTUDY_01_BOTTLE_H
#define GEOALGORITHMSTUDY_01_BOTTLE_H

class Bottle {
public:
    Bottle(double height, double width, double thickness) {
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
    }
};

#endif //GEOALGORITHMSTUDY_01_BOTTLE_H
