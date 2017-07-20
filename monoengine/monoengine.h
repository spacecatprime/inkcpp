// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the MONOENGINE_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MONOENGINE_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef MONOENGINE_EXPORTS
#define MONOENGINE_API __declspec(dllexport)
#else
#define MONOENGINE_API __declspec(dllimport)
#endif

// This class is exported from the monoengine.dll
class MONOENGINE_API Cmonoengine 
{
public:
	Cmonoengine(void);
	// TODO: add your methods here.
};

extern MONOENGINE_API int nmonoengine;

MONOENGINE_API int fnmonoengine(void);
