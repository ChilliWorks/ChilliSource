using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace FontTool
{
    /// <summary>
    /// Helper for arranging many small sprites into a single larger sheet.
    /// </summary>
    public static class SpritePacker
    {
        static int[] supportedPowersOf2 = {1,2,4,8,16,32,64,128,256,512,1024,2048,4096};
        static int NextPowerOf2(int n) {

            
            foreach (int i in supportedPowersOf2){
                if (i >= n)
                    return i;
            }
           

            return -1;
        }

        /// <summary>
        /// Packs a list of sprites into a single big texture,
        /// recording where each one was stored.
        /// </summary>
        public static Bitmap PackSprites(IList<Bitmap> sourceSprites,
                                                ICollection<Rectangle> outputSprites) {

            // Build up a list of all the sprites needing to be arranged.
            List<ArrangedSprite> sprites = new List<ArrangedSprite>();

            for (int i = 0; i < sourceSprites.Count; i++) {
                ArrangedSprite sprite = new ArrangedSprite();

                // Include a single pixel padding around each sprite, to avoid
                // filtering problems if the sprite is scaled or rotated.
                sprite.Width = sourceSprites[i].Width + 2;
                sprite.Height = sourceSprites[i].Height + 2;

                sprite.Index = i;

                sprites.Add(sprite);
            }

            // Sort so the largest sprites get arranged first.
            sprites.Sort(CompareSpriteSizes);

            // Work out how big the output bitmap should be.
            int outputWidth = GuessOutputWidth(sprites);
            int outputHeight = 0;
            int totalSpriteSize = 0;

            // Choose positions for each sprite, one at a time.
            for (int i = 0; i < sprites.Count; i++) {
                PositionSprite(sprites, i, outputWidth);

                outputHeight = Math.Max(outputHeight, sprites[i].Y + sprites[i].Height);
                outputHeight = NextPowerOf2(outputHeight);

                totalSpriteSize += sprites[i].Width * sprites[i].Height;
            }

            // Sort the sprites back into index order.
            sprites.Sort(CompareSpriteIndices);

            return CopySpritesToOutput(sprites, sourceSprites, outputSprites,
                                       outputWidth, outputHeight);
        }


        /// <summary>
        /// Once the arranging is complete, copies the bitmap data for each
        /// sprite to its chosen position in the single larger output bitmap.
        /// </summary>
        static Bitmap CopySpritesToOutput(List<ArrangedSprite> sprites,
                                                 IList<Bitmap> sourceSprites,
                                                 ICollection<Rectangle> outputSprites,
                                                 int width, int height) {
          
            Bitmap output = new Bitmap(width, height, System.Drawing.Imaging.PixelFormat.Format32bppArgb);
            using (Graphics canvas = Graphics.FromImage(output)) {
                canvas.CompositingQuality = System.Drawing.Drawing2D.CompositingQuality.HighQuality;
                
                foreach (ArrangedSprite sprite in sprites) {
                    Bitmap source = sourceSprites[sprite.Index];

                    int x = sprite.X;
                    int y = sprite.Y;

                    int w = source.Width;
                    int h = source.Height;
                   

                    TextureBrush brush = new TextureBrush(source);
                    brush.TranslateTransform(x + 1, y + 1);
                    
                    canvas.FillRectangle(brush,new Rectangle(x + 1, y + 1, w, h));

                    // Remember where we placed this sprite.
                    outputSprites.Add(new Rectangle(x + 1, y + 1, w, h));
                }
            }
            return output;
        }


        /// <summary>
        /// Internal helper class keeps track of a sprite while it is being arranged.
        /// </summary>
        class ArrangedSprite
        {
            public int Index;

            public int X;
            public int Y;

            public int Width;
            public int Height;
        }


        /// <summary>
        /// Works out where to position a single sprite.
        /// </summary>
        static void PositionSprite(List<ArrangedSprite> sprites,
                                   int index, int outputWidth) {
            int x = 0;
            int y = 0;

            while (true) {
                // Is this position free for us to use?
                int intersects = FindIntersectingSprite(sprites, index, x, y);

                if (intersects < 0) {
                    sprites[index].X = x;
                    sprites[index].Y = y;

                    return;
                }

                // Skip past the existing sprite that we collided with.
                x = sprites[intersects].X + sprites[intersects].Width;

                // If we ran out of room to move to the right,
                // try the next line down instead.
                if (x + sprites[index].Width > outputWidth) {
                    x = 0;
                    y++;
                }
            }
        }


        /// <summary>
        /// Checks if a proposed sprite position collides with anything
        /// that we already arranged.
        /// </summary>
        static int FindIntersectingSprite(List<ArrangedSprite> sprites,
                                          int index, int x, int y) {
            int w = sprites[index].Width;
            int h = sprites[index].Height;

            for (int i = 0; i < index; i++) {
                if (sprites[i].X >= x + w)
                    continue;

                if (sprites[i].X + sprites[i].Width <= x)
                    continue;

                if (sprites[i].Y >= y + h)
                    continue;

                if (sprites[i].Y + sprites[i].Height <= y)
                    continue;

                return i;
            }

            return -1;
        }


        /// <summary>
        /// Comparison function for sorting sprites by size.
        /// </summary>
        static int CompareSpriteSizes(ArrangedSprite a, ArrangedSprite b) {
            int aSize = a.Height * 1024 + a.Width;
            int bSize = b.Height * 1024 + b.Width;

            return bSize.CompareTo(aSize);
        }


        /// <summary>
        /// Comparison function for sorting sprites by their original indices.
        /// </summary>
        static int CompareSpriteIndices(ArrangedSprite a, ArrangedSprite b) {
            return a.Index.CompareTo(b.Index);
        }


        /// <summary>
        /// Heuristic guesses what might be a good output width for a list of sprites.
        /// </summary>
        static int GuessOutputWidth(List<ArrangedSprite> sprites) {
            // Gather the widths of all our sprites into a temporary list.
            List<int> widths = new List<int>();

            foreach (ArrangedSprite sprite in sprites) {
                widths.Add(sprite.Width);
            }

            // Sort the widths into ascending order.
            widths.Sort();

            // Extract the maximum and median widths.
            int maxWidth = widths[widths.Count - 1];
            int medianWidth = widths[widths.Count / 2];

            // Heuristic assumes an NxN grid of median sized sprites.
            int width = medianWidth * (int)Math.Round(Math.Sqrt(sprites.Count));

            // Make sure we never choose anything smaller than our largest sprite.
            return NextPowerOf2(Math.Max(width, maxWidth));
        }
    }
}


