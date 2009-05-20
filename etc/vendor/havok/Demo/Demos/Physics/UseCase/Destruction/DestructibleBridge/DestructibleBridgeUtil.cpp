/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/UseCase/Destruction/DestructibleBridge/DestructibleBridgeUtil.h>
#include <Demos/Physics/UseCase/Destruction/DestructibleBridge/DestructibleMoppUtility.h>
#include <Demos/DemoCommon/Utilities/GameUtils/Landscape/FlatLand.h>

#include <Common/Base/Algorithm/Sort/hkSort.h>

#include <Common/Base/Math/Vector/hkVector4Util.h>

#include <Physics/Collide/Shape/Compound/Collection/List/hkpListShape.h>
#include <Physics/Collide/Shape/Convex/ConvexTranslate/hkpConvexTranslateShape.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppBvTreeShape.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppUtility.h>
#include <Physics/Collide/Shape/Convex/hkpConvexShape.h>
#include <Physics/Collide/Shape/Compound/Collection/ExtendedMeshShape/hkpExtendedMeshShape.h>
#include <Physics/Utilities/Destruction/BreakOffParts/hkpBreakOffPartsUtil.h>


//#define USE_MOPP_FOR_AIRPLANE
//#define USE_CONVEX_LIST_SHAPE_FOR_AIRPLANE
#define USE_LIST_SHAPE_FOR_AIRPLANE



#define BRIDGE_ELEMENT_SIZE_X	10.0f
#define BRIDGE_ELEMENT_SIZE_Z	5.0f

#define BRIDGE_NUM_BEAMS		2
#define BRIDGE_BEAM_SIZE_X		BRIDGE_ELEMENT_SIZE_X
#define BRIDGE_BEAM_SIZE_Y		0.5f
#define BRIDGE_BEAM_SIZE_Z		0.5f

#define BRIDGE_NUM_BOARDS		4
#define BRIDGE_BOARD_SIZE_X		BRIDGE_ELEMENT_SIZE_X * 0.2f
#define BRIDGE_BOARD_SIZE_Y		0.25f
#define BRIDGE_BOARD_SIZE_Z		BRIDGE_ELEMENT_SIZE_Z



#define AIRPLANE_BODY_LENGTH	0.75f
#define AIRPLANE_BODY_HEIGHT	0.125f
#define AIRPLANE_BODY_WIDTH		0.125f

#define AIRPLANE_WING_LENGTH	0.5f
#define AIRPLANE_WING_HEIGHT	0.025f
#define AIRPLANE_WING_WIDTH		0.175f

#define AIRPLANE_TAIL_WIDTH		0.5f
#define AIRPLANE_TAIL_HEIGHT	0.025f
#define AIRPLANE_TAIL_LENGTH	0.1f

#define AIRPLANE_RUDDER_WIDTH	0.025f
#define AIRPLANE_RUDDER_HEIGHT	0.125f
#define AIRPLANE_RUDDER_LENGTH	0.075f

#define AIRPLANE_NUM_BOMBS		8
#define AIRPLANE_BOMB_WIDTH		0.075f
#define AIRPLANE_BOMB_HEIGHT	0.075f
#define AIRPLANE_BOMB_LENGTH	0.2f



class BridgeLand : public FlatLand
{
	public:
		BridgeLand(hkReal maxHeight, int numTiles, hkReal canyonWidthPercentage, hkBool smoothBreak);

		hkReal getHeight( int x, int y );
		hkReal getBorderHeight();

		hkpMoppBvTreeShape* createMoppChunkShape(hkpShapeCollection* meshShape);

	protected:

			// maximum depth of the canyon
		hkReal m_maxHeight;

			// canyon's relative half-width; allows for canyons to be smaller than the terrain's total widthmaximum width in relation 
		hkReal m_halfCanyonWidthRatio;

