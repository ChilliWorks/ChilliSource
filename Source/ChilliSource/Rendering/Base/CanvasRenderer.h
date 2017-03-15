//
//  CanvasRenderer.h
//  ChilliSource
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

#ifndef _CHILLISOURCE_RENDERING_BASE_CANVASRENDERER_H_
#define _CHILLISOURCE_RENDERING_BASE_CANVASRENDERER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Geometry/Shapes.h>
#include <ChilliSource/Core/System/AppSystem.h>
#include <ChilliSource/Rendering/Base/CanvasMaterialPool.h>
#include <ChilliSource/Rendering/Base/HorizontalTextJustification.h>
#include <ChilliSource/Rendering/Base/VerticalTextJustification.h>
#include <ChilliSource/Rendering/Texture/UVs.h>

namespace ChilliSource
{
    //----------------------------------------------------------------------------
    /// System that renders simple shapes and text to screen space. Responsible
    /// for rendering the UI system.
    ///
    /// @author S Downie
    //----------------------------------------------------------------------------
    class CanvasRenderer : public AppSystem
    {
    public:
        CS_DECLARE_NAMEDTYPE(CanvasRenderer);
    
        //----------------------------------------------------------------------------
        /// A container for text properties for altering the look of built text.
        ///
        /// @author Ian Copland
        //----------------------------------------------------------------------------
        struct TextProperties
        {
            f32 m_textScale = 1.0f;
            f32 m_minTextScale = 0.75f;
            f32 m_absCharSpacingOffset = 0.0f;
            f32 m_absLineSpacingOffset = 0.0f;
            f32 m_lineSpacingScale = 1.0f;
            u32 m_maxNumLines = 0;
            
            bool m_shouldAutoScale = false;
            
            HorizontalTextJustification m_horizontalJustification = HorizontalTextJustification::k_centre;
            VerticalTextJustification m_verticalJustification = VerticalTextJustification::k_centre;
        };
        //----------------------------------------------------------------------------
        /// Holds the information required to build a sprite for a text character
        ///
        /// @author S Downie
        //----------------------------------------------------------------------------
        struct DisplayCharacterInfo
        {
            UVs m_UVs;
            Vector2 m_position;
            Vector2 m_packedImageSize;
            f32 m_advance = 0.0f;
        };
        //----------------------------------------------------------------------------
        /// Holds the return information for building text including all the characters
        /// and the total width and height
        ///
        /// @author S Downie
        //----------------------------------------------------------------------------
        struct BuiltText
        {
            std::vector<DisplayCharacterInfo> m_characters;

            f32 m_width;
            f32 m_height;
        };
        //----------------------------------------------------------------------------
        /// Defines a type for a vector of bounded lines
        ///
        /// @author HMcLaughlin
        //----------------------------------------------------------------------------
        using WrappedText = std::vector<std::string>;
        //----------------------------------------------------------------------------
        /// @author S Downie
        ///
        /// @param Interface ID
        ///
        /// @return Whether the class implements the given interface
        //----------------------------------------------------------------------------
        bool IsA(InterfaceIDType in_interfaceId) const override;

        /// Increment the the clip mask value. When performing a stencil clip test
        /// this is the value that will be used. Generally it is incremented prior to
        /// rendering the contents of a clipping box and decremented afterwards
        ///
        void IncrementClipMask() noexcept { ++m_clipMaskCount; }
        
        /// Decrement the clip mask
        ///
        void DecrementClipMask() noexcept { --m_clipMaskCount; }
        

        /// Renders the given sprite box to the screen or as a mask
        ///
        /// @param drawMode
        ///     Describes whether to render the box to the canvas or as a mask
        /// @param transform
        ///     2D Transformation matrix
        /// @param size
        ///     Dimensions of the box in canvas space
        /// @param offset
        ///     Offset from TL of the bounds. Used to restore the cropped alpha
        /// @param texture
        ///     Texture
        /// @param uvs
        ///     UVs for texture
        /// @param colour
        ///     Colour to apply to the sprite box
        /// @param anchor
        ///     Origin anchor
        ///
        void DrawBox(CanvasDrawMode drawMode, const Matrix3& transform, const Vector2& size, const Vector2& offset, const TextureCSPtr& texture, const UVs& uvs, const Colour& colour, AlignmentAnchor anchor);
        
        //----------------------------------------------------------------------------
        /// Build the descriptions for all characters. The descriptions can then be
        /// passed into the draw method for rendering. The characters will be
        /// build to fit into the given bounds and will wrap and then clip in
        /// order to fit.
        ///
        /// @author S Downie
        ///
        /// @param in_text - Text to convert to display characters (UTF-8)
        /// @param in_font - Font to use
        /// @param in_bounds - Max bounds
        /// @param in_textProperties - The text properties used to build.
        /// @param [Out] out_textScale - Final scale that should be used
        ///
        /// @return Built text struct containing all the character infos
        //----------------------------------------------------------------------------
        BuiltText BuildText(const std::string& in_text, const FontCSPtr& in_font, const Vector2& in_bounds, const TextProperties& in_textProperties, f32& out_textScale) const;

        /// Build the sprites for each given character and render them to screen.
        ///
        /// @param characters
        ///     List of character display infos in text space
        /// @param transform
        ///     2D transform to screen space
        /// @param colour
        ///     Tint colour to apply to character sprites
        /// @param texture
        ///     Font texture
        ///
        void DrawText(const std::vector<DisplayCharacterInfo>& characters, const Matrix3& transform, const Colour& colour, const TextureCSPtr& texture);

    private:

        friend class Application;
        //----------------------------------------------------------------------------
        /// Creates a new instance of the system.
        ///
        /// @author S Downie
        ///
        /// @return New renderer instance
        //----------------------------------------------------------------------------
        static CanvasRendererUPtr Create();
        //----------------------------------------------------------------------------
        /// Private constructor to enforce use of factory method
        ///
        /// @author S Downie
        //----------------------------------------------------------------------------
        CanvasRenderer() = default;
        //----------------------------------------------------------------------------
        /// Called when the system is created
        ///
        /// @author S Downie
        //----------------------------------------------------------------------------
        void OnInit() override;
        //----------------------------------------------------------------------------
        /// Called when the render snapshot event occurs. This iterates over all UI
        /// adding objects to the render snapshot as appropriate.
        ///
        /// @author Ian Copland
        ///
        /// @param targetType
        ///     Whether the snapshot is for the main screen or an offscreen render target
        /// @param renderSnapshot
        ///     The render snapshot object which contains all snapshotted data.
        /// @param frameAllocator
        ///     Allocate memory for this render frame from here
        //----------------------------------------------------------------------------
        void OnRenderSnapshot(TargetType targetType, RenderSnapshot& renderSnapshot, IAllocator* frameAllocator) noexcept override;
        //----------------------------------------------------------------------------
        /// Called when the system is destroyed
        ///
        /// @author S Downie
        //----------------------------------------------------------------------------
        void OnDestroy() override;

    private:
        IAllocator* m_currentFrameAllocator = nullptr;
        RenderSnapshot* m_currentRenderSnapshot = nullptr;
        u32 m_nextPriority = 0;
        
        s32 m_clipMaskCount = 0;

        CanvasMaterialPoolUPtr m_screenMaterialPool;
        CanvasMaterialPoolUPtr m_screenMaskMaterialPool;
        CanvasMaterialPoolUPtr m_maskMaterialPool;

        ResourcePool* m_resourcePool;
        Screen* m_screen;
        CursorSystem* m_cursorSystem;
    };
}

#endif
