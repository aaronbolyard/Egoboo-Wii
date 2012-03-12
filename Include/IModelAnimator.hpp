// This file is part of Egoboo-Wii.
//
// Egoboo-Wii is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Egoboo-Wii is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Egoboo-Wii.  If not, see <http:// www.gnu.org/licenses/>.

#ifndef ADVENTURE_MODEL_ANIMATOR_INTERFACE_HPP_
#define ADVENTURE_MODEL_ANIMATOR_INTERFACE_HPP_

#include "Standard.hpp"

#include "IDisplay.hpp"
#include "ElementArray.hpp"

namespace Adventure
{
	class IEffect;
	
	struct Animation
	{
		// The name of the animation
		String Name;
		
		// The loop points of an animation
		int StartFrame;
		int EndFrame;
	};
	
	// A rendered frame of an animation
	struct ModelFrame
	{
		// The components of the vertices
		CompressedVector3Array Positions;
		CompressedNormalArray Normals;
		ColorArray Colors;
		CompressedVector2Array TexCoords;
		
		// Indices of the frame
		ModelIndexArray Indices;
		
		// Helper method to render using the provided display
		inline void Draw(IDisplay& display)
		{
			display.DrawModel(Positions.GetElements(), Normals.GetElements(), Colors.GetElements(), TexCoords.GetElements(), Indices, true);
		}
	};
	
	class IModelAnimator
	{
		public:
			virtual ~IModelAnimator() { }
			
			// Initializes the model animator using the provided allocator
			virtual bool Initialize(Allocator* allocator) = 0;
			
			// Initializes a model frame
			virtual bool InitializeFrame(ModelFrame& frame, Allocator* allocator) = 0;
			
			// Gets an animation with the provided name
			// If the animation does not exist, the method returns an empty animation
			virtual bool GetAnimation(const String& name, Animation& animation) const = 0;
			
			// Sets the interval of the animation
			virtual void SetInterval(float interval) = 0;
			
			// Updates the current animation
			// Delta should correspond to the previously set interval
			virtual void UpdateAnimation(float delta) = 0;
			
			// Switches to the provided animation
			virtual bool SwitchTo(const String& name) = 0;
			
			// Renders the model to a cache
			virtual bool Render() = 0;
			
			// Gets the current frame
			virtual bool BuildCurrentFrame(ModelFrame& frame, IEffect& effect) = 0;
			
			// Gets a grip transformation in model space
			//virtual Matrix GetGrip(const String& grip) const = 0;
	};
}

#endif