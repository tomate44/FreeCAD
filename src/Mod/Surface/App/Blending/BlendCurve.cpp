/***************************************************************************
 *   Copyright (c) 2022 Matteo Grellier <matteogrellier@gmail.com>         *
 *                                                                         *
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

#include "../PreCompiled.h"
#ifndef _PreComp_
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BSplCLib.hxx>
#include <Base/Persistence.h>
#include <Base/Vector3D.h>
#include <GeomAPI_PointsToBSplineSurface.hxx>
#include <Geom_BSplineSurface.hxx>
#include <Geom_BezierCurve.hxx>
#include <Mod/Part/App/Geometry.h>
#include <Precision.hxx>
#include <Standard_Version.hxx>
#include <TColStd_Array1OfReal.hxx>
#include <TColgp_Array1OfPnt.hxx>
#include <TopoDS.hxx>
#include <gp_Pnt.hxx>
#include <math_Gauss.hxx>
#include <math_Matrix.hxx>
#endif
#include "BlendCurve.h"

using namespace Surface;

BlendCurve::BlendCurve()
{
}

BlendCurve::BlendCurve(std::vector<BlendPoint> blendPointsList)
{
    // Retrieve number of blendPoints and push them into blendPoints.
    int nb_pts = blendPointsList.size();

    if (nb_pts > 2) {
        throw Base::ValueError("Not implemented");
    }
    else if (nb_pts < 2) {
        throw Base::ValueError("Need two points for working");
    }

    for (size_t i = 0; i < nb_pts; i++) {
        blendPoints.emplace_back(blendPointsList[i]);
    }
}

BlendCurve::~BlendCurve()
{
}

Handle(Geom_BezierCurve) BlendCurve::compute()
{
    // Make BlendCurve from BlendPoints.
    int nb_pts = blendPoints.size();
    try {

        TColStd_Array1OfReal params(1, nb_pts);
        for (int i = 0; i < nb_pts; ++i) {
            params(i + 1) = (double)i / ((double)nb_pts - 1);
        }

        int num_poles = 0;
        for (int i = 0; i < nb_pts; ++i) {
            num_poles += blendPoints[i].vectors.size();
        }
        Handle(Geom_BezierCurve) curve;
        if (num_poles > curve->MaxDegree())// use Geom_BezierCurve max degree
            Standard_Failure::Raise("number of constraints exceeds bezier curve capacity");
        // create a bezier-type knot sequence
        TColStd_Array1OfReal knots(1, 2 * num_poles);
        for (int i = 1; i <= num_poles; ++i) {
            knots(i) = params(1);
            knots(num_poles + i) = params(nb_pts);
        }

        math_Matrix OCCmatrix(1, num_poles, 1, num_poles, 0.0);
        math_Vector res_x(1, num_poles, 0.0);
        math_Vector res_y(1, num_poles, 0.0);
        math_Vector res_z(1, num_poles, 0.0);
        int row_idx = 1;
        int cons_idx = 1;
        for (int i = 0; i < nb_pts; ++i) {
            math_Matrix bezier_eval(1, blendPoints[i].vectors.size(), 1, num_poles, 0.0);
            Standard_Integer first_non_zero;
            BSplCLib::EvalBsplineBasis(blendPoints[i].vectors.size() - 1, num_poles, knots, params(cons_idx), first_non_zero, bezier_eval, Standard_False);
            int idx2 = 1;
            for (int it2 = 0; it2 < blendPoints[i].vectors.size(); ++it2) {
                OCCmatrix.SetRow(row_idx, bezier_eval.Row(idx2));
                Base::Vector3d pnt = blendPoints[i].vectors[it2];
                res_x(row_idx) = pnt.x;
                res_y(row_idx) = pnt.y;
                res_z(row_idx) = pnt.z;
                idx2++;
                row_idx++;
            }
            cons_idx++;
        }
        math_Gauss gauss(OCCmatrix);
        gauss.Solve(res_x);
        if (!gauss.IsDone())
            Standard_Failure::Raise("Failed to solve equations");
        gauss.Solve(res_y);
        if (!gauss.IsDone())
            Standard_Failure::Raise("Failed to solve equations");
        gauss.Solve(res_z);
        if (!gauss.IsDone())
            Standard_Failure::Raise("Failed to solve equations");

        TColgp_Array1OfPnt poles(1, num_poles);
        for (int idx = 1; idx <= num_poles; ++idx) {
            poles.SetValue(idx, gp_Pnt(res_x(idx), res_y(idx), res_z(idx)));
        }

        Handle(Geom_BezierCurve) bezier = new Geom_BezierCurve(poles);

        return bezier;
    }
    catch (Standard_Failure &e) {
        Standard_Failure::Raise("Error");
    }
}

void BlendCurve::setSize(int i, double f)
{
    if (i >= static_cast<int>(blendPoints.size())) {
        Standard_Failure::Raise("BlendPoint Index out of range");
    }
    blendPoints[i].setSize(f);
}

unsigned int BlendCurve::getMemSize(void) const
{
    return 1;
}

PyObject *BlendCurve::getPyObject(void)
{
    return nullptr;
}

void BlendCurve::Save(Base::Writer & /*writer*/) const
{
    throw Base::NotImplementedError("BlendCurve::Save");
}

void BlendCurve::Restore(Base::XMLReader & /*reader*/)
{
    throw Base::NotImplementedError("BlendCurve::Restore");
}
