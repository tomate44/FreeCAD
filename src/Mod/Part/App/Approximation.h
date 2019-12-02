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

#ifndef PART_APPROXIMATION_H
#define PART_APPROXIMATION_H

#include <AppParCurves_MultiPoint.hxx>
#include <AppDef_MultiLine.hxx>
#include <AppDef_MultiPointConstraint.hxx>
#include <TColgp_Array1OfPnt.hxx>
#include <AppDef_BSplineCompute.hxx>
// #include <Geom2d_CartesianPoint.hxx>
// #include <Geom2d_BezierCurve.hxx>
// #include <Geom2d_BSplineCurve.hxx>
// #include <Geom2d_Circle.hxx>
// #include <Geom2d_Ellipse.hxx>
// #include <Geom2d_Hyperbola.hxx>
// #include <Geom2d_Parabola.hxx>
// #include <Geom2d_Line.hxx>
// #include <Geom2d_OffsetCurve.hxx>
// #include <Geom2d_TrimmedCurve.hxx>
// #include <Geom_Surface.hxx>
// #include <TopoDS_Shape.hxx>
#include <gp_Pnt.hxx>
#include <list>
#include <vector>
#include <memory>
#include <Base/Persistence.h>
#include <Base/Vector3D.h>
#include <Base/Tools2D.h>

namespace Part {

class PartExport Approximation : public Base::Persistence
{
    TYPESYSTEM_HEADER();
public:
    virtual ~Approximation();

//     virtual TopoDS_Shape toShape() const = 0;
//     virtual const Handle(Geom2d_Geometry)& handle() const = 0;
    // Persistence implementer ---------------------
    virtual unsigned int getMemSize(void) const;
    virtual void Save(Base::Writer &/*writer*/) const;
    virtual void Restore(Base::XMLReader &/*reader*/);
    /// returns a cloned object 
    virtual Approximation *clone(void) const = 0;

protected:
    Approximation();

private:
    Approximation(const Approximation&);
    Approximation& operator = (const Approximation&);
};

// ------------------------------------------------

class PartExport MultiPoint : public Approximation
{
    TYPESYSTEM_HEADER();
public:
    MultiPoint();
    MultiPoint(const int, const int);
    MultiPoint(const AppParCurves_MultiPoint &);
    MultiPoint(const std::vector<gp_Pnt>&);
    virtual ~MultiPoint();
    virtual Approximation *clone(void) const;
//     virtual TopoDS_Shape toShape() const;
    int NbPoints(void) const;
    int NbPoints2d(void) const;
    Base::Vector3d Point(const int idx);
    Base::Vector2d Point2d(const int idx);
    void setPoint(const int idx, Base::Vector3d &p);
    void setPoint2d(const int idx, Base::Vector2d &p);

   // Persistence implementer ---------------------
    virtual unsigned int getMemSize(void) const;
    virtual void Save(Base::Writer &/*writer*/) const;
    virtual void Restore(Base::XMLReader &/*reader*/);
    // Base implementer ----------------------------
//     virtual PyObject *getPyObject(void);
// 
    const AppParCurves_MultiPoint* handle() const;
// 
//     Base::Vector2d getPoint(void)const;
//     void setPoint(const Base::Vector2d&);

private:
    AppParCurves_MultiPoint* myPoint;
};

// ------------------------------------------------

class PartExport MultiLine : public Approximation
{
    TYPESYSTEM_HEADER();
public:
    MultiLine();
    MultiLine(const Standard_Integer);
    MultiLine(const AppDef_MultiLine &);
    MultiLine(const std::vector<gp_Pnt>&);
    MultiLine(const TColgp_Array1OfPnt&);
    virtual ~MultiLine();
    virtual Approximation *clone(void) const;
//     virtual TopoDS_Shape toShape() const;
    int NbPoints(void) const;
    int NbMultiPoints(void) const;
    AppDef_MultiPointConstraint Value(const Standard_Integer idx);
    void setParameter(const Standard_Integer idx, const Standard_Real u);

   // Persistence implementer ---------------------
    virtual unsigned int getMemSize(void) const;
    virtual void Save(Base::Writer &/*writer*/) const;
    virtual void Restore(Base::XMLReader &/*reader*/);
    // Base implementer ----------------------------
    virtual PyObject *getPyObject(void);
// 
    const AppDef_MultiLine* handle() const;
// 
//     Base::Vector2d getPoint(void)const;
//     void setPoint(const Base::Vector2d&);

private:
    AppDef_MultiLine* myLine;
};

// ------------------------------------------------

class PartExport BSplineCompute : public Approximation
{
    TYPESYSTEM_HEADER();
public:
    BSplineCompute();
    BSplineCompute(const AppDef_BSplineCompute &);
    virtual ~BSplineCompute();
    virtual Approximation *clone(void) const;
//     virtual TopoDS_Shape toShape() const;
    void Perform(const AppDef_MultiLine &);
// OR
//    void Perform (const MultiLine &); ??????????

   // Persistence implementer ---------------------
    virtual unsigned int getMemSize(void) const;
    virtual void Save(Base::Writer &/*writer*/) const;
    virtual void Restore(Base::XMLReader &/*reader*/);
    // Base implementer ----------------------------
//     virtual PyObject *getPyObject(void);
// 
//     const AppDef_MultiLine* handle() const;
// 
//     Base::Vector2d getPoint(void)const;
//     void setPoint(const Base::Vector2d&);

private:
    AppDef_BSplineCompute* myBsCompute;
};

}
#endif // PART_APPROXIMATION_H
