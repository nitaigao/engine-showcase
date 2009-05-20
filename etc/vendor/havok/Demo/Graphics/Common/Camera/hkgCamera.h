/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_CAMERA_H
#define HK_GRAPHICS_CAMERA_H

#include <Graphics/Common/Camera/hkgCameraDefines.h>

class hkgDisplayContext;

/// A simple camera model that can compute its own projection matrices (perspective and orthogonal) 
/// and is able to project points to and from normalized camera space. Camera space is -1,1 in x
/// and y, and 0 to 1 in z (depth) where 1 is far plane. The camera uses the normal Z, not W, so 
/// will suffer from non linear Z values which are much more defined near the near plane. So try 
/// and use as distant a near plane as possible and reduce the near to far distance to improve
/// visual quality.
///
/// When you change values that affect the model view and the projection you will have to get 
/// the corresponding matrices recalculated using one of the two compute methods. If in doubt, 
/// just call both compute methods as they are fast enough. 
/// HKG_CAMERA_COMPUTE_ORTHOGRAPHIC mode uses the perspective mode To, From, etc, to compute
/// the left,right planes etc based on the extents visible at the To plane, so you would move
/// the camera as normal via the from position and the ortho values will be calculated when you
/// update the projection matrix.

class hkgCamera : public hkgReferencedObject
{
	public:

			/// Create a new camera. It will be platform specific and so is able to load the matrices if
			/// required when set as the current camera, This create method will use the default from, 
			/// to, up, etc. so you should set all applicable values for the type of camera you are 
			/// creating as no values are guaranteed.. All cameras are perspective mode by default.
		static hkgCamera* (HK_CALL *create)();

			/// When you change To,From,Up, you could easily end up with a non orthogonal basis for
			/// the camera. This method will adjust the up vector so that it is orthogonal to the 
			/// computed Right vector and the Direction of the camera.
		inline void orthogonalize();
			
			/// Once you change parameters that affect the projection matrix (how the camera converts
			/// the viewable 3D triangles etc into normalized camera space), you should call the 
			/// method to reflect your alterations in the stored matrix. Projection covers both 
			/// perspective and orthogonal projections.
		inline void computeProjection();

			/// Once you change parameters that affect the modelview matrix (the position of the 
			/// camera and orientation in the world, you should call this method to reflect your 
			/// alterations in the stored matrix. 
		void computeModelView(bool forceIdentity = false);

			/// This method gets the projection mode that is being used by the camera, If the 
			/// camera is in pure orthogonal mode (HKG_CAMERA_ORTHOGRAPHIC) it uses just the Ortho* values
			/// to compute the matrix. If it is HKG_CAMERA_COMPUTE_ORTHOGRAPHIC it will first compute
			/// these Ortho* values from the camera modelview and point of interest (the To position).
		inline  HKG_CAMERA_PROJECTION_MODE getProjectionMode() const;

			/// Sets the projection mode for the camera. See the explanation for getProjectionMode()
			/// for the meaning on the values.
		inline  void setProjectionMode(HKG_CAMERA_PROJECTION_MODE m);

			/// See the setHandednessMode comments for more information.
			/// The default mode is right handed (ala OpenGL) but you can switch 
			/// to left if you want but remember to also switch your cull mode as well.
		inline  HKG_CAMERA_HANDEDNESS_MODE getHandednessMode() const;

			/// Switch whether the Z is toward the camera (Right Handed)
			/// or away (Left Handed).
			/// To simplify the process, the Z is flipped in the final
			/// matrices only, so the z column of the view matrix
			/// will be flipped if mode is changed to Left.
			/// This will affect our winding wtc (change the winding cull
			/// from CCW to CW to compensate)
		inline  void setHandednessMode(HKG_CAMERA_HANDEDNESS_MODE m);

			/// Project a given (x,y,z) world point into normalized camera space and then scale the 
			/// resultant to the given window width and height. The (x,y) in result (result[0] and [1]) 
			/// are the window x and y coords, which may be outside of the window bounds. The z in result 
			/// (result[2]) is the depth from 0 to 1, where 0 is nearest. 
		void project( float x, float y, float z, int winWidth, int winHeight, float result[3] ) const;

			/// Unproject a window point (x,y,depth) into a world position, where x and y are in window units 
			/// and the window width and height are given. If you unproject (x,y,0) and (x,y,1) you will get a 
			/// line segment with which to pick for example. The method works with single precission floating
			/// point numbers only.
		void unProject( int x, int y, float depth, int winWidth, int winHeight, float result[3] ) const;
		
