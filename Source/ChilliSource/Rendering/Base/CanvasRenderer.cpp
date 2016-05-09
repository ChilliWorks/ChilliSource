//
//  CanvasRenderer.cpp
//  Chilli Source
//  Created by Scott Downie on 12/01/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#include <ChilliSource/Rendering/Base/CanvasRenderer.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/ColourUtils.h>
#include <ChilliSource/Core/Math/MathUtils.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Core/String/UTF8StringUtils.h>
#include <ChilliSource/Rendering/Font/Font.h>
#include <ChilliSource/Rendering/Material/Material.h>
#include <ChilliSource/Rendering/Material/MaterialFactory.h>
#include <ChilliSource/Rendering/Texture/Texture.h>
#include <ChilliSource/UI/Base/Canvas.h>

#include <algorithm>

namespace ChilliSource
{
    namespace
    {
        const f32 k_maxAutoScaleIterations = 10.0f;//Max number of recursions to find the correct scale
        const f32 k_autoScaleTolerance = 0.01f;//Min difference in max/min scaling to warrant further recursion for AutoScaled text
        
        //------------------------------------------------------
        /// Converts a 2D transformation matrix to a 3D
        /// Transformation matrix. This will only work for
        /// non-projective transforms.
        ///
        /// @author S Downie
        ///
        /// @param The 2D Transform.
        ///
        /// @return The 3D Transform.
        //------------------------------------------------------
        template <typename TType> GenericMatrix4<TType> Convert2DTransformTo3D(const GenericMatrix3<TType>& in_transform)
        {
            return GenericMatrix4<TType>(
                in_transform.m[0], in_transform.m[1], in_transform.m[2], 0,
                in_transform.m[3], in_transform.m[4], in_transform.m[5], 0,
                0, 0, 1, 0,
                in_transform.m[6], in_transform.m[7], in_transform.m[8], 1);
        }
        //----------------------------------------------------------------------------
        /// Get the width of the character
        ///
        /// @author S Downie
        ///
        /// @param Character
        /// @param Font
        /// @param The absolute character spacing offset.
        /// @param The text scale factor.
        //----------------------------------------------------------------------------
        f32 GetCharacterWidth(UTF8Char in_character, const FontCSPtr& in_font, f32 in_absCharSpacingOffset, f32 in_textScale)
        {
            Font::CharacterInfo charInfo;
            if(in_font->TryGetCharacterInfo(in_character, charInfo) == true)
            {
                return (charInfo.m_advance + in_absCharSpacingOffset) * in_textScale;
            }

            return 0.0f;
        }
        //----------------------------------------------------------------------------
        /// @author S Downie
        ///
        /// @param Character
        ///
        /// @return Whether the character can safely be line broken on.
        //----------------------------------------------------------------------------
        bool IsBreakableCharacter(UTF8Char in_character)
        {
            return in_character == ' ' || in_character == '\t' || in_character == '\n' || in_character == '-';
        }
        //----------------------------------------------------------------------------
        /// Calculate the distance in text space to the next 'breakable' character
        ///
        /// @author S Downie
        ///
        /// @param Iterator pointing to start
        /// @param Iterator pointing to end
        /// @param Font
        /// @param The absolute character spacing offset.
        /// @param The text scale factor.
        //----------------------------------------------------------------------------
        f32 CalculateDistanceToNextBreak(std::string::const_iterator in_itStart, std::string::const_iterator in_itEnd, const FontCSPtr& in_font, f32 in_absCharSpacingOffset, f32 in_textScale)
        {
            f32 totalWidth = 0.0f;
            
            while(in_itStart < in_itEnd)
            {
                auto nextCharacter = UTF8StringUtils::Next(in_itStart);
                if (IsBreakableCharacter(nextCharacter) == true)
                {
                    break;
                }
                
                totalWidth += GetCharacterWidth(nextCharacter, in_font, in_absCharSpacingOffset, in_textScale);
            }

            return totalWidth;
        }
        //----------------------------------------------------------------------------
        /// Split the given text into lines based on any '\n' characters. The newline
        /// characters do not appear in the returned lines
        ///
        /// @author S Downie
        ///
        /// @param Text (UTF-8)
        /// @param [Out] Array of lines split by '\n'
        //----------------------------------------------------------------------------
        void SplitByNewLine(const std::string& in_text, std::vector<std::string>& out_lines)
        {
            auto it = in_text.begin();
            std::string line;
            while(it < in_text.end())
            {
                auto character = UTF8StringUtils::Next(it);

                if(character != '\n')
                {
                    UTF8StringUtils::Append(character, line);
                }
                else
                {
                    out_lines.push_back(line);
                    line.clear();
                }
            }

            if(line.size() > 0)
            {
                out_lines.push_back(line);
            }
        }
        //----------------------------------------------------------------------------
        /// Split the given text into lines based on the constrained bounds.
        /// Splits will prefer to happen on breakable whitespace characters but
        /// will split mid-word if neccessary.
        ///
        /// @author S Downie
        ///
        /// @param Text (UTF-8)
        /// @param Font
        /// @param Text scale
        /// @param The absolute character spacing offset.
        /// @param Bounds
        /// @param [Out] Array of lines split to fit in bounds
        //----------------------------------------------------------------------------
        void SplitByBounds(const std::string& in_text, const FontCSPtr& in_font, f32 in_absCharSpacingOffset, f32 in_textScale, const Vector2& in_bounds, CanvasRenderer::WrappedText& out_lines)
        {
            f32 maxLineWidth = in_bounds.x;

            auto it = in_text.begin();
            std::string line;
            f32 currentLineWidth = 0.0f;

            while(it < in_text.end())
            {
                auto character = UTF8StringUtils::Next(it);
                f32 characterWidth = GetCharacterWidth(character, in_font, in_absCharSpacingOffset, in_textScale);
                
                //If we come across a character on which we can wrap we need
                //to check ahead to see if the next space is within the bounds or
                //whether we need to wrap now
                if(IsBreakableCharacter(character) == true)
                {
                    f32 nextBreakWidth = currentLineWidth + characterWidth + CalculateDistanceToNextBreak(it, in_text.end(), in_font, in_absCharSpacingOffset, in_textScale);

                    if(nextBreakWidth >= maxLineWidth && line.size() > 0)
                    {
                        out_lines.push_back(line);
                        line.clear();
                        currentLineWidth = 0.0f;
                        
                        //since we've broken mid line we don't want to start the next line with
                        //whitespace. To ensure this we want to jump the cursor forward to find
                        //the next non-breakable character.
                        auto nextIt = it;
                        UTF8Char nextCharacter = UTF8StringUtils::Next(nextIt);
                        while (IsBreakableCharacter(nextCharacter) == true)
                        {
                            it = nextIt;
                            nextCharacter = UTF8StringUtils::Next(nextIt);
                        }
                        
                        continue;
                    }
                }
                
                //If this is not a breakable character, but it is still beyond the bounds
                //then wrap anyway, even though we are mid word.
                else if((currentLineWidth + characterWidth) >= maxLineWidth)
                {
                    out_lines.push_back(line);
                    line.clear();
                    currentLineWidth = 0.0f;
                }
                
                currentLineWidth += characterWidth;
                UTF8StringUtils::Append(character, line);
            }

            if(line.size() > 0)
            {
                out_lines.push_back(line);
            }
        }
        //----------------------------------------------------------------------------
        /// Create the data required to display a character. This includes
        /// the size, UV and position.
        ///
        /// @author S Downie
        ///
        /// @param Character
        /// @param Font
        /// @param Current cursor X pos
        /// @param Current cursor Y pos
        /// @param Text scale
        /// @param The character spacing offset in pixels.
        ///
        /// @return Display characer info
        //----------------------------------------------------------------------------
        CanvasRenderer::DisplayCharacterInfo BuildCharacter(UTF8Char in_character, const FontCSPtr& in_font, f32 in_cursorX, f32 in_cursorY, f32 in_textScale, f32 in_absCharSpacingOffset)
        {
            CanvasRenderer::DisplayCharacterInfo result;

            Font::CharacterInfo info;
            if(in_font->TryGetCharacterInfo(in_character, info) == true)
            {
                result.m_UVs = info.m_UVs;
                result.m_advance = ((info.m_advance + in_absCharSpacingOffset) * in_textScale);
                result.m_packedImageSize = info.m_size * in_textScale;
                result.m_position.x = in_cursorX + (info.m_offset.x - info.m_origin) * in_textScale;
                result.m_position.y = in_cursorY - (info.m_offset.y - in_font->GetVerticalPadding()) * in_textScale;
            }
            else
            {
                CS_LOG_ERROR("Unknown character not provided by font: " + in_font->GetName());
            }

            return result;
        }
        //----------------------------------------------------------------------------
        /// The text by default is left justfied. If another justification
        /// is required this function will update the character positions of a given line
        ///
        /// @author S Downie
        ///
        /// @param Horizontal justification
        /// @param Bounds width
        /// @param Index of the first character in a line
        /// @param Index of the last character in a line
        /// @param Line width in text space.
        /// @param [In/Out] List of display character infos that will be manipulated.
        ///         These are the charcters for all lines.
        //----------------------------------------------------------------------------
        void ApplyHorizontalTextJustifications(HorizontalTextJustification in_horizontal, f32 in_boundsWidth, u32 in_lineStartIdx, u32 in_lineEndIdx, f32 in_lineWidth,
                                               std::vector<CanvasRenderer::DisplayCharacterInfo>& inout_characters)
        {
            f32 horizontalOffset = 0.0f;

            switch(in_horizontal)
            {
                default:
                case HorizontalTextJustification::k_left:
                    return;
                case HorizontalTextJustification::k_centre:
                    horizontalOffset = (in_boundsWidth * 0.5f) - (in_lineWidth * 0.5f);
                    break;
                case HorizontalTextJustification::k_right:
                    horizontalOffset = in_boundsWidth - in_lineWidth;
                    break;
            }

            for(u32 i = in_lineStartIdx; i <= in_lineEndIdx; ++i)
            {
                inout_characters[i].m_position.x += horizontalOffset;
            }
        }
        //----------------------------------------------------------------------------
        /// The text by default is top justified. If another justification
        /// is required this function will update all the characters positions
        ///
        /// @author S Downie
        ///
        /// @param Vertical justification
        /// @param Bounds height
        /// @param Height of all the built lines combined
        /// @param [In/Out] List of display character infos that will be manipulated
        //----------------------------------------------------------------------------
        void ApplyVerticalTextJustifications(VerticalTextJustification in_vertical, f32 in_boundsHeight, f32 in_totalLinesHeight, std::vector<CanvasRenderer::DisplayCharacterInfo>& inout_characters)
        {
            f32 verticalOffset = 0.0f;

            switch(in_vertical)
            {
                default:
                case VerticalTextJustification::k_top:
                    return;
                case VerticalTextJustification::k_centre:
                    verticalOffset = (in_boundsHeight * 0.5f) - (in_totalLinesHeight * 0.5f);
                    break;
                case VerticalTextJustification::k_bottom:
                    verticalOffset = in_boundsHeight - in_totalLinesHeight;
                    break;
            }

            for(auto& character : inout_characters)
            {
                character.m_position.y -= verticalOffset;
            }
        }
        //-----------------------------------------------------
        /// Build the sprite from the given data
        ///
        /// @author S Downie
        ///
        /// @param Transform
        /// @param Size
        /// @param Offset from top left
        /// @param UVs
        /// @param Colour
        /// @param Alignment
        /// @param [Out] Sprite
        //-----------------------------------------------------
        void UpdateSpriteData(const Matrix4& in_transform, const Vector2& in_size, const Vector2& in_offset, const UVs& in_UVs, const Colour& in_colour, AlignmentAnchor in_alignment,
                              SpriteBatch::SpriteData& out_sprite)
        {
            const f32 k_nearClipDistance = 2.0f;

            ByteColour Col = ColourUtils::ColourToByteColour(in_colour);
            
            out_sprite.sVerts[(u32)SpriteBatch::Verts::k_topLeft].Col = Col;
            out_sprite.sVerts[(u32)SpriteBatch::Verts::k_bottomLeft].Col = Col;
            out_sprite.sVerts[(u32)SpriteBatch::Verts::k_topRight].Col = Col;
            out_sprite.sVerts[(u32)SpriteBatch::Verts::k_bottomRight].Col = Col;
            
            out_sprite.sVerts[(u32)SpriteBatch::Verts::k_topLeft].vTex.x = in_UVs.m_u;
            out_sprite.sVerts[(u32)SpriteBatch::Verts::k_topLeft].vTex.y = in_UVs.m_v;
            out_sprite.sVerts[(u32)SpriteBatch::Verts::k_bottomLeft].vTex.x = in_UVs.m_u;
            out_sprite.sVerts[(u32)SpriteBatch::Verts::k_bottomLeft].vTex.y = in_UVs.m_v + in_UVs.m_t;
            out_sprite.sVerts[(u32)SpriteBatch::Verts::k_topRight].vTex.x = in_UVs.m_u + in_UVs.m_s;
            out_sprite.sVerts[(u32)SpriteBatch::Verts::k_topRight].vTex.y = in_UVs.m_v;
            out_sprite.sVerts[(u32)SpriteBatch::Verts::k_bottomRight].vTex.x = in_UVs.m_u + in_UVs.m_s;
            out_sprite.sVerts[(u32)SpriteBatch::Verts::k_bottomRight].vTex.y = in_UVs.m_v + in_UVs.m_t;

            Vector2 vHalfSize(in_size.x * 0.5f, in_size.y * 0.5f);
            Vector2 anchorPoint = GetAnchorPoint(in_alignment, vHalfSize);

            Vector4 vCentrePos(-anchorPoint.x, -anchorPoint.y, 0, 0);
            Vector4 vTemp(-vHalfSize.x, vHalfSize.y, 0, 1.0f);
            
            Vector4 offsetTL(in_offset.x, in_offset.y, 0.0f, 0.0f);
            
            const Matrix4& matTransform(in_transform);
            vTemp += (vCentrePos + offsetTL);
            out_sprite.sVerts[(u32)SpriteBatch::Verts::k_topLeft].vPos = vTemp * matTransform;
            
            vTemp.x = vHalfSize.x;
            vTemp.y = vHalfSize.y;
            
            vTemp += (vCentrePos + offsetTL);
            out_sprite.sVerts[(u32)SpriteBatch::Verts::k_topRight].vPos = vTemp * matTransform;
            
            vTemp.x = -vHalfSize.x;
            vTemp.y = -vHalfSize.y;
            
            vTemp += (vCentrePos + offsetTL);
            out_sprite.sVerts[(u32)SpriteBatch::Verts::k_bottomLeft].vPos = vTemp * matTransform;
            
            vTemp.x = vHalfSize.x;
            vTemp.y = -vHalfSize.y;
            
            vTemp += (vCentrePos + offsetTL);
            out_sprite.sVerts[(u32)SpriteBatch::Verts::k_bottomRight].vPos = vTemp * matTransform;
            
            out_sprite.sVerts[(u32)SpriteBatch::Verts::k_topLeft].vPos.z = -k_nearClipDistance;
            out_sprite.sVerts[(u32)SpriteBatch::Verts::k_topLeft].vPos.w = 1.0f;
            
            out_sprite.sVerts[(u32)SpriteBatch::Verts::k_bottomLeft].vPos.z = -k_nearClipDistance;
            out_sprite.sVerts[(u32)SpriteBatch::Verts::k_bottomLeft].vPos.w = 1.0f;
            
            out_sprite.sVerts[(u32)SpriteBatch::Verts::k_topRight].vPos.z = -k_nearClipDistance;
            out_sprite.sVerts[(u32)SpriteBatch::Verts::k_topRight].vPos.w = 1.0f;
            
            out_sprite.sVerts[(u32)SpriteBatch::Verts::k_bottomRight].vPos.z = -k_nearClipDistance;
            out_sprite.sVerts[(u32)SpriteBatch::Verts::k_bottomRight].vPos.w = 1.0f;
        }
        //----------------------------------------------------------------------------
        /// Adds an ellipsis to the end of the line of text without increasing the
        /// width of the line of text beyond the given length. If required, characters
        /// will be removed from the string.
        ///
        /// If the font doesn't contain '.' the original string will simply be returned.
        ///
        /// @author Ian Copland
        ///
        /// @param The string to add the ellipsis to.
        /// @param The font the string will be renderered with.
        /// @param The absolute pixel offset to regular character spacing.
        /// @param The text scale factor.
        /// @param The maximum pixel width of the text.
        ///
        /// @return The output string with ellipsis
        //----------------------------------------------------------------------------
        std::string AppendEllipsis(const std::string& in_text, const FontCSPtr& in_font, f32 in_absCharSpacingOffset, f32 in_textScale, f32 in_maxTextWidth)
        {
            const u32 k_numDots = 3;
            
            //Check the font contains '.' for building the ellipsis.
            Font::CharacterInfo charInfo;
            if (in_font->TryGetCharacterInfo((UTF8Char)'.', charInfo) == false)
            {
                return in_text;
            }
            
            //get the width of an ellipsis
            f32 dotWidth = GetCharacterWidth((UTF8Char)'.', in_font, in_absCharSpacingOffset, in_textScale);
            f32 ellipsisWidth = dotWidth * k_numDots;
            
            //if there is space for some of the text and the ellipsis, then calculate the output string.
            std::string outputText;
            if (in_maxTextWidth > ellipsisWidth)
            {
                f32 currentLineWidth = 0.0f;
                auto it = in_text.begin();
                while(it < in_text.end())
                {
                    auto character = UTF8StringUtils::Next(it);
                    
                    currentLineWidth += GetCharacterWidth(character, in_font, in_absCharSpacingOffset, in_textScale);
                    if (currentLineWidth > in_maxTextWidth - ellipsisWidth)
                    {
                        break;
                    }
                    
                    UTF8StringUtils::Append(character, outputText);
                }
                
                //append the ellipsis
                for (u32 i = 0; i < k_numDots; ++i)
                {
                    UTF8StringUtils::Append((UTF8Char)'.', outputText);
                }
            }
            
            //otherwise don't bother trying to use the input text and just build the ellipsis text.
            else
            {
                f32 currentLineWidth = 0.0f;
                for (u32 i = 0; i < k_numDots; ++i)
                {
                    currentLineWidth += dotWidth;
                    if (currentLineWidth > in_maxTextWidth)
                    {
                        break;
                    }
                    
                    UTF8StringUtils::Append((UTF8Char)'.', outputText);
                }
            }
            
            return outputText;
        }
        //----------------------------------------------------------------------------
        /// Returns if the given lines will fit completely in the number of allowed lines/bounds
        ///
        /// @author HMcLaughlin
        ///
        /// @param in_wrappedText - Vector of wrapped lines
        /// @param in_properties - The text properties.
        /// @param in_maxHeight - Absolute Bounded Height
        /// @param in_lineHeight - Absolute Line Height
        /// @param [Out] out_numLines - Number of lines used
        ///
        /// @return If the bounded lines fit
        //----------------------------------------------------------------------------
        bool DoesWrappedTextFit(const CanvasRenderer::WrappedText& in_wrappedText, const CanvasRenderer::TextProperties& in_properties, f32 in_maxHeight, f32 in_lineHeight, u32& out_numLines)
        {
            u32 numLinesOnBounds = static_cast<u32>(in_wrappedText.size());
            u32 numLines = (in_properties.m_maxNumLines == 0) ? numLinesOnBounds : std::min(numLinesOnBounds, in_properties.m_maxNumLines);
            
            out_numLines = std::min(numLines, (u32)(in_maxHeight/in_lineHeight));
            
            if (in_wrappedText.size() > out_numLines)
            {
                return false;
            }
            
            return true;
        }
        //----------------------------------------------------------------------------
        /// Formats the text string into a vector representing each line of the text,
        /// constrained in the bounds
        ///
        /// @author HMcLaughlin
        ///
        /// @param in_text - Text to convert to display characters (UTF-8)
        /// @param in_textScale - Text Scale
        /// @param in_font - Font to use
        /// @param in_bounds - Max bounds to fit the text
        /// @param in_properties - The text properties.
        ///
        /// @return Vector where each entry is a line of in_text,
        ///         constrained by the in_bounds
        //----------------------------------------------------------------------------
        CanvasRenderer::WrappedText GetWrappedText(const std::string& in_text, f32 in_textScale, const FontCSPtr& in_font, const Vector2& in_bounds, const CanvasRenderer::TextProperties& in_properties)
        {
            //NOTE: | denotes the bounds of the box
            //- |The quick brown fox| jumped over\nthe ferocious honey badger
            
            //Split the string into lines by the forced line breaks (i.e. the \n)
            //- |The quick brown fox| jumped over
            //- |the ferocious honey| badger
            std::vector<std::string> linesOnNewLine;
            SplitByNewLine(in_text, linesOnNewLine);
            
            //Split the lines further based on the line width, breakable characters and the bounds
            //- |The quick brown fox|
            //- |jumped over        |
            //- |the ferocious honey|
            //- |badger             |
            CanvasRenderer::WrappedText linesOnBounds;
            for(const auto& line : linesOnNewLine)
            {
                SplitByBounds(line, in_font, in_properties.m_absCharSpacingOffset, in_textScale, in_bounds, linesOnBounds);
            }
            
            return linesOnBounds;
        }
        //----------------------------------------------------------------------------
        /// Build the descriptions for all characters. The descriptions can then be
        /// passed into the draw method for rendering. The characters will be
        /// built to fit into the given bounds and will wrap and then clip in
        /// order to fit. This will return if the text can fit into the bounds given
        /// with the text scale provided
        ///
        /// @author HMcLaughlin
        ///
        /// @param in_text - Text to convert to display characters (UTF-8)
        /// @param in_textScale - Text Scale
        /// @param in_font - Font to use
        /// @param in_bounds - Max bounds to fit the text
        /// @param in_properties - The text properties.
        /// @param out_builtText - Built text
        //----------------------------------------------------------------------------
        void BuildText(const std::string& in_text, f32 in_textScale, const FontCSPtr& in_font, const Vector2& in_bounds, const CanvasRenderer::TextProperties& in_properties, CanvasRenderer::BuiltText& out_builtText)
        {
            f32 lineHeight = in_properties.m_lineSpacingScale * ((in_font->GetLineHeight() + in_properties.m_absLineSpacingOffset) * in_textScale);
            f32 maxHeight = in_bounds.y;
            
            CanvasRenderer::WrappedText linesOnBounds = GetWrappedText(in_text, in_textScale, in_font, in_bounds, in_properties);
            
            u32 numLines = 0;

            //add an ellipsis if the text doesn't fit, unless there is no room for the ellipsis itself.
            if (DoesWrappedTextFit(linesOnBounds, in_properties, maxHeight, lineHeight, numLines) == false && numLines > 0)
            {
                linesOnBounds[numLines-1] = AppendEllipsis(linesOnBounds[numLines-1], in_font, in_properties.m_absCharSpacingOffset, in_textScale, in_bounds.x);
            }
            
            //The middle of the text label is 0,0. We want to be starting at the top left.
            f32 cursorXReturnPos = -in_bounds.x * 0.5f;
            f32 cursorX = cursorXReturnPos;
            f32 cursorY = in_bounds.y * 0.5f;
            
            for(u32 lineIdx = 0; lineIdx < numLines; ++lineIdx)
            {
                u32 lineStartIdx = static_cast<u32>(out_builtText.m_characters.size());
                
                auto characterIt = linesOnBounds[lineIdx].begin();
                while(characterIt != linesOnBounds[lineIdx].end())
                {
                    auto character = UTF8StringUtils::Next(characterIt);
                    auto builtCharacter(BuildCharacter(character, in_font, cursorX, cursorY, in_textScale, in_properties.m_absCharSpacingOffset));
                    
                    cursorX += builtCharacter.m_advance;
                    
                    if(builtCharacter.m_packedImageSize.y > 0.0f)
                    {
                        //No point rendering whitespaces
                        out_builtText.m_characters.push_back(builtCharacter);
                    }
                }
                
                f32 lineWidth = cursorX - cursorXReturnPos;
                ApplyHorizontalTextJustifications(in_properties.m_horizontalJustification, in_bounds.x, lineStartIdx, static_cast<u32>(out_builtText.m_characters.size()) - 1, lineWidth, out_builtText.m_characters);
                
                out_builtText.m_width = std::max(lineWidth, out_builtText.m_width);
                
                cursorX = cursorXReturnPos;
                cursorY -= lineHeight;
            }
            
            out_builtText.m_height = numLines * lineHeight;
            ApplyVerticalTextJustifications(in_properties.m_verticalJustification, in_bounds.y, out_builtText.m_height, out_builtText.m_characters);
        }
        //----------------------------------------------------------------------------
        /// Recursively calculates the closest correct scale of text.
        /// Performs a binary search to find the closest correct approximation within
        /// a tolerance.
        ///
        /// @author HMcLaughlin
        ///
        /// @param in_text - Text to convert to display characters (UTF-8)
        /// @param in_properties - The text properties.
        /// @param in_font - Font to use
        /// @param in_bounds - Max bounds to fit the text
        /// @param in_minMax - Min/Max scales to search
        /// @param in_currentIteration - Current level of recursion
        ///
        /// @return Close to best case fitting scale
        //----------------------------------------------------------------------------
        f32 GetBoundedTextScaleRecursive(const std::string& in_text, const CanvasRenderer::TextProperties& in_properties, const FontCSPtr& in_font, const Vector2& in_bounds, const Vector2& in_minMax, u32 in_currentIteration = 0)
        {
            f32 min = in_minMax.x;
            f32 max = in_minMax.y;
            
            if(min == max)
            {
                return min;
            }
            
            CS_ASSERT(min < max, "Min is greater than the max!");
            
            //Choose a midpoint value between the max and the min
            f32 difference = max - min;
            f32 midpointScale = min + (difference * 0.5f);
            
            //Check if the midpoint value is valid
            const auto& wrappedText = GetWrappedText(in_text, midpointScale, in_font, in_bounds, in_properties);
            
            f32 lineHeight = in_properties.m_lineSpacingScale * ((in_font->GetLineHeight() + in_properties.m_absLineSpacingOffset) * midpointScale);
            
            u32 numLinesUsed = 0;
            bool doesFit = DoesWrappedTextFit(wrappedText, in_properties, in_bounds.y, lineHeight, numLinesUsed);
            
            //Increment current iteration
            ++in_currentIteration;
            
            if(difference <= k_autoScaleTolerance || in_currentIteration >= k_maxAutoScaleIterations)
            {
                //Reached a value below the tolerance/max iterations, return the current valid scale
                return doesFit ? midpointScale : min;
            }
            
            if(doesFit)
            {
                //We recurse further, the valid midpoint scale is now used as the min value
                return GetBoundedTextScaleRecursive(in_text, in_properties, in_font, in_bounds, Vector2(midpointScale, max), in_currentIteration);
            }
            else
            {
                //Recurse further, the midpoint value is used as the max value for this recursion
                //This is based on the assumption that the min value is always a valid fitting scale
                return GetBoundedTextScaleRecursive(in_text, in_properties, in_font, in_bounds, Vector2(min, midpointScale), in_currentIteration);
            }
        }
    }

    
    CS_DEFINE_NAMEDTYPE(CanvasRenderer);

