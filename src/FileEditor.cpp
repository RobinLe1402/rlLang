#include <rlLang/FileEditor.hpp>



namespace rlLang
{

	FileEditor::Result::Result(
		std::vector<std::u8string> &&warnings,
		std::vector<std::u8string> &&errors
	) :
		m_oWarnings(std::move(warnings)),
		m_oErrors  (std::move(errors  ))
	{ }

	FileEditor::Result FileEditor::loadFromFile(const FilepathChar *szFilePath)
	{
		std::vector<std::u8string> warnings;
		std::vector<std::u8string> errors;

		try
		{

		}
		catch (const std::exception &e)
		{
			errors.push_back((const char8_t *)e.what());
		}

		return Result(std::move(warnings), std::move(errors));
	}

	FileEditor::Result FileEditor::saveToFile(const FilepathChar *szFilePath) const
	{
		std::vector<std::u8string> warnings;
		std::vector<std::u8string> errors;

		try
		{

		}
		catch (const std::exception &e)
		{
			errors.push_back((const char8_t *)e.what());
		}

		return Result(std::move(warnings), std::move(errors));
	}

}
