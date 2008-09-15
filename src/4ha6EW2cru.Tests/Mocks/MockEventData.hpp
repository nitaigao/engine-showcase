#ifndef MOCKEVENTDATA_H
#define MOCKEVENTDATA_H

class TestEventData : public IEventData 
{

public:

	TestEventData( const int& const data )
		: _data( data )
	{ };

	inline const int& GetData( ) const { return _data; };

private:

	int _data;

};

#endif