#ifndef SEARCHUTILITY_H
#define SEARCHUTILITY_H

class SearchUtility
{

public:

	inline const std::string GetWildCard( ) { return m_wildCard; };
	inline void SetWildCard( const std::string wildCard ) { m_wildCard = wildCard; };

	inline void SetSearchPattern( const std::string searchPattern ) { m_searchPattern = searchPattern; };
	
	inline const bool IsStartsWithSearch( ) const { return ( m_searchPattern.find_last_of( m_wildCard ) == m_searchPattern.length( ) - 1 ); };
	inline const bool IsEndsWithSearch( ) const { return ( m_searchPattern.find( m_wildCard ) == 0 ); };
	inline const bool IsContainsSearch( ) const { return ( IsEndsWithSearch( ) && IsStartsWithSearch( ) ); };
	inline const bool IsExactSearch( ) const { return ( !IsEndsWithSearch( ) && !IsStartsWithSearch( ) ); };

	inline const std::string GetSearchString( ) const
	{
		std::string searchString = m_searchPattern;

		if ( searchString.find_first_of( m_wildCard ) == 0 )
		{
			searchString = searchString.substr( 1, searchString.length( ) - 1 );
		}

		if( searchString.find_last_of( m_wildCard ) == searchString.length( ) - 1 )
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

		return false;
	}

private:

	std::string m_wildCard;
	std::string m_searchPattern;

};

#endif