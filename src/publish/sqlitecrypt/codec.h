///////////////////////////////////////////////////////////////////////////////
// Name:        codec.h
// Purpose:     
// Author:      Ulrich Telle
// Modified by:
// Created:     2006-12-06
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file codec.h Interface of the codec class

#ifndef _CODEC_H_
#define _CODEC_H_


typedef struct Codec
{
  BOOL          m_isEncrypted;
  BOOL          m_hasReadKey;
  unsigned char m_readKey[16];
  BOOL          m_hasWriteKey;
  unsigned char m_writeKey[16];

  Btree*        m_bt; // Pointer to B-tree used by DB
  unsigned char m_page[SQLITE_MAX_PAGE_SIZE+8];
} Codec;


void CodecCopy(Codec* ctx, Codec* other);

void CodecGenerateReadKey(Codec* ctx, const char* userPassword, int passwordLength);

void CodecGenerateWriteKey(Codec* ctx, const char* userPassword, int passwordLength);

/// Encrypt data
void CodecEncrypt(Codec* ctx, int page, unsigned char* data, int len, BOOL useWriteKey/* = FALSE */);

/// Decrypt data
void CodecDecrypt(Codec* ctx, int page, unsigned char* data, int len);

void CodecCopyKey(Codec* ctx, BOOL read2write);

void CodecSetIsEncrypted(Codec* ctx, BOOL isEncrypted) { ctx->m_isEncrypted = isEncrypted; }
void CodecSetHasReadKey(Codec* ctx, BOOL hasReadKey) { ctx->m_hasReadKey = hasReadKey; }
void CodecSetHasWriteKey(Codec* ctx, BOOL hasWriteKey) { ctx->m_hasWriteKey = hasWriteKey; }
void CodecSetBtree(Codec* ctx, Btree* bt) { ctx->m_bt = bt; }

BOOL CodecIsEncrypted(Codec* ctx) { return ctx->m_isEncrypted; }
BOOL CodecHasReadKey(Codec* ctx) { return ctx->m_hasReadKey; }
BOOL CodecHasWriteKey(Codec* ctx) { return ctx->m_hasWriteKey; }
Btree* CodecGetBtree(Codec* ctx) { return ctx->m_bt; }
unsigned char* CodecGetPageBuffer(Codec* ctx) { return &ctx->m_page[4]; }




#endif
