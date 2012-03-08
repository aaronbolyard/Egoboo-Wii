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

#include <fstream>

#include "ElementArray.hpp"
#include "File.hpp"
#include "Image.hpp"
#include "ITexture.hpp"
#include "Matrix.hpp"
#include "PooledAllocator.hpp"
#include "Vector3.hpp"
#include "WiiDisplay.hpp"
#include "WiiSystem.hpp"
#include "WiiAllocator.hpp"

struct Object
{
	Adventure::CompressedVector3Array Positions;
	Adventure::CompressedNormalArray Normals;
	Adventure::ColorArray Colors;
	Adventure::CompressedVector2Array TexCoords;
	
	Adventure::ModelIndexArray Indices;
	Adventure::ITexture* Texture;
	
	Adventure::Allocator* TexturePool;
	Adventure::Allocator* ElementPool;
	Adventure::Allocator* ScratchPool;
};

bool CreateCube(Object& object, Adventure::ISystem& system)
{
	if (!object.Positions.Initialize(8, object.ElementPool))
	{
		TRACE(DEBUG_GENERAL, "Couldn't allocate positions!");
		return false;
	}
	
	object.Positions.Lock();
	
	object.Positions.Buffer(0.0f, 0.0f, 0.0f);
	object.Positions.Buffer(0.0f, 0.0f, 1.0f);
	object.Positions.Buffer(0.0f, 1.0f, 0.0f);
	object.Positions.Buffer(0.0f, 1.0f, 1.0f);
	object.Positions.Buffer(1.0f, 0.0f, 0.0f);
	object.Positions.Buffer(1.0f, 0.0f, 1.0f);
	object.Positions.Buffer(1.0f, 1.0f, 0.0f);
	object.Positions.Buffer(1.0f, 1.0f, 1.0f);
	
	object.Positions.Unlock();
	
	if (!object.Normals.Initialize(6, object.ElementPool))
	{
		TRACE(DEBUG_GENERAL, "Couldn't allocate normals!");
		return false;
	}
	
	object.Normals.Lock();
	
	object.Normals.Buffer(0.0f, 0.0f, 1.0f);
	object.Normals.Buffer(0.0f, 0.0f, -1.0f);
	object.Normals.Buffer(0.0f, 1.0f, 0.0f);
	object.Normals.Buffer(0.0f, -1.0f, 0.0f);
	object.Normals.Buffer(1.0f, 0.0f, 0.0f);
	object.Normals.Buffer(-1.0f, 0.0f, 0.0f);
	
	object.Normals.Unlock();
	
	if (!object.Colors.Initialize(6, object.ElementPool))
	{
		TRACE(DEBUG_GENERAL, "Couldn't allocate colors!");
		return false;
	}
	
	object.Colors.Lock();
	
	object.Colors.Buffer(Adventure::Color(1.0f, 1.0f, 1.0f));
	object.Colors.Buffer(Adventure::Color(1.0f, 0.0f, 0.0f));
	object.Colors.Buffer(Adventure::Color(0.0f, 1.0f, 0.0f));
	object.Colors.Buffer(Adventure::Color(0.0f, 0.0f, 1.0f));
	object.Colors.Buffer(Adventure::Color(1.0f, 1.0f, 0.0f));
	object.Colors.Buffer(Adventure::Color(1.0f, 0.0f, 1.0f));
	
	object.Colors.Unlock();
	
	if (!object.TexCoords.Initialize(4, object.ElementPool))
	{
		TRACE(DEBUG_GENERAL, "Couldn't allocate texture coordinates!");
		return false;
	}
	
	object.TexCoords.Lock();
	
	object.TexCoords.Buffer(0.0f, 0.0f);
	object.TexCoords.Buffer(0.0f, 1.0f);
	object.TexCoords.Buffer(1.0f, 0.0f);
	object.TexCoords.Buffer(1.0f, 1.0f);
	
	object.TexCoords.Unlock();
	
	if (!object.Indices.Initialize(36, object.ElementPool))
	{
		TRACE(DEBUG_GENERAL, "Couldn't allocate indices");
		return false;
	}
		
	object.Indices.Lock();
	
	// Back face
	object.Indices.Buffer(0, 1, 0, 3);
	object.Indices.Buffer(4, 1, 0, 1);
	object.Indices.Buffer(6, 1, 0, 0);
	
	object.Indices.Buffer(0, 1, 0, 3);
	object.Indices.Buffer(6, 1, 0, 0);
	object.Indices.Buffer(2, 1, 0, 2);
	
	// Left face
	object.Indices.Buffer(0, 5, 1, 1);
	object.Indices.Buffer(2, 5, 1, 0);
	object.Indices.Buffer(3, 5, 1, 2);
	
	object.Indices.Buffer(0, 5, 1, 1);
	object.Indices.Buffer(3, 5, 1, 2);
	object.Indices.Buffer(1, 5, 1, 3);
	
	// Top face
	object.Indices.Buffer(2, 2, 2, 0);
	object.Indices.Buffer(6, 2, 2, 2);
	object.Indices.Buffer(7, 2, 2, 3);
	
	object.Indices.Buffer(2, 2, 2, 0);
	object.Indices.Buffer(7, 2, 2, 3);
	object.Indices.Buffer(3, 2, 2, 1);
	
	// Right face
	object.Indices.Buffer(4, 4, 3, 3);
	object.Indices.Buffer(7, 4, 3, 0);
	object.Indices.Buffer(6, 4, 3, 2);
	
	object.Indices.Buffer(4, 4, 3, 3);
	object.Indices.Buffer(5, 4, 3, 1);
	object.Indices.Buffer(7, 4, 3, 0);
	
	// Bottom face
	object.Indices.Buffer(0, 3, 4, 1);
	object.Indices.Buffer(5, 3, 4, 2);
	object.Indices.Buffer(4, 3, 4, 3);
	
	object.Indices.Buffer(0, 3, 4, 1);
	object.Indices.Buffer(1, 3, 4, 0);
	object.Indices.Buffer(5, 3, 4, 2);
	
	// Front face
	object.Indices.Buffer(1, 0, 5, 1);
	object.Indices.Buffer(7, 0, 5, 2);
	object.Indices.Buffer(5, 0, 5, 3);
	
	object.Indices.Buffer(1, 0, 5, 1);
	object.Indices.Buffer(3, 0, 5, 0);
	object.Indices.Buffer(7, 0, 5, 2);
	
	object.Indices.Unlock();
	
	// Load the texture
	std::fstream stream("sd:/Content/tris0.bmp", std::ios::in | std::ios::binary);
	Adventure::File file(stream, Adventure::File::BigEndian);
	object.Texture = Adventure::Image::LoadBmpFromFile(file, system.GetDisplay(), object.ScratchPool);
	
	if (!object.Texture)
	{
		TRACE(DEBUG_GENERAL, "Couldn't allocate texture!");
		return false;
	}
	
	return true;
}

