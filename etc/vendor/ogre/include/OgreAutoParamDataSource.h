/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org

Copyright (c) 2000-2006 Torus Knot Software Ltd
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.

You may alternatively use this source under the terms of a specific version of
the OGRE Unrestricted License provided you have obtained such a license from
Torus Knot Software Ltd.
-----------------------------------------------------------------------------
*/
#ifndef __AutoParamDataSource_H_
#define __AutoParamDataSource_H_

#include "OgrePrerequisites.h"
#include "OgreCommon.h"
#include "OgreMatrix4.h"
#include "OgreVector4.h"
#include "OgreLight.h"
#include "OgreColourValue.h"

namespace Ogre {

	// forward decls
	struct VisibleObjectsBoundsInfo;


    /** This utility class is used to hold the information used to generate the matrices
    and other information required to automatically populate GpuProgramParameters.
    @remarks
        This class exercises a lazy-update scheme in order to avoid having to update all
        the information a GpuProgramParameters class could possibly want all the time. 
        It relies on the SceneManager to update it when the base data has changed, and
        will calculate concatenated matrices etc only when required, passing back precalculated
        matrices when they are requested more than once when the underlying information has
        not altered.
    */
    class _OgreExport AutoParamDataSource
    {
    protected:
        mutable Matrix4 mWorldMatrix[256];
        mutable size_t mWorldMatrixCount;
        mutable const Matrix4* mWorldMatrixArray;
        mutable Matrix4 mWorldViewMatrix;
        mutable Matrix4 mViewProjMatrix;
        mutable Matrix4 mWorldViewProjMatrix;
        mutable Matrix4 mInverseWorldMatrix;
        mutable Matrix4 mInverseWorldViewMatrix;
        mutable Matrix4 mInverseViewMatrix;
        mutable Matrix4 mInverseTransposeWorldMatrix;
        mutable Matrix4 mInverseTransposeWorldViewMatrix;
        mutable Vector4 mCameraPositionObjectSpace;
        mutable Matrix4 mTextureViewProjMatrix[OGRE_MAX_SIMULTANEOUS_LIGHTS];
        mutable Matrix4 mViewMatrix;
        mutable Matrix4 mProjectionMatrix;
		mutable Real mDirLightExtrusionDistance;
        mutable Vector4 mCameraPosition;

        mutable bool mWorldMatrixDirty;
        mutable bool mViewMatrixDirty;
        mutable bool mProjMatrixDirty;
        mutable bool mWorldViewMatrixDirty;
        mutable bool mViewProjMatrixDirty;
        mutable bool mWorldViewProjMatrixDirty;
        mutable bool mInverseWorldMatrixDirty;
        mutable bool mInverseWorldViewMatrixDirty;
        mutable bool mInverseViewMatrixDirty;
        mutable bool mInverseTransposeWorldMatrixDirty;
        mutable bool mInverseTransposeWorldViewMatrixDirty;
        mutable bool mCameraPositionObjectSpaceDirty;
        mutable bool mCameraPositionDirty;
        mutable bool mTextureViewProjMatrixDirty[OGRE_MAX_SIMULTANEOUS_LIGHTS];
		mutable ColourValue mAmbientLight;
        mutable ColourValue mFogColour;
        mutable Vector4 mFogParams;
        mutable int mPassNumber;
		mutable Vector4 mSceneDepthRange;
		mutable bool mSceneDepthRangeDirty;
		// Ordered by light, populated on demand, dirtied when lights change
		mutable std::vector<Vector4> mShadowCamDepthRanges;
		mutable bool mShadowCamDepthRangesDirty;

        const Renderable* mCurrentRenderable;
        const Camera* mCurrentCamera;
        const LightList* mCurrentLightList;
        const Frustum* mCurrentTextureProjector[OGRE_MAX_SIMULTANEOUS_LIGHTS];
        const RenderTarget* mCurrentRenderTarget;
        const Viewport* mCurrentViewport;
		const SceneManager* mCurrentSceneManager;
		const VisibleObjectsBoundsInfo* mMainCamBoundsInfo;
        const Pass* mCurrentPass;

        Light mBlankLight;
    public:
        AutoParamDataSource();
        ~AutoParamDataSource();
        /** Updates the current renderable */
        void setCurrentRenderable(const Renderable* rend);
        /** Sets the world matrices, avoid query from renderable again */
        void setWorldMatrices(const Matrix4* m, size_t count);
        /** Updates the current camera */
        void setCurrentCamera(const Camera* cam);
        /** Sets the light list that should be used */
        void setCurrentLightList(const LightList* ll);
        /** Sets the current texture projector for a index */
        void setTextureProjector(const Frustum* frust, size_t index);
        /** Sets the current render target */
        void setCurrentRenderTarget(const RenderTarget* target);
        /** Sets the current viewport */
        void setCurrentViewport(const Viewport* viewport);
		/** Sets the shadow extrusion distance to be used for point lights. */
		void setShadowDirLightExtrusionDistance(Real dist);
		/** Sets the main camera's scene bounding information */
		void setMainCamBoundsInfo(VisibleObjectsBoundsInfo* info);
		/** Set the current scene manager for enquiring on demand */
		void setCurrentSceneManager(const SceneManager* sm);
        /** Sets the current pass */
        void setCurrentPass(const Pass* pass);



