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
# include <Standard_Version.hxx>
// # include <TopTools_ListIteratorOfListOfShape.hxx>
#endif

// #include "TopoShapePy.h"
// #include "TopoShapeVertexPy.h"
#include "Mod/Part/App/Approximation/ApproximationPy.h"
// #include "AppDef_MultiPointConstraintPy.cpp"
#include "Mod/Part/App/Approximation.h"
#include "Mod/Part/App/Approximation/MultiLinePy.h"
#include "Mod/Part/App/Approximation/MultiLinePy.cpp"
#include "Tools.h"
#include "OCCError.h"
#include <Base/VectorPy.h>
#include <Base/GeometryPyCXX.h>

using namespace Part;

PyObject *MultiLinePy::PyMake(struct _typeobject *, PyObject *args, PyObject *)  // Python wrapper
{
    // create a new instance of AppDef_MultiLinePy and the Twin object
    int nb;
    if (PyArg_ParseTuple(args, "i", &nb)) {
        if (nb > 1) {
            return new MultiLinePy(new MultiLine(nb));
        }
    }
    PyErr_Clear();
    PyObject* obj;
    if (PyArg_ParseTuple(args, "O", &obj)) {
        try {
            Py::Sequence list(obj);
            TColgp_Array1OfPnt pts(1, list.size());
            int idx = pts.Lower();
            for (Py::Sequence::iterator it = list.begin(); it != list.end(); ++it) {
                Py::Vector v(*it);
                Base::Vector3d point = v.toVector();
                pts.SetValue(idx++, gp_Pnt(point.x,point.y,point.z));
            }
            return new MultiLinePy(new MultiLine(pts));
        }
        catch (Standard_Failure& e) {
            PyErr_SetString(PartExceptionOCCError, e.GetMessageString());
            return 0;
        }
    }
    PyErr_Clear();
    if (PyArg_ParseTuple(args, "")) {
        return new MultiLinePy(new MultiLine);
    }
    PyErr_SetString(PyExc_RuntimeError,
        "Failed to create MultiLine");
    return 0;
}

// constructor method
int MultiLinePy::PyInit(PyObject* /*args*/, PyObject* /*kwd*/)
{
    return 0;
}

// returns a string which represents the object e.g. when printed in python
std::string MultiLinePy::representation(void) const
{
    return std::string("<MultiLine object>");
}

Py::Long MultiLinePy::getNbPoints(void) const
{
    return Py::Long(this->getMultiLinePtr()->NbPoints()); 
}

Py::Long MultiLinePy::getNbMultiPoints(void) const
{
    return Py::Long(this->getMultiLinePtr()->NbMultiPoints()); 
}

PyObject* MultiLinePy::setParameter(PyObject *args)
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
}

PyObject *MultiLinePy::getCustomAttributes(const char* ) const
{
    return 0;
}

int MultiLinePy::setCustomAttributes(const char* , PyObject *)
{
    return 0; 
}
