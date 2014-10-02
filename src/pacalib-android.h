/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     
 * Purpose:     
 * Author:      
 * License:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __SRC_PACALIB_ANDROID_H_INCLUDED__
#define __SRC_PACALIB_ANDROID_H_INCLUDED__

#include <pacalib/pacalib.h>
#include <my-android/access/access-base.h>
#include <android/native_window_jni.h>
#include <android-main.h>
#include <Debug/Debug.h>

#include <Memory/Memory.h>

#include <android/bitmap.h>
#include <endian.h>

namespace PaCaAndroid
{
    class JavaBitmap;
    typedef MEM::shared_ptr<JavaBitmap> JavaBitmapPtr;

    class JavaBitmap: public AndroidAccess::JObject
    {
        inline JavaBitmap(jobject obj, JNIEnv * env):
            AndroidAccess::JObject(obj, env),
            pixel_data(nullptr)
        {
            SYS_DEBUG_MEMBER(DM_PACALIB);
            AndroidBitmap_lockPixels(env, getObject(), &pixel_data);
            ASSERT(pixel_data, "lockPixels() failed");
        }

     public:
        VIRTUAL_IF_DEBUG inline ~JavaBitmap()
        {
            SYS_DEBUG_MEMBER(DM_PACALIB);
            if (pixel_data) {
                AndroidBitmap_unlockPixels(env, getObject());
            }
        }

        static inline JavaBitmapPtr Create(jobject obj, JNIEnv * env = AndroidAccess::getJNIEnv())
        {
            return JavaBitmapPtr(new JavaBitmap(obj, env));
        }

        inline void * getPixelData(void)
        {
            return pixel_data;
        }

     protected:
        void * pixel_data;

     private:
        SYS_DEFINE_CLASS_NAME("PaCaAndroid::JavaBitmap");

    }; // class PaCaAndroid::JavaBitmap

    class JavaTarget;
    typedef MEM::shared_ptr<JavaTarget> JavaTargetPtr;

    class JavaTarget: public AndroidAccess::JClass
    {
        JavaTarget(jobject obj, JNIEnv * env);

     public:
        VIRTUAL_IF_DEBUG ~JavaTarget();

        static inline JavaTargetPtr Create(jobject obj, JNIEnv * env = AndroidAccess::getJNIEnv())
        {
            return JavaTargetPtr(new JavaTarget(obj, env));
        }

        void DrawText(float x, float y, const char * text, int mode, float offset, float size, float aspect);
        void SetBorderSize(float size);
        void SetBorderColour(float r, float g, float b, float a);
        void SetColour(float r, float g, float b, float a);

     private:
        SYS_DEFINE_CLASS_NAME("PaCaAndroid::JavaTarget");

        AndroidAccess::JFuncFloatPtr draw_text;
        AndroidAccess::JFuncVoidPtr  set_border_size;
        AndroidAccess::JFuncVoidPtr  set_border_colour;
        AndroidAccess::JFuncVoidPtr  set_colour;

    }; // class PaCaAndroid::JavaTarget

    class JavaIface
    {
     public:
        inline JavaIface(void)
        {
            SYS_DEBUG_MEMBER(DM_PACALIB);
        }

        VIRTUAL_IF_DEBUG ~JavaIface()
        {
            SYS_DEBUG_MEMBER(DM_PACALIB);
        }

        static inline JavaIface & Get()
        {
            // Note: this class must be used only in one thread, that's why no guard is necessary here.
            //       System calls will not work if it is used from more threads.
            if (!myself) {
                myself = new JavaIface();
            }
            return *myself;
        }

        JavaBitmapPtr CreateBitmap(JNIEnv * env, int32_t width, int32_t height);
        JavaTargetPtr CreateTarget(JNIEnv * env, JavaBitmapPtr & bitmap);
        void SetColour(JNIEnv * env, float r, float g, float b, float a);

     protected:
        struct MyJavaClasses
        {
            MyJavaClasses(void);

            AndroidAccess::JClassPtr        graphics;
            AndroidAccess::JFuncObjectPtr   create_bitmap;
            AndroidAccess::JFuncObjectPtr   create_target;
            AndroidAccess::JFuncFloatPtr    draw_text;

        }; // struct MyJavaClasses

        MyJavaClasses classes;

     private:
        SYS_DEFINE_CLASS_NAME("PaCaAndroid::JavaIface");

        static JavaIface * myself;

    }; // class PaCaAndroid::JavaIface

    /// Java-compatible colour
    struct JColour
    {
        inline JColour(void):
            int_val(0)
        {
        }

        inline JColour(float fr, float fg, float fb, float fa)
        {
            SetRGBA(fr, fg, fb, fa);
        }

        inline JColour(const PaCaLib::Colour & col):
            JColour(col.r, col.g, col.b, col.a)
        {
        }

        union {
            struct {
#if _BYTE_ORDER == _LITTLE_ENDIAN
                uint8_t     r;
                uint8_t     g;
                uint8_t     b;
                uint8_t     a;
#elif _BYTE_ORDER == _BIG_ENDIAN
    #error Big endian is not implemented yet
#else
    #error Wrong endian type
#endif
            };

            uint32_t    int_val;
        };

        inline void SetRGBA(float fr, float fg, float fb, float fa)
        {
            a = (int)(fa * 255.0f + 0.5f);
            r = (int)(fr * 255.0f + 0.5f);
            g = (int)(fg * 255.0f + 0.5f);
            b = (int)(fb * 255.0f + 0.5f);
        }

        inline void Set(const PaCaLib::Colour & col)
        {
            SetRGBA(col.r, col.g, col.b, col.a);
        }