        const Matrix4& getWorldMatrix(void) const;
        const Matrix4* getWorldMatrixArray(void) const;
        size_t getWorldMatrixCount(void) const;
        const Matrix4& getViewMatrix(void) const;
        const Matrix4& getViewProjectionMatrix(void) const;
        const Matrix4& getProjectionMatrix(void) const;
        const Matrix4& getWorldViewProjMatrix(void) const;
        const Matrix4& getWorldViewMatrix(void) const;
        const Matrix4& getInverseWorldMatrix(void) const;
        const Matrix4& getInverseWorldViewMatrix(void) const;
        const Matrix4& getInverseViewMatrix(void) const;
        const Matrix4& getInverseTransposeWorldMatrix(void) const;
        const Matrix4& getInverseTransposeWorldViewMatrix(void) const;
        const Vector4& getCameraPosition(void) const;
        const Vector4& getCameraPositionObjectSpace(void) const;
        /** Get the light which is 'index'th closest to the current object */
        const Light& getLight(size_t index) const;
		void setAmbientLightColour(const ColourValue& ambient);
		const ColourValue& getAmbientLightColour(void) const;
        const ColourValue& getSurfaceAmbientColour(void) const;
        const ColourValue& getSurfaceDiffuseColour(void) const;
        const ColourValue& getSurfaceSpecularColour(void) const;
        const ColourValue& getSurfaceEmissiveColour(void) const;
        Real getSurfaceShininess(void) const;
        ColourValue getDerivedAmbientLightColour(void) const;
        ColourValue getDerivedSceneColour(void) const;
        void setFog(FogMode mode, const ColourValue& colour, Real expDensity, Real linearStart, Real linearEnd);
        const ColourValue& getFogColour(void) const;
        const Vector4& getFogParams(void) const;
        const Matrix4& getTextureViewProjMatrix(size_t index) const;
        const RenderTarget* getCurrentRenderTarget(void) const;
        const Renderable* getCurrentRenderable(void) const;
        const Pass* getCurrentPass(void) const;
        Vector4 getTextureSize(size_t index) const;
        Vector4 getInverseTextureSize(size_t index) const;
        Vector4 getPackedTextureSize(size_t index) const;
		Real getShadowExtrusionDistance(void) const;
		const Vector4& getSceneDepthRange() const;
		const Vector4& getShadowSceneDepthRange(size_t lightIndex) const;
		Matrix4 getInverseViewProjMatrix(void) const;
		Matrix4 getInverseTransposeViewProjMatrix() const;
		Matrix4 getTransposeViewProjMatrix() const;
		Matrix4 getTransposeViewMatrix() const;
        Matrix4 getInverseTransposeViewMatrix() const;
		Matrix4 getTransposeProjectionMatrix() const;
		Matrix4 getInverseProjectionMatrix() const;
		Matrix4 getInverseTransposeProjectionMatrix() const;
		Matrix4 getTransposeWorldViewProjMatrix() const;
		Matrix4 getInverseWorldViewProjMatrix() const;
		Matrix4 getInverseTransposeWorldViewProjMatrix() const;
		Matrix4 getTransposeWorldViewMatrix() const;
		Matrix4 getTransposeWorldMatrix() const;
        Real getTime(void) const;
		Real getTime_0_X(Real x) const;
		Real getCosTime_0_X(Real x) const;
		Real getSinTime_0_X(Real x) const;
		Real getTanTime_0_X(Real x) const;
		Vector4 getTime_0_X_packed(Real x) const;
		Real getTime_0_1(Real x) const;
		Real getCosTime_0_1(Real x) const;
		Real getSinTime_0_1(Real x) const;
		Real getTanTime_0_1(Real x) const;
		Vector4 getTime_0_1_packed(Real x) const;
		Real getTime_0_2Pi(Real x) const;
		Real getCosTime_0_2Pi(Real x) const;
		Real getSinTime_0_2Pi(Real x) const;
		Real getTanTime_0_2Pi(Real x) const;
		Vector4 getTime_0_2Pi_packed(Real x) const;
        Real getFrameTime(void) const;
		Real getFPS() const;
		Real getViewportWidth() const;
		Real getViewportHeight() const;
		Real getInverseViewportWidth() const;
		Real getInverseViewportHeight() const;
		Vector3 getViewDirection() const;
		Vector3 getViewSideVector() const;
		Vector3 getViewUpVector() const;
		Real getFOV() const;
		Real getNearClipDistance() const;
		Real getFarClipDistance() const;
        int getPassNumber(void) const;
        void setPassNumber(const int passNumber);
        void incPassNumber(void);
    };
}

#endif
