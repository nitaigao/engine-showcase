/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   AISystemScene.h
*  @date   2009/04/25
*/
#ifndef __AISYSTEMSCENE_H
#define __AISYSTEMSCENE_H

#include "IAISystemComponent.hpp"
#include "IAISystemScene.hpp"
#include "../Scripting/IScriptSystemScene.hpp"

#include <hash_map>

namespace AI
{
	/*! 
	*  An AI System Specific Scene
	*/
	class AISystemScene : public IAISystemScene
	{

		typedef stdext::hash_multimap< int, IAISystemComponent* > AISystemComponentMap;

	public:

		/*! Default Destructor
		*
		*  @return ()
		*/
		~AISystemScene( ) { };


		/*! Default Constructor
		 *
		 *  @return ()
		 */
		AISystemScene( )
			: _frameNumber( 0 )
			, _lastFrameAssignment( 0 )
		{

		}

		/*!  Initializes the System Scene
		*
		*  @return (void)
		*/
		void Initialize( );

		/*! Steps internal data of the SystemScene
		*
		*  @param[in] float deltaMilliseconds
		*  @return (void)
		*/
		void Update( const float& deltaMilliseconds );


		/*! Destroys the System Scene
		*
		*  @return (void)
		*/
		inline void Destroy( ) { };


		/*! Gets the System::Types::Type of the SystemScene
		*
		*  @return (System::Types::Type)
		*/
		inline System::Types::Type GetType( ) const { return System::Types::AI; };

		/*! Creates a SystemComponent specific to the SystemScene
		*
		*  @param[in] const std::string & name
		*  @param[in] const std::string & type
		*  @return (ISystemComponent*)
		*/
		ISystemComponent* CreateComponent( const std::string& name, const std::string& type );
		
		/*! Destroys a SystemComponent created by the SystemScene
		*
		*  @param[in] ISystemComponent * component
		*  @return (void)
		*/
		void DestroyComponent( ISystemComponent* component );

		//TODO: This should be re factored into the scripting system
		/*! Returns the ScriptScene managing the AI Components
		 *
		 *  @return (ISystemScene*)
		 */
	//	inline ISystemScene* GetScriptScene( ) { return 0; };

	private:

		AISystemComponentMap _components;

		int _lastFrameAssignment;
		int _frameNumber;

		//Script::IScriptSystemScene* _scriptScene;

	};
};

#endif