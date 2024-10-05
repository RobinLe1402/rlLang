/*
	A class for reading rlLang files, decoding only the critical chunks.

	This class is written to be used in applications using rlLang files for translations. It's
	written for speed, so there are no checks for all the conventions the specification requires,
	like unique strings in the "str " section - only the most important checks are done.
*/
#ifndef RLLANG_HPP
#define RLLANG_HPP





#include <memory>
#include <string>
#include <vector>



namespace rlLang
{

	class rlLangFile
	{
	public: // methods

#ifndef _WIN32
		rlLangFile(const char8_t *szFilePath);
#else
		rlLangFile(const wchar_t *szFilePath);
#endif
		~rlLangFile() = default;


	private: // variable

		bool m_bValid = false;
		char m_szLang[3];
		std::unique_ptr<char8_t[]> m_upData;
		std::unique_ptr<const char8_t *[]> m_upStrings;
		size_t m_iStringCount;

	};

}





#endif // RLLANG_HPP