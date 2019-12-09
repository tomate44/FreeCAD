/***************************************************************************
 *   Copyright (c) 2019 Christophe Grellier <cg[at]grellier.fr>            *
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
// # include <gp_Ax2.hxx>
// # include <gp_Dir.hxx>
# include <gp_Pnt.hxx>
// # include <TopoDS.hxx>
// # include <TopoDS_Wire.hxx>
# include <TColgp_Array1OfPnt.hxx>
# include <AppDef_MultiPointConstraint.hxx>
# include <AppDef_Array1OfMultiPointConstraint.hxx>
# include <AppDef_MultiLine.hxx>
# include <AppDef_BSplineCompute.hxx>
# include <Standard_Version.hxx>
// # include <TopTools_ListIteratorOfListOfShape.hxx>
#endif

// #include "TopoShapePy.h"
// #include "TopoShapeVertexPy.h"
#include "Mod/Part/App/Approximation/ApproximationPy.h"
// #include "AppDef_MultiPointConstraintPy.cpp"
#include "Mod/Part/App/Approximation.h"
#include "Mod/Part/App/Approximation/MultiLinePy.h"
#include "Mod/Part/App/Approximation/MultiBSpCurvePy.h"
#include "Mod/Part/App/Approximation/BSplineComputePy.h"
#include "Mod/Part/App/Approximation/BSplineComputePy.cpp"
#include "Tools.h"
#include "OCCError.h"
#include <Base/VectorPy.h>
#include <Base/GeometryPyCXX.h>

using namespace Part;

PyObject *BSplineComputePy::PyMake(struct _typeobject *, PyObject* /*args*/, PyObject *)  // Python wrapper
{
    // create a new instance of BSplineComputePy and the Twin object
    return new BSplineComputePy(new BSplineCompute());
}

// constructor method
int BSplineComputePy::PyInit(PyObject* /*args*/, PyObject* /*kwd*/)
{
    return 0;
}

// returns a string which represents the object e.g. when printed in python
std::string BSplineComputePy::representation(void) const
{
    return std::string("<BSplineCompute object>");
}

PyObject* BSplineComputePy::interpol(PyObject *args)
{
    PyObject *ml;
    if (PyArg_ParseTuple(args, "O!", &(MultiLinePy::Type), &ml)) {
        MultiLine* myml = static_cast<Part::MultiLinePy*>(ml)->getMultiLinePtr();
        this->getBSplineComputePtr()->Interpol(*myml->occObj());
        Py_Return;
    }
    PyErr_SetString(PyExc_RuntimeError,
        "Failed to create interpolation");
    return 0;
}

PyObject* BSplineComputePy::perform(PyObject *args)
{
    PyObject *ml;
    if (PyArg_ParseTuple(args, "O!", &(MultiLinePy::Type), &ml)) {
        MultiLine* myml = static_cast<Part::MultiLinePy*>(ml)->getMultiLinePtr();
        this->getBSplineComputePtr()->Perform(*myml->occObj());
        Py_Return;
    }
    PyErr_SetString(PyExc_RuntimeError,
        "Failed to create approximation");
    return 0;
}

PyObject* BSplineComputePy::value(PyObject *args)
{
    if (PyArg_ParseTuple(args, "")) {
//         MultiBSpCurve* mybsc = new MultiBSpCurve(this->getBSplineComputePtr()->Value()); // works
        // or
        AppParCurves_MultiBSpCurve mybsc = this->getBSplineComputePtr()->Value();
//         PyErr_SetString(PyExc_RuntimeError,
//         "returning value of approximation");
        return new MultiBSpCurvePy(new MultiBSpCurve(mybsc));
    }
    PyErr_SetString(PyExc_RuntimeError,
        "Failed to return approximation");
    return 0;
}

PyObject* BSplineComputePy::setDegrees(PyObject *args)
{
    int degMin, degMax;
    if (PyArg_ParseTuple(args, "ii", &degMin, &degMax))
    {
        this->getBSplineComputePtr()->setDegrees(degMin, degMax);
        Py_Return;
    }
    PyErr_SetString(PyExc_RuntimeError,
        "Failed to set degrees");
    return 0;
}

PyObject* BSplineComputePy::setContinuity(PyObject *args)
{
    int cont;
    if (PyArg_ParseTuple(args, "i", &cont))
    {
        this->getBSplineComputePtr()->setContinuity(cont);
        Py_Return;
    }
    PyErr_SetString(PyExc_RuntimeError,
        "Failed to set continuity");
    return 0;
}

PyObject* BSplineComputePy::setTolerances(PyObject *args)
{
    double tol3d, tol2d;
    if (PyArg_ParseTuple(args, "dd", &tol3d, &tol2d))
    {
        this->getBSplineComputePtr()->setTolerances(tol3d, tol2d);
        Py_Return;
    }
    PyErr_SetString(PyExc_RuntimeError,
        "Failed to set tolerances");
    return 0;
}

PyObject *BSplineComputePy::getCustomAttributes(const char* ) const
{
    return 0;
}

int BSplineComputePy::setCustomAttributes(const char* , PyObject *)
{
    return 0; 
}
