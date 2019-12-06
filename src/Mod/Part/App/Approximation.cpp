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
#include <AppParCurves_MultiCurve.hxx>
#include <AppParCurves_MultiBSpCurve.hxx>
#include <AppDef_MultiLine.hxx>
#include <TColgp_Array1OfPnt.hxx>
#include <TColgp_Array1OfPnt2d.hxx>
#include <AppDef_MultiPointConstraint.hxx>
#include <AppDef_BSplineCompute.hxx>
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

#include <Base/Vector3D.h>
#include <Base/Tools2D.h>
#include <Base/VectorPy.h>

#include <Base/Exception.h>
#include <Base/Writer.h>
#include <Base/Reader.h>
#include <Base/Tools.h>

#include "Approximation.h"
#include <Mod/Part/App/Approximation/ApproximationPy.h>
#include <Mod/Part/App/Approximation/MultiPointPy.h>
#include <Mod/Part/App/Approximation/MultiLinePy.h>
#include <Mod/Part/App/Approximation/BSplineComputePy.h>
// #include <Mod/Part/App/Approximation/MultiCurvePy.h>
// #include <Mod/Part/App/Approximation/MultiBSpCurvePy.h>


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

MultiPoint::MultiPoint(const int NbPoints, const int NbPoints2d)
{
    this->myPoint = new AppParCurves_MultiPoint(NbPoints, NbPoints2d);
}

MultiPoint::MultiPoint(const AppParCurves_MultiPoint &mp)
{
    int nbp3d = mp.NbPoints();
    int nbp2d = mp.NbPoints2d();
    AppParCurves_MultiPoint* mymp = new AppParCurves_MultiPoint(nbp3d, nbp2d);
    for (int it1 = 0; it1 != nbp3d; ++it1) {
        mymp->SetPoint(it1, mp.Point(it1));
    }
    for (int it2 = 0; it2 != nbp2d; ++it2) {
        mymp->SetPoint2d(it2, mp.Point2d(it2));
    }
    this->myPoint = mymp;
}

MultiPoint::MultiPoint(const std::vector<gp_Pnt>& p)
{
    if (p.size() < 1)
        Standard_ConstructionError::Raise();
    TColgp_Array1OfPnt* pts = new TColgp_Array1OfPnt(1, p.size());
    for (std::size_t i=0; i<p.size(); i++) {
        pts->SetValue(i+1, p[i]);
    }
    this->myPoint = new AppParCurves_MultiPoint(*pts);
}

MultiPoint::~MultiPoint()
{
}

Approximation *MultiPoint::clone(void) const
{
    MultiPoint *newPoint = new MultiPoint(*myPoint);
    return newPoint;
}

const AppParCurves_MultiPoint* MultiPoint::occObj() const
{
    return myPoint;
}

int MultiPoint::NbPoints(void) const
{
    return this->myPoint->NbPoints();
}

int MultiPoint::NbPoints2d(void) const
{
    return this->myPoint->NbPoints2d();
}

Base::Vector3d MultiPoint::Point(const int idx)
{
    gp_Pnt gp = this->myPoint->Point(idx);
    Base::Vector3d p(gp.X(), gp.Y(), gp.Z());
    return p;
}
Base::Vector2d MultiPoint::Point2d(const int idx)
{
    gp_Pnt2d gp = this->myPoint->Point2d(idx);
    Base::Vector2d p(gp.X(), gp.Y());
    return p;
}
void MultiPoint::setPoint(const int idx, Base::Vector3d &p)
{
    gp_Pnt gp(p.x, p.y, p.z);
    this->myPoint->SetPoint(idx, gp);
    return;
}
void MultiPoint::setPoint2d(const int idx, Base::Vector2d &p)
{
    gp_Pnt2d gp(p.x, p.y);
    this->myPoint->SetPoint2d(idx, gp);
    return;
}

unsigned int MultiPoint::getMemSize (void) const
{
    throw Base::NotImplementedError("MultiPoint::getMemSize");
}

