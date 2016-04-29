#ifndef _UNPACK_TXSHELL_H_
#define  _UNPACK_TXSHELL_H_

#include <stdint.h>

#pragma once

// from vm/Common.h
typedef uint8_t             u1;
typedef uint16_t            u2;
typedef uint32_t            u4;
typedef uint64_t            u8;
typedef int8_t              s1;
typedef int16_t             s2;
typedef int32_t             s4;
typedef int64_t             s8;

// from libdex/DexFile.h
/*
* 160-bit SHA-1 digest.
*/
enum {
	kSHA1DigestLen = 20,
	kSHA1DigestOutputLen = kSHA1DigestLen * 2 + 1
};


// DexHeader, from libdex/DexFile.h
/*
* Direct-mapped "header_item" struct.
*/
typedef struct DexHeader {
	u1  magic[8];           /* includes version number */
	u4  checksum;           /* adler32 checksum */
	u1  signature[kSHA1DigestLen]; /* SHA-1 hash */
	u4  fileSize;           /* length of entire file */
	u4  headerSize;         /* offset to start of next section */
	u4  endianTag;
	u4  linkSize;
	u4  linkOff;
	u4  mapOff;
	u4  stringIdsSize;
	u4  stringIdsOff;
	u4  typeIdsSize;
	u4  typeIdsOff;
	u4  protoIdsSize;
	u4  protoIdsOff;
	u4  fieldIdsSize;
	u4  fieldIdsOff;
	u4  methodIdsSize;
	u4  methodIdsOff;
	u4  classDefsSize;
	u4  classDefsOff;
	u4  dataSize;
	u4  dataOff;
} DexHeader;


// decrypt header
int jiemi(uint32_t i1, uint32_t i2, uint32_t round, uint32_t *i3, uint32_t *i4);

#endif

