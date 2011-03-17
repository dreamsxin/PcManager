/*
 * aPLib compression library  -  the smaller the better :)
 *
 * Visual C/C++ header file
 *
 * Copyright (c) 1998-2000 by Joergen Ibsen / Jibz
 * All Rights Reserved
 */

#ifndef __APLIB_H_INCLUDED
#define __APLIB_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

unsigned int aP_pack(unsigned char *source,
                             unsigned char *destination,
                             unsigned int length,
                             unsigned char *workmem,
                             int (*callback) (unsigned int, unsigned int));

unsigned int aP_workmem_size(unsigned int inputsize);

unsigned int aP_depack_asm(unsigned char *source, unsigned char *destination);

unsigned int aP_depack_asm_fast(unsigned char *source, unsigned char *destination);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __APLIB_H_INCLUDED */
