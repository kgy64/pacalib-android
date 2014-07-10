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

SurfacePtr PaCaLib::Surface::Get(int width, int height)
{
 return SurfacePtr(new Surface(width, height));
}

void Surface::* getData(void) override
{
}

const Surface::void * getData(void) const override
{
}

int Surface::getWidth(void) const override
{
}

int Surface::getPhysicalWidth(void) const override
{
}

int Surface::getHeight(void) const override
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *         class Target:                                                                 *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

TargetPtr PaCaLib::Target::Get(int width, int height)
{
 return TargetPtr(new Target(width, height));
}

int Target::GetWidth(void) const override
{
}

int Target::GetHeight(void) const override
{
}

const Target::void * GetPixelData(void) const override
{
}

int Target::GetLogicalWidth(void) const override
{
}

void Target::Scale(double w, double h) override
{
}

void Target::Stroke(void) override
{
}

void Target::Fill(void) override
{
}

void Target::FillPreserve(void) override
{
}

void Target::SetLineWidth(double width) override
{
}

void Target::Move(double x, double y) override
{
}

void Target::Line(double x, double y) override
{
}

void Target::SetLineCap(cairo_line_cap_t mode) override
{
}

void Target::SetColour(double r, double g, double b) override
{
}

void Target::SetColour(double r, double g, double b, double a) override
{
}

void Target::SetColour(const Colour & col) override
{
}

void Target::Rectangle(double x, double y, double w, double h) override
{
}

void Target::Arc(double xc, double yc, double r, double a1, double a2) override
{
}

void Target::NewPath(void) override
{
}

void Target::NewSubPath(void) override
{
}

void Target::ClosePath(void) override
{
}

double Target::DrawText(double x, double y, TextMode mode, const char * text, double size, double aspect = 1.0) override
{
}

void Target::SetTextOutlineColour(double r, double g, double b, double a = 1.0) override
{
}

void Target::SetTextOutline(double outline) override
{
}

void Target::Paint(void) override
{
}

void Target::Paint(double alpha) override
{
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
