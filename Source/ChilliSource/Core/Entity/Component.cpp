//
//  Component.cpp
//  Chilli Source
//  Created by Stuart McGaw on 28/09/2010.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2010 Tag Games Limited
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

#include <ChilliSource/Core/Entity/Component.h>

#include <ChilliSource/Core/Entity/Entity.h>

namespace ChilliSource
{
	namespace Core
	{
        //----------------------------------------------------
        //----------------------------------------------------
		Component::Component()
		: m_entity(nullptr)
		{
			
		}
		//----------------------------------------------------
		//----------------------------------------------------
		Entity * Component::GetEntity()
		{
			return m_entity;
		}
		//----------------------------------------------------
		//----------------------------------------------------
		const Entity * Component::GetEntity() const
		{
			return m_entity;
		}
		//----------------------------------------------------
		//----------------------------------------------------
		void Component::SetEntity(Entity* in_newOwner)
		{
			m_entity = in_newOwner;
		}
        //----------------------------------------------------
        //----------------------------------------------------
        void Component::RemoveFromEntity()
        {
            CS_ASSERT(m_entity != nullptr, "Must have an entity to remove from");
            m_entity->RemoveComponent(this);
        }
	}
}