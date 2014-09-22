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

namespace PaCaAndroid
{
    class JavaBitmap;
    typedef MEM::shared_ptr<JavaBitmap> JavaBitmapPtr;

    class JavaBitmap
    {
        inline JavaBitmap(AndroidAccess::JGlobalRefPtr obj, JNIEnv * env = AndroidAccess::getJNIEnv()):
            env(env),
            obj(obj),
            pixel_data(nullptr)
        {
            SYS_DEBUG_MEMBER(DM_PACALIB);
            AndroidBitmap_lockPixels(env, obj->get(), &pixel_data);
            ASSERT(pixel_data, "lockPixels() failed");
        }

     public:
        VIRTUAL_IF_DEBUG inline ~JavaBitmap()
        {
            SYS_DEBUG_MEMBER(DM_PACALIB);
            if (pixel_data) {
                AndroidBitmap_unlockPixels(env, obj->get());
            }
        }

        static inline JavaBitmapPtr Create(AndroidAccess::JGlobalRefPtr obj, JNIEnv * env = AndroidAccess::getJNIEnv())
        {
            return JavaBitmapPtr(new JavaBitmap(obj, env));
        }

        inline jobject get(void) const
        {
            return obj->get();
        }

        inline void * getPixelData(void)
        {
            return pixel_data;
        }

     protected:
        JNIEnv * env;

        AndroidAccess::JGlobalRefPtr obj;

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
        inline JavaIface(void)
        {
            SYS_DEBUG_MEMBER(DM_PACALIB);
        }

        VIRTUAL_IF_DEBUG inline ~JavaIface()
        {
            SYS_DEBUG_MEMBER(DM_PACALIB);
        }

     public:
        static inline JavaIface & Get()
        {
            // Note: this class must be used only in one thread, that's why no guard is necessary here.
            //       System calls will not work if it is used from more threads.
            if (!myself) {
                myself = new JavaIface();
            }
            return *myself;
        }

        JavaBitmapPtr CreateBitmap(int32_t width, int32_t height);
        void SetColour(float r, float g, float b, float a);
        void DrawText(const JavaBitmapPtr & bitmap, const char * text, float size);

     protected:
        struct MyJavaClasses
        {
            MyJavaClasses(void);
            MyJavaClasses(JNIEnv * env);

            AndroidAccess::JClassPtr        graphics;
            AndroidAccess::JFuncObjectPtr   create_bitmap;
            AndroidAccess::JFuncVoidPtr     set_colour;
            AndroidAccess::JFuncVoidPtr     draw_text;

        }; // struct MyJavaClasses

        MyJavaClasses classes;

     private:
        SYS_DEFINE_CLASS_NAME("PaCaAndroid::JavaIface");

        static JavaIface * myself;

    }; // class JavaIface

    class Surface
    {
     public:
        Surface(int width, int height);
        VIRTUAL_IF_DEBUG ~Surface();

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
            GetJavaIface().SetColour(r, g, b, a);
        }

        inline void DrawText(const char * text, float size)
        {
            SYS_DEBUG_MEMBER(DM_PACALIB);
            SYS_DEBUG(DL_INFO2, "Drawing text '" << text << "', size=" << size);
            GetJavaIface().DrawText(bitmap, text, size);
        }

     protected:
        inline static JavaIface & GetJavaIface(void)
        {
            return PaCaAndroid::JavaIface::Get();
        }

        inline JavaBitmapPtr CreateBitmap(int width, int height)
        {
            SYS_DEBUG_MEMBER(DM_PACALIB);
            SYS_DEBUG(DL_INFO2, "Creating bitmap " << width << "x" << height);
            return GetJavaIface().CreateBitmap(width, height);
        }

        struct argb {
            uint8_t b, g, r, a;
        };

        int myWidth;

        int myHeight;

        JavaBitmapPtr bitmap;

     private:
        SYS_DEFINE_CLASS_NAME("PaCaAndroid::Surface");

    }; // class Surface;

    class Target: public PaCaLib::Target
    {
        Surface mySurface;

     public:
        Target(int width, int height);
        virtual ~Target();

        virtual int GetWidth(void) const override;
        virtual int GetHeight(void) const override;
        virtual const void * GetPixelData(void) const override;
        virtual int GetLogicalWidth(void) const override;
        virtual void Scale(double w, double h) override;
        virtual void Stroke(void) override;
        virtual void Fill(void) override;
        virtual void FillPreserve(void) override;
        virtual void SetLineWidth(double width) override;
        virtual void Move(double x, double y) override;
        virtual void Line(double x, double y) override;
        virtual void SetLineCap(PaCaLib::LineCap mode) override;
        virtual void SetColour(double r, double g, double b) override;
        virtual void SetColour(double r, double g, double b, double a) override;
        virtual void SetColour(const PaCaLib::Colour & col) override;
        virtual void Rectangle(double x, double y, double w, double h) override;
        virtual void Arc(double xc, double yc, double r, double a1, double a2) override;
        virtual void NewPath(void) override;
        virtual void NewSubPath(void) override;
        virtual void ClosePath(void) override;
        virtual double DrawText(double x, double y, PaCaLib::TextMode mode, const char * text, double size, double aspect = 1.0) override;
        virtual void SetTextOutlineColour(double r, double g, double b, double a = 1.0) override;
        virtual void SetTextOutline(double outline) override;
        virtual void Paint(void) override;
        virtual void Paint(double alpha) override;
        virtual void Operator(PaCaLib::Oper op) override;

     private:
        SYS_DEFINE_CLASS_NAME("PaCaAndroid::Target");

    }; // class Target

} // namespace PaCaLib

#endif /* __SRC_PACALIB_ANDROID_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
