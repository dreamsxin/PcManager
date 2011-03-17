/*
.c -- Binary patcher

Copyright 2003,2004 Colin Percival

For the terms under which this work may be distributed, please see
the adjoining file "LICENSE".
*/
#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>

#include <windows.h>
#include <process.h>
#include <sys/types.h>
#include "mergepublic.h"

long setoffset(int d, long offset)
{
	return _lseek(d,offset,SEEK_SET);
}

ssize_t loopread(int d,u_char *buf,size_t nbytes)
{
	size_t ptr,lenread;

	for(ptr=0;ptr<nbytes;ptr+=lenread) {
		lenread=_read(d,buf+ptr,(unsigned int)(nbytes-ptr));
		if(lenread==0) return (ssize_t)ptr;
		if(lenread==-1) return -1;
	};
	return (ssize_t)ptr;
}

off_t offtin(u_char *buf)
{
	off_t y;

	y=buf[7]&0x7F;
	y=y*256;y+=buf[6];
	y=y*256;y+=buf[5];
	y=y*256;y+=buf[4];
	y=y*256;y+=buf[3];
	y=y*256;y+=buf[2];
	y=y*256;y+=buf[1];
	y=y*256;y+=buf[0];

	if(buf[7]&0x80) y=-y;

	return y;
}

bool merge_2_files(
	const wchar_t* oldfile, 
	const wchar_t* newfile, 
	const wchar_t* patchfile
	)
{
	bool b_ret = false;
	int fdold, fdnew, fdpatch;
	long offtctrl, offtdiff, offtextra;
	pid_t pids[6] = {0};
	ssize_t patchsize,oldsize,newsize;
	ssize_t bzctrllen,bzdatalen;
	ssize_t bzctrlend,bzdataend,extraend;
	u_char header[32],buf[8];
	int version=0;
	u_char *old, *_new;
	off_t oldpos,newpos;
	off_t ctrl[3];
	off_t lenread;
	off_t i;

	if(((fdpatch=_wopen(patchfile,O_RDONLY|O_BINARY,0))<0) ||
		((patchsize=_lseek(fdpatch,0,SEEK_END))==-1) ||
		(_lseek(fdpatch,0,SEEK_SET)!=0)) errx((L"open patchfile failed\n"));
	if(patchsize<32) errx(L"Corrupt patch\n");

	/*
	Ok, this is going to be messy.  There are two different patch
	formats which we need to support.

	The old format (pre-4.0) is:
	0	8	"QSUFDIFF" or "BSDIFF30"
	8	8	X
	16	8	Y
	24	8	sizeof(newfile)
	32	X	bzip2(control block)
	32+X	Y	bzip2(data block)
	with control block a set of pairs (x,y) meaning "seek forward
	in oldfile by y bytes, and add the next x bytes to x bytes from
	the data block".

	The new format (4.0) is:
	0	8	"BSDIFF40"
	8	8	X
	16	8	Y
	24	8	sizeof(newfile)
	32	X	bzip2(control block)
	32+X	Y	bzip2(diff block)
	32+X+Y	???	bzip2(extra block)
	with control block a set of triples (x,y,z) meaning "add x bytes
	from oldfile to x bytes from the diff block; copy y bytes from the
	extra block; seek forwards in oldfile by z bytes".
	*/

	if(_read(fdpatch,header,32)!=32) errx(L"patchfile is too small, Corrupt patch\n");
	if(memcmp(header,"QSUFDIFF",8)==0) version=1;
	if(memcmp(header,"BSDIFF30",8)==0) version=1;
	if(memcmp(header,"BSDIFF40",8)==0) version=2;

	if(!version) errx(L"wrong version, Corrupt patch\n");

	bzctrllen=offtin(header+8);
	bzdatalen=offtin(header+16);
	newsize=offtin(header+24);
	if((bzctrllen<0) || (bzdatalen<0) || (newsize<0) ||
		((version==1) && (32+bzctrllen+bzdatalen!=patchsize)))
		errx(L"bzctrllen bzdatalen or newsize is wrong, Corrupt patch\n");

	offtctrl = 32;
	offtdiff = 32 + bzctrllen;
	offtextra = 32+bzctrllen+bzdatalen;
	
	bzctrlend = 32+bzctrllen;
	bzdataend = 32+bzctrllen+bzdatalen;
	extraend  = patchsize;

	if(((fdold=_wopen(oldfile,O_RDONLY|O_BINARY,0))<0) ||
		((oldsize=_lseek(fdold,0,SEEK_END))==-1) ||
		((old=(u_char*)malloc(oldsize+1))==NULL) ||
		(_lseek(fdold,0,SEEK_SET)!=0) ||
		(_read(fdold,old,oldsize)!=oldsize) ||
		(_close(fdold)==-1)) errx(L"open oldfile failed\n");
	if((_new=(u_char*)malloc(newsize+1))==NULL) errx(L"no memory\n");

	oldpos=0;newpos=0;
	while(newpos<newsize) {
		if (offtctrl>bzctrlend) errx(NULL);
		setoffset(fdpatch, offtctrl);
		for(i=0;i<=version;i++) {
			if((lenread=loopread(fdpatch,buf,8))<0) errx(L"loopread patchfile failed\n");
			if(lenread<8) errx(L"loopread error, Corrupt patch\n");
			ctrl[i]=offtin(buf);
			offtctrl+=8;
		};

		if(version==1) oldpos+=ctrl[1];
		if(newpos+ctrl[0]>newsize) errx(L"newpos+ctrl[0]>newsize, Corrupt patch\n");
		
		if (offtdiff>bzdataend) errx(L"offtdiff>bzdataend\n");
		setoffset(fdpatch, offtdiff);
		if((lenread=loopread(fdpatch,_new+newpos,ctrl[0]))<0)
			errx(L"loopread patchfile failed\n");
		offtdiff += ctrl[0];

		if(lenread!=ctrl[0]) errx(L"lenread!=ctrl[0], Corrupt patch\n");
		for(i=0;i<ctrl[0];i++)
			if((oldpos+i>=0) && (oldpos+i<oldsize))
				_new[newpos+i]+=old[oldpos+i];
		newpos+=ctrl[0];
		oldpos+=ctrl[0];

		if(version==2) {
			if(newpos+ctrl[1]>newsize) errx(L"newpos+ctrl[1]>newsize, Corrupt patch\n");
			if (offtextra>extraend) errx(L"offtextra>extraend, Corrupt patch\n");
			setoffset(fdpatch, offtextra);
			if((lenread=loopread(fdpatch,_new+newpos,ctrl[1]))<0)
				errx(L"loopread patchfile failed\n");
			if(lenread!=ctrl[1]) errx(L"lenread!=ctrl[1], Corrupt patch\n");
			offtextra += ctrl[1];

			newpos+=ctrl[1];
			oldpos+=ctrl[2];
		};
	};

	_close(fdpatch);
	if(((fdnew=_wopen(newfile,O_CREAT|O_TRUNC|O_WRONLY|O_BINARY,0666))<0) ||
		(_write(fdnew,_new,newsize)!=newsize) || (_close(fdnew)==-1))
		errx(L"write new file failed\n");

	free(_new);
	free(old);

	b_ret =  true;
	
ERROR_EXIT:
	return b_ret;
}
