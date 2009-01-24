#ifndef __SEARCHUTILITY_H
#define __SEARCHUTILITY_H

class SearchUtility
{

public:

	inline const std::string GetWildCard( ) { return _wildCard; };
	inline void SetWildCard( const std::string wildCard ) { _wildCard = wildCard; };

	inline void SetSearchPattern( const std::string searchPattern ) { _searchPattern = searchPattern; };
	
	inline const bool IsStartsWithSearch( ) const { return ( _searchPattern.find_last_of( _wildCard ) == _searchPattern.length( ) - 1 ); };
	inline const bool IsEndsWithSearch( ) const { return ( _searchPattern.find( _wildCard ) == 0 ); };
	inline const bool IsContainsSearch( ) const { return ( IsEndsWithSearch( ) && IsStartsWithSearch( ) ); };
	inline const bool IsExactSearch( ) const { return ( !IsEndsWithSearch( ) && !IsStartsWithSearch( ) ); };

	inline const std::string GetSearchString( ) const
	{
		std::string searchString = _searchPattern;

		if ( searchString.find_first_of( _wildCard ) == 0 )
		{
			searchString = searchString.substr( 1, searchString.length( ) - 1 );
		}

		if( searchString.find_last_of( _wildCard ) == searchString.length( ) - 1 )
		{
			searchString = searchString.substr( 0, searchString.length( ) - 1 );
		}

		return searchString;
	};

	const bool FindMatch( const std::string inputString ) const 
	{
		if ( IsExactSearch( ) )
		{
			return ( inputString.find( GetSearchString( ) ) != std::string::npos );
		}

		if ( IsStartsWithSearch( ) )
		{
			return ( inputString.find( GetSearchString( ) ) == 0 );
		}

		if ( IsContainsSearch( ) )
		{
			return ( inputString.find( GetSearchString( ) ) != std::string::npos );
		}

		if ( IsEndsWithSearch( ) )
		{
			int result = inputString.find( GetSearchString( ) );
 			return ( result > -1 );
		}
	}

private:

	std::string _wildCard;
	std::string _searchPattern;

};

#endif