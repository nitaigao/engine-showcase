#ifndef __COLOR_H
#define __COLOR_H

class Color
{

public:

	Color( const float& red, const float& green, const float& blue )
		: _red( red )
		, _green( green )
		, _blue( blue )
	{

	}

	inline float GetRed( ) { return _red; };
	inline float GetGreen( ) { return _green; };
	inline float GetBlue( ) { return _blue; };

private:

	float _red;
	float _blue;
	float _green;

};


#endif