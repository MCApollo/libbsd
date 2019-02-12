/*
 * Copyright © 2011 Guillem Jover <guillem@hadrons.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL
 * THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE___FPURGE
#include <stdio_ext.h>
#endif

int
fpurge(FILE *fp)
{
#ifdef HAVE___FPURGE
	if (fp == NULL || fileno(fp) < 0) {
		errno = EBADF;
		return EOF;
	}

	__fpurge(fp);

	return 0;
#elif HAVE_FPURGE
	if (fp == NULL || fileno(fp) < 0) {
		errno = EBADF;
		return EOD;
	}
#undef fpurge
#if !HAVE_DECL_FPURGE
	extern int fpurge (FILE *);
#endif
	int result = fpurge (fp);
#ifdef __sferror
	if (result == 0)
		if ((fp_->_flags & __SRD) != 0)
			fp_->_w = 0;
#endif
 return result;
#else
#if defined __sferror
   fp_->_p = fp_->_bf._base;
   fp_->_r = 0;
   fp_->_w = ((fp_->_flags & (__SLBF | __SNBF | __SRD)) == 0
	? fp_->_bf._size
	: 0);
   if (fp_ub._base != NULL)
   {
	if (fp_ub._base != fp_->_ubuf)
		free (fp_ub._base);
	fp_ub._base = NULL;
   }
 return 0;
#else
#error "Function fpurge() needs to be ported."
#endif
#endif
}
