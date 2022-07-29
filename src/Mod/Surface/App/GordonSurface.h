/***************************************************************************
 *   Copyright (c) 2022 Christophe Grellier <cg[at]grellier.fr>            *
 *                                                                         *
 *   This file is part of the FreeCAD CAx development system.              *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License as published by the Free Software Foundation; either          *
 *   version 2 of the License, or (at your option) any later version.      *
 *                                                                         *
 *   This library  is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Library General Public License for more details.                  *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this library; see the file COPYING.LIB. If not,    *
 *   write to the Free Software Foundation, Inc., 59 Temple Place,         *
 *   Suite 330, Boston, MA  02111-1307, USA                                *
 *                                                                         *
 ***************************************************************************/

#ifndef SURFACE_GORDONSURFACE_H
#define SURFACE_GORDONSURFACE_H

// #include <App/PropertyStandard.h>
// #include <App/PropertyUnits.h>
// #include <App/PropertyLinks.h>
// #include <Mod/Part/App/FeaturePartSpline.h>
// #include <Mod/Surface/SurfaceGlobal.h>

namespace Surface
{
/**
 * @brief GordonSurface class is used to perform the Gordon Surface algorithm.
 * Given two sets of iso curves, three surfaces must be built :
 * S1 is a cubic interpolation of the U iso curves
 * S2 is a cubic interpolation of the V iso curves
 * S3 is a cubic interpolation of the intersection points
 * S1, S2 and S3 are supposed to have matching orientation,
 * and normalized parameter space.
 * GordonSurface class takes these three surfaces as argument.
 * It matches the degrees, and does knots insertions to make them compatible
 * and does P1 + P2 - P3 for each pole P of these surfaces.
 * This produces a BSpline surface that interpolates both U and V iso curves.
 */
class SurfaceExport GordonSurface: public Base::BaseClass
{
    TYPESYSTEM_HEADER();

public:
    GordonSurface();
    ~GordonSurface();

//     bool checkMatchingCorner(double u, double v, double tol);
//     bool checkMatchingCorners(double tol);
//     bool matchBounds(double u0, double u1, double v0, double v1);
    /**
     * @brief matchDegrees: add another wire, edge, or compound. If compound is
     * added, its internals will be treated as isolated from the rest, and the
     * compounding structure of result will follow.
     * @param sh
     */
    bool matchDegrees();
    /**
     * @brief addShape: add another wire, edge, or compound. If compound is
     * added, its internals will be treated as isolated from the rest, and the
     * compounding structure of result will follow.
     * @param sh
     */
    bool matchKnots();
    /**
     * @brief addShape: add another wire, edge, or compound. If compound is
     * added, its internals will be treated as isolated from the rest, and the
     * compounding structure of result will follow.
     * @param sh
     */
    bool perform();

protected:
    std::vector<TopoDS_Shape> mySourceShapes; //wire or compound
    std::vector<TopoDS_Wire> myWires; //wires from mySourceShapes
    std::vector<TopoDS_Compound> myCompounds; //compounds, for recursive processing
    std::vector<TopoDS_Shape> myShapesToReturn;
};



}//namespace Surface
#endif // SURFACE_GORDONSURFACE_H
