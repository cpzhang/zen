#pragma once
class TerrainBlock
{
public:
private:
	TerrainTextureLayers	textureLayers_;
	VertexBuffer			vertexBuffer_;
	IndexBuffer				indexBuffer_;

	uint32					nVertices_;
	uint32					nIndices_;
	uint32					nPrimitives_;
};