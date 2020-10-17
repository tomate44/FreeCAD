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
# include <math_Matrix.hxx>
#endif

# include "Math/MatrixPy.h"
# include "Math/MatrixPy.cpp"
// #include "Geometry.h"
// #include "TopoShapeEdgePy.h"
// #include "TopoShapeFacePy.h"
// #include <Base/VectorPy.h>

using namespace Part;


PyObject *MatrixPy::PyMake(struct _typeobject *, PyObject *, PyObject *)  // Python wrapper
{
    // create a new instance of MatrixPy
    return new MatrixPy(nullptr);
}

// returns a string which represents the object e.g. when printed in python
std::string MatrixPy::representation(void) const
{
    math_Matrix* mat = getMath_MatrixPtr();
    std::stringstream str;
    str << "<Matrix (";
    str << mat->RowNumber() << " x " ;
    str << mat->ColNumber() << ") object>";
    return str.str();
//     return std::string("<math_Matrix object>");
}

// constructor method
int MatrixPy::PyInit(PyObject* args, PyObject* /*kwds*/)
{
    int m;
    int n;
    double fill = 0.0;
    if (PyArg_ParseTuple(args, "ii|d", &m, &n, &fill)) {
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
                    "Matrix(num_row, num_col)\n"
                    "Matrix(num_row, num_col, fill_value)\n");
    return -1;
}

PyObject *MatrixPy::getCustomAttributes(const char* /*attr*/) const
{
    return 0;
}

int MatrixPy::setCustomAttributes(const char* /*attr*/, PyObject* /*obj*/)
{
    return 0;
}