			/// This is the same as the unProject() method, but uses double precision floats internally, so is 
			/// slightly more accurate if that is required.
		void unProjectExact( int x, int y, float depth, int winWidth, int winHeight,  float result[3] ) const;

			/// This method loads the current projection and modelview matrices into the appropriate
			/// platform specific area. Call this method to make the camera the current one for rendering.
		virtual void setAsCurrent(hkgDisplayContext* state) = 0;
			
			/// Pan the camera long x (Right) or y (Up) by the given amount. 
			/// If you want to move the To, or focus, the same amount as well you can.
			/// The amount is the fraction of the length from To to From, so is relative 
			/// to the distance from the point of interest.
			/// This method will automatically recompute the affected matrices.
			/// If proportional is false the the x and y amount are actual distances not ratios wrt to the distance to the poi.
		void pan(float x_amount, float y_amount, bool move_focus = false, bool proportional = true);
			
			/// Dolly the camera in or out along the Dir vector. The amount is the relative amount to
			/// move wrt to the distance from To to From. 
			/// This method will automatically recompute the affected matrices.
			/// If proportional is false the the z amount is an actual distance not ratios wrt to the distance to the poi.
		void dolly(float z_amount, bool move_focus = false, bool proportional = true);

			/// Rotate the camera From by Angle radians about the given world axis, at To. 
			/// If you want to rotate the Up vector too, then specify upLock as false. 
			/// This method will automatically recompute the affected matrices.
		void rotateAboutTo(float angle, const float axis[3], bool upLock = false);

			/// Rotate the camera To by Angle radians about the given world axis, at From. 
			/// If you want to rotate the Up vector too, then specify upLock as false. 
			/// This method will automatically recompute the affected matrices.
		void rotateAboutFrom(float angle, const float axis[3], bool upLock = false);

			/// Start the trackball calculations with the give mouse x and y, which are in 
			/// relation to the given width and height. This does not move the camera.
		void startTrackball( unsigned int winWidth, unsigned int winHeight, int mouseX, int mouseY );

			/// Move the camera based in the new mouse x and y, which are in relation to the 
			/// given width and height. This method calls the rotateAboutTo() once it computes the
			/// angle and axis based on the mouse movement.
			/// This method will automatically recompute the affected matrices.
		void moveTrackball( unsigned int winWidth, unsigned int winHeight, int mouseX, int mouseY );
	
			/// Get the camera From world point. Used by Perspective mode or Compute Orthographic mode only.
		inline void		getFrom(float f[3]) const;				

			/// Get the camera To world point. Used by Perspective mode or Compute Orthographic mode only.
		inline void		getTo(float t[3]) const;

			/// Get the (computed) camera Direction world vector (normalized). Used by Perspective mode or Compute Orthographic mode only.
		inline void		getDir(float d[3]) const;

			/// Get the (computed) camera Right world vector (normalized). Used by Perspective mode or Compute Orthographic mode only.
			/// Get the "right", as the user looking through the camera would see it, in world space
		inline void		getRight(float r[3]) const;

			/// Get the camera Up world vector (unit length). Used by Perspective mode or Compute Orthographic mode only.
		inline void		getUp(float u[3]) const;
		void			getBestAlignedUpVector(float bestWorldUp[3]) const;
		
			/// Get the camera FoV (Field of View) in degrees. Used by Perspective mode or Compute Orthographic mode only.
		inline float	getFOV() const;

			/// Get the camera Near plane in units from the From point. Used by Perspective mode or Compute Orthographic mode only. It can not be negative.
		inline float	getNear() const;		
		
			/// Get the camera Far plane in units from the From point. Used by Perspective mode or Compute Orthographic mode only. It can not be negative.
		inline float	getFar() const;		

			/// Get the camera Aspect ratio of the form (width/height). Used by Perspective mode or Compute Orthographic mode only. 
		inline float	getAspect() const;		
		
			/// Get the camera Up world vector (unit length) as a pointer to the internal member. Used by Perspective mode or Compute Orthographic mode only.
		inline float*	getUpPtr();				

			/// Get the camera To world point as a pointer to the internal member. Used by Perspective mode or Compute Orthographic mode only.
		inline float*	getToPtr();				
		
			/// Get the camera From world point as a pointer to the internal member. Used by Perspective mode or Compute Orthographic mode only.
		inline float*	getFromPtr();				
		
