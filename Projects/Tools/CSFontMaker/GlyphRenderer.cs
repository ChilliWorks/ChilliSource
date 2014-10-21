/**
 * GlyphRenderer.cs
 * Chilli Source
 * Created by Stuart McGaw on 01/12/2011.
 * 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2011 Tag Games Limited
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Drawing.Text;
using System.Drawing.Imaging;

namespace CSFontMaker
{
    public class GlyphRenderer
    {

        #region Properties
        public Font Font
        {
            get;
            set;
        }
        public Color TextColour
        {
            get;
            set;
        }

        public bool DrawShadow
        {
            get;
            set;
        }
        public Color ShadowColour
        {
            get;
            set;
        }
        public int ShadowXOffset { get; set; }
        public int ShadowYOffset { get; set; }

        public bool DrawOutline
        {
            get;
            set;
        }
        public Color OutlineColour
        {
            get;
            set;
        }
        public int OutlineThickness
        {
            get;
            set;
        }

        public bool HighQualityRendering{
            get;set;
        }

        #endregion

        static public int MeasureDisplayStringWidth(Graphics graphics, string text,
                                            Font font)
        {
            StringFormat format = new StringFormat();
            RectangleF rect = new RectangleF(0, 0, 1000, 1000);
            CharacterRange[] ranges = { new System.Drawing.CharacterRange(0, text.Length) };
            Region[] regions = new System.Drawing.Region[1];

            format.SetMeasurableCharacterRanges(ranges);

            regions = graphics.MeasureCharacterRanges(text, font, rect, format);
            rect = regions[0].GetBounds(graphics);

            return (int)(rect.Right + 1.0f);
        }

        public List<Bitmap> RenderGlyphs(string characters){
            
            Graphics measurer = Graphics.FromImage(new Bitmap(1000, 1000, System.Drawing.Imaging.PixelFormat.Format32bppArgb));

            Brush brush = new SolidBrush(TextColour);
            Brush shadowBrush = null;
            
            if (DrawShadow){
                shadowBrush = new SolidBrush(ShadowColour);
            } else if (DrawOutline){
                shadowBrush = new SolidBrush(OutlineColour);
            }
            
           
            List<Bitmap> glyphs = new List<Bitmap>();
            foreach (char c in characters)
            {

                SizeF glyphSize = measurer.MeasureString(c.ToString(), Font);
                int glyphWidth = MeasureDisplayStringWidth(measurer, c.ToString(), Font);
                int glyphHeight = Font.Height;

                if (DrawShadow)
                {
                    glyphHeight += Math.Abs(ShadowYOffset);
                    glyphWidth += Math.Abs(ShadowXOffset);
                }
                else if (DrawOutline)
                {
                    glyphHeight += 2 * OutlineThickness;
                    glyphWidth += 2 * OutlineThickness;
                }

                Bitmap glyphMap = new Bitmap(glyphWidth, glyphHeight);

                using (Graphics canvas = Graphics.FromImage(glyphMap)) {
                   ApplyGraphicsQualityOptions(canvas,HighQualityRendering);
                   if (DrawShadow)
                   {
                       PointF shadowOffset = new PointF(Math.Max(ShadowXOffset, 0), Math.Max(ShadowYOffset, 0));
                       canvas.DrawString(c.ToString(), Font, shadowBrush, shadowOffset);
                       PointF glyphOffset = new PointF(-Math.Min(ShadowXOffset, 0), -Math.Min(ShadowYOffset, 0));
                       canvas.DrawString(c.ToString(), Font, brush, glyphOffset);
                   }
                   else if (DrawOutline)
                   {
                       ApplyGraphicsQualityOptions(canvas, true); //AA with outlines is a blurry mess.
                       for (int nPixelThickness = 1; nPixelThickness <= OutlineThickness; nPixelThickness++)
                       {
                           PointF [] points = new PointF[8]{
                               new PointF(OutlineThickness + nPixelThickness, OutlineThickness + nPixelThickness ),   
                               new PointF(OutlineThickness + nPixelThickness, OutlineThickness),
                               new PointF(OutlineThickness + nPixelThickness , OutlineThickness - nPixelThickness ),
                               new PointF(OutlineThickness, OutlineThickness - nPixelThickness ),
                               new PointF(OutlineThickness - nPixelThickness , OutlineThickness - nPixelThickness ),
                               new PointF(OutlineThickness - nPixelThickness , OutlineThickness),
                               new PointF(OutlineThickness - nPixelThickness , OutlineThickness + nPixelThickness ),
                               new PointF(OutlineThickness, OutlineThickness + nPixelThickness )
                           };
                           foreach (PointF point in points)
                            canvas.DrawString(c.ToString(), Font, shadowBrush,  point);
                       }
                       canvas.DrawString(c.ToString(), Font, brush, new PointF(OutlineThickness, OutlineThickness));
                   }
                   else
                   {
                       canvas.DrawString(c.ToString(), Font, brush, new PointF(0, 1));
                   }
                   
                }
                glyphs.Add(glyphMap);
            }

            return glyphs;
        }
        private void ApplyGraphicsQualityOptions(Graphics canvas, bool highQuality)
        {
            if (highQuality)
            {
                canvas.TextRenderingHint = TextRenderingHint.AntiAliasGridFit;
                canvas.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.AntiAlias;
                canvas.CompositingQuality = System.Drawing.Drawing2D.CompositingQuality.HighQuality;
            }
            else
            {
                canvas.TextRenderingHint = TextRenderingHint.SingleBitPerPixelGridFit;
                canvas.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.HighSpeed;
                canvas.CompositingQuality = System.Drawing.Drawing2D.CompositingQuality.HighSpeed;
            }
        }
    }
}
