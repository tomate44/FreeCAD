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
# include <AppDef_MultiPointConstraint.hxx>
# include <AppDef_Array1OfMultiPointConstraint.hxx>
# include <AppDef_MultiLine.hxx>
# include <Standard_Version.hxx>
// # include <TopTools_ListIteratorOfListOfShape.hxx>
#endif

// #include "TopoShapePy.h"
// #include "TopoShapeVertexPy.h"
#include "AppDef_MultiPointConstraintPy.h"
// #include "AppDef_MultiPointConstraintPy.cpp"
#include "AppDef_MultiLinePy.h"
#include "AppDef_MultiLinePy.cpp"
#include "Tools.h"
#include "OCCError.h"
#include <Base/VectorPy.h>
#include <Base/GeometryPyCXX.h>

using namespace Part;

PyObject *AppDef_MultiLinePy::PyMake(struct _typeobject *, PyObject *args, PyObject *)  // Python wrapper
{
    // create a new instance of AppDef_MultiLinePy and the Twin object 
    int nb;
    if (PyArg_ParseTuple(args, "d", &nb))
        if (nb > 1)
            return new AppDef_MultiLinePy(new AppDef_MultiLine(nb));
    
//     PyObject* obj;
//     if (PyArg_ParseTuple(args, "O", &obj)) {
//         try {
//             Py::Sequence list(obj);
//             Standard_Integer len = list.size();
//             AppDef_Array1OfMultiPointConstraint arraympc(1, len);
//             Standard_Integer idx = 0;
//             for (Py::Sequence::iterator it1 = list.begin(); it1 != list.end(); ++it1) {
//                 idx++;
//                 Py::Object pyo(*it1);
//                 AppDef_MultiPointConstraint mympc(mpc.getAppDef_MultiPointConstraintPtr());
//                 arraympc.SetValue(idx, mympc);
//             }
//         
//             return new AppDef_MultiLinePy(new AppDef_MultiLine(arraympc));
//         }
//     }

    PyErr_SetString(PartExceptionOCCError, "Failed to create MultiLine");
    return 0;
}

// constructor method
int AppDef_MultiLinePy::PyInit(PyObject* /*args*/, PyObject* /*kwd*/)
{
    return 0;
}

// returns a string which represents the object e.g. when printed in python
std::string AppDef_MultiLinePy::representation(void) const
{
    return std::string("<AppDef_MultiLine object>");
}

PyObject* AppDef_MultiLinePy::setValue(PyObject *args)
{
    int idx;
    PyObject *obj;
    if (!PyArg_ParseTuple(args, "iO!", &idx, &Part::AppDef_MultiPointConstraintPy::Type, &obj))
        return 0;

    try {
        // AppDef_MultiPointConstraint mpc(obj->getAppDef_MultiPointConstraintPtr());
        this->getAppDef_MultiLinePtr()->SetValue(idx, obj->getAppDef_MultiPointConstraintPtr());
        Py_Return;
    }
    catch (Standard_Failure& e) {
        PyErr_SetString(PartExceptionOCCError, e.GetMessageString());
        return 0;
    }
}

PyObject *AppDef_MultiLinePy::getCustomAttributes(const char* ) const
{
    return 0;
}

int AppDef_MultiLinePy::setCustomAttributes(const char* , PyObject *)
{
    return 0; 
}
