/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IAnimationNode.hpp
*  @date   2009/06/03
*/
#ifndef IANIMATIONNODE_HPP
#define IANIMATIONNODE_HPP

#include <deque>
#include <string>

#include "Renderer/AnimationController.h"

namespace Animation
{
	/*! 
	 *  An Animation Node
	 */
	class IAnimationNode
	{

	public:

		typedef std::deque< IAnimationNode* > AnimationNodeList;

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		virtual ~IAnimationNode( ) { };


		/*! Adds a child Node to this Node
		*
		* @param[in] IAnimationNode * node
		* @return (  )
		*/
		virtual void AddNode( IAnimationNode* node ) = 0;


		/*! Returns the Animation Name this Node is responsible for
		*
		* @return ( const std::string& )
		*/
		virtual std::string GetAnimationName( ) const = 0;


		/*! Sets the weight of this Node
		*
		* @return ( void )
		*/
		virtual void SetWeight( const float& weight ) = 0;


		/*! Removes a Node from the Tree
		*
		* @param[in] const std::string & animationName
		* @return ( IAnimationNode* )
		*/
		virtual IAnimationNode* RemoveNode( const std::string& animationName ) = 0;


		/*! Returns the Animation Controller within the Node
		*
		* @return ( Renderer::IAnimationController* )
		*/
		virtual Renderer::IAnimationController* GetAnimationController( ) const = 0;


		/*! Updates the internal Animation State of the Node
		*
		* @param[in] const float & deltaMilliseconds
		* @return ( void )
		*/
		virtual void Update( const float& deltaMilliseconds ) = 0;
		
	};
};

#endif