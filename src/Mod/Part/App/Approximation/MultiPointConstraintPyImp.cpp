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
# include <gp_Vec.hxx>
# include <gp_Vec2d.hxx>
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
            int idx = 1;
            for (Py::Sequence::iterator it1 = list1.begin(); it1 != list1.end(); ++it1) {
                Py::Vector v(*it1);
                Base::Vector3d point = v.toVector();
                mymp->setPoint(idx, point);
                idx++;
            }
            int idx2 = 0;
            for (Py::Sequence::iterator it2 = list2.begin(); it2 != list2.end(); ++it2) {
                Base::Vector2d point = Py::toVector2d(*it2);
                mymp->setPoint2d(idx + idx2, point);
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

Py::Long MultiPointConstraintPy::getNbPoints(void) const
{
    return Py::Long(this->getMultiPointConstraintPtr()->NbPoints()); 
}

Py::Long MultiPointConstraintPy::getNbPoints2d(void) const
{
    return Py::Long(this->getMultiPointConstraintPtr()->NbPoints2d()); 
}

PyObject *MultiPointConstraintPy::getPoint(PyObject * args)
{
    int index;
    if (!PyArg_ParseTuple(args, "i", &index))
        return 0;
    try {
        Standard_OutOfRange_Raise_if
            (index < 1 || index > this->getMultiPointConstraintPtr()->NbPoints(), "Pole index out of range");
        Base::Vector3d pnt = this->getMultiPointConstraintPtr()->Point(index);
        Base::VectorPy* vec = new Base::VectorPy(pnt);
        return vec;
    }
    catch (Standard_Failure& e) {

        PyErr_SetString(PartExceptionOCCError, e.GetMessageString());
        return 0;
    }
}

PyObject *MultiPointConstraintPy::getPoint2d(PyObject * args)
{
    int index;
    if (!PyArg_ParseTuple(args, "i", &index))
        return 0;
    try {
        int nb3d = this->getMultiPointConstraintPtr()->NbPoints();
        int nb2d = this->getMultiPointConstraintPtr()->NbPoints2d();
        Standard_OutOfRange_Raise_if
            (index < (nb3d + 1) || index > (nb3d + nb2d), "Pole index out of range");
        Base::Vector2d pnt = this->getMultiPointConstraintPtr()->Point2d(index);

        Py::Module module("__FreeCADBase__");
        Py::Callable method(module.getAttr("Vector2d"));
        Py::Tuple arg(2);
        arg.setItem(0, Py::Float(pnt.x));
        arg.setItem(1, Py::Float(pnt.y));
        return Py::new_reference_to(method.apply(arg));
    }
    catch (Standard_Failure& e) {

        PyErr_SetString(PartExceptionOCCError, e.GetMessageString());
        return 0;
    }
}

PyObject *MultiPointConstraintPy::setPoint(PyObject * args)
{
    int index;
    PyObject* p;
    if (!PyArg_ParseTuple(args, "iO!", &index, &(Base::VectorPy::Type), &p))
        return 0;
    Base::Vector3d vec = static_cast<Base::VectorPy*>(p)->value();
    try {
        Standard_OutOfRange_Raise_if
            (index < 1 || index > this->getMultiPointConstraintPtr()->NbPoints(), "Pole index out of range");
        this->getMultiPointConstraintPtr()->setPoint(index, vec);
        Py_Return;
    }
    catch (Standard_Failure& e) {
        PyErr_SetString(PartExceptionOCCError, e.GetMessageString());
        return 0;
    }
}

PyObject *MultiPointConstraintPy::setPoint2d(PyObject * args)
{
    int index;
    PyObject* p;
    if (!PyArg_ParseTuple(args, "iO!", &index, Base::Vector2dPy::type_object(), &p))
        return 0;
    try {
        int nb3d = this->getMultiPointConstraintPtr()->NbPoints();
        int nb2d = this->getMultiPointConstraintPtr()->NbPoints2d();
        Standard_OutOfRange_Raise_if
            (index < (nb3d + 1) || index > (nb3d + nb2d), "Pole index out of range");
        Base::Vector2d vec = Py::toVector2d(p);
        this->getMultiPointConstraintPtr()->setPoint2d(index, vec);
        Py_Return;
    }
    catch (Standard_Failure& e) {
        PyErr_SetString(PartExceptionOCCError, e.GetMessageString());
        return 0;
    }
}

PyObject *MultiPointConstraintPy::getPoints(PyObject * args)
{
    if (!PyArg_ParseTuple(args, ""))
        return 0;
    try {
        Py::List pts;
        for (Standard_Integer i=1; i<=this->getMultiPointConstraintPtr()->NbPoints(); i++) {
            Base::Vector3d pnt = this->getMultiPointConstraintPtr()->Point(i);
            Base::VectorPy* vec = new Base::VectorPy(pnt);
            pts.append(Py::asObject(vec));
        }
        return Py::new_reference_to(pts);
    }
    catch (Standard_Failure& e) {

        PyErr_SetString(PartExceptionOCCError, e.GetMessageString());
        return 0;
    }
}

PyObject *MultiPointConstraintPy::getPoints2d(PyObject * args)
{
    if (!PyArg_ParseTuple(args, ""))
        return 0;
    try {
        int nb3d = this->getMultiPointConstraintPtr()->NbPoints();
        int nb2d = this->getMultiPointConstraintPtr()->NbPoints2d();
        Py::List pts;
        Py::Module module("__FreeCADBase__");
        Py::Callable method(module.getAttr("Vector2d"));
        Py::Tuple vec2(2);
        for (Standard_Integer i=(nb3d + 1); i<=(nb3d + nb2d); i++) {
            Base::Vector2d pnt = this->getMultiPointConstraintPtr()->Point2d(i);
            vec2.setItem(0, Py::Float(pnt.x));
            vec2.setItem(1, Py::Float(pnt.y));
            pts.append(method.apply(vec2));
        }
        return Py::new_reference_to(pts);
    }
    catch (Standard_Failure& e) {

        PyErr_SetString(PartExceptionOCCError, e.GetMessageString());
        return 0;
    }
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

PyObject *MultiPointConstraintPy::getTang(PyObject * args)
{
    int index;
    if (!PyArg_ParseTuple(args, "i", &index))
        return 0;
    if (!this->getMultiPointConstraintPtr()->isTangencyPoint())
    {
        PyErr_SetString(PyExc_RuntimeError,
        "This point has no tangent");
    return 0;
    }
    try {
        int nb3d = this->getMultiPointConstraintPtr()->NbPoints();
        int nb2d = this->getMultiPointConstraintPtr()->NbPoints2d();
        Standard_OutOfRange_Raise_if
            (index < 1 || index > (nb3d + nb2d), "index out of range");
        int dim = this->getMultiPointConstraintPtr()->occObj()->Dimension(index);
        if (dim == 3)
        {
            gp_Vec vec = this->getMultiPointConstraintPtr()->occObj()->Tang(index);
            Base::Vector3d pnt(vec.X(),vec.Y(),vec.Z());
            Base::VectorPy* vector = new Base::VectorPy(pnt);
            return vector;
        }
        else if (dim == 2)
        {
            Py::Module module("__FreeCADBase__");
            Py::Callable method(module.getAttr("Vector2d"));
            Py::Tuple vec2(2);
            gp_Vec2d vec = this->getMultiPointConstraintPtr()->occObj()->Tang2d(index);
            vec2.setItem(0, Py::Float(vec.X()));
            vec2.setItem(1, Py::Float(vec.Y()));
            return Py::new_reference_to(method.apply(vec2));
//             Base::VectorPy* vector = new Base::VectorPy(pnt);
//             return vector;
        }
    }
    catch (Standard_Failure& e) {

        PyErr_SetString(PartExceptionOCCError, e.GetMessageString());
        return 0;
    }
}


PyObject *MultiPointConstraintPy::setTang(PyObject * args)
{
    int index;
    PyObject* p;
    int nb3d = this->getMultiPointConstraintPtr()->NbPoints();
    int nb2d = this->getMultiPointConstraintPtr()->NbPoints2d();
    if (PyArg_ParseTuple(args, "iO!", &index, &(Base::VectorPy::Type), &p))
    {
        try
        {
            Base::Vector3d vec = static_cast<Base::VectorPy*>(p)->value();
            Standard_OutOfRange_Raise_if
                (index < 1 || index > nb3d, "Pole index out of range");
            this->getMultiPointConstraintPtr()->setTang(index, vec);
            Py_Return;
        }
        catch (Standard_Failure& e) {
        PyErr_SetString(PartExceptionOCCError, e.GetMessageString());
        return 0;
        }
    }
    if (PyArg_ParseTuple(args, "iO!", &index, Base::Vector2dPy::type_object(), &p))
    {
        try
        {
            Standard_OutOfRange_Raise_if
                (index < 1 || index > nb3d, "Pole index out of range");
            Base::Vector2d vec = Py::toVector2d(p);
            this->getMultiPointConstraintPtr()->setTang2d(index, vec);
            Py_Return;
        }
        catch (Standard_Failure& e) {
        PyErr_SetString(PartExceptionOCCError, e.GetMessageString());
        return 0;
        }
    }
}

PyObject *MultiPointConstraintPy::getCustomAttributes(const char* ) const
{
    return 0;
}

int MultiPointConstraintPy::setCustomAttributes(const char* , PyObject *)
{
    return 0; 
}
