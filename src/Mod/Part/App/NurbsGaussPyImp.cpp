/***************************************************************************
 *   Copyright (c) 2020 Christophe Grellier <cg[at]grellier.fr>            *
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


#include "PreCompiled.h"
#ifndef _PreComp_
# include <Precision.hxx>
# include <Standard_Failure.hxx>
# include <math_Matrix.hxx>
# include <math_Gauss.hxx>
#endif

#include "NurbsGaussPy.h"
#include "NurbsGaussPy.cpp"
#include "OCCError.h"
#include <Base/VectorPy.h>
#include "Geometry.h"
#include "TopoShapePy.h"
#include "TopoShapeEdgePy.h"
#include "TopoShapeFacePy.h"
#include "PartPyCXX.h"
#include "NurbsMatrixPy.h"

using namespace Part;


PyObject *NurbsGaussPy::PyMake(struct _typeobject *, PyObject *, PyObject *)  // Python wrapper
{
    // create a new instance of NurbsGaussPy
    return new NurbsGaussPy(nullptr);
}

// returns a string which represents the object e.g. when printed in python
std::string NurbsGaussPy::representation(void) const
{
    return std::string("<NurbsGauss object>");
}

// constructor method
int NurbsGaussPy::PyInit(PyObject* args, PyObject* /*kwds*/)
{
    PyObject* obj;
    if (PyArg_ParseTuple(args, "O", &obj)) {
        try {
            if (PyObject_TypeCheck(obj, &NurbsMatrixPy::Type)) {
                math_Matrix* mat = static_cast<NurbsMatrixPy*>(obj)->getmath_MatrixPtr();
                std::unique_ptr<math_Gauss> ptr(new math_Gauss(*mat));
                setTwinPointer(ptr.release());
                return 0;
            }
        }
        catch (const Standard_Failure& e) {
            PyErr_SetString(PyExc_RuntimeError, e.GetMessageString());
            return -1;
        }
    }

    PyErr_SetString(PyExc_TypeError, "NurbsGauss(matrix)\n");
    return -1;
}

PyObject *NurbsGaussPy::solve(PyObject *args)
{
    PyObject* obj1;
    PyObject* obj2;
    if (!PyArg_ParseTuple(args, "OO", &obj1, &obj2))
        return 0;
    try {
        Py::Sequence constr1(obj1);
        Py::Sequence constr2(obj2);
        int num_poles = constr1.size() + constr2.size();
        if ((constr1.size() < 1) || (constr1.size() < 1))
            Standard_Failure::Raise("not enough points given");
        
        math_Vector res_x(1, num_poles, 0.0);
        math_Vector res_y(1, num_poles, 0.0);
        math_Vector res_z(1, num_poles, 0.0);
        int row_idx = 1;
        int cons_idx = 1;
        for (Py::Sequence::iterator it1 = constr1.begin(); it1 != constr1.end(); ++it1) {
            Py::Vector v(*it1);
            Base::Vector3d pnt = v.toVector();
            res_x(row_idx) = pnt.x;
            res_y(row_idx) = pnt.y;
            res_z(row_idx) = pnt.z;
            row_idx++;
        }
        for (Py::Sequence::iterator it2 = constr2.begin(); it2 != constr2.end(); ++it2) {
            Py::Vector v(*it2);
            Base::Vector3d pnt = v.toVector();
            res_x(row_idx) = pnt.x;
            res_y(row_idx) = pnt.y;
            res_z(row_idx) = pnt.z;
        }
        math_Gauss gauss = getmath_GaussPtr();
        gauss.Solve(res_x);
        if (!gauss.IsDone())
            Standard_Failure::Raise("Failed to solve equations");
        gauss.Solve(res_y);
        if (!gauss.IsDone())
            Standard_Failure::Raise("Failed to solve equations");
        gauss.Solve(res_z);
        if (!gauss.IsDone())
            Standard_Failure::Raise("Failed to solve equations");

        TColgp_Array1OfPnt poles(1,num_poles);
        for (int idx=1; idx<=num_poles; ++idx) {
            poles.SetValue(idx, gp_Pnt(res_x(idx),res_y(idx),res_z(idx)));
        }
    }
}

// PyObject* NurbsGaussPy::getValues(PyObject *args)
// {
//     if (!PyArg_ParseTuple(args, ""))
//         return 0;
//     try {
//         math_Matrix* mat = getmath_MatrixPtr();
//         Py::List values;
//         for (Standard_Integer i=mat->LowerRow(); i<=mat->UpperRow(); i++) {
//             Py::List row;
//             for (Standard_Integer j=mat->LowerCol(); j<=mat->UpperCol(); j++) {
//                 double v = mat->Value(i,j);
//                 row.append(Py::Float(v));
//             }
//             values.append(row);
//         }
//         return Py::new_reference_to(values);
//     }
//     catch (Standard_Failure& e) {
//         PyErr_SetString(PartExceptionOCCError, e.GetMessageString());
//         return 0;
//     }
// }

PyObject *NurbsGaussPy::getCustomAttributes(const char* /*attr*/) const
{
    return 0;
}

int NurbsGaussPy::setCustomAttributes(const char* /*attr*/, PyObject* /*obj*/)
{
    return 0;
}
