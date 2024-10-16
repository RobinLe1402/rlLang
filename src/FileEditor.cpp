#include <rlLang/FileEditor.hpp>

#include "private/Types.hpp"

#include <fstream>



using namespace std::string_literals;



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

		std::ifstream file(szFilePath, std::ios::binary);

		try
		{
			if (!file)
				throw std::exception("Failed to open file for reading!");

			file.exceptions(std::ios::badbit | std::ios::eofbit | std::ios::failbit);

			// read + check file header
			FileHeader fh;
			file.read((char *)&fh, sizeof(fh));
			if (memcmp(fh.sMagicNo, szMAGIC_NUMBER, sizeof(fh.sMagicNo)) != 0)
				throw std::exception("Invalid magic number!");
			if (memcmp(fh.iVersion, iVERSION, sizeof(iVERSION)) != 0)
				throw std::exception("Unknown file format version!");

			// check the given file size (can possibly be ignored)
			{
				const auto offset = file.tellg();
				const size_t iFilesize = file.seekg(0, std::ios_base::end).tellg();
				if (iFilesize != fh.iFileSize)
					warnings.push_back(
						u8"File size in header ("s +
						(const char8_t *)std::to_string(fh.iFileSize).c_str() +
						u8" bytes) didn't match actual file size (" +
						(const char8_t *)std::to_string(fh.iFileSize).c_str() +
						u8" bytes)!"
					);

				file.seekg(offset);
			}


			// create an index of the chunks present in the file
			
			enum class ChunkType
			{
				unknown,
				hdr,
				str,
				data,
				astr,
				cstm,
				dir,
				id,
				note,
				odat,
				tag
			};

			struct ChunkIndexEntry
			{
				ChunkType      eType;
				size_t         iSize;
				std::streamoff oOffset; // right after the header
			};

			std::vector<ChunkIndexEntry> oChunkIndex;
			{
				ChunkHeader ch;
				while (file.peek() != EOF)
				{
					ChunkIndexEntry entry;
					file.read((char *)&ch, sizeof(ch));

					switch (ChunkTypeIdentStringToInt(ch.sChunkType))
					{
					case ChunkTypeIdentStringToInt(sCHUNK_TYPE_HDR):
						entry.eType = ChunkType::hdr;
						break;

					case ChunkTypeIdentStringToInt(sCHUNK_TYPE_STR):
						entry.eType = ChunkType::str;
						break;

					case ChunkTypeIdentStringToInt(sCHUNK_TYPE_DATA):
						entry.eType = ChunkType::data;
						break;


						// TODO: read optional chunk types
					}

					entry.oOffset = file.tellg();
					entry.iSize   = ch.iChunkSize;

				}
			}

			// check the general file structure (chunk order, if file ends on chunk boundary)
			
		}
		catch (const std::ifstream::failure &e)
		{
			if (file.eof())
				errors.push_back(u8"Unexpected EOF!");
			else
				errors.push_back(u8"File read error!");
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