			// if set to true, the valley/canyon walls will be smooth (i.e. a proper cosinus shape)
			// if set to false, the valley/canyon walls will be almost vertical (i.e. flat until we reach the canyon's edge, then vertical, then a proper remaining cosinus shape)
		hkBool m_smoothBreak;
};

BridgeLand::BridgeLand(hkReal maxHeight, int numTiles, hkReal canyonWidthRatio, hkBool smoothBreak) : FlatLand(numTiles)
{
	HK_ASSERT2(0xaf25fe34, canyonWidthRatio > 0.0f && canyonWidthRatio < 1.0f, "The canyon's width ratio must be between (and excluding) 0 and 1.");

	m_maxHeight				= maxHeight;
	m_halfCanyonWidthRatio	= canyonWidthRatio * 0.5f;
	m_smoothBreak			= smoothBreak;
}

hkReal BridgeLand::getHeight( int x, int y )
{
	hkReal percentageX = hkReal(x) / hkReal(m_side);

	// flatten terrain if we are outside of canyon area
	if ( percentageX < (0.5f - m_halfCanyonWidthRatio) || percentageX > (0.5f + m_halfCanyonWidthRatio) )
	{
		return 0.0f;
	}

	// smoothen out slope if flag is set
	if ( m_smoothBreak )
	{
		percentageX = (percentageX - (0.5f - m_halfCanyonWidthRatio)) / (m_halfCanyonWidthRatio*2.0f);
	}

	hkReal adjustedCosinus = (hkMath::cos(percentageX*2.0f*HK_REAL_PI) - 1.0f) * 0.5f;

	return adjustedCosinus * m_maxHeight;
}

hkReal BridgeLand::getBorderHeight()
{
	return 0.0f;
}

hkpMoppBvTreeShape* BridgeLand::createMoppChunkShape(hkpShapeCollection* meshShape)
{
	hkpMoppCompilerInput mci;
	mci.m_enableChunkSubdivision = true;

	mci.m_cachePrimitiveExtents = m_cachedBuildEnabled;
	hkpMoppCode* code = hkpMoppUtility::buildCode( meshShape , mci);
	hkpMoppBvTreeShape* moppShape = new hkpMoppBvTreeShape(meshShape, code);

	code->removeReference();
	meshShape->removeReference();

	return moppShape;
}

DestructibleBridgeUtil::DestructibleBridgeUtil()
{
}

DestructibleBridgeUtil::~DestructibleBridgeUtil()
{
}

