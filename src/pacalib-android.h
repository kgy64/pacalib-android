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
        inline JavaBitmap(jobject obj, JNIEnv * env = AndroidAccess::getJNIEnv()):
            AndroidAccess::JObject(obj, env),
            pixel_data(nullptr)
        {
            SYS_DEBUG_MEMBER(DM_PACALIB);
            AndroidBitmap_lockPixels(env, get(), &pixel_data);
            ASSERT(pixel_data, "lockPixels() failed");
        }

     public:
        VIRTUAL_IF_DEBUG inline ~JavaBitmap()
        {
            SYS_DEBUG_MEMBER(DM_PACALIB);
            if (pixel_data) {
                AndroidBitmap_unlockPixels(env, get());
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

    class JavaString
    {
     public:
        inline JavaString(const char * text, JNIEnv * env = AndroidAccess::getJNIEnv()):
            myJNIenv(env),
            myString(myJNIenv->NewStringUTF(text))
        {
            SYS_DEBUG_MEMBER(DM_PACALIB);
        }

        inline ~JavaString()
        {
            SYS_DEBUG_MEMBER(DM_PACALIB);

            myJNIenv->DeleteLocalRef(myString);
        }

        inline const jstring & get(void) const
        {
            return myString;
        }

     private:
        SYS_DEFINE_CLASS_NAME("PaCaAndroid::JavaString");

        JNIEnv * myJNIenv;

        jstring myString;

    }; // class PaCaAndroid::JavaString

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
        void SetColour(JNIEnv * env, float r, float g, float b, float a);
        void DrawText(JNIEnv * env, const JavaBitmapPtr & bitmap, const char * text, float x, float y, int mode, float offset, int textColor, float textsize, int borderColor, float borderSize, float aspect);

     protected:
        struct MyJavaClasses
        {
            MyJavaClasses(void);

            AndroidAccess::JClassPtr        graphics;
            AndroidAccess::JFuncObjectPtr   create_bitmap;
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

        inline void SetColour(float r, float g, float b, float a)
        {
            drawColour.SetRGBA(r, g, b, a);
        }

        inline void SetColour(const PaCaLib::Colour & col)
        {
            drawColour.Set(col);
        }

        inline void SetTextOutlineColour(float r, float g, float b, float a = 1.0)
        {
            textOutlineColour.SetRGBA(r, g, b, a);
        }

        inline void SetTextOutline(float outline)
        {
            textOutlineWidth = outline;
        }

        void DrawText(float x, float y, const char * text, int mode, float offset, float size, float aspect);

     protected:
        inline JavaIface & GetJavaIface(void)
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

        JColour drawColour;

        JColour textOutlineColour;

        float textOutlineWidth;

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

     protected:
        virtual int GetWidth(void) const override;
        virtual int GetHeight(void) const override;
        virtual const void * GetPixelData(void) const override;
        virtual int GetLogicalWidth(void) const override;
        virtual DrawPtr Draw(void) override;

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

        inline JNIEnv * getEnv(void)
        {
            return target.getSurface().getEnv();
        }

        PaCaAndroid::Target & target;

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

        inline JavaIface & GetJavaIface(void)
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
