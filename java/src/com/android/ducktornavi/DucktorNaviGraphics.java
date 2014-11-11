/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     Ducktor Navi - the Android port
 * Purpose:     Graphic Interface to C++ side
 * Author:      György Kövesdi <kgy@teledigit.eu>
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

package com.android.ducktornavi;

import java.lang.Math;
// import android.util.Log;
import android.graphics.Bitmap;
import android.graphics.Paint;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Rect;
import android.graphics.RectF;
import android.graphics.Path;
import android.graphics.Paint.Cap;
import android.graphics.Paint.Style;
import android.graphics.Paint.FontMetrics;
import android.graphics.Xfermode;
import android.graphics.PorterDuffXfermode;
import android.graphics.PorterDuff;
import android.graphics.PorterDuff.Mode;
import android.graphics.Matrix;

public class DucktorNaviGraphics {

    private static final String TAG = "Ducktor Navi Graphics";

    /// Graphical target for a given bitmap
    /*! This class can draw graphical primitives to the given bitmap. */
    public class Draw {

        /*! \param  my_bitmap   The target bitmap
         *  \note   The colours and sizes must be set, because they have invisible default values. */
        public Draw(Bitmap my_bitmap) {
            bitmap = my_bitmap;
            w = bitmap.getWidth();
            h = bitmap.getHeight();
            canvas = new Canvas(bitmap);
        }

        public void SetScale(float vs, float hs) {
            canvas.scale(vs, hs);
        }

        /// Fill the whole bitmap with the current colour
        public void DrawFill() {
            bitmap.eraseColor(colour);
        }

        public void DrawPath(android.graphics.Path path, int mode) {
            switch (mode) {
                case 0:
                    paint.setStyle(Paint.Style.STROKE);
                    paint.setStrokeCap(strokeCap);
                    paint.setStrokeWidth(lineWidth);
                break;
                case 1:
                    paint.setStyle(Paint.Style.FILL);
                break;
            }

            paint.setColor(colour);

            canvas.drawPath(path, paint);
        }

        public void DrawArc(Path path, float left, float top, float right, float bottom, float start, float sweep) {
            path.arcTo(new RectF(left, top, right, bottom), start, sweep);
        }

        public void SetColorBlendMode(int mode) {
            Xfermode filter = null;
            switch (mode) {
                case 0:
                    // leave the pointer null: clear the filter
                break;
                case 1:
                    // ADD:
                    filter = new PorterDuffXfermode(PorterDuff.Mode.ADD);
                break;
                case 2:
                    // SUBTRACT:
                    filter = new PorterDuffXfermode(PorterDuff.Mode.SRC_OVER);
                break;
                case 3:
                    // OVERWRITE:
                    filter = new PorterDuffXfermode(PorterDuff.Mode.SRC);
                break;
            }
            paint.setXfermode(filter);
        }

        public void SetLineCap(int mode) {
            switch (mode) {
                case 0:
                    strokeCap = Paint.Cap.BUTT;
                break;
                case 1:
                    strokeCap = Paint.Cap.SQUARE;
                break;
                case 2:
                    strokeCap = Paint.Cap.ROUND;
                break;
            }
        }

        public void SetLineWidth(float width) {
            lineWidth = width * h * 0.5f;
        }

        public void SetBorderColour(int colour) {
            borderColor = colour;
        }

        public void SetBorderSize(float size) {
            borderSize = size * h;
        }

        public void SetColour(int col) {
            colour = col;
        }