void MultiPoint::Save(Base::Writer &/*writer*/) const
{
    throw Base::NotImplementedError("MultiPoint::Save");
}

void MultiPoint::Restore(Base::XMLReader &/*reader*/)
{
    throw Base::NotImplementedError("MultiPoint::Restore");
}

// -------------------------------------------------

TYPESYSTEM_SOURCE(Part::MultiLine, Part::Approximation)

MultiLine::MultiLine()
{
    this->myLine = new AppDef_MultiLine();
}

MultiLine::MultiLine(const Standard_Integer NbMult)
{
    this->myLine = new AppDef_MultiLine(NbMult);
}

MultiLine::MultiLine(const AppDef_MultiLine &ml)
{
    Standard_Integer nbmpc = ml.NbMultiPoints();
    if (nbmpc > 0) {
        AppDef_MultiLine* myml = new AppDef_MultiLine(nbmpc);
        for (int it1 = 0; it1 != nbmpc; ++it1) {
            myml->SetValue(it1, ml.Value(it1));
        }
        this->myLine = myml;
    }
}

MultiLine::MultiLine(const std::vector<gp_Pnt>& p)
{
    if (p.size() < 1)
        Standard_ConstructionError::Raise();
    TColgp_Array1OfPnt pts(1, p.size());
    for (std::size_t i=0; i<p.size(); i++) {
        pts.SetValue(i+1, p[i]);
    }
    this->myLine = new AppDef_MultiLine(pts);
}

MultiLine::MultiLine(const TColgp_Array1OfPnt& p)
{
    if (p.Size() < 1)
        Standard_ConstructionError::Raise();
    this->myLine = new AppDef_MultiLine(p);
}

MultiLine::~MultiLine()
{
}

Approximation *MultiLine::clone(void) const
{
    MultiLine *newLine = new MultiLine(*myLine);
    return newLine;
}

const AppDef_MultiLine* MultiLine::occObj() const
{
    return myLine;
}

int MultiLine::NbPoints(void) const
{
    return this->myLine->NbPoints();
}

int MultiLine::NbMultiPoints(void) const
{
    return this->myLine->NbMultiPoints();
}

AppDef_MultiPointConstraint MultiLine::Value(int idx)
{
    try {
        Standard_OutOfRange_Raise_if
            (idx < 1 || idx > this->myLine->NbMultiPoints(), "MultiPoint index out of range");
        return this->myLine->Value(idx);
    }
    catch (Standard_Failure& e) {
        THROWM(Base::CADKernelError,e.GetMessageString())
    }
}

void MultiLine::setParameter(const Standard_Integer idx, const Standard_Real u)
{
    try {
        Standard_OutOfRange_Raise_if
            (idx < 1 || idx > this->myLine->NbMultiPoints(), "MultiPoint index out of range");
//         this->myLine->SetParameter(idx, u);
    }
    catch (Standard_Failure& e) {
        THROWM(Base::CADKernelError,e.GetMessageString())
    }
}

unsigned int MultiLine::getMemSize (void) const
{
    throw Base::NotImplementedError("MultiLine::getMemSize");
}

void MultiLine::Save(Base::Writer &/*writer*/) const
{
    throw Base::NotImplementedError("MultiLine::Save");
}

void MultiLine::Restore(Base::XMLReader &/*reader*/)
{
    throw Base::NotImplementedError("MultiLine::Restore");
}

PyObject *MultiLine::getPyObject(void)
{
    return new MultiLinePy(static_cast<MultiLine*>(this->clone()));
}

// -------------------------------------------------

TYPESYSTEM_SOURCE(Part::BSplineCompute, Part::Approximation)

BSplineCompute::BSplineCompute()
{
    this->myBsCompute = new AppDef_BSplineCompute();
}

BSplineCompute::BSplineCompute(const AppDef_BSplineCompute &bsc)
{
    // TODO : create a real copy funcion
    this->myBsCompute = new AppDef_BSplineCompute();
}

