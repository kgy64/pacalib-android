/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     
 * Purpose:     
 * Author:      
 * License:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "pacalib-android.h"

using namespace PaCaAndroid;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *         class Surface:                                                                *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

PaCaLib::SurfacePtr PaCaLib::Surface::Create(int width, int height)
{
 return PaCaLib::SurfacePtr(new PaCaAndroid::Surface(width, height));
}

void * Surface::getData(void)
{
}

const void * Surface::getData(void) const
{
}

int Surface::getWidth(void) const
{
}

int Surface::getPhysicalWidth(void) const
{
}

int Surface::getHeight(void) const
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *         class Target:                                                                 *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

PaCaLib::TargetPtr PaCaLib::Target::Create(int width, int height)
{
 return PaCaLib::TargetPtr(new PaCaAndroid::Target(width, height));
}

int Target::GetWidth(void) const
{
}

int Target::GetHeight(void) const
{
}

const void * Target::GetPixelData(void) const
{
}

int Target::GetLogicalWidth(void) const
{
}

void Target::Scale(double w, double h)
{
}

void Target::Stroke(void)
{
}

void Target::Fill(void)
{
}

void Target::FillPreserve(void)
{
}

void Target::SetLineWidth(double width)
{
}

void Target::Move(double x, double y)
{
}

void Target::Line(double x, double y)
{
}

void Target::SetLineCap(PaCaLib::LineCap mode)
{
}

void Target::SetColour(double r, double g, double b)
{
}

void Target::SetColour(double r, double g, double b, double a)
{
}

void Target::SetColour(const PaCaLib::Colour & col)
{
}

void Target::Rectangle(double x, double y, double w, double h)
{
}

void Target::Arc(double xc, double yc, double r, double a1, double a2)
{
}

void Target::NewPath(void)
{
}

void Target::NewSubPath(void)
{
}

void Target::ClosePath(void)
{
}

double Target::DrawText(double x, double y, PaCaLib::TextMode mode, const char * text, double size, double aspect)
{
}

void Target::SetTextOutlineColour(double r, double g, double b, double a)
{
}

void Target::SetTextOutline(double outline)
{
}

void Target::Paint(void)
{
}

void Target::Paint(double alpha)
{
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
