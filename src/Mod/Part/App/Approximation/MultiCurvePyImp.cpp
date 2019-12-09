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
# include <AppParCurves_MultiCurve.hxx>
# include <Standard_Version.hxx>
// # include <TopTools_ListIteratorOfListOfShape.hxx>
#endif

// #include "TopoShapePy.h"
// #include "TopoShapeVertexPy.h"
#include "Mod/Part/App/Approximation/ApproximationPy.h"
// #include "AppDef_MultiPointConstraintPy.cpp"
#include "Mod/Part/App/Approximation.h"
#include "Mod/Part/App/Approximation/MultiCurvePy.h"
#include "Mod/Part/App/Approximation/MultiCurvePy.cpp"
#include "Tools.h"
#include "OCCError.h"
#include <Base/VectorPy.h>
#include <Base/GeometryPyCXX.h>

using namespace Part;

PyObject *MultiCurvePy::PyMake(struct _typeobject *, PyObject *, PyObject *)  // Python wrapper
{
    // create a new instance of MultiCurvePy and the Twin object
    return new MultiCurvePy(new MultiCurve());
}

// constructor method
int MultiCurvePy::PyInit(PyObject* /*args*/, PyObject* /*kwd*/)
{
    return 0;
}

// returns a string which represents the object e.g. when printed in python
std::string MultiCurvePy::representation(void) const
{
    return std::string("<MultiCurve object>");
}

Py::Long MultiCurvePy::getNbCurves(void) const
{
    return Py::Long(this->getMultiCurvePtr()->occObj()->NbCurves()); 
}

Py::Long MultiCurvePy::getDegree(void) const
{
    return Py::Long(this->getMultiCurvePtr()->occObj()->Degree()); 
}

Py::Long MultiCurvePy::getNbPoles(void) const
{
    return Py::Long(this->getMultiCurvePtr()->occObj()->NbPoles()); 
}

void MultiCurvePy::setNbPoles(Py::Long nb)
{
    this->getMultiCurvePtr()->setNbPoles((int)nb); 
}
/*
Py::Long MultiCurvePy::getNbMultiPoints(void) const
{
    return Py::Long(this->getMultiLinePtr()->NbMultiPoints()); 
}

PyObject* MultiCurvePy::setParameter(PyObject *args)
{
    int idx;
    double par;
    if (!PyArg_ParseTuple(args, "id", &idx, &par))
        return 0;
    try {
        this->getMultiLinePtr()->setParameter(idx, par);
        return 0;
    }
    catch (Standard_Failure& e) {
        PyErr_SetString(PartExceptionOCCError, e.GetMessageString());
        return 0;
    }
}*/

PyObject *MultiCurvePy::getCustomAttributes(const char* ) const
{
    return 0;
}

int MultiCurvePy::setCustomAttributes(const char* , PyObject *)
{
    return 0; 
}
