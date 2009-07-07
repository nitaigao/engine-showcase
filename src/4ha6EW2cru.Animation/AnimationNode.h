/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   AnimationNode.h
*  @date   2009/06/03
*/
#ifndef ANIMATIONNODE_H
#define ANIMATIONNODE_H

#include "IAnimationNode.hpp"

namespace Animation
{
	/*! 
	 *  An Animation Node
	 */
	class AnimationNode : public IAnimationNode
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		~AnimationNode( );


		/*! Default Constructor
		*
		* @return (  )
		*/
		AnimationNode( Renderer::IAnimationController* animationController )
			: m_animationController( animationController )
			, m_targetWeight( 0.0f )
			, m_timeLeft( 0.0f )
			, m_blendDuration( 1.0f )
		{

		}


		/*! Adds a child Node to this Node
		*
		* @param[in] IAnimationNode * node
		* @return (  )
		*/
		void AddNode( IAnimationNode* node );


		/*! Returns the Animation Name this Node is responsible for
		*
		* @return ( const std::string& )
		*/
		std::string GetAnimationName( ) const { return m_animationController->GetName( ); };


		/*! Sets the weight of this Node
		*
		* @return ( void )
		*/
		void SetWeight( const float& weight );


		/*! Removes a Node from the Tree
		*
		* @param[in] const std::string & animationName
		* @return ( IAnimationNode* )
		*/
		IAnimationNode* RemoveNode( const std::string& animationName );


		/*! Returns the Animation Controller within the Node
		*
		* @return ( Renderer::IAnimationController* )
		*/
		Renderer::IAnimationController* GetAnimationController( ) const { return m_animationController; };

		/*! Updates the internal Animation State of the Node
		*
		* @param[in] const float & deltaMilliseconds
		* @return ( void )
		*/
		void Update( const float& deltaMilliseconds );

	private:

		AnimationNode( const AnimationNode & copy ) { };
		AnimationNode & operator = ( const AnimationNode & copy ) { return *this; };

		IAnimationNode::AnimationNodeList m_children;
		Renderer::IAnimationController* m_animationController;
		float m_targetWeight;
		float m_timeLeft;
		float m_blendDuration;
		
	};
};

#endif