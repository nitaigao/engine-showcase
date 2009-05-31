#include "Line3D.h"

using namespace Ogre;
using namespace std;

namespace Renderer
{
	Line3D::Line3D( const std::string& name )
	{
		mRenderOp.vertexData = new VertexData();
		mDrawn = false;

		mName = name;

		this->setMaterial("BaseWhiteNoLighting");
	}

	Line3D::~Line3D(void)
	{
		delete mRenderOp.vertexData;
	}

	void Line3D::addPoint(const Vector3 &p)
	{
		mPoints.push_back(p);
	}

	const Vector3 &Line3D::getPoint(unsigned short index) const
	{
		assert(index < mPoints.size() && "Point index is out of bounds!!");

		return mPoints[index];
	}

	unsigned short Line3D::getNumPoints(void) const
	{
		return (unsigned short)mPoints.size();
	}

	void Line3D::updatePoint(unsigned short index, const Vector3 &value)
	{
		assert(index < mPoints.size() && "Point index is out of bounds!!");

		mPoints[index] = value;
	}

	void Line3D::drawLine(Vector3 &start, Vector3 &end)
	{
		if(mPoints.size())
			mPoints.clear();

		mPoints.push_back(start);
		mPoints.push_back(end);

		drawLines();
	}

	void Line3D::drawLines(void)
	{
		if(mDrawn)
			return;
		else
			mDrawn = true;

		// Initialization stuff
		mRenderOp.indexData = 0;
		mRenderOp.vertexData->vertexCount = mPoints.size();
		mRenderOp.vertexData->vertexStart = 0;
		mRenderOp.operationType = RenderOperation::OT_LINE_STRIP; // OT_LINE_LIST, OT_LINE_STRIP
		mRenderOp.useIndexes = false;

		VertexDeclaration *decl = mRenderOp.vertexData->vertexDeclaration;
		VertexBufferBinding *bind = mRenderOp.vertexData->vertexBufferBinding;

		decl->addElement(POSITION_BINDING, 0, VET_FLOAT3, VES_POSITION);

		HardwareVertexBufferSharedPtr vbuf =
			HardwareBufferManager::getSingleton().createVertexBuffer(
			decl->getVertexSize(POSITION_BINDING),
			mRenderOp.vertexData->vertexCount,
			HardwareBuffer::HBU_STATIC_WRITE_ONLY);

		bind->setBinding(POSITION_BINDING, vbuf);

		// Drawing stuff
		int size = mPoints.size();
		Vector3 vaabMin = mPoints[0];
		Vector3 vaabMax = mPoints[0];

		Real *prPos = static_cast<Real*>(vbuf->lock(HardwareBuffer::HBL_DISCARD));

		for(int i = 0; i < size; i++)
		{
			*prPos++ = mPoints[i].x;
			*prPos++ = mPoints[i].y;
			*prPos++ = mPoints[i].z;

			if(mPoints[i].x < vaabMin.x)
				vaabMin.x = mPoints[i].x;
			if(mPoints[i].y < vaabMin.y)
				vaabMin.y = mPoints[i].y;
			if(mPoints[i].z < vaabMin.z)
				vaabMin.z = mPoints[i].z;

			if(mPoints[i].x > vaabMax.x)
				vaabMax.x = mPoints[i].x;
			if(mPoints[i].y > vaabMax.y)
				vaabMax.y = mPoints[i].y;
			if(mPoints[i].z > vaabMax.z)
				vaabMax.z = mPoints[i].z;
		}

		vbuf->unlock();

		mBox.setExtents(vaabMin, vaabMax);
	}

	Real Line3D::getSquaredViewDepth(const Camera *cam) const
	{
		Vector3 vMin, vMax, vMid, vDist;
		vMin = mBox.getMinimum();
		vMax = mBox.getMaximum();
		vMid = ((vMin - vMax) * 0.5) + vMin;
		vDist = cam->getDerivedPosition() - vMid;

		return vDist.squaredLength();
	}

	Real Line3D::getBoundingRadius(void) const
	{
		return Math::Sqrt(max(mBox.getMaximum().squaredLength(), mBox.getMinimum().squaredLength()));
	}

	const Quaternion &Line3D::getWorldOrientation(void) const
	{
		return Quaternion::IDENTITY;
	}

	const Vector3 &Line3D::getWorldPosition(void) const
	{
		return Vector3::ZERO;
	}
}