// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the INKENGINE_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// INKENGINE_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef INKENGINE_EXPORTS
#define INKENGINE_API __declspec(dllexport)
#else
#define INKENGINE_API __declspec(dllimport)
#endif

// This class is exported from the inkengine.dll
class INKENGINE_API Cinkengine {
public:
	Cinkengine(void);
	// TODO: add your methods here.
};

extern INKENGINE_API int ninkengine;

INKENGINE_API int fninkengine(void);