        inline uint32_t getInt(void) const
        {
            return int_val;
        }

    }; // class PaCaAndroid::JColour

    class Surface
    {
     public:
        Surface(int width, int height);
        VIRTUAL_IF_DEBUG ~Surface();

        inline JNIEnv * getEnv(void)
        {
            return myJNIEnv;
        }

        inline JavaBitmapPtr getBitmap(void)
        {
            return bitmap;
        }

        inline void * getData(void)
        {
            return bitmap->getPixelData();
        }

        inline const void * getData(void) const
        {
            return bitmap->getPixelData();
        }

        inline int getWidth(void) const
        {
            return myWidth;
        }

        inline int getPhysicalWidth(void) const
        {
            return myWidth;
        }

        inline int getHeight(void) const
        {
            return myHeight;
        }

     protected:
        static inline JavaIface & GetJavaIface(void)
        {
            return PaCaAndroid::JavaIface::Get();
        }

        inline JavaBitmapPtr CreateBitmap(int width, int height)
        {
            SYS_DEBUG_MEMBER(DM_PACALIB);
            SYS_DEBUG(DL_INFO2, "Creating bitmap " << width << "x" << height);
            return GetJavaIface().CreateBitmap(getEnv(), width, height);
        }

        JNIEnv * myJNIEnv;

        int myWidth;

        int myHeight;

        JavaBitmapPtr bitmap;

     private:
        SYS_DEFINE_CLASS_NAME("PaCaAndroid::Surface");

    }; // class PaCaAndroid::Surface;

    typedef PaCaLib::PathPtr PathPtr;
    typedef PaCaLib::DrawPtr DrawPtr;

    class Target: public PaCaLib::Target
    {
        friend class PaCaLib::Target;

        Target(int width, int height);

        Surface mySurface;

     public:
        virtual ~Target();

        inline Surface & getSurface(void)
        {
            return mySurface;
        }

        inline JNIEnv * getEnv(void)
        {
            return getSurface().getEnv();
        }

     protected:
        virtual int GetWidth(void) const override;
        virtual int GetHeight(void) const override;
        virtual const void * GetPixelData(void) const override;
        virtual int GetLogicalWidth(void) const override;
        virtual DrawPtr Draw(void) override;

     private:
        SYS_DEFINE_CLASS_NAME("PaCaAndroid::Target");

    }; // class PaCaAndroid::Target

    class Path;

    class Draw: public PaCaLib::Draw
    {
        friend class PaCaAndroid::Target;
        friend class PaCaAndroid::Path;

     protected:
        Draw(PaCaAndroid::Target & target);

        virtual void Scale(float w, float h) override;
        virtual void SetLineWidth(float width) override;
        virtual void Move(float x, float y) override;
        virtual void Line(float x, float y) override;
        virtual void SetLineCap(PaCaLib::LineCap mode) override;
        virtual void SetColour(float r, float g, float b, float a) override;
        virtual void Rectangle(float x, float y, float w, float h) override;
        virtual void Arc(float xc, float yc, float r, float a1, float a2) override;
        virtual float DrawTextInternal(float x, float y, PaCaLib::TextMode mode, const char * text, float size, float offset, float aspect = 1.0) override;
        virtual void SetTextOutlineColour(float r, float g, float b, float a = 1.0) override;
        virtual void SetTextOutline(float outline) override;
        virtual void Paint(void) override;
        virtual void Paint(float alpha) override;
        virtual void Operator(PaCaLib::Oper op) override;
        virtual PathPtr NewPath(void) override;

        static inline JavaIface & GetJavaIface(void)
        {
            return PaCaAndroid::JavaIface::Get();
        }

        inline JNIEnv * getEnv(void)
        {
            return target.getSurface().getEnv();
        }

        inline JavaTargetPtr CreateTarget(JavaBitmapPtr bitmap)
        {
            SYS_DEBUG_MEMBER(DM_PACALIB);
            return GetJavaIface().CreateTarget(AndroidAccess::getJNIEnv(), bitmap);
        }

        PaCaAndroid::Target & target;

        JavaTargetPtr javaTarget;

     private:
        SYS_DEFINE_CLASS_NAME("PaCaAndroid::Draw");

    }; // class PaCaAndroid::Draw

    class Path: public PaCaLib::Path
    {
        friend class Draw;

     public:
        virtual ~Path();

     protected:
        Path(Draw & parent);

        static inline JavaIface & GetJavaIface(void)
        {
            return PaCaAndroid::JavaIface::Get();
        }

        virtual void Move(float x, float y) override;
        virtual void Line(float x, float y) override;
        virtual void Arc(float xc, float yc, float r, float a1, float a2) override;
        virtual void Close(void) override;
        virtual void Clear(void) override;
        virtual void SetLineWidth(float width) override;
        virtual void SetLineCap(PaCaLib::LineCap mode) override;
        virtual void Fill(void) override;
        virtual void Stroke(void) override;
        virtual void SetColour(float r, float g, float b, float a) override;

     protected:
        Draw & parent;

        struct MyJavaPath
        {
            MyJavaPath(JNIEnv * env);

            AndroidAccess::JClassPtr        path;
            AndroidAccess::JFuncVoidPtr     arc;

        }; // struct PaCaAndroid::Target::Path

        MyJavaPath path;

     private:
        SYS_DEFINE_CLASS_NAME("PaCaAndroid::Path");

    }; // class PaCaAndroid::Path

} // namespace PaCaAndroid

#endif /* __SRC_PACALIB_ANDROID_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