			/// Set the camera FoV (Field of View) in degrees. Used by Perspective mode or Compute Orthographic mode only.
		inline void		setFOV(float f); 

			/// Set the camera Up world vector. This should be of unit length. Make sure that it is orthogonal to Dir and Right by calling orthoginalize(). Used by Perspective mode or Compute Orthographic mode only.
		inline void		setUp(const float u[3]);
			
			/// Set the camera To world point. Used by Perspective mode or Compute Orthographic mode only.
		inline void		setTo(const float t[3]);

			/// Set the camera From world point. Used by Perspective mode or Compute Orthographic mode only.
		inline void		setFrom(const float f[3]);

			/// Set the camera Far plane, in units from the From point along Dir. This must be positive due to a GameCube restriction. Used by Perspective mode or Compute Orthographic mode only.
		inline void		setFar(float f);

			/// Set the camera Near plane, in units from the From point along Dir. This must be positive due to a GameCube restriction. Used by Perspective mode or Compute Orthographic mode only.
		inline void		setNear(float f);

			/// Set the Aspect ratio in the form (width/height). Used by Perspective mode or Compute Orthographic mode only.
		inline void		setAspect(float f);


			/// Get the value that will map to the left hand side of the camera. Typically -w/2 where w is the width of scene to view. Used by Orthographic mode only.
		inline float	getOrthoLeft() const;

			/// Get the value that will map to the right hand side of the camera. Typically w/2 where w is the width of scene to view.. Used by Orthographic mode only.
		inline float	getOrthoRight() const;

			/// Get the value that will map to the top of the camera. Typically h/2 where h is the height of scene to view.. Used by Orthographic mode only.
		inline float	getOrthoTop() const;

			/// Get the value that will map to the bottom of the camera. Typically -h/2 where h is the height of scene to view.. Used by Orthographic mode only.
		inline float	getOrthoBottom() const;

			/// Get the Near plane distance from the camera. This must be positive due to a GameCube restriction.. Used by Orthographic mode only.
		inline float	getOrthoNear() const;
		
			/// Get the Far plane distance from the camera. This must be positive due to a GameCube restriction.. Used by Orthographic mode only.
		inline float	getOrthoFar() const;

			/// Set the value that will map to the left hand side of the camera. Typically -w/2 where w is the width of scene to view. Used by Orthographic mode only.
		inline void		setOrthoLeft(float l);
	
			/// Set the value that will map to the right hand side of the camera. Typically w/2 where w is the width of scene to view.. Used by Orthographic mode only.
		inline void		setOrthoRight(float r);
		
			/// Set the value that will map to the top of the camera. Typically h/2 where h is the height of scene to view.. Used by Orthographic mode only.
		inline void		setOrthoTop(float t);
		
			/// Set the value that will map to the bottom of the camera. Typically -h/2 where h is the height of scene to view.. Used by Orthographic mode only.
		inline void 	setOrthoBottom(float b);
		
			/// Set the Near plane distance from the camera. This must be positive due to a GameCube restriction.. Used by Orthographic mode only.
		inline void		setOrthoNear(float n);
		
			/// Set the Far plane distance from the camera. This must be positive due to a GameCube restriction.. Used by Orthographic mode only.
		inline void		setOrthoFar(float f);

			/// Compute the orthographic projection matrix give the 6 plane distances. Will set the cameras orthographic parameters to match the give values too. 
			/// This method is called automatically by computeProjection() if needed so there should be no need to call it directly.
		void computeOrtho( float left, float right, float bottom, float top, float znear, float zfar);

			/// Compute the orthographic projection parameters based on keeping the To point in view, with the current FoV and From point. Will set the cameras
			/// orthographic parameters to match the computed values. This method is called automatically by computeProjection() if needed so there should be no 
			/// need to call it directly.
		void computeOrthoParams();
		
			/// Compute the perspective matrix based on the current camera Fov, Aspect and clip planes.
		void computePerspective( float fov, float aspect, float znear, float zfar);

			/// Compute the planes that represent the frustum. These planes can then be used to clip against for visibility.
		void computeFrustumPlanes() const;	
			
			/// Compute the inverse projection matrix (including the inverse modelview) that maps camera space
			/// to world space. It internally calls the computeInverse* methods. This is automatically called by the unproject method for instance if the inverse
			/// is dirty. 
		void computeCameraInverse() const; // will store as m_camInv locally. Will call the following 2 funcs

