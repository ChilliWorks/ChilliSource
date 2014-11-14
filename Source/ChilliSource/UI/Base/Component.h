//
//  Component.h
//  Chilli Source
//  Created by Ian Copland on 14/11/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
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

#ifndef _CHILLISOURCE_UI_BASE_COMPONENT_H_
#define _CHILLISOURCE_UI_BASE_COMPONENT_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/QueryableInterface.h>

namespace ChilliSource
{
    namespace UI
    {
        //------------------------------------------------------------------------
        /// A component which can be attached to a widget to provide additional
        /// functionality. Components are attached to a widget on construction
        /// and live for the entire life of the widget. A component def is used
        /// to create an instance of a widget.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------
        class Component : public Core::QueryableInterface
        {
        public:
            CS_DECLARE_NAMEDTYPE(Component);
            CS_DECLARE_NOCOPY(Component);
            //----------------------------------------------------------------
            /// Destructor
            ///
            /// @author S McGaw
            //----------------------------------------------------------------
            virtual ~Component(){}
        protected:
            //----------------------------------------------------------------
            /// Constructor
            ///
            /// @author Ian Copland
            //----------------------------------------------------------------
            Component(Widget* in_widget);
            //----------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return A pointer to the widget that owns this component.
            //----------------------------------------------------------------
            Widget* GetWidget();
            //----------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return A constant pointer to the widget that owns this
            /// component.
            //----------------------------------------------------------------
            const Widget* GetWidget() const;
            //----------------------------------------------------------------
            /// A method which is called when all components owned by the parent
            /// widget have been created and added. Inheriting classes should use
            /// this for any required initialisation.
            ///
            /// @author Ian Copland
            //----------------------------------------------------------------
            virtual void OnInit() {}
            //----------------------------------------------------------------
            /// This is called when the owning widget is added to the canvas.
            ///
            /// @author Ian Copland
            //----------------------------------------------------------------
            virtual void OnAddedToCanvas() {}
            //----------------------------------------------------------------
            /// This is called when the application is resumed while the owning
            /// widget is attached to the canvas. This is also also called
            /// when added to the canvas if the application is already active.
            ///
            /// @author Ian Copland
            //----------------------------------------------------------------
            virtual void OnResume() {}
            //----------------------------------------------------------------
            /// This is called when the application is foregrounded while the
            /// owning widget is attached to the canvas. This is also called
            /// when added to the canvas if the application is already
            /// foregrounded.
            ///
            /// @author Ian Copland
            //----------------------------------------------------------------
            virtual void OnForeground() {}
            //----------------------------------------------------------------
            /// This is called every frame while the application is active
            /// and the owning widget is on the canvas. This should be used
            /// for any update logic in the component.
            ///
            /// @author Ian Copland
            ///
            /// @param The delta time since the last update.
            //----------------------------------------------------------------
            virtual void OnUpdate(f32 in_deltaTime) {}
            //----------------------------------------------------------------
            /// This is called at a fixed time interval while the application
            /// is active and the owning widget is on the canvas. This should
            /// be used for any update logic that requires a fixed time
            /// between updates in the component.
            ///
            /// @author Ian Copland
            ///
            /// @param The delta time between fixed updates.
            //----------------------------------------------------------------
            virtual void OnFixedUpdate(f32 in_fixedUpdateDeltaTime) {}
            //----------------------------------------------------------------
            /// This is called when the application is backgrounded while the
            /// owning widget is on the canvas. This will also be called when
            /// the owning widget is removed from the canvas if the application
            /// is foregrounded.
            ///
            /// @author Ian Copland
            //----------------------------------------------------------------
            virtual void OnBackground() {}
            //----------------------------------------------------------------
            /// This is called when the application is suspended while the
            /// owning widget is on the canvas. This will also be called when
            /// the owning widget is removed from the canvas if the application
            /// is active.
            ///
            /// @author Ian Copland
            //----------------------------------------------------------------
            virtual void OnSuspend() {}
            //----------------------------------------------------------------
            /// This is called when the owning widget is removed from the
            /// canvas.
            ///
            /// @author Ian Copland
            //----------------------------------------------------------------
            virtual void OnRemovedFromCanvas() {}
            //----------------------------------------------------------------
            /// This is called when the component--and the owning widget--is
            /// about to be destroyed. Components are destroyed in reverse
            /// initialisation order.
            ///
            /// @author Ian Copland
            //----------------------------------------------------------------
            virtual void OnDestroy() {}
        private:
            friend class Widget;

            Widget* m_widget = nullptr;
        };
    }
}

#endif
