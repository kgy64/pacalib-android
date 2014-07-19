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

#include <android-render.h>

using namespace PaCaAndroid;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *         class JavaIface:                                                              *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

PaCaAndroid::JavaIface * PaCaAndroid::JavaIface::myself;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *         class Surface:                                                                *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

PaCaLib::SurfacePtr PaCaLib::Surface::Create(int width, int height)
{
 return PaCaLib::SurfacePtr(new PaCaAndroid::Surface(width, height));
}

PaCaAndroid::Surface::Surface(int width, int height):
    myWidth(width),
    myHeight(height),
    bitmap(CreateBitmap(width, height))
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 argb * pix = (argb*)getData();

 SYS_DEBUG(DL_INFO1, "pix data at " << pix);

 return;

 for (int i = 0; i < myHeight; ++i) {
    float y = 255.0 * (float)i / (float)myHeight;
    argb * pixel = &pix[i * myWidth];
    for (int j = 0; j < myWidth; ++j, ++pixel) {
        float x = 255.0 * (float)j / (float)myWidth;
        pixel->r = (int)(x*y);
        pixel->g = (int)x;
        pixel->b = (int)y;
        pixel->a = 180;
    }
 }
}

PaCaAndroid::Surface::~Surface()
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
}

void * Surface::getData(void)
{
 return bitmap->getPixelData();
}

const void * Surface::getData(void) const
{
 return bitmap->getPixelData();
}

int Surface::getWidth(void) const
{
 return myWidth;
}

int Surface::getPhysicalWidth(void) const
{
 return myWidth;
}

int Surface::getHeight(void) const
{
 return myHeight;
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

PaCaAndroid::Target::Target(int width, int height):
    mySurface(PaCaLib::Surface::Create(width, height))
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
}

PaCaAndroid::Target::~Target()
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
}

int Target::GetHeight(void) const
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 return mySurface->getHeight();
}

int Target::GetWidth(void) const
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 return mySurface->getWidth();
}

int Target::GetLogicalWidth(void) const
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 return mySurface->getWidth();
}

const void * Target::GetPixelData(void) const
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 return mySurface->getData();
}

void Target::Scale(double w, double h)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
}

void Target::Stroke(void)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
}

void Target::Fill(void)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
}

void Target::FillPreserve(void)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
}

void Target::SetLineWidth(double width)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
}

void Target::Move(double x, double y)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
}

void Target::Line(double x, double y)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
}

void Target::SetLineCap(PaCaLib::LineCap mode)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
}

void Target::SetColour(double r, double g, double b)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
}

void Target::SetColour(double r, double g, double b, double a)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
}

void Target::SetColour(const PaCaLib::Colour & col)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
}

void Target::Rectangle(double x, double y, double w, double h)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
}

void Target::Arc(double xc, double yc, double r, double a1, double a2)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
}

void Target::NewPath(void)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
}

void Target::NewSubPath(void)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
}

void Target::ClosePath(void)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
}

double Target::DrawText(double x, double y, PaCaLib::TextMode mode, const char * text, double size, double aspect)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
}

void Target::SetTextOutlineColour(double r, double g, double b, double a)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
}

void Target::SetTextOutline(double outline)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
}

void Target::Paint(void)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
}

void Target::Paint(double alpha)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
}

void Target::Operator(PaCaLib::Oper op)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