        /// Draw a text into the target bitmap
        /*! \param      text        The text to be printed.
         *  \param      x           The horizontal position of the target.<br>
         *                          The position is handled in OpenGL-way: the -1.0 means leftmost, and +1.0 means rightmost.
         *  \param      y           The vertical position of the target.<br>
         *                          The position is handled in OpenGL-way: the -1.0 means bottommost, and +1.0 means topmost.
         *  \param      mode        Text justify, it can be:
         *                          - 0:    Left justified
         *                          - 1:    Center justified
         *                          - 2:    Right justified
         *  \param      offset      Vertical offset of the current line.<br>
         *                          It is used to display multi-line text. Its unit is the text line height, negative value moves up.
         *  \param      textsize    The relative vertical size of the text. Its unit is the vertical size of the target bitmap.
         *  \param      aspect      The target aspect ratio. It modifies the horizontal size of the text: if it is 1.0
         *                          (the default value), then one symbol of size 1.0 fills the whole bitmap (but still
         *                          readable). Wider bitmap needs value > 1 here.
         *  \param      rotate      Rotation of the whole text (in radians).
         *  \param      shear_x     Horizontal shear: positive value means tilt right.
         *  \param      shear_y     Vertical shear: positive value means rise right.
         *  \retval     float       The horizontal size of the printed text is returned, in the unit of the target bitmap height.
         *                          This value is used to position within OpenGL object, so half height means 1.0 for compatibility
         *                          reasons. */
        public float DrawText(String text, float x, float y, int mode, float offset, float textsize, float aspect, float rotate, float shear_x, float shear_y) {

            // Log.i(TAG, "DrawText(\"" + text + "\", x=" + x + ", y=" + y + ", mode=" + mode + ", off=" + offset + ", size=" + textsize + ", aspect=" + aspect + ", rot=" + rotate + ", sx=" + shear_x + ", sy=" + shear_y + ")");

            // General size correction factor for glyphs: (Heuristic value)
            final float correction = 0.65f;

            final float multiplier = aspect * w / h;
            final float hor  = multiplier;
            final float vert = correction;

            x = w * (1.0f + x) / (2.0f * multiplier);
            y = h * (1.0f - y) / 2.0f;  // -1.0f => h; +1.0f => 0.0

            paint.setTextSize(h * textsize);

            canvas.save(Canvas.MATRIX_SAVE_FLAG);
            canvas.scale(hor, -1.0f);
            // Log.i(TAG, "DrawText(): scale(" + hor + ", " + -vert + ")");

            Paint.FontMetrics metrics = paint.getFontMetrics();
            Rect bounds = new Rect();
            paint.getTextBounds(text, 0, text.length(), bounds);
            float width = bounds.right - bounds.left;
            float height = correction * (metrics.bottom - metrics.top);  // negative factor

            // Baseline: this is the offset of the bottom part (positive value here), must be moved
            // upwards to see the bottom part.
            float baseline = metrics.bottom;

            switch (mode) {
                case 0:
                    paint.setTextAlign(Paint.Align.LEFT);
                break;
                case 1:
                    paint.setTextAlign(Paint.Align.CENTER);
                break;
                case 2:
                    paint.setTextAlign(Paint.Align.RIGHT);
                break;
            }

            y = baseline - y;

            // Log.i(TAG, "DrawText(\"" + text + "\"): x=" + x + ", y=" + y + " (w=" + w + ", h=" + h + ")");

            if (rotate != 0.0f) {
                canvas.rotate(rotate * (180.0f / (float)Math.PI), x, y - 0.3f * height);
            }

            if (shear_x != 0.0f || shear_y != 0.0f) {
                Matrix m = new Matrix();
                m.preSkew(-shear_x, -shear_y, x, y - 0.3f * height);
                canvas.concat(m);
            }

            y -= height * offset;

            if (borderSize > 0.0f) {
                paint.setColor(colour);
                paint.setStyle(Paint.Style.FILL);
                canvas.drawText(text, x, y, paint);
                paint.setColor(borderColor);
                paint.setStyle(Paint.Style.STROKE);
                paint.setStrokeWidth(borderSize);
                canvas.drawText(text, x, y, paint);
            } else if (borderSize < 0.0f) {
                paint.setColor(borderColor);
                paint.setStyle(Paint.Style.STROKE);
                paint.setStrokeWidth(-borderSize);
                canvas.drawText(text, x, y, paint);
            } else { // 0.0
                paint.setColor(colour);
                paint.setStyle(Paint.Style.FILL);
                canvas.drawText(text, x, y, paint);
            }

            canvas.restore();

            float result = 2.0f * aspect / h;
            if (width != 0.0f) {
                result *= width;
            } else {
                result *= 0.3 * height; // Size of whitespace characters
            }

            return result;

        } // DucktorNaviGraphics.Draw.DrawText()

        /* ************** Member Variables Below: **************** */

        private Bitmap bitmap;

        private Canvas canvas;

        private Paint paint = new Paint(Paint.ANTI_ALIAS_FLAG | Paint.DITHER_FLAG | Paint.LINEAR_TEXT_FLAG /* | Paint.SUBPIXEL_TEXT_FLAG*/);

        /// The width of the target bitmap
        private float w;

        /// The height of the target bitmap
        private float h;

        private Paint.Cap strokeCap = Paint.Cap.ROUND;

        private float lineWidth;

        private float borderSize;

        private int colour;

        private int borderColor;

    } // class DucktorNaviGraphics.Draw

    public Draw CreateDraw(Bitmap bitmap) {
        return new Draw(bitmap);
    }

    public Path CreatePath() {
        return new Path();
    }

    public Bitmap CreateBitmap(int width, int height, int mode) {
        Bitmap.Config format = Bitmap.Config.ARGB_8888;
        switch (mode) {
            case 1:
                format = Bitmap.Config.ARGB_8888;
            break;
            case 2:
                format = Bitmap.Config.RGB_565;
            break;
        }
        return Bitmap.createBitmap(width, height, format);
    }

} // class DucktorNaviGraphics

// * * * * * * * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * * * * *
