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
# include <gp_Pnt2d.hxx>
// # include <TopoDS.hxx>
// # include <TopoDS_Wire.hxx>
# include <TColgp_Array1OfPnt.hxx>
// # include <AppDef_Array1OfMultiPointConstraint.hxx>
# include <AppParCurves_MultiPoint.hxx>
# include <AppDef_MultiPointConstraint.hxx>
# include <Standard_Version.hxx>
// # include <TopTools_ListIteratorOfListOfShape.hxx>
#endif

// #include "TopoShapePy.h"
// #include "TopoShapeVertexPy.h"
#include "Mod/Part/App/Approximation/MultiPointPy.h"
#include "Mod/Part/App/Approximation.h"
#include "Mod/Part/App/Approximation/MultiPointConstraintPy.h"
#include "Mod/Part/App/Approximation/MultiPointConstraintPy.cpp"
#include "Tools.h"
#include "OCCError.h"
#include <Base/VectorPy.h>
#include <Base/GeometryPyCXX.h>

using namespace Part;

PyObject *MultiPointConstraintPy::PyMake(struct _typeobject *, PyObject *args, PyObject *)  // Python wrapper
{
    // create a new instance of AppDef_MultiPointPy and the Twin object
    int nb3d, nb2d;
    if (PyArg_ParseTuple(args, "ii", &nb3d, &nb2d)) {
        return new MultiPointConstraintPy(new MultiPointConstraint(nb3d, nb2d));
    }
    PyErr_Clear();
    PyObject *obj1, *obj2;
    if (PyArg_ParseTuple(args, "OO", &obj1, &obj2)) {
        try {
            Py::Sequence list1(obj1);
            Py::Sequence list2(obj2);
            MultiPointConstraint* mymp = new MultiPointConstraint(list1.size(), list2.size());
            int idx = 0;
            for (Py::Sequence::iterator it1 = list1.begin(); it1 != list1.end(); ++it1) {
                Py::Vector v(*it1);
                Base::Vector3d point = v.toVector();
                mymp->setPoint(idx, point);
                idx++;
            }
            int idx2 = 0;
            for (Py::Sequence::iterator it2 = list2.begin(); it2 != list2.end(); ++it2) {
                Base::Vector2d point = Py::toVector2d(*it2);
                mymp->setPoint2d(list1.size() + idx2, point);
                idx2++;
            }
            return new MultiPointConstraintPy(mymp);
        }
        catch (Standard_Failure& e) {
            PyErr_SetString(PartExceptionOCCError, e.GetMessageString());
            return 0;
        }
    }
    PyErr_SetString(PyExc_RuntimeError,
        "Failed to create MultiPointConstraint");
    return 0;
}

// constructor method
int MultiPointConstraintPy::PyInit(PyObject* /*args*/, PyObject* /*kwd*/)
{
    return 0;
}

// returns a string which represents the object e.g. when printed in python
std::string MultiPointConstraintPy::representation(void) const
{
    return std::string("<MultiPointConstraint object>");
}

PyObject* MultiPointConstraintPy::isTangencyPoint(PyObject*)
{
    Standard_Boolean istan = this->getMultiPointConstraintPtr()->isTangencyPoint();
    return PyBool_FromLong(istan ? 1 : 0);
}

PyObject* MultiPointConstraintPy::isCurvaturePoint(PyObject*)
{
    Standard_Boolean iscur = this->getMultiPointConstraintPtr()->isCurvaturePoint();
    return PyBool_FromLong(iscur ? 1 : 0);
}

PyObject *MultiPointConstraintPy::getCustomAttributes(const char* ) const
{
    return 0;
}

int MultiPointConstraintPy::setCustomAttributes(const char* , PyObject *)
{
    return 0; 
}
