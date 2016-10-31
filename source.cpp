#include <cstdio>

#include "png.h"

#include <string>

#pragma pack(push, 1)
struct IHDR {
	uint32_t width;
	uint32_t height;
	uint8_t depth;
	uint8_t type;
	uint8_t compression;
	uint8_t filter;
	uint8_t interlace;
};

struct tEXt {
	uint8_t *key;
	uint8_t *text;
};
#pragma pack(pop)

int main() {
	PNG png;
	memset(&png, 0, sizeof(PNG));

	if (read_png_file(&png, "C:\\Users\\dark-s0ul\\Desktop\\100x100.png") == 1) {
		auto ihdr = reinterpret_cast<IHDR *>(png.IHDR.data);

		printf("IHDR\n");
		printf("	Width			%u\n", reverse_uint32_t(ihdr->width));
		printf("	Height			%u\n", reverse_uint32_t(ihdr->height));
		printf("	Bit depth		%u\n", ihdr->depth);
		printf("	Color type		%u\n", ihdr->type);
		printf("	Compression method	%u\n", ihdr->compression);
		printf("	Filter method		%u\n", ihdr->filter);
		printf("	Interlace metho		%u\n", ihdr->interlace);
		printf("	CRC			%04X\n", reverse_uint32_t(png.IHDR.crc));

		printf("\n");

		tEXt text;
		text.key = png.tEXt.data;
		text.text = png.tEXt.data + strlen((char*)text.key) + 1;

		printf("tEXt\n");
		printf("	Key			%s\n", text.key);
		printf("	Text			%s\n", text.text);
	} else {
		memset(&png, 0, sizeof(PNG));
	}
}
