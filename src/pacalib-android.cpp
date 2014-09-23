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

 jobject obj = (*classes.create_bitmap)(env, width, height);
 JavaBitmapPtr p = JavaBitmap::Create(AndroidAccess::JGlobalRef::Create(obj, env), env);
 env->DeleteLocalRef(obj);
 return p;
}

void JavaIface::DrawText(JNIEnv * env, const JavaBitmapPtr & bitmap, const char * text, float x, float y, int mode, int textColor, float textsize, int borderColor, float borderSize)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 JavaString js(text, env);
 (*classes.draw_text)(env, bitmap->get(), js.get(), x, y, mode, textColor, textsize, borderColor, borderSize);
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

void PaCaAndroid::Surface::DrawText(const char * text, float size)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO2, "Drawing text '" << text << "', size=" << size);

 GetJavaIface().DrawText(myJNIEnv, bitmap, text, 0.0, 0.0, 0, 0xff000000, size, 0, 0.0);
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

void Target::Scale(double w, double h)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Scale(" << w << ", " << h << ")");
}

void Target::Stroke(void)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Stroke()");
}

void Target::Fill(void)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Fill()");
}

void Target::FillPreserve(void)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "FillPreserve()");
}

void Target::SetLineWidth(double width)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "SetLineWidth(" << width << ")");
}

void Target::Move(double x, double y)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Move(" << x << ", " << y << ")");
}

void Target::Line(double x, double y)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Line(" << x << ", " << y << ")");
}

void Target::SetLineCap(PaCaLib::LineCap mode)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "SetLineCap(" << (int)mode << ")");
}

void Target::SetColour(double r, double g, double b)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "SetColour(" << r << ", " << g << ", " << b << ")");

}

void Target::SetColour(double r, double g, double b, double a)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "SetColour(" << r << ", " << g << ", " << b << ", " << a << ")");

}

void Target::SetColour(const PaCaLib::Colour & col)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "SetColour(" << col << ")");

}

void Target::Rectangle(double x, double y, double w, double h)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Rectangle(" << x << ", " << y << ", " << w << ", " << h << ")");
}

void Target::Arc(double xc, double yc, double r, double a1, double a2)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Arc(" << xc << ", " << yc << ", " << r << ", " << a1 << ", " << a2 << ")");
}

void Target::NewPath(void)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "NewPath()");
}

void Target::NewSubPath(void)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "NewSubPath()");
}

void Target::ClosePath(void)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "ClosePath()");
}

double Target::DrawText(double x, double y, PaCaLib::TextMode mode, const char * text, double size, double aspect)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "DrawText(" << x << ", " << y << ", " << (int)mode << ", '" << text << "', " << size << ", " << aspect << ")");
 mySurface.DrawText(text, size);
}

void Target::SetTextOutlineColour(double r, double g, double b, double a)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "SetTextOutlineColour(" << r << ", " << g << ", " << b << ", " << a << ")");
}

void Target::SetTextOutline(double outline)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "SetTextOutline(" << outline << ")");
}

void Target::Paint(void)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Paint()");
}

void Target::Paint(double alpha)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Paint(" << alpha << ")");
}

void Target::Operator(PaCaLib::Oper op)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Operator(" << (int)op << ")");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *         class JavaIface::MyJavaClasses:                                               *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

JavaIface::MyJavaClasses::MyJavaClasses(void):
    graphics(AndroidAccess::JClass::Create("com/android/ducktornavi/DucktorNaviGraphics", true, AndroidAccess::jenv)),
    create_bitmap(AndroidAccess::JFuncObject::Create(*graphics, "CreateBitmap", "(II)Landroid/graphics/Bitmap;")),
    draw_text(AndroidAccess::JFuncFloat::Create(*graphics, "DrawText", "(Landroid/graphics/Bitmap;Ljava/lang/String;FFIIFIF)F"))
{
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
