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

namespace PaCaAndroid
{
    class Surface: public PaCaLib::Surface
    {
     public:
        Surface(int width, int height);

        virtual void * getData(void) override;
        virtual const void * getData(void) const override;
        virtual int getWidth(void) const override;
        virtual int getPhysicalWidth(void) const override;
        virtual int getHeight(void) const override;

     private:
        SYS_DEFINE_CLASS_NAME("PaCaAndroid::Surface");

    }; // class Surface;

    class Target: public PaCaLib::Target
    {
     public:
        Target(int width, int height);

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
        virtual void SetLineCap(cairo_line_cap_t mode) override;
        virtual void SetColour(double r, double g, double b) override;
        virtual void SetColour(double r, double g, double b, double a) override;
        virtual void SetColour(const Colour & col) override;
        virtual void Rectangle(double x, double y, double w, double h) override;
        virtual void Arc(double xc, double yc, double r, double a1, double a2) override;
        virtual void NewPath(void) override;
        virtual void NewSubPath(void) override;
        virtual void ClosePath(void) override;
        virtual double DrawText(double x, double y, TextMode mode, const char * text, double size, double aspect = 1.0) override;
        virtual void SetTextOutlineColour(double r, double g, double b, double a = 1.0) override;
        virtual void SetTextOutline(double outline) override;
        virtual void Paint(void) override;
        virtual void Paint(double alpha) override;

     private:
        SYS_DEFINE_CLASS_NAME("PaCaAndroid::Target");

    }; // class Target

} // namespace PaCaLib

#endif /* __SRC_PACALIB_ANDROID_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
