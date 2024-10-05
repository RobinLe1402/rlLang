#include <rlLang.hpp>

#include <cstdint>
#include <fstream>



namespace rlLang
{

	namespace
	{

#ifdef __GNUC__
	#define PACKED __attribute__((packed))
#elif defined(_MSC_VER)
	#define PACKED
	#pragma pack(push, 1)
#else
	#error Turning of automatic padding is not configured for your compiler.
#endif

		constexpr char szMAGIC_NUMBER[] = "rlLang";
		constexpr uint8_t iVERSION[2] = { 0x01, 0x00 };

		constexpr size_t iCHUNK_TYPE_IDENT_LENGTH = 4;
		constexpr char szCHUNK_TYPE_HDR [iCHUNK_TYPE_IDENT_LENGTH] = { 0x68, 0x64, 0x72, 0x20 };
		constexpr char szCHUNK_TYPE_STR [iCHUNK_TYPE_IDENT_LENGTH] = { 0x73, 0x74, 0x72, 0x20 };
		constexpr char szCHUNK_TYPE_DATA[iCHUNK_TYPE_IDENT_LENGTH] = { 0x64, 0x61, 0x74, 0x61 };



		struct FileHeader
		{
			char     sMagicNo[6];
			uint8_t  iVersion[2];
			uint64_t iFileSize;
		} PACKED;

		struct ChunkHeader
		{
			char     sChunkType[4];
			uint64_t iChunkSize;
		};

		struct Chunk_hdr
		{
			char     sLang[2];
			uint64_t iCount;
		};



#ifdef _MSC_VER
	#pragma pack(pop)
#endif



		bool ReadLangFile(
			std::ifstream                     &&file,
			char                              (&szLang)[3],
			std::unique_ptr<char8_t[]>         &data,
			std::unique_ptr<const char8_t *[]> &ptrs,
			size_t                             &iStringCount
		)
		{
			file.exceptions(std::ios::badbit | std::ios::eofbit | std::ios::failbit);

			try
			{
				FileHeader  fh;
				ChunkHeader ch;
				Chunk_hdr   hdr;


				// read the file header
				file.read((char *)&fh, sizeof(fh));

				// check the magic number
				if (memcmp(szMAGIC_NUMBER, fh.sMagicNo, sizeof(fh.sMagicNo)) != 0)
					throw 0; // wrong magic number

				// check the version number
				if (memcmp(iVERSION, fh.iVersion, sizeof(fh.iVersion)) != 0)
					throw 0; // unknown version

				// check the file size
				{
					const auto prevOffset = file.tellg();
					file.seekg(0, std::ios_base::end);
					if (file.tellg() != fh.iFileSize)
						throw 0; // wrong file size
				}



				// read the "hdr " chunk

				// read the chunk header
				file.read((char *)&ch, sizeof(ch));
				if (memcmp(szCHUNK_TYPE_HDR, ch.sChunkType, iCHUNK_TYPE_IDENT_LENGTH) != 0)
					throw 0; // not a "hdr " chunk
				if (ch.iChunkSize != sizeof(Chunk_hdr))
					throw 0; // wrong size

				// read the data
				file.read((char *)&hdr, sizeof(hdr));

				// copy the language string
				szLang[0] = hdr.sLang[0];
				szLang[1] = hdr.sLang[1];
				szLang[2] = 0;

				iStringCount = hdr.iCount;
				if (iStringCount == 0)
					throw 0; // no strings
				ptrs = std::make_unique<const char8_t *[]>(iStringCount);



				// read the "str " chunk

				// read the chunk header
				file.read((char *)&ch, sizeof(ch));
				if (memcmp(szCHUNK_TYPE_STR, ch.sChunkType, iCHUNK_TYPE_IDENT_LENGTH) != 0)
					throw 0; // not a "str " chunk

				// read the data
				if (ch.iChunkSize == 0)
					throw 0; // no string data
				const auto iDataSize = ch.iChunkSize;
				data = std::make_unique<char8_t[]>(iDataSize);

				file.read((char *)data.get(), iDataSize);

				

				// read the "data" chunk

				// read the chunk header
				file.read((char *)&ch, sizeof(ch));
				if (memcmp(szCHUNK_TYPE_DATA, ch.sChunkType, iCHUNK_TYPE_IDENT_LENGTH) != 0)
					throw 0; // not a "data" chunk

				// read the data
				if (ch.iChunkSize != iStringCount * sizeof(uint64_t))
					throw 0; // wrong size of "data" chunk
				size_t iMaxOffset = 0;
				size_t iOffset;
				for (size_t iStringID = 0; iStringID < iStringCount; ++iStringID)
				{
					file.read((char *)&iOffset, sizeof(iOffset));
					if (iOffset > iMaxOffset)
						iMaxOffset = iOffset;
					ptrs[iStringID] = data.get() + iMaxOffset;
				}
				if (iMaxOffset >= iDataSize)
					throw 0; // largest given offset is beyond the valid data

				// check for at least a single trailing zero
				if (data.get()[iDataSize - 1])
					return false;

			}
			catch (...)
			{
				data         = nullptr;
				ptrs         = nullptr;
				iStringCount = 0;
				return false;
			}

			return true;
		}

	}

#ifndef _WIN32
	rlLangFile::rlLangFile(const char8_t *szFilePath)
	{
		m_bValid = ReadLangFile(
			std::ifstream((const char *)szFilePath, std::ios::binary),
			m_szLang,
			m_upData,
			m_upStrings,
			m_iStringCount
		);
	}
#else
	rlLangFile::rlLangFile(const wchar_t *szFilePath)
	{
		m_bValid = ReadLangFile(
			std::ifstream(szFilePath, std::ios::binary),
			m_szLang,
			m_upData,
			m_upStrings,
			m_iStringCount
		);
	}
#endif

}
