#ifndef SQLITE_OMIT_DISKIO

#include "codec.h"

void sqlite3_activate_see(const char *info)
{
}

// Free the encryption data structure associated with a pager instance.
// (called from the modified code in pager.c) 
void sqlite3pager_free_codecarg(void *pCodecArg)
{
  if (pCodecArg)
  {
    free(pCodecArg);
  }
}

// Encrypt/Decrypt functionality, called by pager.c
void* sqlite3Codec(void* pCodecArg, void* data, Pgno nPageNum, int nMode)
{
  Codec* codec = NULL;
  unsigned char* pageBuffer = NULL;
  int pageSize = 0;

  if (pCodecArg == NULL)
  {
    return data;
  }
  codec = (Codec*) pCodecArg;
  if (!CodecIsEncrypted(codec))
  {
    return data;
  }
  
  pageSize = sqlite3BtreeGetPageSize(CodecGetBtree(codec));

  switch(nMode)
  {
    case 0: // Undo a "case 7" journal file encryption
    case 2: // Reload a page
    case 3: // Load a page
      if (CodecHasReadKey(codec))
      {
        CodecDecrypt(codec, nPageNum, (unsigned char*) data, pageSize);
      }
      break;

    case 6: // Encrypt a page for the main database file
      if (CodecHasWriteKey(codec))
      {
        pageBuffer = CodecGetPageBuffer(codec);
        memcpy(pageBuffer, data, pageSize);
        data = pageBuffer;
        CodecEncrypt(codec, nPageNum, (unsigned char*) data, pageSize, TRUE);
      }
      break;

    case 7: // Encrypt a page for the journal file
      /* Under normal circumstances, the readkey is the same as the writekey.  However,
         when the database is being rekeyed, the readkey is not the same as the writekey.
         The rollback journal must be written using the original key for the
         database file because it is, by nature, a rollback journal.
         Therefore, for case 7, when the rollback is being written, always encrypt using
         the database's readkey, which is guaranteed to be the same key that was used to
         read the original data.
      */
      if (CodecHasReadKey(codec))
      {
        pageBuffer = CodecGetPageBuffer(codec);
        memcpy(pageBuffer, data, pageSize);
        data = pageBuffer;
        CodecEncrypt(codec, nPageNum, (unsigned char*) data, pageSize, FALSE);
      }
      break;
  }
  return data;
}

void* sqlite3pager_get_codecarg(Pager *pPager);

int sqlite3CodecAttach(sqlite3* db, int nDb, const void* zKey, int nKey)
{
  // Attach a key to a database.
  Codec* codec = (Codec*)malloc(sizeof(Codec));
  if (!codec)
      return SQLITE_NOMEM;

  // No key specified, could mean either use the main db's encryption or no encryption
  if (zKey == NULL || nKey <= 0)
  {
    // No key specified
    if (nDb != 0 && nKey < 0)
    {
      Codec* mainCodec = (Codec*) sqlite3pager_get_codecarg(sqlite3BtreePager(db->aDb[0].pBt));
      // Attached database, therefore use the key of main database, if main database is encrypted
      if (mainCodec != NULL && CodecIsEncrypted(mainCodec))
      {
        CodecCopy(codec, mainCodec);
        CodecSetBtree(codec, db->aDb[nDb].pBt);
        sqlite3PagerSetCodec(sqlite3BtreePager(db->aDb[nDb].pBt), sqlite3Codec, codec);
        db->aDb[nDb].pAux = codec;
        db->aDb[nDb].xFreeAux = sqlite3pager_free_codecarg;
      }
      else
      {
        CodecSetIsEncrypted(codec, FALSE);
        free(codec);
        codec = NULL;
      }
    }
  }
  else
  {
    // Key specified, setup encryption key for database
    CodecSetIsEncrypted(codec, TRUE);
    CodecSetHasReadKey(codec, TRUE);
    CodecSetHasWriteKey(codec, TRUE);
    CodecGenerateReadKey(codec, (const char*) zKey, nKey);
    CodecCopyKey(codec, TRUE);
    CodecSetBtree(codec, db->aDb[nDb].pBt);

    sqlite3PagerSetCodec(sqlite3BtreePager(db->aDb[nDb].pBt), sqlite3Codec, codec);
    db->aDb[nDb].pAux = codec;
    db->aDb[nDb].xFreeAux = sqlite3pager_free_codecarg;
  }
  return SQLITE_OK;
}

