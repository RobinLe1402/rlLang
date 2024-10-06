/*
	A class for reading and writing rlLang files, decoding/encoding all currently defined chunks.

	This class is written to be used in rlLang file creation/editing applications. A check for the
	conventions given in the specification is also present.
*/
#ifndef RLLANG_FILE_EDITOR
#define RLLANG_FILE_EDITOR





#include "PlatformSpecific.hpp"

#include <memory>
#include <string>
#include <vector>



namespace rlLang
{

	class FileEditor final
	{
	public: // types

		using StringPtr = std::shared_ptr<std::u8string>;

		class Result final
		{
		public: // methods

			Result(std::vector<std::u8string> &&warnings, std::vector<std::u8string> &&errors);
			~Result() = default;

			operator bool() const { return success(); }

			bool success    () const { return  m_oErrors  .empty(); }
			bool hasWarnings() const { return !m_oWarnings.empty(); }

			const std::vector<std::u8string> &warnings() const { return m_oWarnings; }
			const std::vector<std::u8string> &errors  () const { return m_oErrors;   }


		private: // variables

			std::vector<std::u8string> m_oWarnings;
			std::vector<std::u8string> m_oErrors;

		};


	public: // methods

		Result loadFromFile(const FilepathChar *szFilePath);
		Result saveToFile  (const FilepathChar *szFilePath) const;


	private: // variables

		std::vector<StringPtr> m_oMainStrings;
		std::vector<StringPtr> m_oOrigStrings;
		std::vector<StringPtr> m_oNotes;
		std::vector<StringPtr> m_oTags;

		std::vector<std::string> m_oWarnings;
		std::vector<std::string> m_oErrors;

	};

}





#endif // RLLANG_FILE_EDITOR