hkpRigidBody* DestructibleBridgeUtil::createTerrainAndBridges(hkpBreakOffPartsUtil* breakOffUtil, const TerrainData& data, hkReferencedObject*& terrainDataOut )
{
	//
	// create terrain shape
	//
	hkpMoppBvTreeShape* terrainShape;
	hkInplaceArray<hkpShapeKey,512> breakableKeys;

	{
		BridgeLand* terrainBuilder;
		{
			terrainBuilder = new BridgeLand(data.m_maxHeight, data.m_numTiles, data.m_canyonWidthRatio, data.m_smoothBreak); 
			terrainBuilder->setScaling(data.m_scale);
			terrainBuilder->enableCachedBuild(true);
		}

		hkpExtendedMeshShape* meshShape = terrainBuilder->createExtendedMeshShape();

		//
		// add the bridge(s)
		//
		{
			hkArray<hkpConvexShape*> shapes;
			createBridge(shapes);

			hkReal posZ = data.m_numBridges*0.5f * -BRIDGE_ELEMENT_SIZE_Z * 4.0f;
			{
				hkArray<hkpConvexShape*> translatedShapes;
				for (int i=0; i<data.m_numBridges; i++)
				{
					translatedShapes.clearAndDeallocate();
					
					//The getChildShape function of ExtendedMeshShape will add an hkpConvexTranslateShape or 
					//  hkpConvexTransformShape if the ShapesSubpart.m_transform is not the identity matrix.  This would 
					//  result in nesting multiple hkpConvexTransformShapes or hkpConvexTranslateShapes.  The following 
					//  code adds the translation to each hkpConvexTranslateShape Shape directly to avoid nesting 
					//  hkpConvexTranslateShapes.
					hkTransform transform;
					transform.getTranslation().setZero4();
					transform.getRotation().setIdentity();
					{
						for ( int x = 0; x < shapes.getSize(); x++ )
						{		
							hkpConvexShape* tempShape = shapes[x];
							if( shapes[x]->getType() == HK_SHAPE_CONVEX_TRANSLATE )
							{																
								hkpConvexTranslateShape* translateShape = static_cast<hkpConvexTranslateShape*> ( shapes[x] );
								hkVector4 translation = translateShape->getTranslation();
								translation(2) += posZ;
								hkpConvexTranslateShape* newTranslateShape = new hkpConvexTranslateShape(translateShape->getChildShape(),translation );
								tempShape = newTranslateShape;
							}							
							translatedShapes.pushBack(tempShape );
						}											
					}
					hkpExtendedMeshShape::ShapesSubpart part(translatedShapes.begin(), translatedShapes.getSize(), transform);					
					
					int numSubParts = meshShape->getNumTrianglesSubparts() + meshShape->getNumShapesSubparts();
					hkpShapeKey keyBase = meshShape->getSubpartShapeKeyBase( numSubParts );

					meshShape->addShapesSubpart( part );
					
					for (int s =0; s < translatedShapes.getSize(); s++ )
					{
						breakableKeys.pushBack( keyBase + s );
					}					

					for (int x = 0; x < translatedShapes.getSize(); x++)
					{
						translatedShapes[x]->removeReference();
					}

					posZ += BRIDGE_ELEMENT_SIZE_Z * 4.0f;
				}
			}

			{
				for (int i=0; i<shapes.getSize(); i++)
				{
					shapes[i]->removeReference();
				}
				shapes.clear();
			}

		}

		terrainShape = terrainBuilder->createMoppChunkShape(meshShape);
		meshShape->computeWeldingInfo( terrainShape, hkpWeldingUtility::WELDING_TYPE_ANTICLOCKWISE );

		terrainDataOut = terrainBuilder;
	}

	//
	// create terrain object
	//
	hkpRigidBody* terrain;
	{
		hkpRigidBodyCinfo terrainInfo;
		{
			terrainInfo.m_shape			= terrainShape;
			terrainInfo.m_position		= data.m_position;
			terrainInfo.m_motionType	= hkpMotion::MOTION_FIXED;
			terrainInfo.m_friction		= 0.5f;
			terrainInfo.m_numUserDatasInContactPointProperties = 1;
		}

		terrain = new hkpRigidBody(terrainInfo);

		terrainShape->removeReference();
	}

	//
	//	Flag pieces breakable
	//
	{
		for ( int s = 0; s < breakableKeys.getSize(); s++ )
		{
			breakOffUtil->markPieceBreakable( terrain, breakableKeys[s], data.m_maxBreakOffImpulse );
		}
	}

	return terrain;
}



void DestructibleBridgeUtil::createSingleBoardOrBeam(const hkpConvexShape* shape, const hkVector4& position, hkArray<hkpConvexShape*>& shapeArray)
{
	hkpConvexTranslateShape* translatedShape = new hkpConvexTranslateShape(shape, position);
	shapeArray.pushBack(translatedShape);
}

