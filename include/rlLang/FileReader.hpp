/*
	A class for reading rlLang files, decoding only the critical chunks.

	This class is written to be used in applications using rlLang files for translations. It's
	written for speed, so there are no checks for all the conventions the specification requires,
	like unique strings in the "str " section - only the most important checks are done.
*/
#ifndef RLLANG_FILE_READER
#define RLLANG_FILE_READER





#include "PlatformSpecific.hpp"

#include <memory>
#include <string>
#include <vector>



namespace rlLang
{

	class FileReader final
	{
	public: // methods

		FileReader(const FilepathChar *szFilePath);
		~FileReader() = default;

		explicit operator bool() const { return m_bValid; }
		bool           isValid() const { return m_bValid; }

		const char *lang() const { return m_szLang; }

		size_t count() const { return m_iStringCount; }
		const char8_t *get(size_t idx) const { return m_upStrings[idx]; }


	private: // variable

		bool m_bValid    = false;
		char m_szLang[3] = {};
		std::unique_ptr<char8_t[]> m_upData;
		std::unique_ptr<const char8_t *[]> m_upStrings;
		size_t m_iStringCount = 0;

	};

}





#endif // RLLANG_FILE_READER