    //----------------------------------------------------------------------------
    //----------------------------------------------------------------------------
    CanvasRendererUPtr CanvasRenderer::Create()
    {
        return CanvasRendererUPtr(new CanvasRenderer());
    }
    //----------------------------------------------------------------------------
    //----------------------------------------------------------------------------
    bool CanvasRenderer::IsA(InterfaceIDType in_interfaceId) const
    {
        return in_interfaceId == CanvasRenderer::InterfaceID;
    }
    //----------------------------------------------------------------------------
    //----------------------------------------------------------------------------
    void CanvasRenderer::OnInit()
    {
        m_resourcePool = Application::Get()->GetResourcePool();
        CS_ASSERT(m_resourcePool != nullptr, "Must have a resource pool");

        RenderSystem* renderSystem = Application::Get()->GetRenderSystem();
        CS_ASSERT(renderSystem != nullptr, "Canvas renderer cannot find render system");

        m_screen = Application::Get()->GetSystem<Screen>();
        CS_ASSERT(m_screen != nullptr, "Canvas renderer cannot find screen system");

        auto materialFactory = Application::Get()->GetSystem<MaterialFactory>();
        CS_ASSERT(materialFactory != nullptr, "Must have a material factory");
        
        m_materialPool = CanvasMaterialPoolUPtr(new CanvasMaterialPool(materialFactory));
        m_overlayBatcher = DynamicSpriteBatchUPtr(new DynamicSpriteBatch(renderSystem));
    }
    //----------------------------------------------------------
    //----------------------------------------------------------
    void CanvasRenderer::Render(Canvas* in_canvas)
    {
        CS_ASSERT(in_canvas != nullptr, "Canvas cannot render null UI canvas");

        in_canvas->Draw(this);

        m_overlayBatcher->DisableScissoring();
        m_overlayBatcher->ForceRender();

        m_materialPool->Clear();
        m_canvasSprite.pMaterial = nullptr;
    }
    //----------------------------------------------------------------------------
    //----------------------------------------------------------------------------
    void CanvasRenderer::PushClipBounds(const Vector2& in_blPosition, const Vector2& in_size)
    {
        if(m_scissorPositions.empty())
        {
            m_scissorPositions.push_back(in_blPosition);
            m_scissorSizes.push_back(in_size);
        }
        else
        {
            Vector2 vOldBottomLeft = m_scissorPositions.back();
            Vector2 vOldTopRight = m_scissorSizes.back() + vOldBottomLeft;
            Vector2 vNewBottomLeft = in_blPosition;
            Vector2 vNewTopRight = in_blPosition + in_size;

            //If the scissor region extends outside the bounds of the screen this is undefined behaviour and
            //the render system may wrap the values causing artefacts. We clamp them here to make sure this
            //doesn't happen.
            vNewBottomLeft.x = MathUtils::Clamp(std::max(vNewBottomLeft.x, vOldBottomLeft.x), 0.0f, m_screen->GetResolution().x);
            vNewBottomLeft.y = MathUtils::Clamp(std::max(vNewBottomLeft.y, vOldBottomLeft.y), 0.0f, m_screen->GetResolution().y);

            vNewTopRight.x = MathUtils::Clamp(std::min(vNewTopRight.x, vOldTopRight.x), 0.0f, m_screen->GetResolution().x);
            vNewTopRight.y = MathUtils::Clamp(std::min(vNewTopRight.y, vOldTopRight.y), 0.0f, m_screen->GetResolution().y);

            Vector2 vNewSize = vNewTopRight - vNewBottomLeft;

            m_scissorPositions.push_back(vNewBottomLeft);
            m_scissorSizes.push_back(vNewSize);
        }

        m_overlayBatcher->EnableScissoring(m_scissorPositions.back(), m_scissorSizes.back());
    }
    //----------------------------------------------------------------------------
    //----------------------------------------------------------------------------
    void CanvasRenderer::PopClipBounds()
    {
        if(!m_scissorPositions.empty())
        {
            m_scissorPositions.erase(m_scissorPositions.end()-1);
            m_scissorSizes.erase(m_scissorSizes.end()-1);

            if(!m_scissorPositions.empty())
            {
                m_overlayBatcher->EnableScissoring(m_scissorPositions.back(), m_scissorSizes.back());
            }
        }

        if(m_scissorPositions.empty())
        {
            m_overlayBatcher->DisableScissoring();
        }
    }
    //----------------------------------------------------------------------------
    //----------------------------------------------------------------------------
    void CanvasRenderer::DrawBox(const Matrix3& in_transform, const Vector2& in_size, const Vector2& in_offset, const TextureCSPtr& in_texture, const UVs& in_UVs,
                                 const Colour& in_colour, AlignmentAnchor in_anchor)
    {
        m_canvasSprite.pMaterial = m_materialPool->GetMaterial(in_texture);

        UpdateSpriteData(Convert2DTransformTo3D(in_transform), in_size, in_offset, in_UVs, in_colour, in_anchor, m_canvasSprite);

        m_overlayBatcher->Render(m_canvasSprite);
    }
    //----------------------------------------------------------------------------
    //----------------------------------------------------------------------------
    CanvasRenderer::BuiltText CanvasRenderer::BuildText(const std::string& in_text, const FontCSPtr& in_font, const Vector2& in_bounds, const TextProperties& in_properties, f32& out_textScale) const
    {
        BuiltText result;
        result.m_width = 0.0f;
        result.m_height = 0.0f;
        
        f32 textScale = in_properties.m_textScale;
        
        //Don't build the text if scale is invalid
        if(textScale <= 0.0f)
        {
            return result;
        }
        
        f32 lineHeight = in_properties.m_lineSpacingScale * ((in_font->GetLineHeight() + in_properties.m_absLineSpacingOffset) * textScale);
        
        result.m_characters.reserve(in_text.size());

        if(in_properties.m_shouldAutoScale)
        {
            CS_ASSERT(in_properties.m_minTextScale <= in_properties.m_textScale, "Cannot autoscale as the MinTextAutoScale is more than the TextScale property!");

            bool doesFit = false;
            u32 numLines = 0;
            
            //Recalculate line height for the ideal scale
            lineHeight = in_properties.m_lineSpacingScale * ((in_font->GetLineHeight() + in_properties.m_absLineSpacingOffset) * textScale);

            //Check to see if the text fits at ideal scale
            auto idealWrappedText = GetWrappedText(in_text, textScale, in_font, in_bounds, in_properties);
            doesFit = DoesWrappedTextFit(idealWrappedText, in_properties, in_bounds.y, lineHeight, numLines);
            
            //If the ideal doesn't fit then attempt the minimum, if that doesn't fit then there is nothing we can do scale-wise
            if(!doesFit && textScale != in_properties.m_minTextScale)
            {
                //Recalculate line height for the minimum
                lineHeight = in_properties.m_lineSpacingScale * ((in_font->GetLineHeight() + in_properties.m_absLineSpacingOffset) * in_properties.m_minTextScale);

                //Check to see if the text can fit at the smallest scale
                auto minWrappedText = GetWrappedText(in_text, in_properties.m_minTextScale, in_font, in_bounds, in_properties);
                doesFit = DoesWrappedTextFit(minWrappedText, in_properties, in_bounds.y, lineHeight, numLines);
                
                if(!doesFit)
                {
                    CS_LOG_WARNING("Doesn't fit at min scale, consider resizing the label manually to fit the text better!");
                    textScale = in_properties.m_minTextScale;
                }
                else
                {
                    //We should search for a more optimal scale between the min and the ideal, while still fitting
                    textScale = GetBoundedTextScaleRecursive(in_text, in_properties, in_font, in_bounds, Vector2(in_properties.m_minTextScale, textScale));
                }
            }
        }

        //Update the final scale
        out_textScale = textScale;
        
        //Carry out the building of the text with the resolved scale
        ChilliSource::BuildText(in_text, textScale, in_font, in_bounds, in_properties, result);
        
        return result;
    }
    //----------------------------------------------------------------------------
    //----------------------------------------------------------------------------
    void CanvasRenderer::DrawText(const std::vector<DisplayCharacterInfo>& in_characters, const Matrix3& in_transform, const Colour& in_colour, const TextureCSPtr& in_texture)
    {
        m_canvasSprite.pMaterial = m_materialPool->GetMaterial(in_texture);

        Matrix4 matTransform = Convert2DTransformTo3D(in_transform);
        Matrix4 matTransformedLocal;

        for (const auto& character : in_characters)
        {
            matTransformedLocal = Matrix4::CreateTranslation(Vector3(character.m_position, 0.0f)) * matTransform;
            UpdateSpriteData(matTransformedLocal, character.m_packedImageSize, Vector2::k_zero, character.m_UVs, in_colour, AlignmentAnchor::k_topLeft, m_canvasSprite);
            m_overlayBatcher->Render(m_canvasSprite);
        }
    }
    //----------------------------------------------------------------------------
    //----------------------------------------------------------------------------
    void CanvasRenderer::OnDestroy()
    {
        m_overlayBatcher = nullptr;
        m_canvasSprite.pMaterial = nullptr;
        
        m_materialPool->Clear();
        m_materialPool.reset();
    }
}
