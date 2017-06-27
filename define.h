#ifndef MYSHELL_DEFINE_H
#define MYSHELL_DEFINE_H

#define BLOCK_SIZE 512
#define IALLOC 0100000 // 0x1000
#define IFMT   060000  // 0x6000
#define IFDIR  040000  // 0x800
#define IFCHR  020000  // 0x2000
#define IFBLK  060000  // 0x6000
#define ILARG  010000  // 0x1000
#define ISUID  04000   // 0x800
#define ISGID  02000   // 0x400
#define ISVTX  01000   // 0x200
#define IREAD  0400    // 0x100
#define IWRITE 0200    // 0x80
#define IEXEC  0100    // 0x40

#endif //MYSHELL_DEFINE_H
