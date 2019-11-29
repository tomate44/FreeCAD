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
#include <AppParCurves_MultiPoint.hxx>
// # include <BRepBuilderAPI_MakeEdge2d.hxx>
// # include <BRepBuilderAPI_MakeVertex.hxx>
// # include <Geom2dConvert_CompCurveToBSplineCurve.hxx>
// # include <Geom2dAPI_Interpolate.hxx>
// # include <Geom2dAPI_ProjectPointOnCurve.hxx>
// # include <Geom2dConvert.hxx>
// # include <Geom2dLProp_CLProps2d.hxx>
// # include <gp.hxx>
// # include <gp_Ax22d.hxx>
// # include <gp_Circ2d.hxx>
// # include <gp_Elips2d.hxx>
// # include <gp_Hypr2d.hxx>
// # include <gp_Lin2d.hxx>
// # include <gp_Parab2d.hxx>
// # include <gce_ErrorType.hxx>
// # include <Standard_Real.hxx>
// # include <Standard_Version.hxx>
// # include <Standard_ConstructionError.hxx>
// # include <TColgp_Array1OfPnt2d.hxx>
// # include <TColgp_Array1OfVec2d.hxx>
// # include <TColgp_HArray1OfPnt2d.hxx>
// # include <TColStd_HArray1OfBoolean.hxx>
// # include <TColStd_Array1OfReal.hxx>
// # include <TColStd_Array1OfInteger.hxx>
// # include <GCE2d_MakeCircle.hxx>
// # include <GCE2d_MakeArcOfCircle.hxx>
// # include <GCE2d_MakeEllipse.hxx>
// # include <GCE2d_MakeArcOfEllipse.hxx>
// # include <GCE2d_MakeParabola.hxx>
// # include <GCE2d_MakeArcOfParabola.hxx>
// # include <GCE2d_MakeHyperbola.hxx>
// # include <GCE2d_MakeArcOfHyperbola.hxx>
// # include <GCE2d_MakeLine.hxx>
// # include <GCE2d_MakeSegment.hxx>
# include <Precision.hxx>
#endif

#include <Base/VectorPy.h>

#include <Base/Exception.h>
#include <Base/Writer.h>
#include <Base/Reader.h>
#include <Base/Tools.h>

#include "Approximation.h"
// #include <Mod/Part/App/Geom2d/Circle2dPy.h>
// #include <Mod/Part/App/Geom2d/Ellipse2dPy.h>
// #include <Mod/Part/App/Geom2d/Hyperbola2dPy.h>
// #include <Mod/Part/App/Geom2d/Parabola2dPy.h>
// #include <Mod/Part/App/Geom2d/ArcOfCircle2dPy.h>
// #include <Mod/Part/App/Geom2d/ArcOfEllipse2dPy.h>
// #include <Mod/Part/App/Geom2d/ArcOfHyperbola2dPy.h>
// #include <Mod/Part/App/Geom2d/ArcOfParabola2dPy.h>
// #include <Mod/Part/App/Geom2d/BezierCurve2dPy.h>
// #include <Mod/Part/App/Geom2d/BSplineCurve2dPy.h>
// #include <Mod/Part/App/Geom2d/Line2dSegmentPy.h>
// #include <Mod/Part/App/Geom2d/Line2dPy.h>
// #include <Mod/Part/App/Geom2d/OffsetCurve2dPy.h>

using namespace Part;
using namespace std;

// extern const char* gce_ErrorStatusText(gce_ErrorType et);


TYPESYSTEM_SOURCE_ABSTRACT(Part::Approximation, Base::Persistence)

Approximation::Approximation()
{
}

Approximation::~Approximation()
{
}

unsigned int Approximation::getMemSize (void) const
{
    return sizeof(Approximation);
}

void Approximation::Save(Base::Writer & /*writer*/) const
{
    throw Base::NotImplementedError("Save");
}

void Approximation::Restore(Base::XMLReader & /*reader*/)
{
    throw Base::NotImplementedError("Restore");
}

// -------------------------------------------------

TYPESYSTEM_SOURCE(Part::MultiPoint, Part::Approximation)

MultiPoint::MultiPoint()
{
    this->myPoint = new AppParCurves_MultiPoint();
}

MultiPoint::MultiPoint(const Standard_Integer NbPoints, const Standard_Integer NbPoints2d)
{
    this->myPoint = new AppParCurves_MultiPoint(NbPoints, NbPoints2d);
}

MultiPoint::~MultiPoint()
{
}

// Approximation *MultiPoint::clone(void) const
// {
//     MultiPoint *newPoint = new MultiPoint(myPoint);
//     return newPoint;
// }
// 
// Base::Vector2d MultiPoint::getPoint(void)const
// {
//     return Base::Vector2d(myPoint->X(),myPoint->Y());
// }
// 
// void MultiPoint::setPoint(const Base::Vector2d& p)
// {
//     this->myPoint->SetCoord(p.x,p.y);
// }

unsigned int MultiPoint::getMemSize (void) const
{
    throw Base::NotImplementedError("MultiPoint::getMemSize");
}

void MultiPoint::Save(Base::Writer &writer) const
{
    throw Base::NotImplementedError("MultiPoint::Save");
}

void MultiPoint::Restore(Base::XMLReader &reader)
{
    throw Base::NotImplementedError("MultiPoint::Restore");
}

// PyObject *MultiPoint::getPyObject(void)
// {
//     Handle(Geom2d_CartesianPoint) c = Handle(Geom2d_CartesianPoint)::DownCast(handle());
//     gp_Pnt2d xy = c->Pnt2d();
// 
//     Py::Tuple tuple(2);
//     tuple.setItem(0, Py::Float(xy.X()));
//     tuple.setItem(1, Py::Float(xy.Y()));
//     return Py::new_reference_to(tuple);
// }

// -------------------------------------------------

