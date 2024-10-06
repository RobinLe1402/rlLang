#ifndef RLLANG_PLATFORM_SPECIFIC
#define RLLANG_PLATFORM_SPECIFIC





namespace rlLang
{

#ifndef _WIN32
	// on most system, filepaths are in raw/UTF-8 format.
	using FilepathChar = char;
#else
	// on Windows, filepaths are UTF-16, encoded via wchar_t.
	using FilepathChar = wchar_t;
#endif

}





#endif // RLLANG_PLATFORM_SPECIFIC