			/// Compute the inverse of the projection matrix, of set form (assumes correct matrix type). Returns 
			/// the inverse in the passed matrix, and does not set any camera parameters. 
		void computeInverseProjectionMatrix( float inv[16] ) const;

			/// Compute the inverse of the view matrix, of set form (assumes correct matrix type). Returns 
			/// the inverse in the passed matrix, and does not set any camera parameters. 
		void computeInverseViewMatrix( float inv[16] ) const;

			/// Compute the spherical view dependant bill board matrix for an
			/// object located at point p. The matrix will be such so that 
			/// Z is forward, X is side and Y is up.
			/// Returns the distance from the camera to pos as a convenience
			/// as it is computed as a by product of the function.
		float computeBillboardMatrix( const float pos[4], float billboard[16] ) const;

			/// Compute the size to draw a billboarded icon in 3D world space
			/// at distanceFromCam units from the camera such that it will cover
			/// about desiredPixelCoverage pixels vertically on a viewportHeight viewport
		inline float computeIconVerticalDrawSize( float distanceFromCam, int desiredPixelCoverage, int viewportHeight );

			/// Returns the pointer to the projection matrix stored in this camera, as computed by computeProjection()
		inline float* getProjectionMatrix();

			/// Returns the pointer to the view matrix stored in this camera, as computed by computeModelView()
		inline float* getViewMatrix();

			/// Returns the pointer to the inverse camera matrix (view and projection, inverted) stored in this camera, as computed by computeInverseViewMatrix() and will
			/// call that method automatically if the dirty flag is set, so this method will always return correct data.
		inline float* getCameraInverseMatrix();

			/// There are 6 frustum planes, in the order Top,Bottom,Left,Right.Near,Far. Each plane is of the form (A,B,C,D) where D is distance and ABC is the normal.
			/// Will automatically compute the planes if the dirty flag is set, so the data should always be correct.
		inline float* getFrustumPlane(unsigned int i); 

			/// Returns whether the given sphere at world position c and radius r has a point within the 6 frustum planes.
			/// Will automatically compute the planes if the dirty flag is set, so the data should always be correct.
			/// This can be used for simple bounding sphere visibility tests. Each of the possible planes tests requires a
			/// dot product and a float sub and compare, so is not too slow.
		inline bool sphereVisible(const float c[3], float radius) const; // in world space

			/// Set the camera's name.
		inline void setCameraName( const char* name );

			/// Return the name assigned to the camera.
		inline const char* getCameraName() const;

			/// On some systems, the matrices get altered to fit, for instance DX has a unit cube of 0 to 1, not -1 to 1
			/// You can use this call to get the used matrice if you are feeding shaders directly etc.
			/// This default impl just returns the normal view and proj matrix. 
		virtual float* getPlatformViewMatrix();
		virtual float* getPlatformProjectionMatrix();

			// Util funcs
		static hkgCamera* HK_CALL createFixedShadowFrustumCamera( const class hkgLight& light, const class hkgAabb& areaOfInterest, bool tightProjection = true, float extraNear = 0, float extraFar = 0);

	protected:

		friend class ExplodingRagdollsDemo;

		inline hkgCamera(const float* from, const float* to, const float* up, float fov);
		virtual ~hkgCamera();

		HKG_CAMERA_PROJECTION_MODE m_mode;
		HKG_CAMERA_HANDEDNESS_MODE m_handedness;

		float		m_from[3];
		float		m_to[3];
		float		m_up[3];
		float		m_fov;
		float		m_near; 
		float		m_far; 
		float		m_aspect;

		float		m_oleft;
		float		m_oright;
		float		m_otop;
		float		m_obottom;
		float		m_onear;
		float		m_ofar;

		float		m_trackballVec[3];

		char*		m_cameraName;

		HK_ALIGN16( float m_viewMat[16] );
		HK_ALIGN16( float m_projMat[16] );

		mutable bool		m_camInvDirty;
		mutable HK_ALIGN16( float m_camInv[16] );
		mutable bool		m_frustumPlanesDirty;
		
		// the 6 planes in world space that mark the view frustum 
		// 4 sides (t,b,l,r) and near (n) and far(f). Stored in that order.
		mutable float		m_frustumPlanes[6][4]; 
		
		static float DEFAULT_FROM[3];
		static float DEFAULT_TO[3];
		static float DEFAULT_UP[3];
		static float DEFAULT_FOV;
};

#include <Graphics/Common/Camera/hkgCamera.inl>

#endif // HK_GRAPHICS_CAMERA_H

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
