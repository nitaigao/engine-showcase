#ifdef EXPORT
#    define GAMEAPI __declspec( dllexport )
#else
#    define GAMEAPI __declspec( dllimport )
#endif