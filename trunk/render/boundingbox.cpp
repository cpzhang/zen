#include "boundingbox.h"
#include "common/dxHead.h"
#include "render/rendercontext.h"
#include "render/Colour.h"
BoundingBox::BoundingBox() : min_( FLT_MAX, FLT_MAX, FLT_MAX ),
max_( -FLT_MAX, -FLT_MAX, -FLT_MAX )
{

}

BoundingBox::BoundingBox( const Vector3 & min, const Vector3 & max ) :min_( min ),
max_( max )
{

}

const Vector3 & BoundingBox::minBounds() const
{
	return min_;
}

const Vector3 & BoundingBox::maxBounds() const
{
	return max_;
}

bool BoundingBox::intersectsRay( const Ray & r) const
{
	return D3DXBoxBoundProbe(&min_, &max_, &r.origion_, &r.direction_);
}

void BoundingBox::render(const Matrix& wm, u32 color)
{
	//光照默认打开
	getRenderContex()->getDxDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);
	getRenderContex()->SetTransform(D3DTS_WORLD, &wm);
	getRenderContex()->applyViewMatrix();
	getRenderContex()->applyProjectionMatrix();
	getRenderContex()->setVertexDeclaration(sVDT_PositionColor::getType());
	getRenderContex()->getDxDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//draw
	Vector3 a = min_ + Vector3(-0.005f, -0.005f, -0.005f);
	Vector3 b = max_ + Vector3(0.005f, 0.005f, 0.005f);
	sVDT_PositionColor ps[8];
	ps[0].color_ = ps[1].color_ = ps[2].color_ = ps[3].color_ = ps[4].color_ = ps[5].color_ = ps[6].color_ = ps[7].color_ = color;
	ps[0].position_ = Vector3(a.x, a.y, a.z);
	ps[1].position_ = Vector3(b.x, a.y, a.z);
	ps[2].position_ = Vector3(b.x, a.y, b.z);
	ps[3].position_ = Vector3(a.x, a.y, b.z);
	ps[4].position_ = Vector3(a.x, a.y, a.z);
	getRenderContex()->drawPrimitiveUP(D3DPT_LINESTRIP, 4, &ps[0], sVDT_PositionColor::getSize());
	ps[0].position_ = Vector3(a.x, b.y, a.z);
	ps[1].position_ = Vector3(b.x, b.y, a.z);
	ps[2].position_ = Vector3(b.x, b.y, b.z);
	ps[3].position_ = Vector3(a.x, b.y, b.z);
	ps[4].position_ = Vector3(a.x, b.y, a.z);
	getRenderContex()->drawPrimitiveUP(D3DPT_LINESTRIP, 4, &ps[0], sVDT_PositionColor::getSize());
	ps[0].position_ = Vector3(a.x, a.y, a.z);
	ps[1].position_ = Vector3(a.x, b.y, a.z);
	//
	ps[2].position_ = Vector3(b.x, a.y, a.z);
	ps[3].position_ = Vector3(b.x, b.y, a.z);
	//
	ps[4].position_ = Vector3(b.x, a.y, b.z);
	ps[5].position_ = Vector3(b.x, b.y, b.z);
	//
	ps[6].position_ = Vector3(a.x, a.y, b.z);
	ps[7].position_ = Vector3(a.x, b.y, b.z);
	getRenderContex()->drawPrimitiveUP(D3DPT_LINELIST, 4, &ps[0], sVDT_PositionColor::getSize());
	getRenderContex()->getDxDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}
