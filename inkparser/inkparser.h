// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the INKPARSER_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// INKPARSER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef INKPARSER_EXPORTS
#define INKPARSER_API __declspec(dllexport)
#else
#define INKPARSER_API __declspec(dllimport)
#endif

// This class is exported from the inkparser.dll
class INKPARSER_API InkParser
{
	public:
		InkParser(void);
		~InkParser();
};

extern INKPARSER_API int ninkparser;

INKPARSER_API int fninkparser(void);
