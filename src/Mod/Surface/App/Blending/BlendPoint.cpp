/***************************************************************************
 *   Copyright (c) 2022 Matteo Grellier <matteogrellier@gmail.com>         *
 *                                                                         *
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

#include "../PreCompiled.h"
#ifndef _PreComp_
#include <Base/Persistence.h>
#include <Base/Vector3D.h>
#include <Precision.hxx>
#include <Standard_Version.hxx>
#include <TopoDS.hxx>
#include <gp_Pnt.hxx>
#endif
#include "BlendPoint.h"


using namespace Surface;

//PROPERTY_SOURCE(Surface::BlendPoint, Base::Persistence)

unsigned int BlendPoint::getMemSize(void) const
{
    return 1;
}


BlendPoint::BlendPoint(std::vector<Base::Vector3d> vectorList)
{
    if (vectorList.size() == 0) {
        throw Base::ValueError("the vector List is empty");
    }
    for (int i = 0; i < vectorList.size(); i++) {
        vectors.emplace_back(vectorList[i]);
    }
}

BlendPoint::BlendPoint()
{
}

BlendPoint::~BlendPoint()
{
}

void BlendPoint::multiply(double f)
{
    for (size_t i = 0; i < vectors.size(); i++) {
        vectors[i] *= Pow(f, i);
    }
}

void BlendPoint::setSize(double f)
{
    if (vectors.size() > 1) {
        double il = vectors[1].Length();
        if (il > Precision::Confusion()) {
            multiply(f / il);
        }
    }
}

PyObject *BlendPoint::getPyObject(void)
{
    PyObject *pcObject = PyList_New(vectors.size());
    return Py::new_reference_to(pcObject);
}

void BlendPoint::Save(Base::Writer & /*writer*/) const
{
    throw Base::NotImplementedError("BlendPoint::Save");
}

void BlendPoint::Restore(Base::XMLReader & /*reader*/)
{
    throw Base::NotImplementedError("BlendPoint::Restore");
}
