/*
 * Copyright 2018 Chris Hooper <amiga@cdh.eebugs.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted so long as any redistribution retains the
 * above copyright notice, this condition, and the below disclaimer.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _UFS_H
#define _UFS_H

#define ID_BFFS_DISK    (0x42464653L)  /* 'BFFS' filesystem */
#if 0
#define ID_BFFS_DISK    ID_FFS_DISK
#define ID_BFFS_DISK    ID_OFS_DISK
#endif

#define STARTUPMSG      ((struct FileSysStartupMsg *) BTOC(DevNode->dn_Startup))
#define ENVIRONMENT     ((struct DosEnvec *) BTOC(STARTUPMSG->fssm_Environ))

#define DISK_DEVICE     ((char *) BTOC(STARTUPMSG->fssm_Device)) + 1
#define DISK_UNIT       STARTUPMSG->fssm_Unit
#define DISK_FLAGS      STARTUPMSG->fssm_Flags

#define BOOT_BLOCK      0         /* Physical position of disk boot block */
#define SUPER_BLOCK     16        /* Position of superblock in fs         */
#define MAX_PART        8         /* Number disk partitions supported     */


typedef struct {
    struct Task *sigTask;
    ULONG        sigMask;
} intdata_t;
extern intdata_t IntData;

extern struct fs *superblock;    /* current filesystem superblock  */
extern struct IOExtTD *trackIO;  /* pointer to trackdisk IORequest */
extern int   bsd44fs;            /* 0 = 4.3 BSD, 1 = 4.4 BSD       */
extern int   fs_partition;       /* current partition number       */
extern ULONG phys_sectorsize;    /* physical disk sector size, env */
extern ULONG psectoffset;        /* disk partition start sector    */
extern ULONG psectmax;           /* disk partition end sector      */
extern ULONG tranmask;           /* device DMA transfer mask       */
extern ULONG pfragshift;         /* pointers in a frag             */
extern ULONG pfragmask;          /* mask current pointer in frag   */
extern ULONG fblkshift;          /* frags in a block               */
extern ULONG fblkmask;           /* mask current frag in block     */
extern ULONG fs_lfmask;          /* quick mask, bits flipped ala q */
extern ULONG fs_lbmask;          /* quick mask, bits flipped ala q */

#ifndef NULL
#define NULL 0
#endif

#define MAXSYMLINKLEN (12 + 3) * 4 - 1

/*
 * SYMLINK_IN_INODE() is true when a symlink's target is stored in the
 * inode's block pointer area ("fast" symlink) rather than in a data block.
 *
 * BSD 4.4 stores every target shorter than MAXSYMLINKLEN that way.
 * NeXTSTEP / OPENSTEP also create fast symlinks, but on BSD 4.3 format
 * filesystems (fs_inodefmt == -1), where bsd44fs is 0.  NeXT marks them
 * with IC_FASTLINK (0x1) in ic_flags and they hold no data blocks, so
 * ic_blocks == 0 identifies them (a block-based symlink always holds at
 * least one fragment).  The target may fill all (NDADDR + NIADDR) * 4 = 60
 * bytes, so no terminating NUL can be assumed for the 4.3 case.
 */
#define FASTLINK_MAX  ((12 + 3) * 4)
#define SYMLINK_IN_INODE(inode, size)                                   \
        (bsd44fs ? ((size) < (MAXSYMLINKLEN)) :                         \
                   (((inode)->ic_blocks == 0) && ((size) != 0) &&       \
                    ((size) <= FASTLINK_MAX)))

int  open_dchange(void);
void close_dchange(int normal);
void motor_off(void);
void motor_on(void);
void close_device(void);
int  open_device(void);
int  find_superblock(void);
int  superblock_flush(void);
void superblock_destroy(void);
int  data_read(void *buf, ULONG num, ULONG size);
int  data_write(void *buf, ULONG num, ULONG size);

#endif /* _UFS_H */
