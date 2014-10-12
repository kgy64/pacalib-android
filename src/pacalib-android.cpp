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

JavaBitmapPtr JavaIface::CreateJavaBitmap(JNIEnv * env, int32_t width, int32_t height, Glesly::PixelFormat format)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 int mode = 0;

 switch (format) {
    case Glesly::FORMAT_BGRA_8888:
        mode = 1;
    break;
    case Glesly::FORMAT_RGB_565:
        mode = 2;
    break;
    default:
        ASSERT(false, "pixel format " << format << " is not supported");
    break;
 }

 return JavaBitmap::Create((*classes.create_bitmap)(env, width, height, mode), env);
}

JavaDrawPtr JavaIface::CreateJavaDraw(JNIEnv * env, JavaBitmapPtr & bitmap)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 auto instance = (*classes.create_draw)(env, bitmap->getObject());
 JavaDrawPtr p = JavaDraw::Create(instance, env);
 env->DeleteLocalRef(instance);
 return p;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *         class JavaDraw:                                                               *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

JavaDraw::JavaDraw(jobject obj, JNIEnv * env):
    JClass("com/android/ducktornavi/DucktorNaviGraphics$Draw", obj, env),
    set_scale          (JFuncVoid::Create (*this, "SetScale",        "(FF)V")),
    set_border_size    (JFuncVoid::Create (*this, "SetBorderSize",   "(F)V")),
    set_border_colour  (JFuncVoid::Create (*this, "SetBorderColour", "(I)V")),
    set_colour         (JFuncVoid::Create (*this, "SetColour",       "(I)V")),
    set_line_cap       (JFuncVoid::Create (*this, "SetLineCap",      "(I)V")),
    set_line_width     (JFuncVoid::Create (*this, "SetLineWidth",    "(F)V")),
    draw_text          (JFuncFloat::Create(*this, "DrawText",        "(Ljava/lang/String;FFIFFF)F")),
    draw_path          (JFuncVoid::Create (*this, "DrawPath",        "(Landroid/graphics/Path;I)V")),
    draw_arc           (JFuncVoid::Create (*this, "DrawArc",         "(Landroid/graphics/Path;FFFFFF)V")),
    draw_fill          (JFuncVoid::Create (*this, "DrawFill",        "()V"))
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
}

JavaDraw::~JavaDraw()
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
}

float JavaDraw::DrawText(float x, float y, const char * text, int mode, float offset, float textsize, float aspect)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 JavaString js(text, getEnv());
 return (*draw_text)(getEnv(), js.get(), x, y, mode, offset, textsize, aspect); // LFFIFFF
}

void JavaDraw::DrawArc(jobject path, float left, float top, float right, float bottom, float start, float sweep)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 (*draw_arc)(getEnv(), path, left, top, right, bottom, start, sweep);
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
    case PaCaLib::LINE_CAP_NONE:
        mode = 0;
    break;
    case PaCaLib::LINE_CAP_SQUARE:
        mode = 1;
    break;
    case PaCaLib::LINE_CAP_ROUND:
        mode = 2;
    break;
 }

 (*set_line_cap)(getEnv(), mode);
}

void JavaDraw::SetLineWidth(float width)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 (*set_line_width)(getEnv(), width);
}

void JavaDraw::DrawPath(jobject path, int mode)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 (*draw_path)(getEnv(), path, mode);
}

void JavaDraw::DrawFill(void)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 (*draw_fill)(getEnv());
}

void JavaDraw::Scale(float sw, float sh)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 (*set_scale)(getEnv(), sw, sh);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *         class Surface:                                                                *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Surface::Surface(int width, int height, Glesly::PixelFormat format):
    myJNIEnv(AndroidAccess::getJNIEnv()),
    myWidth(width),
    myHeight(height),
    myFormat(format),
    bitmap(CreateJavaBitmap(width, height, format))
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

PaCaLib::TargetPtr PaCaLib::Target::Create(int width, int height, Glesly::PixelFormat format)
{
 if (format == Glesly::FORMAT_DEFAULT) {
    // Unfortunately, RGBA seems not handled in Android
    format = Glesly::FORMAT_BGRA_8888;
 }

 return PaCaLib::TargetPtr(new PaCaAndroid::Target(width, height, format));
}

Target::Target(int width, int height, Glesly::PixelFormat format):
    mySurface(width, height, format)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Created target: " << width << "x" << height);
}

Target::~Target()
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Deleted target: " << getSurface().getWidth() << "x" << getSurface().getHeight());
}

Glesly::PixelFormat Target::GetPixelFormat(void) const
{
 return mySurface.getFormat();
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
    javaDraw(CreateJavaDraw(target.getSurface().getBitmap())),
    myEnv(AndroidAccess::getJNIEnv())   // Must use the current environment
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
}

void Draw::Scale(float sw, float sh)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Scale(" << sw << ", " << sh << ")");

 javaDraw->Scale(sw, sh);
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

 return javaDraw->DrawText(x, y, text, JTextMode, offset, size, aspect);
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

 javaDraw->SetLineCap(cap);
}

void Draw::Paint(void)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 javaDraw->DrawFill();
}

void Draw::DrawPath(jobject path, int mode)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 javaDraw->DrawPath(path, mode);
}

void Draw::DrawArc(jobject path, float left, float top, float right, float bottom, float start, float sweep)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 javaDraw->DrawArc(path, left, top, right, bottom, start, sweep);
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
    half_width((float)width * 0.5f),
    height(parent.GetHeight()),
    half_height((float)height * 0.5f)
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

 (*path.draw_move)(parent.getEnv(), half_width  * (x + 1.0f), half_height * (y + 1.0f));
}

void Path::Line(float x, float y)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 (*path.draw_line)(parent.getEnv(), half_width  * (x + 1.0f), half_height * (y + 1.0f));
}

void Path::Arc(float xc, float yc, float r, float a1, float a2)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 xc += 1.0f;
 yc += 1.0f;

 float left = half_width * (xc - r);
 float top = half_height * (yc - r);
 float right = half_width * (xc + r);
 float bottom = half_height * (yc + r);

 float start = a1 * (float)(180.0/M_PI);
 float sweep = (a2-a1) * (float)(180.0/M_PI);

 parent.DrawArc(path.getPath(), left, top, right, bottom, start, sweep);
}

void Path::Close(void)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 (*path.close)(parent.getEnv());
}

void Path::Clear(void)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

}

void Path::Stroke(void)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 parent.DrawPath(path.getPath(), 0);
}

void Path::Fill(void)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 parent.DrawPath(path.getPath(), 1);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *         class Path:MyJavaPath:                                                        *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Path::MyJavaPath::MyJavaPath(JNIEnv * env):
    path(JClass::Create("android/graphics/Path", nullptr, env)),
    draw_line   (JFuncVoid::Create  (*path, "lineTo", "(FF)V")),
    draw_move   (JFuncVoid::Create  (*path, "moveTo", "(FF)V")),
    close       (JFuncVoid::Create  (*path, "close", "()V"))
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *         class JavaIface::MyJavaClasses:                                               *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

JavaIface::MyJavaClasses::MyJavaClasses(void):
    graphics(JClass::Create("com/android/ducktornavi/DucktorNaviGraphics")),
    create_bitmap(JFuncObject::Create(*graphics, "CreateBitmap", "(III)Landroid/graphics/Bitmap;")),
    create_draw(JFuncObject::Create(*graphics, "CreateDraw", "(Landroid/graphics/Bitmap;)Lcom/android/ducktornavi/DucktorNaviGraphics$Draw;"))
{
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
