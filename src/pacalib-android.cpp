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

JavaBitmapPtr JavaIface::CreateBitmap(JNIEnv * env, int32_t width, int32_t height)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 return JavaBitmap::Create((*classes.create_bitmap)(env, width, height), env);
}

void JavaIface::DrawText(JNIEnv * env, const JavaBitmapPtr & bitmap, const char * text, float x, float y, int mode, float offset, int textColor, float textsize, int borderColor, float borderSize, float aspect)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 JavaString js(text, env);
 (*classes.draw_text)(env, bitmap->get(), js.get(), x, y, mode, offset, textColor, textsize, borderColor, borderSize, aspect); // FFIFIFIFF
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *         class Surface:                                                                *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

PaCaAndroid::Surface::Surface(int width, int height):
    myJNIEnv(AndroidAccess::getJNIEnv()),
    myWidth(width),
    myHeight(height),
    bitmap(CreateBitmap(width, height))
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Created surface: " << myWidth << "x" << myHeight);
}

PaCaAndroid::Surface::~Surface()
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Deleted surface: " << myWidth << "x" << myHeight);
}

void PaCaAndroid::Surface::DrawText(float x, float y, const char * text, int mode, float offset, float size, float aspect)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO2, "Drawing text '" << text << "', size=" << size);

 GetJavaIface().DrawText(myJNIEnv, bitmap, text, x, y, mode, offset, drawColour.getInt(), size, textOutlineColour.getInt(), textOutlineWidth, aspect);
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
    mySurface(width, height)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Created target: " << width << "x" << height);
}

PaCaAndroid::Target::~Target()
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Deleted target");
}

int Target::GetHeight(void) const
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 return mySurface.getHeight();
}

int Target::GetWidth(void) const
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 return mySurface.getWidth();
}

int Target::GetLogicalWidth(void) const
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 return mySurface.getWidth();
}

const void * Target::GetPixelData(void) const
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 return mySurface.getData();
}

void Target::Scale(float w, float h)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Scale(" << w << ", " << h << ")");
}

void Target::SetLineWidth(float width)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "SetLineWidth(" << width << ")");
}

void Target::Move(float x, float y)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Move(" << x << ", " << y << ")");
}

void Target::Line(float x, float y)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Line(" << x << ", " << y << ")");
}

void Target::SetLineCap(PaCaLib::LineCap mode)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "SetLineCap(" << (int)mode << ")");
}

void Target::SetColour(float r, float g, float b)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "SetColour(" << r << ", " << g << ", " << b << ")");

 mySurface.SetColour(r, g, b, 1.0f);
}

void Target::SetColour(float r, float g, float b, float a)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "SetColour(" << r << ", " << g << ", " << b << ", " << a << ")");

 mySurface.SetColour(r, g, b, a);
}

void Target::SetColour(const PaCaLib::Colour & col)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "SetColour(" << col << ")");

 mySurface.SetColour(col);
}

void Target::Rectangle(float x, float y, float w, float h)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Rectangle(" << x << ", " << y << ", " << w << ", " << h << ")");
}

void Target::Arc(float xc, float yc, float r, float a1, float a2)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Arc(" << xc << ", " << yc << ", " << r << ", " << a1 << ", " << a2 << ")");
}

float Target::DrawTextInternal(float x, float y, PaCaLib::TextMode mode, const char * text, float size, float offset, float aspect)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "DrawText(" << x << ", " << y << ", " << (int)mode << ", '" << text << "', size=" << size << ", offset=" << offset << ", aspect=" << aspect << ")");

 int JTextMode = 0;

 switch (mode) {
    case PaCaLib::LEFT:
        JTextMode = 0;
    break;
    case PaCaLib::CENTER:
        JTextMode = 1;
    break;
    case PaCaLib::RIGHT:
        JTextMode = 2;
    break;
 }

 mySurface.DrawText(x, y, text, JTextMode, offset, size, aspect);
}

void Target::SetTextOutlineColour(float r, float g, float b, float a)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "SetTextOutlineColour(" << r << ", " << g << ", " << b << ", " << a << ")");

 mySurface.SetTextOutlineColour(r, g, b, a);
}

void Target::SetTextOutline(float outline)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "SetTextOutline(" << outline << ")");

 mySurface.SetTextOutline(outline);
}

void Target::Paint(void)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Paint()");
}

void Target::Paint(float alpha)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Paint(" << alpha << ")");
}

void Target::Operator(PaCaLib::Oper op)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Operator(" << (int)op << ")");
}

PathPtr Target::NewPath(void)
{
 return PathPtr(new Path(*this));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *         class Path:                                                                   *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Path::Path(Target & parent):
    parent(parent),
    path(parent.getEnv())
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
}

Path::~Path()
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
}

void Path::Move(float x, float y)
{
}

void Path::Line(float x, float y)
{
}

void Path::Arc(float xc, float yc, float r, float a1, float a2)
{
}

void Path::Close(void)
{
}

void Path::Clear(void)
{
}

void Path::SetLineWidth(float width)
{
}

void Path::SetLineCap(PaCaLib::LineCap mode)
{
}

void Path::Fill(void)
{
}

void Path::Stroke(void)
{
}

void Path::SetColour(float r, float g, float b)
{
}

void Path::SetColour(float r, float g, float b, float a)
{
}

void Path::SetColour(const PaCaLib::Colour & col)
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *         class Path:MyJavaPath:                                                        *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Path::MyJavaPath::MyJavaPath(JNIEnv * env):
    path(AndroidAccess::JClass::Create("android/graphics/Path", true, env)),
    arc(AndroidAccess::JFuncVoid::Create(*path, "addArc", "(Landroid/graphics/RectF;FF)V"))
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *         class JavaIface::MyJavaClasses:                                               *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

JavaIface::MyJavaClasses::MyJavaClasses(void):
    graphics(AndroidAccess::JClass::Create("com/android/ducktornavi/DucktorNaviGraphics")),
    create_bitmap(AndroidAccess::JFuncObject::Create(*graphics, "CreateBitmap", "(II)Landroid/graphics/Bitmap;")),
    draw_text(AndroidAccess::JFuncFloat::Create(*graphics, "DrawText", "(Landroid/graphics/Bitmap;Ljava/lang/String;FFIFIFIFF)F"))
{
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
