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
#ifndef __Renderable_H__
#define __Renderable_H__

#include "OgrePrerequisites.h"
#include "OgreCommon.h"

#include "OgreRenderOperation.h"
#include "OgreMatrix4.h"
#include "OgreMaterial.h"
#include "OgrePlane.h"
#include "OgreGpuProgram.h"
#include "OgreVector4.h"
#include "OgreException.h"

namespace Ogre {

    /** Abstract class defining the interface all renderable objects must implement.
        @remarks
            This interface abstracts renderable discrete objects which will be queued in the render pipeline,
            grouped by material. Classes implementing this interface must be based on a single material, a single
            world matrix (or a collection of world matrices which are blended by weights), and must be 
            renderable via a single render operation.
        @par
            Note that deciding whether to put these objects in the rendering pipeline is done from the more specific
            classes e.g. entities. Only once it is decided that the specific class is to be rendered is the abstract version
            created (could be more than one per visible object) and pushed onto the rendering queue.
    */
    class _OgreExport Renderable
    {
    public:
		Renderable() : mPolygonModeOverrideable(true), mUseIdentityProjection(false), mUseIdentityView(false) {}
        /** Virtual destructor needed as class has virtual methods. */
        virtual ~Renderable() { }
        /** Retrieves a weak reference to the material this renderable object uses.
        @remarks
            Note that the Renderable also has the option to override the getTechnique method
            to specify a particular Technique to use instead of the best one available.
        */
        virtual const MaterialPtr& getMaterial(void) const = 0;
        /** Retrieves a pointer to the Material Technique this renderable object uses.
        @remarks
            This is to allow Renderables to use a chosen Technique if they wish, otherwise
            they will use the best Technique available for the Material they are using.
        */
        virtual Technique* getTechnique(void) const { return getMaterial()->getBestTechnique(); }
        /** Gets the render operation required to send this object to the frame buffer.
        */
        virtual void getRenderOperation(RenderOperation& op) = 0;
        /** Gets the world transform matrix / matrices for this renderable object.
            @remarks
                If the object has any derived transforms, these are expected to be up to date as long as
                all the SceneNode structures have been updated before this is called.
            @par
                This method will populate xform with 1 matrix if it does not use vertex blending. If it
                does use vertex blending it will fill the passed in pointer with an array of matrices,
                the length being the value returned from getNumWorldTransforms.
            @note
                Internal Ogre never supports non-affine matrix for world transform matrix/matrices,
                the behavior is undefined if returns non-affine matrix here. @see Matrix4::isAffine.
        */
        virtual void getWorldTransforms(Matrix4* xform) const = 0;
        /** Gets the worldspace orientation of this renderable; this is used in order to
            more efficiently update parameters to vertex & fragment programs, since inverting Quaterion
            and Vector in order to derive object-space positions / directions for cameras and
            lights is much more efficient than inverting a complete 4x4 matrix, and also 
            eliminates problems introduced by scaling. */
        virtual const Quaternion& getWorldOrientation(void) const = 0;
        /** Gets the worldspace position of this renderable; this is used in order to
            more efficiently update parameters to vertex & fragment programs, since inverting Quaterion
            and Vector in order to derive object-space positions / directions for cameras and
            lights is much more efficient than inverting a complete 4x4 matrix, and also 
            eliminates problems introduced by scaling. */
        virtual const Vector3& getWorldPosition(void) const = 0;

        /** Returns the number of world transform matrices this renderable requires.
        @remarks
            When a renderable uses vertex blending, it uses multiple world matrices instead of a single
            one. Each vertex sent to the pipeline can reference one or more matrices in this list
            with given weights.
            If a renderable does not use vertex blending this method returns 1, which is the default for 
            simplicity.
        */
        virtual unsigned short getNumWorldTransforms(void) const { return 1; }

        /** Sets whether or not to use an 'identity' projection.
        @remarks
            Usually Renderable objects will use a projection matrix as determined
            by the active camera. However, if they want they can cancel this out
            and use an identity projection, which effectively projects in 2D using
            a {-1, 1} view space. Useful for overlay rendering. Normal renderables
            need not change this. The default is false.
        @see Renderable::getUseIdentityProjection
        */
        void setUseIdentityProjection(bool useIdentityProjection)
        {
            mUseIdentityProjection = useIdentityProjection;
        }

        /** Returns whether or not to use an 'identity' projection.
        @remarks
            Usually Renderable objects will use a projection matrix as determined
            by the active camera. However, if they want they can cancel this out
            and use an identity projection, which effectively projects in 2D using
            a {-1, 1} view space. Useful for overlay rendering. Normal renderables
            need not change this.
        @see Renderable::setUseIdentityProjection
        */
        bool getUseIdentityProjection(void) const { return mUseIdentityProjection; }

        /** Sets whether or not to use an 'identity' view.
        @remarks
            Usually Renderable objects will use a view matrix as determined
            by the active camera. However, if they want they can cancel this out
            and use an identity matrix, which means all geometry is assumed
            to be relative to camera space already. Useful for overlay rendering. 
            Normal renderables need not change this. The default is false.
        @see Renderable::getUseIdentityView
        */
        void setUseIdentityView(bool useIdentityView)
        {
            mUseIdentityView = useIdentityView;
        }

