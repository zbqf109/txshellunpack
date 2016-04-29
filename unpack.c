#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "unpack.h"

// unpack TxAppEntity shell

enum { FORMAT_INVALID, FORMAT_DEX, FORMAT_ODEX };

int txshell_validate(FILE *fp)
{
#define ODEX_MAGIC_SIZE 8
	char magic[ODEX_MAGIC_SIZE];
	size_t sz;
	long curpos;
	int dexformat;
	

	if (fp == NULL)
	{
		return 0;
	}

	curpos = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	
	sz = fread(magic, ODEX_MAGIC_SIZE, 1, fp);
	if (sz != 1)
	{
		return 0;
	}

	if (0 == strcmp(magic, "dey\x0A\x30\x33\x36")) // dey\n036
	{
		dexformat = FORMAT_ODEX;
	}
	else if (0 == strcmp(magic, "dex\n035")) // dex\n035
	{
		dexformat = FORMAT_DEX;
	}
	else
	{
		dexformat = FORMAT_INVALID;
	}

	fseek(fp, curpos, SEEK_SET);

	return dexformat;
}

size_t txshell_unpack_header(const uint8_t *buffer, size_t len, DexHeader *pDexHeader)
{
	uint32_t i1, i2;
	uint32_t i3, i4;
	size_t i;

	//uint8_t *header_buffer;
	uint8_t *p;

	//DexHeader header;

	assert((len & 7) == 0);

	/*header_buffer = (uint8_t *)malloc(len);
	if (header_buffer == NULL)
	{
		return 0;
	}
	p = header_buffer;
	*/
	p = (uint8_t *)pDexHeader;

#define MAKE_UINT32(a, b, c, d) ((a) | ((b)<<8) | ((c)<<16) | ((d)<<24))

	for (i = 0; i < len; i += 8)
	{
		i1 = MAKE_UINT32(buffer[i], buffer[i + 1], buffer[i + 2], buffer[i + 3]);
		i2 = MAKE_UINT32(buffer[i + 4], buffer[i + 5], buffer[i + 6], buffer[i + 7]);

		jiemi(i1, i2, 0x20, &i3, &i4);

		memcpy(p, &i3, sizeof(i3));
		p += sizeof(i3);
		memcpy(p, &i4, sizeof(i4));
		p += sizeof(i4);
	}

	//free(header_buffer);

	return i;
}

int txshell_dump(FILE *fp, DexHeader *pDexHeader, const char *file)
{
	FILE *fw;
	u4 sz, tail;
	u4 i;
	u1 buffer[16];

	if (pDexHeader == NULL)
	{
		printf("Error: DEX header is NULL\n");
		return 5;
	}
	
	fw = fopen(file, "wb");
	if (fw == NULL)
	{
		printf("Error: cannot open file for write: %s\n", file);
		return 6;
	}

	fwrite(pDexHeader, sizeof(DexHeader), 1, fw);

	tail = (pDexHeader->fileSize - sizeof(DexHeader)) % sizeof(buffer);
	sz = pDexHeader->fileSize - sizeof(DexHeader) - tail;

	for (i = 0; i < sz; i+=sizeof(buffer))
	{ 
		fread(buffer, sizeof(buffer), 1, fp);
		fwrite(buffer, sizeof(buffer), 1, fw);
	}

	if (tail > 0)
	{
		fread(buffer, tail, 1, fp);
		fwrite(buffer, tail, 1, fw);
	}

	fclose(fw);

	return 0;
}

int txshell_unpack(const char *odex_file, const char *out_file)
{
#define  DEX_HEADER_SIZE 0x70
	FILE *fp = fopen(odex_file, "rb");
	uint8_t buffer[DEX_HEADER_SIZE]; // DEX header size
	size_t sz;
	size_t dex_fsize; // unpacked DEX file size
	DexHeader dexHeader;
	int result;
	int dexformat;
	int failed;

	if (fp == NULL)
	{
		printf("Error: Cannot open file: %s\n", odex_file);
		return 1;
	}

	// check if file valid
	dexformat = txshell_validate(fp);
	if (!dexformat)
	{
		// wrong file format 
		printf("Error: wrong file format: %s\n", odex_file);
		fclose(fp);
		return 2;
	}

	failed = 0;
	if (dexformat == FORMAT_ODEX)
	{
		if (0 != fseek(fp, 0x3028, SEEK_SET))
		{
			// seek failed
			failed = 1;
		}

		
	}
	else if (dexformat == FORMAT_DEX)
	{
		if (0 != fseek(fp, 0x3000, SEEK_SET))
		{
			failed = 1;
		}
	}
	else
	{
		failed = 1;
	}

	if (failed)
	{
		// seek failed
		printf("Error: file size too small: %s\n", odex_file);
		fclose(fp);
		return 3;
	}
	
	sz = fread(buffer, DEX_HEADER_SIZE, 1, fp);
	if (sz != 1)
	{
		// read header failed
		printf("Error: reader DEX header failed: %s\n", odex_file);
		fclose(fp);
		return 4;
	}

	dex_fsize = txshell_unpack_header(buffer, DEX_HEADER_SIZE, &dexHeader);

	printf("DEX file size: %u\n", dexHeader.fileSize);

	result = txshell_dump(fp, &dexHeader, out_file);

	fclose(fp);

	return result;
}

int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		printf("usage: %s <ODEX> <DEX>\n", argv[0]);
		return 0;
	}

	return txshell_unpack(argv[1], argv[2]);
}