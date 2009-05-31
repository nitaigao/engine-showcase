#ifndef LINE3D_H__
#define LINE3D_H__

#include "Ogre.h"
#include <vector>

#define POSITION_BINDING 0
#define TEXCOORD_BINDING 1

namespace Renderer
{
	class Line3D : public Ogre::SimpleRenderable
	{

	public:

		Line3D( const std::string& name );

		~Line3D(void);

		void addPoint(const Ogre::Vector3 &p);
		const Ogre::Vector3 &getPoint(unsigned short index) const;
		unsigned short getNumPoints(void) const;
		void updatePoint(unsigned short index, const Ogre::Vector3 &value);
		void drawLine(Ogre::Vector3 &start, Ogre::Vector3 &end);
		void drawLines(void);

		Ogre::Real getSquaredViewDepth(const Ogre::Camera *cam) const;
		Ogre::Real getBoundingRadius(void) const;

		static std::string TypeName( ) { return "Line"; };

	protected:

		//void getWorldTransforms(Matrix4 *xform) const;
		const Ogre::Quaternion &getWorldOrientation(void) const;
		const Ogre::Vector3 &getWorldPosition(void) const;

		std::vector<Ogre::Vector3> mPoints;

		bool mDrawn;
	};
};

#endif /* __LINE3D_H__ */