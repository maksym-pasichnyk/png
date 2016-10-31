#include <cstdint>
#include <memory>

typedef uint8_t HEADER[8];

#pragma pack(push, 1)
struct INFO {
	uint32_t length;
	uint8_t type[5];
	uint8_t *data;
	uint32_t crc;
};

struct PNG {
	HEADER header;
	INFO IHDR;
	INFO tIME;
	INFO tEXt;
	INFO zTXt;
	INFO iTXt;
	INFO iCCP;
	INFO tRNS;
	INFO gAMA;
	INFO cHRM;
	INFO PLTE;
	INFO hlST;
	INFO bKGD;
	INFO pHYs;
	INFO sPLT;
	INFO sRGB;
	INFO IDAT;
	INFO sBIT;
	INFO IEND;
};
#pragma pack(pop)

uint16_t reverse_uint16_t(uint16_t value) {
	return (value >> 8) | (value << 8);
}

uint32_t reverse_uint32_t(uint32_t value) {
	return (value >> 24) | ((value >> 8) & 0xFF00) | ((value & 0xFF00) << 8) | (value << 24);
}

#define check_type(png, info, _type_) if (memcmp(#_type_, info.type, 4) == 0) { png->_type_ = info; }

static HEADER png_signature = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };

class File {
private:
	FILE *file;

public:
	File(const char *filename) {
		file = fopen(filename, "rb");
	}

	~File() { 
		if (file != NULL) {
			fclose(file); 
		} 
	}

	operator FILE*(){ return file; }
};

size_t read_png_file(PNG *png, const char *filename) {
	File file(filename);

	if (file == NULL) { return 0; }

	HEADER header;
	if (fread(header, sizeof(HEADER), 1, file) != 1) { return 0; }

	if (memcmp(header, png_signature, 8) != 0) { return 0; }

	while (true) {
		INFO info;
		memset(&info, 0, sizeof(INFO));

		if (fread(&info, 8, 1, file) != 1) { return 0; }

		size_t length = reverse_uint32_t(info.length);

		info.data = new uint8_t[length + 1]{ 0 };
		if (fread(info.data, 1, length, file) != length) {
			return 0;
		}

		if (fread(&info.crc, sizeof(info.crc), 1, file) != 1) {
			return 0;
		}

		//printf("TYPE:	%s\n", info.type);
		//printf("LENGTH:	%i\n", length);
		//printf("CRC:	0x%04X\n\n", reverse_uint32_t(info.crc));

		check_type(png, info, IHDR)
		else check_type(png, info, tIME)
		else check_type(png, info, tEXt)
		else check_type(png, info, zTXt)
		else check_type(png, info, iTXt)
		else check_type(png, info, iCCP)
		else check_type(png, info, tRNS)
		else check_type(png, info, gAMA)
		else check_type(png, info, cHRM)
		else check_type(png, info, PLTE)
		else check_type(png, info, hlST)
		else check_type(png, info, bKGD)
		else check_type(png, info, pHYs)
		else check_type(png, info, sPLT)
		else check_type(png, info, sRGB)
		else check_type(png, info, IDAT)
		else check_type(png, info, sBIT)
		else check_type(png, info, IEND);

		if (memcmp(info.type, "IEND", 4) == 0) { break; }
	}
	
	return 1;
}
