/***************************************************************************
 *   Copyright (c) 2020 Christophe Grellier <cg (at) grellier.fr>          *
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


# include "PreCompiled.h"

#ifndef _PreComp_
# include <iostream>
# include <fstream>
# include <string>
# include <sstream>
# include <cstdio>
# include <cstdlib>
# include <stdexcept>
# include <vector>

# include <TColStd_Array1OfReal.hxx>
# include <TColStd_Array1OfInteger.hxx>
# include <TColgp_Array1OfPnt.hxx>
# include <TColgp_Array2OfPnt.hxx>
# include <TColgp_Array1OfPnt2d.hxx>
# include <Geom_BSplineSurface.hxx>
# include <BSplCLib.hxx>
# include <math_Matrix.hxx>

#endif // _PreComp

# include <Base/Vector3D.h>
# include <Base/Exception.h>
# include <Base/Writer.h>
# include <Base/Reader.h>
# include <Base/Tools.h>

#include "BezierSolver.h"


using namespace Part;

TYPESYSTEM_SOURCE_ABSTRACT(Part::BezierSolver, Base::BaseClass)

// BezierSolver::BezierSolver()
// : matrix(1, 1, 1, 1)
// {
//     Init();
// }
// 
// void BezierSolver::Init()
// {
//     nb_start_vecs = 1;
//     nb_end_vecs = 1;
//     BezierSolver::buildMatrix();
// }

BezierSolver::BezierSolver(const std::vector<Base::Vector3d>& start_constraints, const std::vector<Base::Vector3d>& end_constraints)
: matrix(1, start_constraints.size(), 1, end_constraints.size())
{
    if ((start_constraints.size() == 0) || (end_constraints.size() == 0)) {
        Standard_Failure::Raise("Not enough constraints");
    }
    nb_start_vecs = start_constraints.size();
    nb_end_vecs = end_constraints.size();
    BezierSolver::buildMatrix();
    BezierSolver::solve(start_constraints, end_constraints);
}
/*
BezierSolver::~BezierSolver()
{
}*/

// Geometry *ConstrainedBezierCurve::copy(void) const
// {
//     ConstrainedBezierCurve *newCurve = new ConstrainedBezierCurve(myCurve);
//     newCurve->Construction = this->Construction;
//     return newCurve;
// }

void BezierSolver::buildMatrix()
{
    int num_poles = nb_start_vecs + nb_end_vecs;
    if (num_poles > 24) // this->myCurve->MaxDegree())
        Standard_Failure::Raise("number of constraints exceeds bezier curve capacity");
    // create a bezier-type knot sequence
    TColStd_Array1OfReal knots(1, 2*num_poles);
    for (int idx=1; idx<=num_poles; ++idx) {
        knots(idx) = 0.0;
        knots(num_poles+idx) = 1.0;
    }
    math_Matrix OCCmatrix(1, num_poles, 1, num_poles, 0.0);
    int row_idx = 1;
    math_Matrix bezier_eval(1, this->nb_start_vecs, 1, num_poles, 0.0);
    Standard_Integer first_non_zero;
    Standard_Integer error_code = BSplCLib::EvalBsplineBasis(this->nb_start_vecs-1, num_poles, knots, 0.0, first_non_zero, bezier_eval, Standard_False);
    for (int idx=1; idx<=this->nb_start_vecs; ++idx) {
        OCCmatrix.SetRow(row_idx, bezier_eval.Row(idx));
        row_idx++;
    }
    math_Matrix bezier_eval2(1, this->nb_end_vecs, 1, num_poles, 0.0);
    error_code = BSplCLib::EvalBsplineBasis(this->nb_end_vecs-1, num_poles, knots, 1.0, first_non_zero, bezier_eval2, Standard_False);
    for (int idx=1; idx<=this->nb_end_vecs; ++idx) {
        OCCmatrix.SetRow(row_idx, bezier_eval2.Row(idx));
        row_idx++;
    }
    matrix = OCCmatrix;
}

TColgp_Array1OfPnt BezierSolver::solve(const std::vector<Base::Vector3d>& start_constraints, const std::vector<Base::Vector3d>& end_constraints)
{
    if ((start_constraints.size() == 0) || (end_constraints.size() == 0))
        throw Base::ValueError("Not enough constraints");
    if (((int)start_constraints.size() != nb_start_vecs) || ((int)end_constraints.size() != nb_end_vecs)) {
        nb_start_vecs = start_constraints.size();
        nb_end_vecs = end_constraints.size();
        BezierSolver::buildMatrix();
    }
    int num_poles = nb_start_vecs + nb_end_vecs;
    math_Vector res_x(1, num_poles, 0.0);
    math_Vector res_y(1, num_poles, 0.0);
    math_Vector res_z(1, num_poles, 0.0);
    TColgp_Array1OfPnt cons(1, num_poles);
    for (std::size_t i = 1; i <= start_constraints.size(); i++) {
        res_x(i) = start_constraints[i-1].x;
        res_y(i) = start_constraints[i-1].y;
        res_z(i) = start_constraints[i-1].z;
    }
    for (std::size_t i = 1; i <= end_constraints.size(); i++) {
        res_x(start_constraints.size() + i) = end_constraints[i-1].x;
        res_y(start_constraints.size() + i) = end_constraints[i-1].y;
        res_z(start_constraints.size() + i) = end_constraints[i-1].z;
    }

//     math_Matrix OCCmatrix(*this->matrix);
//     math_Gauss gauss(OCCmatrix); // *this->matrix);
//     gauss.Solve(res_x);
//     gauss.Solve(res_y);
//     gauss.Solve(res_z);
//     if (!gauss.IsDone())
//         Standard_Failure::Raise("Failed to solve equations");

    TColgp_Array1OfPnt poles(1,num_poles);
    for (int idx=1; idx<=num_poles; ++idx) {
        poles.SetValue(idx, gp_Pnt(res_x(idx),res_y(idx),res_z(idx)));
    }
    return poles;

//     Handle(Geom_BezierCurve) b = new Geom_BezierCurve(poles);
//     this->myCurve = b;
//     }
//     catch (Standard_Failure& e) {
//         THROWM(Base::CADKernelError,e.GetMessageString())
//     }
}
 
