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

	script:registerEventHandler( Loading.onEvent )
	
	Loading.onHideLoading( )
	
	local loading = ux:findWidget( 'loadingbox' )
	local loadingX = ( ux:getScreenWidth( ) - loading:getDimensions( ).width ) / 2
	local loadingY = ux:getScreenHeight( ) - ( loading:getDimensions( ).height * 3 )
	loading:setPosition( loadingX, loadingY )

end

function Loading.onShowLoading( levelName )

	local progress = ux:findWidget( 'loading_progress' ):asProgressBar( )
	progress:setProgress( 0 )

	local loading = ux:findWidget( 'loading' )
	loading:setVisible( true ) 
	
	print( 'Loading Started for Level ' .. levelName )

end

function Loading.onHideLoading( )

	local progress = ux:findWidget( 'loading_progress' ):asProgressBar( )
	progress:setProgress( 105 )

	local loading = ux:findWidget( 'loading' )
	loading:setVisible( false )
	
	print( 'Loading Finished' )

end

function Loading.onLoadProgress( totalProgress )

	local progress = ux:findWidget( 'loading_progress' ):asProgressBar( )
	progress:setProgress( tonumber( totalProgress ) )

end

function Loading.onEvent( eventName, var1 )

	if ( eventName == 'WORLD_LOADING_STARTED' ) then
	
		Loading.onShowLoading( var1 )
	
	end
	
	if ( eventName == 'WORLD_LOADING_PROGRESS' ) then
	
		Loading.onLoadProgress( var1 )
	
	end
	
	if ( eventName == 'WORLD_LOADING_FINISHED' ) then
	
		Loading.onHideLoading( )
	
	end

end

Loading.initialize( )