void sqlite3CodecGetKey(sqlite3* db, int nDb, void** zKey, int* nKey)
{
  // The unencrypted password is not stored for security reasons
  // therefore always return NULL
  *zKey = NULL;
  *nKey = -1;
}

int sqlite3_key(sqlite3 *db, const void *zKey, int nKey)
{
  // The key is only set for the main database, not the temp database
  return sqlite3CodecAttach(db, 0, zKey, nKey);
}

int sqlite3_rekey(sqlite3 *db, const void *zKey, int nKey)
{
  // Changes the encryption key for an existing database.
  int rc = SQLITE_ERROR;
  int pageSize = 0;
  int nPageCount = -1;
  Btree* pbt = NULL;
  Pager* pPager = NULL;
  Codec* codec = NULL;
  Pgno nPage = 0;
  Pgno nSkip = 0;
  DbPage *pPage;
  Pgno n;


  pbt = db->aDb[0].pBt;
  pPager = sqlite3BtreePager(pbt);
  codec = (Codec*) sqlite3pager_get_codecarg(pPager);

  if ((zKey == NULL || nKey == 0) && (codec == NULL || !CodecIsEncrypted(codec)))
  {
    // Database not encrypted and key not specified
    // therefore do nothing
    return SQLITE_OK;
  }

  if (codec == NULL || !CodecIsEncrypted(codec))
  {
    // Database not encrypted, but key specified
    // therefore encrypt database
    if (codec == NULL)
    {
        codec = (Codec*) malloc(sizeof(Codec));
        if (!codec)
            return SQLITE_NOMEM;
    }

    CodecSetIsEncrypted(codec, TRUE);
    CodecSetHasReadKey(codec, FALSE); // Original database is not encrypted
    CodecSetHasWriteKey(codec, TRUE);
    CodecGenerateWriteKey(codec, (const char*) zKey, nKey);
    CodecSetBtree(codec, pbt);
    sqlite3PagerSetCodec(pPager, sqlite3Codec, codec);
    db->aDb[0].pAux = codec;
    db->aDb[0].xFreeAux = sqlite3pager_free_codecarg;
  }
  else if (zKey == NULL || nKey == 0)
  {
    // Database encrypted, but key not specified
    // therefore decrypt database
    // Keep read key, drop write key
    CodecSetHasWriteKey(codec, FALSE);
  }
  else
  {
    // Database encrypted and key specified
    // therefore re-encrypt database with new key
    // Keep read key, change write key to new key
    CodecGenerateWriteKey(codec, (const char*) zKey, nKey);
    CodecSetHasWriteKey(codec, TRUE);
  }

  // Start transaction
  rc = sqlite3BtreeBeginTrans(pbt, 1);
  if (!rc)
  {
    // Rewrite all pages using the new encryption key (if specified)
    rc = sqlite3PagerPagecount(pPager, &nPageCount);
    nPage = (Pgno) nPageCount;

    pageSize = sqlite3BtreeGetPageSize(pbt);
    nSkip = PAGER_MJ_PGNO(pPager);


    for (n = 1; rc == SQLITE_OK && n <= nPage; n++)
    {
      if (n == nSkip) continue;
      rc = sqlite3PagerGet(pPager, n, &pPage);
      if (!rc)
      {
        rc = sqlite3PagerWrite(pPage);
        sqlite3PagerUnref(pPage);
      }
    }
  }

  if (rc == SQLITE_OK)
  {
    // Commit transaction if all pages could be rewritten
    rc = sqlite3BtreeCommit(pbt);
  }
  if (rc != SQLITE_OK)
  {
    // Rollback in case of error
    sqlite3BtreeRollback(pbt);
  }

  if (rc == SQLITE_OK)
  {
    // Set read key equal to write key if necessary
    if (CodecHasWriteKey(codec))
    {
      CodecCopyKey(codec, FALSE);
      CodecSetHasReadKey(codec, TRUE);
    }
    else
    {
      CodecSetIsEncrypted(codec, FALSE);
    }
  }
  else
  {
    // Restore write key if necessary
    if (CodecHasReadKey(codec))
    {
      CodecCopyKey(codec, TRUE);
    }
    else
    {
      CodecSetIsEncrypted(codec, FALSE);
    }
  }

  if (!CodecIsEncrypted(codec))
  {
    // Remove codec for unencrypted database
    sqlite3PagerSetCodec(pPager, NULL, NULL);
    db->aDb[0].pAux = NULL;
    db->aDb[0].xFreeAux = NULL;
    sqlite3pager_free_codecarg(codec);
  }
  return rc;
}

#endif // SQLITE_OMIT_DISKIO