int main(int argumentCount, char** arguments)
{
	Adventure::WiiSystem system;
	Adventure::WiiAllocator baseAllocator;
	Adventure::PooledAllocator texturePool(&baseAllocator, 32, 2560);
	Adventure::PooledAllocator elementPool(&baseAllocator, 32, 2560);
	Adventure::PooledAllocator scratchPool(&baseAllocator, 32, 1280);
	
	system.SetArguments(argumentCount, (const char**)arguments);
	
	if (!system.Initialize())
	{
		TRACE(DEBUG_GENERAL, "System initialized failed!");
		return 1;
	}
	TRACE(DEBUG_GENERAL, "System initialized.");
	
	Adventure::IDisplay& display = system.GetDisplay();
	display.SetTextureAllocator(&texturePool);
	
	Object cube;
	
	cube.TexturePool = &texturePool;
	cube.ElementPool = &elementPool;
	cube.ScratchPool = &scratchPool;
	
	if (!CreateCube(cube, system))
	{
		TRACE(DEBUG_GENERAL, "Couldn't create cube!");
		return false;
	}
	TRACE(DEBUG_GENERAL, "Cube created.");
	
	TRACE(DEBUG_GENERAL, "Memory usage for texture pool: 0x%lx bytes", texturePool.GetMemoryUsage());
	TRACE(DEBUG_GENERAL, "Memory usage for element pool: 0x%lx bytes", elementPool.GetMemoryUsage());
	TRACE(DEBUG_GENERAL, "Memory usage for scratch pool: 0x%lx bytes", scratchPool.GetMemoryUsage());
	
	Adventure::GraphicsMode mode = display.GetGraphicsMode();
	Adventure::Matrix projection = Adventure::Matrix::Perspective(M_PI * 0.25f, (float)mode.Width / (float)mode.Height, 1.0f, 1000.0f);
	Adventure::Matrix view = Adventure::Matrix::LookAt(Adventure::Vector3(0.0f, 0.0f, -8.0f), Adventure::Vector3(0.0f, 0.0f, 0.0f), Adventure::Vector3(0.0f, 1.0f, 0.0f));
	float rotation = 0.0f;
	
	while (true)
	{
		rotation += 0.01f;
		Adventure::Matrix x = Adventure::Matrix::Rotate(Adventure::Vector3(1.0f, 0.0f, 0.0f), rotation);
		Adventure::Matrix z = Adventure::Matrix::Rotate(Adventure::Vector3(0.0f, 0.0f, 1.0f), rotation);
		
		display.Begin();
				
		display.SetProjectionMatrix(projection);
		display.SetModelViewMatrix(view * x * z);
		
		cube.Texture->Bind();
		display.DrawModel(cube.Positions.GetElements(), cube.Normals.GetElements(), cube.Colors.GetElements(), cube.TexCoords.GetElements(), cube.Indices, true);
				
		display.End();
	}
	
	return 0;
}