        /** Returns whether or not to use an 'identity' view.
        @remarks
            Usually Renderable objects will use a view matrix as determined
            by the active camera. However, if they want they can cancel this out
            and use an identity matrix, which means all geometry is assumed
            to be relative to camera space already. Useful for overlay rendering. 
            Normal renderables need not change this.
        @see Renderable::setUseIdentityView
        */
        bool getUseIdentityView(void) const { return mUseIdentityView; }

		/** Returns the camera-relative squared depth of this renderable.
		@remarks
			Used to sort transparent objects. Squared depth is used rather than
			actual depth to avoid having to perform a square root on the result.
		*/
		virtual Real getSquaredViewDepth(const Camera* cam) const = 0;

        /** Returns whether or not this Renderable wishes the hardware to normalise normals. */
        virtual bool getNormaliseNormals(void) const { return false; }

        /** Gets a list of lights, ordered relative to how close they are to this renderable.
        @remarks
            Directional lights, which have no position, will always be first on this list.
        */
        virtual const LightList& getLights(void) const = 0;

        virtual const PlaneList& getClipPlanes() const { return msDummyPlaneList; };

        /** Method which reports whether this renderable would normally cast a
            shadow. 
        @remarks
            Subclasses should override this if they could have been used to 
            generate a shadow.
        */
        virtual bool getCastsShadows(void) const { return false; }

        /** Sets a custom parameter for this Renderable, which may be used to 
            drive calculations for this specific Renderable, like GPU program parameters.
        @remarks
            Calling this method simply associates a numeric index with a 4-dimensional
            value for this specific Renderable. This is most useful if the material
            which this Renderable uses a vertex or fragment program, and has an 
            ACT_CUSTOM parameter entry. This parameter entry can refer to the
            index you specify as part of this call, thereby mapping a custom
            parameter for this renderable to a program parameter.
        @param index The index with which to associate the value. Note that this
            does not have to start at 0, and can include gaps. It also has no direct
            correlation with a GPU program parameter index - the mapping between the
            two is performed by the ACT_CUSTOM entry, if that is used.
        @param value The value to associate.
        */
        void setCustomParameter(size_t index, const Vector4& value) 
        {
            mCustomParameters[index] = value;
        }

        /** Gets the custom value associated with this Renderable at the given index.
        @param
            @see setCustomParaemter for full details.
        */
        const Vector4& getCustomParameter(size_t index) const
        {
            CustomParameterMap::const_iterator i = mCustomParameters.find(index);
            if (i != mCustomParameters.end())
            {
                return i->second;
            }
            else
            {
                OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
                    "Parameter at the given index was not found.",
                    "Renderable::getCustomParameter");
            }
        }

        /** Update a custom GpuProgramParameters constant which is derived from 
            information only this Renderable knows.
        @remarks
            This method allows a Renderable to map in a custom GPU program parameter
            based on it's own data. This is represented by a GPU auto parameter
            of ACT_CUSTOM, and to allow there to be more than one of these per
            Renderable, the 'data' field on the auto parameter will identify
            which parameter is being updated. The implementation of this method
            must identify the parameter being updated, and call a 'setConstant' 
            method on the passed in GpuProgramParameters object, using the details
            provided in the incoming auto constant setting to identify the index
            at which to set the parameter.
        @par
            You do not need to override this method if you're using the standard
            sets of data associated with the Renderable as provided by setCustomParameter
            and getCustomParameter. By default, the implementation will map from the
            value indexed by the 'constantEntry.data' parameter to a value previously
            set by setCustomParameter. But custom Renderables are free to override
            this if they want, in any case.
        @param constantEntry The auto constant entry referring to the parameter
            being updated
        @param params The parameters object which this method should call to 
            set the updated parameters.
        */
        virtual void _updateCustomGpuParameter(
            const GpuProgramParameters::AutoConstantEntry& constantEntry,
            GpuProgramParameters* params) const
        {
            CustomParameterMap::const_iterator i = mCustomParameters.find(constantEntry.data);
            if (i != mCustomParameters.end())
            {
                params->_writeRawConstant(constantEntry.physicalIndex, i->second, 
					constantEntry.elementCount);
            }
        }

		/** Sets whether this renderable's chosen detail level can be
			overridden (downgraded) by the camera setting. 
		@param override true means that a lower camera detail will override this
			renderables detail level, false means it won't.
		*/
		virtual void setPolygonModeOverrideable(bool override)
		{
			mPolygonModeOverrideable = override;
		}

		/** Gets whether this renderable's chosen detail level can be
			overridden (downgraded) by the camera setting. 
		*/
		virtual bool getPolygonModeOverrideable(void) const
		{
			return mPolygonModeOverrideable;
		}


    protected:
        static const PlaneList msDummyPlaneList;
        typedef std::map<size_t, Vector4> CustomParameterMap;
        CustomParameterMap mCustomParameters;
		bool mPolygonModeOverrideable;
        bool mUseIdentityProjection;
        bool mUseIdentityView;
    };



}

#endif //__Renderable_H__
