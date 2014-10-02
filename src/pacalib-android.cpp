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
DEFINE_JAVA_CLASS(B, "com/android/ducktornavi/DucktorNaviGraphics$Draw");

using namespace PaCaAndroid;
using namespace AndroidAccess;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *         class JavaIface:                                                              *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

PaCaAndroid::JavaIface * PaCaAndroid::JavaIface::myself;

JavaBitmapPtr JavaIface::CreateJavaBitmap(JNIEnv * env, int32_t width, int32_t height)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 return JavaBitmap::Create((*classes.create_bitmap)(env, width, height), env);
}

JavaDrawPtr JavaIface::CreateJavaDraw(JNIEnv * env, JavaBitmapPtr & bitmap)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 return JavaDraw::Create((*classes.create_target)(env, bitmap->getObject()), env);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *         class JavaDraw:                                                               *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

JavaDraw::JavaDraw(jobject obj, JNIEnv * env):
    JClass("com/android/ducktornavi/DucktorNaviGraphics$Draw", obj, env),
    draw_text          (JFuncFloat::Create(*this, "DrawText",        "(Ljava/lang/String;FFIFFF)F")),
    set_border_size    (JFuncVoid::Create (*this, "SetBorderSize",   "(F)V")),
    set_border_colour  (JFuncVoid::Create (*this, "SetBorderColour", "(I)V")),
    set_colour         (JFuncVoid::Create (*this, "SetColour",       "(I)V")),
    set_line_cap       (JFuncVoid::Create (*this, "SetLineCap",      "(I)V")),
    set_line_width     (JFuncVoid::Create (*this, "SetLineWidth",    "(F)V")),
    draw_path          (JFuncVoid::Create (*this, "DrawPath",        "(Landroid/graphics/Path;)V")),
    draw_arc           (JFuncVoid::Create (*this, "DrawArc",         "(Landroid/graphics/Path;FFFFF)V"))
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
}

JavaDraw::~JavaDraw()
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
}

void JavaDraw::DrawText(float x, float y, const char * text, int mode, float offset, float textsize, float aspect)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 JavaString js(text, getEnv());
 (*draw_text)(getEnv(), js.get(), x, y, mode, offset, textsize, aspect); // LFFIFFF
}

void JavaDraw::DrawArc(jobject path, float xc, float yc, float r, float a1, float a2)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 float start = a1 * (float)(180.0/M_PI);
 float sweep = (a2-a1) * (float)(180.0/M_PI);

 (*draw_arc)(getEnv(), path, xc, yc, r, start, sweep);
}

void JavaDraw::SetBorderSize(float size)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 (*set_border_size)(getEnv(), size);
}

void JavaDraw::SetBorderColour(float r, float g, float b, float a)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 JColour colour(r, g, b, a);
 (*set_border_colour)(getEnv(), colour.getInt());
}

void JavaDraw::SetColour(float r, float g, float b, float a)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 JColour colour(r, g, b, a);
 (*set_colour)(getEnv(), colour.getInt());
}

void JavaDraw::SetLineCap(PaCaLib::LineCap cap)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 int mode = 0;

 switch (cap) {
    case PaCaLib::LINE_CAP_ROUND:
        mode = 0;
    break;
    case PaCaLib::LINE_CAP_SQUARE:
        mode = 1;
    break;
 }

 (*set_line_cap)(getEnv(), mode);
}

void JavaDraw::SetLineWidth(float width)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 (*set_line_width)(getEnv(), width);
}

void JavaDraw::Stroke(jobject path)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 (*draw_path)(getEnv(), path);
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
    bitmap(CreateJavaBitmap(width, height))
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
    javaDraw(CreateJavaDraw(target.getSurface().getBitmap()))
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
}

void Draw::Scale(float w, float h)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Scale(" << w << ", " << h << ")");
}

void Draw::SetColour(float r, float g, float b, float a)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "SetColour(" << r << ", " << g << ", " << b << ", " << a << ")");

 javaDraw->SetColour(r, g, b, a);
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

 javaDraw->DrawText(x, y, text, JTextMode, offset, size, aspect);
}

void Draw::SetTextOutlineColour(float r, float g, float b, float a)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "SetTextOutlineColour(" << r << ", " << g << ", " << b << ", " << a << ")");

 javaDraw->SetBorderColour(r, g, b, a);
}

void Draw::SetTextOutline(float outline)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "SetTextOutline(" << outline << ")");

 javaDraw->SetBorderSize(outline);
}

void Draw::SetLineWidth(float width)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 javaDraw->SetLineWidth(width);
}

void Draw::SetLineCap(PaCaLib::LineCap cap)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 javaTarget->SetLineCap(cap);
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

void Draw::Fill(void)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

}

void Draw::Stroke(jobject path)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 javaDraw->Stroke(path);
}

void Draw::DrawArc(jobject path, float xc, float yc, float r, float a1, float a2)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 javaDraw->DrawArc(path, xc, yc, r, a1, a2);
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
    path(parent.getEnv()),
    width(parent.GetWidth()),
    height(parent.GetHeight())
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
}

Path::~Path()
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
}

void Path::Move(float x, float y)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 x = (float)(width/2) * (x + 1.0f);
 y = (float)(height/2) * (y + 1.0f);

 (*path.draw_move)(parent.getEnv(), x, y);
}

void Path::Line(float x, float y)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 x = (float)(width/2) * (x + 1.0f);
 y = (float)(height/2) * (y + 1.0f);

 (*path.draw_line)(parent.getEnv(), x, y);
}

void Path::Arc(float xc, float yc, float r, float a1, float a2)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 parent.DrawArc(path.getPath(), xc, yc, r, a1, a2);
}

void Path::Close(void)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

}

void Path::Clear(void)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

}

void Path::Stroke(void)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 parent.Stroke(path.getPath());
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *         class Path:MyJavaPath:                                                        *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Path::MyJavaPath::MyJavaPath(JNIEnv * env):
    path(JClass::Create("android/graphics/Path", nullptr, env)),
    draw_line   (JFuncVoid::Create  (*path, "lineTo", "(FF)V")),
    draw_move   (JFuncVoid::Create  (*path, "moveTo", "(FF)V"))
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *         class JavaIface::MyJavaClasses:                                               *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

JavaIface::MyJavaClasses::MyJavaClasses(void):
    graphics(JClass::Create("com/android/ducktornavi/DucktorNaviGraphics")),
    create_bitmap(JFuncObject::Create(*graphics, "CreateBitmap", "(II)Landroid/graphics/Bitmap;")),
    create_target(JFuncObject::Create(*graphics, "CreateDraw", "(Landroid/graphics/Bitmap;)Lcom/android/ducktornavi/DucktorNaviGraphics$Draw;"))
{
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
