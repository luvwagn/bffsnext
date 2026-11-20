/*
 * gcc_compat.h - forced include (-include) for building BFFS with
 * m68k-amigaos-gcc instead of DICE C.
 *
 * BFFS was written for DICE, which has no 64-bit integer type; the source
 * treats int64_t / uint64_t as arrays of two longs and carries its own
 * stdint.h.  Pull in the compiler's headers first, then map the names the
 * BFFS source redefines onto private types so both can coexist.
 */
#ifndef _BFFS_GCC_COMPAT_H
#define _BFFS_GCC_COMPAT_H

#include <stdint.h>
#include <sys/types.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/intuition.h>
#include <proto/alib.h>

#ifndef _STDINT_H
#define _STDINT_H               /* keep fs/stdint.h (DICE) out */
#endif

typedef long          bffs_int64_t[2];
typedef unsigned long bffs_uint64_t[2];
#define int64_t  bffs_int64_t
#define uint64_t bffs_uint64_t
#define ino_t    bffs_ino_t

#endif /* _BFFS_GCC_COMPAT_H */
