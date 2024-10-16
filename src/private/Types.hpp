#ifndef RLLANG_PRIVATE_TYPES
#define RLLANG_PRIVATE_TYPES

#ifdef __GNUC__
	#define ROBINLE_PACKED __attribute__((packed))
#elif defined(_MSC_VER)
	#define ROBINLE_PACKED
	#pragma pack(push, 1)
#else
	#error Turning of automatic padding is not configured for your compiler.
#endif





#include <cstdint>



constexpr char    szMAGIC_NUMBER[] = "rlLang";
constexpr uint8_t iVERSION[2]      = { 0x01, 0x00 };

constexpr size_t iCHUNK_TYPE_IDENT_LENGTH = 4;
constexpr char sCHUNK_TYPE_HDR [iCHUNK_TYPE_IDENT_LENGTH] = { 0x68, 0x64, 0x72, 0x20 };
constexpr char sCHUNK_TYPE_STR [iCHUNK_TYPE_IDENT_LENGTH] = { 0x73, 0x74, 0x72, 0x20 };
constexpr char sCHUNK_TYPE_DATA[iCHUNK_TYPE_IDENT_LENGTH] = { 0x64, 0x61, 0x74, 0x61 };

// todo: define optional chunk type ident strings

constexpr uint32_t ChunkTypeIdentStringToInt(const char (&sChunkTypeIdent)[4])
{
	return
		(uint32_t(sChunkTypeIdent[0]) << 24) |
		(uint32_t(sChunkTypeIdent[1]) << 16) |
		(uint32_t(sChunkTypeIdent[2]) <<  8) |
		(uint32_t(sChunkTypeIdent[3]) <<  0);
}



struct FileHeader
{
	char     sMagicNo[6];
	uint8_t  iVersion[2];
	uint64_t iFileSize;
} ROBINLE_PACKED;

struct ChunkHeader
{
	char     sChunkType[4];
	uint64_t iChunkSize;
} ROBINLE_PACKED;

struct Chunk_hdr
{
	char     sLang[2];
	uint64_t iCount;
} ROBINLE_PACKED;





#undef ROBINLE_PACKED

#ifdef _MSC_VER
	#pragma pack(pop)
#endif

#endif // RLLANG_PRIVATE_TYPES