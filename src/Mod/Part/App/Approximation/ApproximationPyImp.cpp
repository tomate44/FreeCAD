/***************************************************************************
 *   Copyright (c) 2016 Werner Mayer <wmayer[at]users.sourceforge.net>     *
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
// # include <gp_Dir2d.hxx>
// # include <gp_Pnt2d.hxx>
// # include <gp_Vec2d.hxx>
// # include <gp_Trsf2d.hxx>
// # include <gp_Trsf.hxx>
// # include <Geom2d_Geometry.hxx>
// # include <Geom2d_Curve.hxx>
// # include <Precision.hxx>
// # include <Standard_Failure.hxx>
#endif

// #include <Base/GeometryPyCXX.h>
// #include <Base/Matrix.h>
// #include <Base/MatrixPy.h>
// #include <Base/Tools2D.h>
// #include <Base/Rotation.h>
// #include <Base/Placement.h>
// #include <Base/PlacementPy.h>

#include <Mod/Part/App/OCCError.h>
#include <Mod/Part/App/Approximation.h>
#include <Mod/Part/App/Approximation/ApproximationPy.h>
#include <Mod/Part/App/Approximation/ApproximationPy.cpp>

// #include <Mod/Part/App/TopoShape.h>
// #include <Mod/Part/App/TopoShapePy.h>

using namespace Part;

// returns a string which represents the object e.g. when printed in python
std::string ApproximationPy::representation(void) const
{
    return "<Approximation object>";
}

PyObject *ApproximationPy::PyMake(struct _typeobject *, PyObject *, PyObject *)  // Python wrapper
{
    // never create such objects with the constructor
    PyErr_SetString(PyExc_RuntimeError,
        "You cannot create an instance of the abstract class 'Approximation'.");
    return 0;
}

// constructor method
int ApproximationPy::PyInit(PyObject* /*args*/, PyObject* /*kwd*/)
{
    return 0;
}

PyObject* ApproximationPy::copy(PyObject *args)
{
    if (!PyArg_ParseTuple(args, ""))
        return NULL;

    Part::Approximation* approx = this->getApproximationPtr();
    PyTypeObject* type = this->GetType();
    PyObject* cpy = 0;
    // let the type object decide
    if (type->tp_new)
        cpy = type->tp_new(type, this, 0);
    if (!cpy) {
        PyErr_SetString(PyExc_TypeError, "failed to create copy of Approximation object");
        return 0;
    }

    Part::ApproximationPy* approxpy = static_cast<Part::ApproximationPy*>(cpy);
    // the PyMake function must have created the corresponding instance of the 'Approximation' subclass
    // so delete it now to avoid a memory leak
    if (approxpy->_pcTwinPointer) {
        Part::Approximation* clone = static_cast<Part::Approximation*>(approxpy->_pcTwinPointer);
        delete clone;
    }
    approxpy->_pcTwinPointer = approx->clone();
    return cpy;
}

PyObject *ApproximationPy::getCustomAttributes(const char* /*attr*/) const
{
    return 0;
}

int ApproximationPy::setCustomAttributes(const char* /*attr*/, PyObject* /*obj*/)
{
    return 0; 
}
