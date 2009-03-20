#include "InputSystemScene.h"

#include "InputSystemComponent.h"

InputSystemScene::~InputSystemScene()
{
	_inputManager->destroyInputSystem( _inputManager );

	delete _keyboardListener;
	delete _mouseListener;
}

InputSystemScene::InputSystemScene( const int& screenWidth, const int& screenHeight ) 
{
	_inputManager = OIS::InputManager::createInputSystem( Management::GetInstance( )->GetPlatformManager( )->GetHwnd( ) );
	
	_keyboard = static_cast< OIS::Keyboard* >( _inputManager->createInputObject( OIS::OISKeyboard, true ) );
	_keyboardListener = new KeyboardListener( _keyboard );
	_keyboard->setEventCallback( _keyboardListener );

	_mouse = static_cast< OIS::Mouse* >( _inputManager->createInputObject( OIS::OISMouse, true ) );
	_mouseListener = new MouseListener( );
	_mouse->setEventCallback( _mouseListener );

	_mouse->getMouseState( ).width = screenWidth;
	_mouse->getMouseState( ).height = screenHeight;
}

ISystemComponent* InputSystemScene::CreateComponent( const std::string& name )
{
	InputSystemComponent* inputComponent = new InputSystemComponent( name );
	_keyboardListener->AddInputComponent( inputComponent );
	return inputComponent;
}

void InputSystemScene::DestroyComponent( ISystemComponent* component )
{
	delete component;
	component = 0;
}

void InputSystemScene::Update( float deltaMilliseconds )
{
	_mouse->capture( );
	_keyboard->capture( );
}