/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   src\4ha6EW2cru.Library\AI\AISystemScene.h
*  @date   2009/04/25
*/
#ifndef __AISYSTEMSCENE_H
#define __AISYSTEMSCENE_H

#include "../Scripting/ScriptSystemScene.h"

namespace AI
{
	/*! 
	*  An AI System Specific Scene
	*/
	class AISystemScene : public ISystemScene
	{

	public:

		/*! Default Destructor
		*
		*  @return ()
		*/
		~AISystemScene( );


		/*! Default Constructor
		 *
		 *  @return ()
		 */
		AISystemScene( );

		/*! Initializes the AI System Scene
		*
		*  @return (void)
		*/
		void Initialize( );

		/*! Steps internal data of the AI SystemScene
		*
		*  @param[in] float deltaMilliseconds
		*  @return (void)
		*/
		void Update( float deltaMilliseconds );

		/*! Gets the System::Types::Type of the AI SystemScene
		*
		*  @return (System::Types::Type)
		*/
		inline System::Types::Type GetType( ) { return System::Types::AI; };

		/*! Creates an AI SystemComponent specific to the AI SystemScene
		*
		*  @param[in] const std::string & name
		*  @param[in] const std::string & type
		*  @return (ISystemComponent*)
		*/
		ISystemComponent* CreateComponent( const std::string& name, const std::string& type );
		
		/*! Destroys an AI SystemComponent created by the AI SystemScene
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
		inline ISystemScene* GetScriptScene( ) { return _scriptScene; };

	private:

		ScriptSystemScene* _scriptScene;
		SystemComponentList _components;

	};
};

#endif