BSplineCompute::~BSplineCompute()
{
}

Approximation *BSplineCompute::clone(void) const
{
    BSplineCompute *newComp = new BSplineCompute(*myBsCompute);
    return newComp;
}

void BSplineCompute::Perform (const AppDef_MultiLine &line)
{
    this->myBsCompute->Perform(line);
}

void BSplineCompute::Interpol (const AppDef_MultiLine &line)
{
    this->myBsCompute->Interpol(line);
}

AppParCurves_MultiBSpCurve BSplineCompute::Value() const
{
    return this->myBsCompute->Value();
}

const AppDef_BSplineCompute* BSplineCompute::occObj() const
{
    return myBsCompute;
}

unsigned int BSplineCompute::getMemSize (void) const
{
    throw Base::NotImplementedError("BSplineCompute::getMemSize");
}

void BSplineCompute::Save(Base::Writer &/*writer*/) const
{
    throw Base::NotImplementedError("BSplineCompute::Save");
}

void BSplineCompute::Restore(Base::XMLReader &/*reader*/)
{
    throw Base::NotImplementedError("BSplineCompute::Restore");
}

PyObject *BSplineCompute::getPyObject(void)
{
    return new BSplineComputePy(static_cast<BSplineCompute*>(this->clone()));
}

// -------------------------------------------------

TYPESYSTEM_SOURCE(Part::MultiCurve, Part::Approximation)

MultiCurve::MultiCurve()
{
    this->myCurve = new AppParCurves_MultiCurve();
}

MultiCurve::MultiCurve(const AppParCurves_MultiCurve &)
{
    // TODO : create a real copy funcion
    this->myCurve = new AppParCurves_MultiCurve();
}

MultiCurve::~MultiCurve()
{
}

Approximation *MultiCurve::clone(void) const
{
    MultiCurve *newComp = new MultiCurve(*myCurve);
    return newComp;
}

const AppParCurves_MultiCurve* MultiCurve::occObj() const
{
    return myCurve;
}

unsigned int MultiCurve::getMemSize (void) const
{
    throw Base::NotImplementedError("MultiCurve::getMemSize");
}

void MultiCurve::Save(Base::Writer &/*writer*/) const
{
    throw Base::NotImplementedError("MultiCurve::Save");
}

void MultiCurve::Restore(Base::XMLReader &/*reader*/)
{
    throw Base::NotImplementedError("MultiCurve::Restore");
}

// -------------------------------------------------

TYPESYSTEM_SOURCE(Part::MultiBSpCurve, Part::MultiCurve)

MultiBSpCurve::MultiBSpCurve()
{
    this->myCurve = new AppParCurves_MultiBSpCurve();
}

MultiBSpCurve::MultiBSpCurve(const AppParCurves_MultiCurve &)
{
    // TODO : create a real copy funcion
    this->myCurve = new AppParCurves_MultiBSpCurve();
}

MultiBSpCurve::~MultiBSpCurve()
{
}

Approximation *MultiBSpCurve::clone(void) const
{
    MultiBSpCurve *newComp = new MultiBSpCurve(*myCurve);
    return newComp;
}

const AppParCurves_MultiBSpCurve *MultiBSpCurve::occObj(void) const
{
    return myCurve;
}

unsigned int MultiBSpCurve::getMemSize (void) const
{
    throw Base::NotImplementedError("MultiBSpCurve::getMemSize");
}

void MultiBSpCurve::Save(Base::Writer &/*writer*/) const
{
    throw Base::NotImplementedError("MultiBSpCurve::Save");
}

void MultiBSpCurve::Restore(Base::XMLReader &/*reader*/)
{
    throw Base::NotImplementedError("MultiBSpCurve::Restore");
}

// PyObject *MultiBSpCurve::getPyObject(void)
// {
//     return new MultiBSpCurvePy(static_cast<MultiBSpCurve*>(this->clone()));
// }

