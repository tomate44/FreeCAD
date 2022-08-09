/***************************************************************************
 *   Copyright (c) 2022 Matteo Grellier <matteogrellier@gmail.com>         *
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

#ifndef BLEND_CURVE_H
#define BLEND_CURVE_H
#include <Geom_BezierCurve.hxx>
#include <Mod/Part/App/Geometry.h>
#include <App/PropertyLinks.h>
#include <App/PropertyStandard.h>
#include <App/PropertyUnits.h>
#include <Base/Persistence.h>
#include <Mod/Part/App/PartFeature.h>
#include <Mod/Surface/SurfaceGlobal.h>
#include <Mod/Surface/App/Blending/BlendPoint.h>

namespace Surface
{
/*!
* Create a BezierCurve interpolating a list of BlendPoints
*/
class SurfaceExport BlendCurve: public Base::Persistence
{
public:

    std::vector<BlendPoint> blendPoints;

    /*!
    *  Constructor
    *\param std::vector<BlendPoint>
    */

    void setSize(int, double);

    BlendCurve();
    BlendCurve(std::vector<BlendPoint> blendPointsList);
    virtual ~BlendCurve();
    /*!
    *  Perform the interpolate algorithm
    *\return the BezierCurve
    */
    Handle(Geom_BezierCurve) compute();
    virtual PyObject *getPyObject(void);
    // Persistence implementer ---------------------
    virtual unsigned int getMemSize(void) const;
    virtual void Save(Base::Writer & /*writer*/) const;
    virtual void Restore(Base::XMLReader & /*reader*/);

// protected:
//     math_Matrix            _BsplineCoefMatrix;
};
}// namespace Surface

#endif