void DestructibleBridgeUtil::createBridgeElementLevel2(const hkpConvexShape* beamShape, const hkpConvexShape* boardShape, const hkVector4& elementCenter, hkArray<hkpConvexShape*>& shapeArray)
{
	hkReal halfBridgeSizeX = BRIDGE_ELEMENT_SIZE_X * 0.5f;
	hkReal halfBridgeSizeY = BRIDGE_ELEMENT_SIZE_Z * 0.5f;

	// create beams
	{
		hkVector4 beamPos;

		beamPos = elementCenter;
		beamPos(1) -= (BRIDGE_BOARD_SIZE_Y + BRIDGE_BEAM_SIZE_Y) * 0.5f;
		beamPos(2) -= (halfBridgeSizeY * 0.65f);
		createSingleBoardOrBeam(beamShape, beamPos, shapeArray);

		beamPos = elementCenter;
		beamPos(1) -= (BRIDGE_BOARD_SIZE_Y + BRIDGE_BEAM_SIZE_Y) * 0.5f;
		beamPos(2) += (halfBridgeSizeY * 0.65f);
		createSingleBoardOrBeam(beamShape, beamPos, shapeArray);
	}

	// create boards
	{
		hkVector4 boardPos = elementCenter;

		boardPos(0) -= (halfBridgeSizeX * 0.75f);

		createSingleBoardOrBeam(boardShape, boardPos, shapeArray);
		boardPos(0) += (halfBridgeSizeX * 0.5f);

		createSingleBoardOrBeam(boardShape, boardPos, shapeArray);
		boardPos(0) += (halfBridgeSizeX * 0.5f);

		createSingleBoardOrBeam(boardShape, boardPos, shapeArray);
		boardPos(0) += (halfBridgeSizeX * 0.5f);

		createSingleBoardOrBeam(boardShape, boardPos, shapeArray);
	}

}

void DestructibleBridgeUtil::createBridgeElementLevel1(const hkpConvexShape* beamShape, const hkpConvexShape* boardShape, const hkVector4& elementCenter, hkArray<hkpConvexShape*>& shapeArray)
{
	hkReal halfBridgeSizeY = BRIDGE_ELEMENT_SIZE_Z * 0.5f;

	{
		hkVector4 subElemPos;

		subElemPos = elementCenter;
		subElemPos(2) -= halfBridgeSizeY * 1.0f;
		createBridgeElementLevel2(beamShape, boardShape, subElemPos, shapeArray);

		subElemPos = elementCenter;
		subElemPos(2) += halfBridgeSizeY * 1.0f;
		createBridgeElementLevel2(beamShape, boardShape, subElemPos, shapeArray);
	}

}

void DestructibleBridgeUtil::createBridgeElementLevel0(const hkpConvexShape* beamShape, const hkpConvexShape* boardShape, const hkVector4& elementCenter, hkArray<hkpConvexShape*>& shapeArray)
{
	hkReal posX = elementCenter(0);
	posX -= 3.5f * BRIDGE_ELEMENT_SIZE_X;
	{
		for (int i=0; i<8; i++)
		{
			hkVector4 subElemPos;

			subElemPos = elementCenter;
			subElemPos(0) = posX + BRIDGE_ELEMENT_SIZE_X * i;
			createBridgeElementLevel1(beamShape, boardShape, subElemPos, shapeArray);
		}
	}

}

void DestructibleBridgeUtil::createBridge(hkArray<hkpConvexShape*>& shapes)
{
	hkpBoxShape* beamShape;
	{
		hkVector4 beamSize(BRIDGE_BEAM_SIZE_X*0.5f, BRIDGE_BEAM_SIZE_Y*0.5f, BRIDGE_BEAM_SIZE_Z*0.5f);
		beamShape = new hkpBoxShape(beamSize, 0);
	}

	hkpBoxShape* boardShape;
	{
		hkVector4 boardSize(BRIDGE_BOARD_SIZE_X*0.5f, BRIDGE_BOARD_SIZE_Y*0.5f, BRIDGE_BOARD_SIZE_Z*0.5f);
		boardShape = new hkpBoxShape(boardSize, 0);
	}

	hkVector4 position(0.0f, 0.0f, 0.0f);
	createBridgeElementLevel0(beamShape, boardShape, position, shapes);

	beamShape->removeReference();
	boardShape->removeReference();
}



