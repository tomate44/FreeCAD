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
// # include <gp_Pnt.hxx>
// # include <TopoDS.hxx>
// # include <TopoDS_Wire.hxx>
# include <TColgp_Array1OfPnt.hxx>
# include <TColgp_Array1OfVec.hxx>
# include <AppDef_MultiPointConstraint.hxx>
# include <Standard_Version.hxx>
// # include <TopTools_ListIteratorOfListOfShape.hxx>
#endif

// #include "TopoShapePy.h"
// #include "TopoShapeVertexPy.h"
#include "AppDef_MultiPointConstraintPy.h"
#include "AppDef_MultiPointConstraintPy.cpp"
#include "Tools.h"
#include "OCCError.h"
#include <Base/VectorPy.h>
#include <Base/GeometryPyCXX.h>

using namespace Part;

PyObject *AppDef_MultiPointConstraintPy::PyMake(struct _typeobject *, PyObject *args, PyObject *)  // Python wrapper
{
    // create a new instance of AppDef_MultiPointConstraintPy and the Twin object 
    PyObject* obj;
    if (PyArg_ParseTuple(args, "O", &obj)) {
        try {
            Py::Sequence list(obj);
            Standard_Integer len = list.size();
            TColgp_Array1OfPnt pts(1, len);
            Standard_Integer idx = 0;
            for (Py::Sequence::iterator it1 = list.begin(); it1 != list.end(); ++it1) {
                idx++;
                Py::Vector v(*it1);
                Base::Vector3d pnt = v.toVector();
                gp_Pnt newPoint(pnt.x,pnt.y,pnt.z);
                pts.SetValue(idx, newPoint);
            }
            return new AppDef_MultiPointConstraintPy(new AppDef_MultiPointConstraint(pts));
        }
        catch (Standard_Failure& e) {
            PyErr_SetString(PartExceptionOCCError, e.GetMessageString());
            return 0;
        }
    }

    PyErr_Clear();


    PyErr_SetString(PartExceptionOCCError,
      "Argument list signature is incorrect.\n\nSupported signatures:\n"
      "(list_of_points)\n"
      "(list_of_points, list_of_tangent_vectors)\n"
      "(list_of_points, list_of_tangent_vectors, list_of_curvature_vectors)\n");
    return 0;
}

// constructor method
int AppDef_MultiPointConstraintPy::PyInit(PyObject* /*args*/, PyObject* /*kwd*/)
{
    return 0;
}

// returns a string which represents the object e.g. when printed in python
std::string AppDef_MultiPointConstraintPy::representation(void) const
{
    return std::string("<AppDef_MultiPointConstraint object>");
}

Py::Long AppDef_MultiPointConstraintPy::getNbPoints(void) const
{
//     if (!PyArg_ParseTuple(args, "")
//         return 0;

    try {
        int n = this->getAppDef_MultiPointConstraintPtr()->NbPoints();
        return Py::Long(n);
    }
    catch (Standard_Failure& e) {
        PyErr_SetString(PartExceptionOCCError, e.GetMessageString());
        return Py::Long(0);
    }
}

PyObject* AppDef_MultiPointConstraintPy::Dimension(PyObject * args)
{
    int index;
    if (!PyArg_ParseTuple(args, "i", &index))
        return 0;
    try {
        int dim = this->getAppDef_MultiPointConstraintPtr()->Dimension(index);
        return Py_BuildValue("i", dim);
    }
    catch (Standard_Failure& e) {
        PyErr_SetString(PartExceptionOCCError, e.GetMessageString());
        return 0;
    }
}

PyObject* AppDef_MultiPointConstraintPy::Point(PyObject * args)
{
    int index;
    if (!PyArg_ParseTuple(args, "i", &index))
        return 0;
    try {
        gp_Pnt pnt = this->getAppDef_MultiPointConstraintPtr()->Point(index);
        Base::VectorPy* vec = new Base::VectorPy(Base::Vector3d(
            pnt.X(), pnt.Y(), pnt.Z()));
        return vec;
    }
    catch (Standard_Failure& e) {
        PyErr_SetString(PartExceptionOCCError, e.GetMessageString());
        return 0;
    }
}

PyObject *AppDef_MultiPointConstraintPy::getCustomAttributes(const char* ) const
{
    return 0;
}

int AppDef_MultiPointConstraintPy::setCustomAttributes(const char* , PyObject *)
{
    return 0; 
}
