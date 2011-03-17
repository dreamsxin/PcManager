///////////////////////////////////////////////////////////////////////////////
// Name:        codec.cpp
// Purpose:     
// Author:      Ulrich Telle
// Modified by:
// Created:     2006-12-06
// RCS-ID:      $$
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence + RSA Data Security license
///////////////////////////////////////////////////////////////////////////////

/// \file codec.cpp Implementation of MD5, RC4 and AES algorithms

/*
 **********************************************************************
 ** Copyright (C) 1990, RSA Data Security, Inc. All rights reserved. **
 **                                                                  **
 ** License to copy and use this software is granted provided that   **
 ** it is identified as the "RSA Data Security, Inc. MD5 Message     **
 ** Digest Algorithm" in all material mentioning or referencing this **
 ** software or this function.                                       **
 **                                                                  **
 ** License is also granted to make and use derivative works         **
 ** provided that such works are identified as "derived from the RSA **
 ** Data Security, Inc. MD5 Message Digest Algorithm" in all         **
 ** material mentioning or referencing the derived work.             **
 **                                                                  **
 ** RSA Data Security, Inc. makes no representations concerning      **
 ** either the merchantability of this software or the suitability   **
 ** of this software for any particular purpose.  It is provided "as **
 ** is" without express or implied warranty of any kind.             **
 **                                                                  **
 ** These notices must be retained in any copies of any part of this **
 ** documentation and/or software.                                   **
 **********************************************************************
 */

#include "codec.h"

#include "openssl\md5.h"
#include "codecfunc.h"

void
CodecCopy(Codec* codec, Codec* other)
{
  int j;
  codec->m_isEncrypted = other->m_isEncrypted;
  codec->m_hasReadKey  = other->m_hasReadKey;
  codec->m_hasWriteKey = other->m_hasWriteKey;
  for (j = 0; j < 16; j++)
  {
    codec->m_readKey[j]  = other->m_readKey[j];
    codec->m_writeKey[j] = other->m_writeKey[j];
  }
  codec->m_bt = other->m_bt;
  //RijndaelInvalidate(&codec->m_aes);
}

void
CodecCopyKey(Codec* codec, BOOL read2write)
{
  int j;
  if (read2write)
  {
    for (j = 0; j < 16; j++)
    {
      codec->m_writeKey[j] = codec->m_readKey[j];
    }
  }
  else
  {
    for (j = 0; j < 16; j++)
    {
      codec->m_readKey[j] = codec->m_writeKey[j];
    }
  }
}


void
CodecGenerateReadKey(Codec* codec, const char* userPassword, int passwordLength)
{
  CodecGenerateEncryptionKey(userPassword, passwordLength, codec->m_readKey);
}

void
CodecGenerateWriteKey(Codec* codec, const char* userPassword, int passwordLength)
{
  CodecGenerateEncryptionKey(userPassword, passwordLength, codec->m_writeKey);
}


void
CodecEncrypt(Codec* codec, int page, unsigned char* data, int len, BOOL useWriteKey)
{
  unsigned char* key = (useWriteKey) ? codec->m_writeKey : codec->m_readKey;
  CodecAES(page, TRUE, key, data, len, data);

}

void
CodecDecrypt(Codec* codec, int page, unsigned char* data, int len)
{
  CodecAES(page, FALSE, codec->m_readKey, data, len, data);
}