hkpBoxShape* DestructibleBridgeUtil::createBoxShape(hkReal width, hkReal height, hkReal length, hkReal scale)
{
	hkVector4 bodySize(width, height, length);
	bodySize.mul4(0.5f * scale);
	return new hkpBoxShape(bodySize, 0);
}

void DestructibleBridgeUtil::createConvexTranslateShapeAndAddToArray(hkpBoxShape* shape, hkReal posX, hkReal posY, hkReal posZ, hkReal scale, hkArray<hkpShape*>& shapes)
{
	hkVector4 position(posX, posY, posZ);
	position.mul4(scale);
	hkpConvexTranslateShape* translatedShape = new hkpConvexTranslateShape(shape, position);
	shapes.pushBack(translatedShape);
}

hkpRigidBody* DestructibleBridgeUtil::createAirplane(const AirplaneData& data)
{

	//
	// create all necessary shapes that make up our airplane
	//
	hkArray<hkpShape*> shapes;
	{
		// create airplane's body
		{
			hkpBoxShape* bodyShape = createBoxShape(AIRPLANE_BODY_WIDTH, AIRPLANE_BODY_HEIGHT, AIRPLANE_BODY_LENGTH, data.m_wingSpan);
			createConvexTranslateShapeAndAddToArray(bodyShape, 0.0f, 0.0f, 0.0f, data.m_wingSpan, shapes);
			bodyShape->removeReference();
		}

		// create airplane's left and right wing
		{
			hkpBoxShape* wingShape = createBoxShape(AIRPLANE_WING_LENGTH, AIRPLANE_WING_HEIGHT, AIRPLANE_WING_WIDTH, data.m_wingSpan);
			createConvexTranslateShapeAndAddToArray(wingShape, -AIRPLANE_WING_LENGTH * 0.5f, (AIRPLANE_BODY_HEIGHT+AIRPLANE_WING_HEIGHT) * 0.5f, AIRPLANE_BODY_LENGTH / 6.0f, data.m_wingSpan, shapes);
			createConvexTranslateShapeAndAddToArray(wingShape,  AIRPLANE_WING_LENGTH * 0.5f, (AIRPLANE_BODY_HEIGHT+AIRPLANE_WING_HEIGHT) * 0.5f, AIRPLANE_BODY_LENGTH / 6.0f, data.m_wingSpan, shapes);
			wingShape->removeReference();
		}

		// create airplane's tail wing
		{
			hkpBoxShape* tailWingShape = createBoxShape(AIRPLANE_TAIL_WIDTH, AIRPLANE_TAIL_HEIGHT, AIRPLANE_TAIL_LENGTH, data.m_wingSpan);
			createConvexTranslateShapeAndAddToArray(tailWingShape, 0.0f, (AIRPLANE_BODY_HEIGHT+AIRPLANE_WING_HEIGHT) * 0.5f, -AIRPLANE_BODY_LENGTH * 0.5f, data.m_wingSpan, shapes);
			tailWingShape->removeReference();
		}

		// create airplane's rudder
		{
			hkpBoxShape* rudderShape = createBoxShape(AIRPLANE_RUDDER_WIDTH, AIRPLANE_RUDDER_HEIGHT, AIRPLANE_RUDDER_LENGTH, data.m_wingSpan);
			createConvexTranslateShapeAndAddToArray(rudderShape, 0.0f, (AIRPLANE_BODY_HEIGHT+AIRPLANE_RUDDER_HEIGHT) * 0.5f, -AIRPLANE_BODY_LENGTH * 0.5f, data.m_wingSpan, shapes);
			rudderShape->removeReference();
		}

		// create the bombs
		{
			hkpBoxShape* bombShape = createBoxShape(AIRPLANE_BOMB_WIDTH, AIRPLANE_BOMB_HEIGHT, AIRPLANE_BOMB_LENGTH, data.m_wingSpan);

			{
				hkReal distance = (AIRPLANE_WING_LENGTH*2.0f) / AIRPLANE_NUM_BOMBS;
				hkReal posX = AIRPLANE_WING_LENGTH-AIRPLANE_BOMB_WIDTH;
				{
					for (int i=0; i<AIRPLANE_NUM_BOMBS; i++)
					{
						createConvexTranslateShapeAndAddToArray(bombShape, posX, (AIRPLANE_BODY_HEIGHT-AIRPLANE_BOMB_HEIGHT) * 0.5f, AIRPLANE_BODY_LENGTH / 6.0f, data.m_wingSpan, shapes);
						posX -= distance;
					}
				}
			}

			bombShape->removeReference();
		}
	}

	//
	// create the complete shape for the airplane
	//
	hkpShape* airplaneShape;
	{
#if defined(USE_MOPP_FOR_AIRPLANE)
		hkpListShape* listShape = new hkpListShape(shapes.begin(), shapes.getSize());

		hkpMoppCompilerInput mci;
		mci.m_enableChunkSubdivision = true;

		hkpMoppCode* code = hkpMoppUtility::buildCode( listShape , mci);
		airplaneShape = new hkpMoppBvTreeShape(listShape, code);
		listShape->removeReference();
#elif defined(USE_CONVEX_LIST_SHAPE_FOR_AIRPLANE)
		airplaneShape = new hkpConvexListShape( (const hkpConvexShape*const*)shapes.begin(), shapes.getSize());
#else // if defined(USE_LIST_SHAPE_FOR_AIRPLANE)
		airplaneShape = new hkpListShape(shapes.begin(), shapes.getSize());
#endif

		{
			for (int i=0; i<shapes.getSize(); i++)
			{
				shapes[i]->removeReference();
			}
		}
	}

	//
	// create airplane
	//
	hkVector4 airplaneDirection;
	hkpRigidBody* airplane;
	{
		hkpRigidBodyCinfo planeInfo;
		{
			planeInfo.m_shape			= airplaneShape;
			planeInfo.m_position		= data.m_position;
			planeInfo.m_motionType		= hkpMotion::MOTION_DYNAMIC;
			planeInfo.m_friction		= 0.5f;
			planeInfo.m_qualityType		= HK_COLLIDABLE_QUALITY_MOVING;
			planeInfo.m_angularDamping	= 0.7f;
			hkpInertiaTensorComputer::setShapeVolumeMassProperties(airplaneShape, data.m_mass, planeInfo);
		}

		airplaneDirection = data.m_destination;
		airplaneDirection.sub4(data.m_position);
		airplaneDirection.normalize3();

		hkRotation rotation;
		{
			hkVector4 upVector(0.0f, 1.0f, 0.0f);
			hkVector4Util::buildOrthonormal(airplaneDirection, upVector, rotation);
			rotation.getColumn(2).setNeg4(rotation.getColumn(2));
			hkAlgorithm::swap16(rotation.getColumn(0), rotation.getColumn(2));
		}
		planeInfo.m_rotation.setAndNormalize(rotation);

		airplane = new hkpRigidBody(planeInfo);

		airplaneShape->removeReference();
	}

	//
	// push airplane
	//
	{
		hkVector4 velocityVec;
		velocityVec.setMul4( data.m_velocity, airplaneDirection );
		airplane->setLinearVelocity( velocityVec );
	}

	return airplane;
}



/*
* Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20090216)
* 
* Confidential Information of Havok.  (C) Copyright 1999-2009
* Telekinesys Research Limited t/a Havok. All Rights Reserved. The Havok
* Logo, and the Havok buzzsaw logo are trademarks of Havok.  Title, ownership
* rights, and intellectual property rights in the Havok software remain in
* Havok and/or its suppliers.
* 
* Use of this software for evaluation purposes is subject to and indicates
* acceptance of the End User licence Agreement for this product. A copy of
* the license is included with this software and is also available at www.havok.com/tryhavok.
* 
*/
