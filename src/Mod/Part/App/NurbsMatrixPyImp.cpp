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
#endif

#include "NurbsMatrixPy.h"
#include "NurbsMatrixPy.cpp"
#include "OCCError.h"
#include <Base/VectorPy.h>
#include "Geometry.h"
#include "TopoShapePy.h"
#include "TopoShapeEdgePy.h"
#include "TopoShapeFacePy.h"
#include "PartPyCXX.h"

using namespace Part;


PyObject *NurbsMatrixPy::PyMake(struct _typeobject *, PyObject *, PyObject *)  // Python wrapper
{
    // create a new instance of NurbsMatrixPy
    return new NurbsMatrixPy(nullptr);
}

// returns a string which represents the object e.g. when printed in python
std::string NurbsMatrixPy::representation(void) const
{
    math_Matrix* mat = getmath_MatrixPtr();
    std::stringstream str;
    str << "<NurbsMatrix (";
    str << mat->RowNumber() << " x " ;
    str << mat->ColNumber() << ") object>";
    return str.str();
//     return std::string("<NurbsMatrix object>");
}

// constructor method
int NurbsMatrixPy::PyInit(PyObject* args, PyObject* /*kwds*/)
{
    int m=2;
    int n=2;
    double fill = 0.0;
    if (PyArg_ParseTuple(args, "|iid", &m, &n, &fill)) {
        try {
            std::unique_ptr<math_Matrix> ptr(new math_Matrix(1, m, 1, n, fill));
            setTwinPointer(ptr.release());
            return 0;
        }
        catch (const Standard_Failure& e) {
            PyErr_SetString(PyExc_RuntimeError, e.GetMessageString());
            return -1;
        }
    }

    PyErr_SetString(PyExc_TypeError, "supported signatures:\n"
                    "NurbsMatrix(num_row, num_col)\n"
                    "NurbsMatrix(num_row, num_col, fill_value)\n");
    return -1;
}

PyObject *NurbsMatrixPy::setEndContinuity(PyObject *args)
{
    int sc;
    int ec;
    if (PyArg_ParseTuple(args, "ii", &sc, &ec)) {
        try {
            int m = sc+1;
            int n = ec+1;
            int num_poles = m + n;
            TColStd_Array1OfReal knots(1, 2*num_poles);
            for (int idx=1; idx<=num_poles; ++idx) {
                knots(idx) = 0.0;
                knots(num_poles+idx) = 1.0;
            }
            std::unique_ptr<math_Matrix> ptr(new math_Matrix(1, num_poles, 1, num_poles, 0.0));
            // math_Matrix OCCmatrix(1, num_poles, 1, num_poles, 0.0);
            math_Matrix bezier_eval(1, m, 1, num_poles, 0.0);
            Standard_Integer first_non_zero;
            Standard_Integer error_code = BSplCLib::EvalBsplineBasis(sc, num_poles, knots, 0.0, first_non_zero, bezier_eval, Standard_False);
            if (error_code != 0) {
                Standard_Failure::Raise("B-spline evaluation failed");
            }
            for (int j = 1; j <= m; ++j) {
                ptr->SetRow(j, bezier_eval.Row(j));
            }
            math_Matrix bezier_eval2(1, n, 1, num_poles, 0.0);
            error_code = BSplCLib::EvalBsplineBasis(ec, num_poles, knots, 1.0, first_non_zero, bezier_eval2, Standard_False);
            if (error_code != 0) {
                Standard_Failure::Raise("B-spline evaluation failed");
            }
            for (int j = 1; j <= n; ++j) {
                ptr->SetRow(j+m, bezier_eval2.Row(j));
            }
            setTwinPointer(ptr.release());
            Py_Return;
        }
        catch (const Standard_Failure& e) {
            PyErr_SetString(PyExc_RuntimeError, e.GetMessageString());
            return 0;
        }
    }

    PyErr_SetString(PyExc_TypeError, "NurbsMatrix.setEndContinuity(start_continuity, end_continuity)\n");
    return 0;
}

PyObject* NurbsMatrixPy::getValues(PyObject *args)
{
    if (!PyArg_ParseTuple(args, ""))
        return 0;
    try {
        math_Matrix* mat = getmath_MatrixPtr();
        Py::List values;
        for (Standard_Integer i=mat->LowerRow(); i<=mat->UpperRow(); i++) {
            Py::List row;
            for (Standard_Integer j=mat->LowerCol(); j<=mat->UpperCol(); j++) {
                double v = mat->Value(i,j);
                row.append(Py::Float(v));
            }
            values.append(row);
        }
        return Py::new_reference_to(values);
    }
    catch (Standard_Failure& e) {
        PyErr_SetString(PartExceptionOCCError, e.GetMessageString());
        return 0;
    }
}

PyObject *NurbsMatrixPy::getCustomAttributes(const char* /*attr*/) const
{
    return 0;
}

int NurbsMatrixPy::setCustomAttributes(const char* /*attr*/, PyObject* /*obj*/)
{
    return 0;
}
