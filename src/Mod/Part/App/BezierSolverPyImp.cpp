/***************************************************************************
 *   Copyright (c) 2008 Werner Mayer <wmayer[at]users.sourceforge.net>     *
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
# include <gp.hxx>
# include <Precision.hxx>
#endif

#include <Base/VectorPy.h>
#include <Base/GeometryPyCXX.h>

#include "OCCError.h"
#include "Geometry.h"
#include <Mod/Part/App/BezierSolverPy.h>
#include <Mod/Part/App/BezierSolverPy.cpp>

using namespace Part;

// returns a string which represents the object e.g. when printed in python
std::string BezierSolverPy::representation(void) const
{
    return "<BezierSolver object>";
}

PyObject *BezierSolverPy::PyMake(struct _typeobject *, PyObject* args, PyObject* kwds)  // Python wrapper
{
    PyObject* sc;
    PyObject* ec;
    if (!PyArg_ParseTuple(args, "OO", &sc, &ec))
        return 0;
    try {
        Py::Sequence start(sc);
        std::vector<Base::Vector3d> stcons;
        stcons.reserve(start.size());
        for (Py::Sequence::iterator it = start.begin(); it != start.end(); ++it) {
            Py::Vector v(*it);
            Base::Vector3d vec = v.toVector();
            stcons.emplace_back(vec.x, vec.y, vec.z);
        }
        Py::Sequence end(ec);
        std::vector<Base::Vector3d> endcons;
        endcons.reserve(end.size());
        for (Py::Sequence::iterator it2 = end.begin(); it2 != end.end(); ++it2) {
            Py::Vector v(*it2);
            Base::Vector3d vec = v.toVector();
            endcons.emplace_back(vec.x, vec.y, vec.z);
        }
        // create a new instance of LinePy and the Twin object 
        return new BezierSolverPy(new BezierSolver(stcons, endcons));
    }
    catch (Standard_Failure& e) {
        PyErr_SetString(PartExceptionOCCError, e.GetMessageString());
        return 0;
    }
}

// constructor method
int BezierSolverPy::PyInit(PyObject* args, PyObject* /*kwd*/)
{
        return 0;
}

PyObject *LinePy::getCustomAttributes(const char* /*attr*/) const
{
    return 0;
}

int LinePy::setCustomAttributes(const char* /*attr*/, PyObject* /*obj*/)
{
    return 0; 
}
