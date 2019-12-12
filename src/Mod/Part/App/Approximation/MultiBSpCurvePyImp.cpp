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
# include <TColgp_Array1OfPnt2d.hxx>
# include <Standard_Real.hxx>
# include <Standard_Integer.hxx>
# include <NCollection_Array1.hxx>
# include <AppDef_MultiPointConstraint.hxx>
# include <AppDef_Array1OfMultiPointConstraint.hxx>
# include <AppParCurves_MultiBSpCurve.hxx>
# include <Standard_Version.hxx>
// # include <TopTools_ListIteratorOfListOfShape.hxx>
#endif

// #include "TopoShapePy.h"
// #include "TopoShapeVertexPy.h"
#include "Mod/Part/App/Approximation/MultiCurvePy.h"
// #include "AppDef_MultiPointConstraintPy.cpp"
#include "Mod/Part/App/Approximation.h"
#include "Mod/Part/App/Approximation/MultiBSpCurvePy.h"
#include "Mod/Part/App/Approximation/MultiBSpCurvePy.cpp"
#include "Tools.h"
#include "OCCError.h"
#include <Base/VectorPy.h>
#include <Base/GeometryPyCXX.h>

using namespace Part;

PyObject *MultiBSpCurvePy::PyMake(struct _typeobject *, PyObject *, PyObject *)  // Python wrapper
{
    // create a new instance of MultiBSpCurvePy and the Twin object
    return new MultiBSpCurvePy(new MultiBSpCurve());
}

// constructor method
int MultiBSpCurvePy::PyInit(PyObject* /*args*/, PyObject* /*kwd*/)
{
    return 0;
}

// returns a string which represents the object e.g. when printed in python
std::string MultiBSpCurvePy::representation(void) const
{
    return std::string("<MultiBSpCurve object>");
}


Py::Long MultiBSpCurvePy::getNbCurves(void) const
{
    return Py::Long(this->getMultiBSpCurvePtr()->getNbCurves()); 
}

Py::Long MultiBSpCurvePy::getDegree(void) const
{
    return Py::Long(this->getMultiBSpCurvePtr()->getDegree()); 
}

Py::Long MultiBSpCurvePy::getNbPoles(void) const
{
    return Py::Long(this->getMultiBSpCurvePtr()->getNbPoles()); 
}

void MultiBSpCurvePy::setNbPoles(Py::Long nb)
{
    this->getMultiBSpCurvePtr()->setNbPoles((int)nb); 
}

PyObject * MultiBSpCurvePy::getKnots(PyObject *)
{
    TColStd_Array1OfReal array = this->getMultiBSpCurvePtr()->occObj()->Knots();
    Py::Tuple ktlist(array.Size());
    int idx = 0;
    for (int i=array.Lower(); i<=array.Upper(); i++)
    {
        ktlist.setItem(idx, Py::Float(array(i)));
        idx++;
    }
    return Py::new_reference_to(ktlist);
}

PyObject * MultiBSpCurvePy::getMultiplicities(PyObject *)
{
    TColStd_Array1OfInteger array = this->getMultiBSpCurvePtr()->occObj()->Multiplicities();
    Py::Tuple mullist(array.Size());
    int idx = 0;
    for (int i=array.Lower(); i<=array.Upper(); i++)
    {
        mullist.setItem(idx, Py::Long(array(i)));
        idx++;
    }
    return Py::new_reference_to(mullist);
}

PyObject * MultiBSpCurvePy::getPoles(PyObject *args)
{
    int index;
    if (!PyArg_ParseTuple(args, "i", &index))
        return 0;
    try
    {
        int nb = this->getMultiBSpCurvePtr()->getNbPoles();
        int dim = this->getMultiBSpCurvePtr()->occObj()->Dimension(index);
        if (dim == 3)
        {
            TColgp_Array1OfPnt Poles(1,nb);
            this->getMultiBSpCurvePtr()->occObj()->Curve(index, Poles);
            Py::List plist(nb);
            for (int i=Poles.Lower(); i<=Poles.Upper(); i++)
            {
                gp_Pnt pnt = this->getMultiBSpCurvePtr()->occObj()->Pole(index, i);
                Base::Vector3d p(pnt.X(), pnt.Y(), pnt.Z());
                Base::VectorPy* vec = new Base::VectorPy(p);
                plist.append(Py::asObject(vec));
            }
            return Py::new_reference_to(plist);
        }
        else if (dim == 2)
        {
            TColgp_Array1OfPnt2d Poles(1,nb);
            this->getMultiBSpCurvePtr()->occObj()->Curve(index, Poles);
            Py::List plist(nb);
            for (int i=Poles.Lower(); i<=Poles.Upper(); i++)
            {
                gp_Pnt2d  pnt = this->getMultiBSpCurvePtr()->occObj()->Pole2d(index, i);
                Py::Module module("__FreeCADBase__");
                Py::Callable method(module.getAttr("Vector2d"));
                Py::Tuple vec(2);
                vec.setItem(0, Py::Float(pnt.X()));
                vec.setItem(1, Py::Float(pnt.Y()));
                plist.append(method.apply(vec));
            }
            return Py::new_reference_to(plist);
        }
    }
    catch (Standard_Failure& e) {

        PyErr_SetString(PartExceptionOCCError, e.GetMessageString());
        return 0;
    }
}


PyObject *MultiBSpCurvePy::getCustomAttributes(const char* ) const
{
    return 0;
}

int MultiBSpCurvePy::setCustomAttributes(const char* , PyObject *)
{
    return 0; 
}
