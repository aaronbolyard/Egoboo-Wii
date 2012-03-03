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

#include <cstdlib>
#include <cstring>
#include <malloc.h>
#include <ogc/system.h>

#include "Matrix.hpp"
#include "WiiDisplay.hpp"
#include "WiiTexture.hpp"

Adventure::WiiDisplay::WiiDisplay()
{
	renderMode = NULL;
	
	framebuffers[0] = NULL;
	framebuffers[1] = NULL;
	framebufferIndex = 0;
	
	graphicsFifo = 0;
}

Adventure::WiiDisplay::~WiiDisplay()
{
}

bool Adventure::WiiDisplay::SetGraphicsMode(const GraphicsMode& mode)
{
	// There is only one graphics mode on the Wii, which is set by the console itself
	return true;
}

bool Adventure::WiiDisplay::Initialize()
{
	VIDEO_Init();
	
	renderMode = VIDEO_GetPreferredMode(NULL);
	
	framebuffers[0] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(renderMode));
	framebuffers[1] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(renderMode));
	
	VIDEO_Configure(renderMode);
	VIDEO_SetNextFramebuffer(framebuffers[framebufferIndex]);
	VIDEO_SetBlack(false);
	VIDEO_Flush();
	
	VIDEO_WaitVSync();
	
	if (renderMode->viTVMode & VI_NON_INTERLACE)
		VIDEO_WaitVSync();
	
	graphicsFifo = memalign(32, DefaultGraphicsFifoSize);
	memset(graphicsFifo, 0, DefaultGraphicsFifoSize);
	
	GX_Init(graphicsFifo, DefaultGraphicsFifoSize);
	
	// Routine GX initialization
	GXColor backgroundColor = { 0, 0, 0, 0xFF };
	GX_SetCopyClear(backgroundColor, 0x00FFFFFF);
	
	int yScale = GX_GetYScaleFactor(renderMode->efbHeight, renderMode->xfbHeight);
	int externalFramebufferHeight = GX_SetDispCopyYScale(yScale);
	
	GX_SetScissor(0, 0, renderMode->fbWidth, renderMode->efbHeight);
	
	GX_SetDispCopySrc(0, 0, renderMode->fbWidth, renderMode->efbHeight);
	GX_SetDispCopyDst(renderMode->fbWidth, externalFramebufferHeight);
	GX_SetCopyFilter(renderMode->aa, renderMode->sample_pattern, GX_TRUE, renderMode->vfilter);
	
	bool halfAspect = (renderMode->viHeight == renderMode->xfbHeight * 2);
	GX_SetFieldMode(renderMode->field_rendering, halfAspect ? GX_ENABLE : GX_DISABLE);
	
	if (renderMode->aa)
		GX_SetPixelFmt(GX_PF_RGB565_Z16, GX_ZC_LINEAR);
	else
		GX_SetPixelFmt(GX_PF_RGB8_Z24, GX_ZC_LINEAR);
		
	// Prepare the three vertex format types
	GX_ClearVtxDesc();
	
	GX_SetVtxAttrFmt(ModelFormat, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GX_SetVtxAttrFmt(ModelFormat, GX_VA_NRM, GX_NRM_XYZ, GX_F32, 0);
	GX_SetVtxAttrFmt(ModelFormat, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
	GX_SetVtxAttrFmt(ModelFormat, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
	
	GX_SetVtxAttrFmt(CompressedModelFormat, GX_VA_POS, GX_POS_XYZ, GX_S16, 7);
	GX_SetVtxAttrFmt(CompressedModelFormat, GX_VA_NRM, GX_NRM_XYZ, GX_S16, 14);
	GX_SetVtxAttrFmt(CompressedModelFormat, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
	GX_SetVtxAttrFmt(CompressedModelFormat, GX_VA_TEX0, GX_TEX_ST, GX_S16, 7);
	
	GX_SetVtxAttrFmt(ParticleFormat, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GX_SetVtxAttrFmt(ParticleFormat, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
	GX_SetVtxAttrFmt(ParticleFormat, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
	
	GX_SetVtxAttrFmt(CompressedParticleFormat, GX_VA_POS, GX_POS_XYZ, GX_S16, 7);
	GX_SetVtxAttrFmt(CompressedParticleFormat, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
	GX_SetVtxAttrFmt(CompressedParticleFormat, GX_VA_TEX0, GX_TEX_ST, GX_S16, 7);
	
	GX_SetVtxAttrFmt(SpriteFormat, GX_VA_POS, GX_POS_XY, GX_F32, 0);
	GX_SetVtxAttrFmt(SpriteFormat, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
	GX_SetVtxAttrFmt(SpriteFormat, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
	
	GX_SetVtxAttrFmt(CompressedSpriteFormat, GX_VA_POS, GX_POS_XY, GX_S16, 7);
	GX_SetVtxAttrFmt(CompressedSpriteFormat, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
	GX_SetVtxAttrFmt(CompressedSpriteFormat, GX_VA_TEX0, GX_TEX_ST, GX_S16, 7);
	
	return true;
}

void Adventure::WiiDisplay::Begin()
{
	if (renderMode->field_rendering)
		GX_SetViewportJitter(0.0f, 0.0f, renderMode->fbWidth, renderMode->efbHeight, 0.0f, 1.0f, VIDEO_GetNextField());
	else
		GX_SetViewport(0.0f, 0.0f, renderMode->fbWidth, renderMode->efbHeight, 0.0f, 1.0f);
		
	// Prepare the TEV
	GX_SetNumChans(1);
	GX_SetNumTexGens(1);
	GX_SetNumTevStages(1);
	
	GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0);
	GX_SetTevOp(GX_TEVSTAGE0, GX_BLEND);
}

Adventure::ITexture* Adventure::WiiDisplay::CreateTexture()
{
	return new WiiTexture(*this);
}

void Adventure::WiiDisplay::End()
{
	// Swap the buffers
	framebufferIndex ^= 1;
	
	// Copy the scene to the provided framebuffer
	GX_CopyDisp(framebuffers[framebufferIndex], GX_TRUE);
	GX_DrawDone();
	
	// Tell the video subsystem to use the provided buffer for the next frame
	VIDEO_SetNextFramebuffer(framebuffers[framebufferIndex]);
	VIDEO_Flush();
	VIDEO_WaitVSync();
}

void Adventure::WiiDisplay::DrawModel(const void* positions, const void* normals, const void* materials, const void* uvs, const ModelIndexArray& indexArray, bool compressed)
{
	GX_ClearVtxDesc();
	
	GX_SetVtxDesc(GX_VA_POS, GX_INDEX16);
	GX_SetVtxDesc(GX_VA_NRM, GX_INDEX16);
	GX_SetVtxDesc(GX_VA_CLR0, GX_INDEX16);
	GX_SetVtxDesc(GX_VA_TEX0, GX_INDEX16);
	
	GX_SetArray(GX_VA_POS, (void*)positions, compressed ? CompressedVector3Array::ElementStride : Vector3Array::ElementStride);
	GX_SetArray(GX_VA_NRM, (void*)normals, compressed ? CompressedNormalArray::ElementStride : NormalArray::ElementStride);
	GX_SetArray(GX_VA_CLR0, (void*)materials, ColorArray::ElementStride);
	GX_SetArray(GX_VA_TEX0, (void*)uvs, compressed ? CompressedVector2Array::ElementStride : Vector2Array::ElementStride);
	
	GX_Begin(GX_TRIANGLES, compressed ? CompressedModelFormat : ModelFormat, indexArray.GetElementCount());
	
	const unsigned short* indices = indexArray.GetElements();
	for (int i = 0; i < indexArray.GetElementCount(); i += ModelIndexArray::ElementMembers)
	{
		GX_Position1x16(indices[i]);
		GX_Normal1x16(indices[i + 1]);
		GX_Color1x16(indices[i + 2]);
		GX_TexCoord1x16(indices[i + 3]);
	}
	
	GX_End();
}

void Adventure::WiiDisplay::SetProjectionMatrix(const Matrix& matrix, ProjectionHint hint)
{
	union
	{
		float matrix[16];
		Mtx44 array;
	} projectionMatrix;
	
	matrix.ToFloat16(projectionMatrix.matrix);
	
	if (hint == Perspective)
		GX_LoadProjectionMtx(projectionMatrix.array, GX_PERSPECTIVE);
	else
		GX_LoadProjectionMtx(projectionMatrix.array, GX_ORTHOGRAPHIC);
}

void Adventure::WiiDisplay::SetModelViewMatrix(const Matrix& matrix)
{
	union
	{
		float matrix[16];
		Mtx array;
	} modelViewMatrix;
	
	matrix.ToFloat16(modelViewMatrix.matrix);
	
	GX_LoadPosMtxImm(modelViewMatrix.array, GX_PNMTX0);
	GX_SetCurrentMtx(GX_PNMTX0);
}
