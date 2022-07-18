///***************************************************************************
// *   Copyright (c) 2022 Matt�o Grellier <matteogrellier@gmail.com>            *
// *                                                                         *
// *   This file is part of the FreeCAD CAx development system.              *
// *                                                                         *
// *   This library is free software; you can redistribute it and/or         *
// *   modify it under the terms of the GNU Library General Public           *
// *   License as published by the Free Software Foundation; either          *
// *   version 2 of the License, or (at your option) any later version.      *
// *                                                                         *
// *   This library  is distributed in the hope that it will be useful,      *
// *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
// *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
// *   GNU Library General Public License for more details.                  *
// *                                                                         *
// *   You should have received a copy of the GNU Library General Public     *
// *   License along with this library; see the file COPYING.LIB. If not,    *
// *   write to the Free Software Foundation, Inc., 59 Temple Place,         *
// *   Suite 330, Boston, MA  02111-1307, USA                                *
// *                                                                         *
// ***************************************************************************/
//
//
#include "../PreCompiled.h"
#ifndef _PreComp_
#include <GC_MakeCircle.hxx>
#include <Geom_Circle.hxx>
#include <TColStd_Array1OfReal.hxx>
#include <TColgp_Array1OfPnt.hxx>
#include <gp_Circ.hxx>
#endif
#include "Mod/Part/App/BezierCurvePy.h"
#include "Mod/Part/App/Geometry.h"
#include "Mod/Surface/App/Blending/BlendCurvePy.cpp"
#include "Mod/Surface/App/Blending/BlendCurvePy.h"
#include "Mod/Surface/App/Blending/BlendPointPy.h"
#include <Base/GeometryPyCXX.h>
#include <Base/VectorPy.h>
#include <Mod/Part/App/BezierCurvePy.h>


using namespace Surface;

extern const char *gce_ErrorStatusText(gce_ErrorType et);

std::string BlendCurvePy::representation(void) const
{
    return "BlendCurve";
}

PyObject *BlendCurvePy::PyMake(struct _typeobject *, PyObject *, PyObject *)// Python wrapper
{
    // create a new instance of BlendCurvePy
    return new BlendCurvePy(new BlendCurve);
}


int BlendCurvePy::PyInit(PyObject *args, PyObject *kwds)
{
    PyObject *plist;
    if (!PyArg_ParseTuple(args, "O", &plist)) {
        PyErr_SetString(PyExc_TypeError, "Constructor Need 1 argument");
        return -1;
    }
    try {
        Py::Sequence list(plist);
        std::vector<BlendPoint> bpList;
        for (Py::Sequence::iterator it = list.begin(); it != list.end(); ++it) {
            Py::Object obj(*it);
            if (PyObject_TypeCheck(obj.ptr(), &BlendPointPy::Type)) {
                BlendPoint *geom = static_cast<BlendPointPy *>(obj.ptr())->getBlendPointPtr();
                bpList.emplace_back(*geom);
            }
        }

        this->getBlendCurvePtr()->blendPoints = bpList;

        //Handle(Geom_BezierCurve) point = new BlendPoint();


        return 0;
    }

    catch (Standard_Failure &e) {
        PyErr_SetString(PyExc_RuntimeError,
                        "Error no argument");
        return 1;
    }
}

PyObject *BlendCurvePy::compute(PyObject *args)
{
    BlendCurve *bc = getBlendCurvePtr();

    Handle(Geom_BezierCurve) gc = bc->compute();

    return new Part::BezierCurvePy(new Part::GeomBezierCurve(gc));
}


PyObject *BlendCurvePy::setSize(PyObject *args)
{
    int i;
    double d;
    if (!PyArg_ParseTuple(args, "di", &d, &i)) {
        PyErr_SetString(PyExc_TypeError, "Constructor Need 2 argument");
    }
    try {
        getBlendCurvePtr()->setSize(d, i);
        Py_Return;
    }
    catch (Standard_Failure &e) {
        PyErr_SetString(PyExc_Exception, e.GetMessageString());
        return nullptr;
    }
}

PyObject *BlendCurvePy::getCustomAttributes(const char * /*attr*/) const
{
    return nullptr;
}

int BlendCurvePy::setCustomAttributes(const char * /*attr*/, PyObject * /*obj*/)
{
    return 0;
}
