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

DEFINE_JAVA_CLASS(A, "com/android/ducktornavi/DucktorNaviGraphics");
DEFINE_JAVA_CLASS(B, "com/android/ducktornavi/DucktorNaviGraphics$Target");

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

JavaTargetPtr JavaIface::CreateTarget(JNIEnv * env, JavaBitmapPtr & bitmap)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 return JavaTarget::Create((*classes.create_target)(env, bitmap->getObject()), env);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *         class JavaTarget:                                                             *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

JavaTarget::JavaTarget(jobject obj, JNIEnv * env):
    AndroidAccess::JClass("com/android/ducktornavi/DucktorNaviGraphics$Target", obj, env),
    draw_text(AndroidAccess::JFuncFloat::Create(*this, "DrawText", "(Ljava/lang/String;FFIFFF)F")),
    set_border_size(AndroidAccess::JFuncVoid::Create(*this, "SetBorderSize", "(F)V")),
    set_border_colour(AndroidAccess::JFuncVoid::Create(*this, "SetBorderColour", "(I)V")),
    set_colour(AndroidAccess::JFuncVoid::Create(*this, "SetColour", "(I)V"))
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
}

JavaTarget::~JavaTarget()
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
}

void JavaTarget::DrawText(float x, float y, const char * text, int mode, float offset, float textsize, float aspect)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 AndroidAccess::JavaString js(text, getEnv());
 (*draw_text)(getEnv(), js.get(), x, y, mode, offset, textsize, aspect); // LFFIFFF
}

void JavaTarget::SetBorderSize(float size)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 (*set_border_size)(getEnv(), size);
}

void JavaTarget::SetBorderColour(float r, float g, float b, float a)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 JColour colour(r, g, b, a);
 (*set_border_colour)(getEnv(), colour.getInt());
}

void JavaTarget::SetColour(float r, float g, float b, float a)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 JColour colour(r, g, b, a);
 (*set_colour)(getEnv(), colour.getInt());
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *         class Surface:                                                                *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Surface::Surface(int width, int height):
    myJNIEnv(AndroidAccess::getJNIEnv()),
    myWidth(width),
    myHeight(height),
    bitmap(CreateBitmap(width, height))
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Created surface: " << myWidth << "x" << myHeight);
}

Surface::~Surface()
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Deleted surface: " << myWidth << "x" << myHeight);
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

Target::Target(int width, int height):
    mySurface(width, height)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Created target: " << width << "x" << height);
}

Target::~Target()
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Deleted target: " << getSurface().getWidth() << "x" << getSurface().getHeight());
}

const void * Target::GetPixelData(void) const
{
 return mySurface.getData();
}

int Target::GetHeight(void) const
{
 return mySurface.getHeight();
}

int Target::GetWidth(void) const
{
 return mySurface.getWidth();
}

int Target::GetLogicalWidth(void) const
{
 return mySurface.getWidth();
}

PaCaLib::DrawPtr Target::Draw(void)
{
 return PaCaLib::DrawPtr(new PaCaAndroid::Draw(*this));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *         class Draw:                                                                   *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Draw::Draw(PaCaAndroid::Target & target):
    target(target),
    javaTarget(CreateTarget(target.getSurface().getBitmap()))
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

}

void Draw::Scale(float w, float h)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Scale(" << w << ", " << h << ")");
}

void Draw::SetLineWidth(float width)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "SetLineWidth(" << width << ")");
}

void Draw::Move(float x, float y)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Move(" << x << ", " << y << ")");
}

void Draw::Line(float x, float y)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Line(" << x << ", " << y << ")");
}

void Draw::SetLineCap(PaCaLib::LineCap mode)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "SetLineCap(" << (int)mode << ")");
}

void Draw::SetColour(float r, float g, float b, float a)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "SetColour(" << r << ", " << g << ", " << b << ", " << a << ")");

 javaTarget->SetColour(r, g, b, a);
}

void Draw::Rectangle(float x, float y, float w, float h)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Rectangle(" << x << ", " << y << ", " << w << ", " << h << ")");
}

void Draw::Arc(float xc, float yc, float r, float a1, float a2)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Arc(" << xc << ", " << yc << ", " << r << ", " << a1 << ", " << a2 << ")");
}

float Draw::DrawTextInternal(float x, float y, PaCaLib::TextMode mode, const char * text, float size, float offset, float aspect)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "DrawTextInternal(" << x << ", " << y << ", " << (int)mode << ", '" << text << "', size=" << size << ", offset=" << offset << ", aspect=" << aspect << ")");

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

 javaTarget->DrawText(x, y, text, JTextMode, offset, size, aspect);
}

void Draw::SetTextOutlineColour(float r, float g, float b, float a)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "SetTextOutlineColour(" << r << ", " << g << ", " << b << ", " << a << ")");

 javaTarget->SetBorderColour(r, g, b, a);
}

void Draw::SetTextOutline(float outline)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "SetTextOutline(" << outline << ")");

 javaTarget->SetBorderSize(outline);
}

void Draw::Paint(void)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Paint()");
}

void Draw::Paint(float alpha)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Paint(" << alpha << ")");
}

void Draw::Operator(PaCaLib::Oper op)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Operator(" << (int)op << ")");
}

PathPtr Draw::NewPath(void)
{
 return PathPtr(new Path(*this));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *         class Path:                                                                   *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Path::Path(Draw & parent):
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

void Path::SetColour(float r, float g, float b, float a)
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *         class Path:MyJavaPath:                                                        *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Path::MyJavaPath::MyJavaPath(JNIEnv * env):
    path(AndroidAccess::JClass::Create("android/graphics/Path", nullptr, env)),
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
    create_target(AndroidAccess::JFuncObject::Create(*graphics, "CreateTarget", "(Landroid/graphics/Bitmap;)Lcom/android/ducktornavi/DucktorNaviGraphics$Target;"))
{
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
