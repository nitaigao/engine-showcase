----------------------------------------------------------------
-- Global Variables
----------------------------------------------------------------

Loading = {}

----------------------------------------------------------------
-- Local Variables
----------------------------------------------------------------
----------------------------------------------------------------
-- Loading Functions
----------------------------------------------------------------

function Loading.initialize( )

	registerEventHandler( Loading.onEvent );
	
	Loading.onHideLoading( );

end

function Loading.onShowLoading( )

	local progress = findWidget( 'loading_progress' ):asProgressBar( );
	progress:setProgress( 0 );

	local loading = findWidget( 'loading' );
	loading:setVisible( true ); 

end

function Loading.onHideLoading( )

	local progress = findWidget( 'loading_progress' ):asProgressBar( );
	progress:setProgress( 100 );

	local loading = findWidget( 'loading' );
	loading:setVisible( false );
	
	print( 'loading complete' );

end

function Loading.onLoadProgress( totalProgress )

	local progress = findWidget( 'loading_progress' ):asProgressBar( );
	print( '' .. tonumber( totalProgress ) );
	progress:setProgress( tonumber( totalProgress ) );

end

function Loading.onEvent( eventName, var1 )

	if ( eventName == 'WORLD_LOADING_STARTED' ) then
	
		Loading.onShowLoading( );
	
	end
	
	if ( eventName == 'WORLD_LOADING_PROGRESS' ) then
	
		Loading.onLoadProgress( var1 );
	
	end
	
	if ( eventName == 'WORLD_LOADING_FINISHED' ) then
	
		Loading.onHideLoading( );
	
	end

end

Loading.initialize( );