#ifndef MYSHELL_STRUCT_H
#define MYSHELL_STRUCT_H

typedef struct {
    uint16_t i_mode;
    char i_nlink;
    char i_uid;
    char i_gid;
    uint8_t i_size0;
    uint16_t i_size1;
    uint16_t i_addr[8];
    int16_t i_atime[2];
    int16_t i_mtime[2];
} inode_t;

typedef struct {
    short s_isize;
    short s_fsize;
    short s_nfree;
    short s_free[100];
    short s_ninode;
    short s_inode[100];
    char s_flock;
    char s_ilock;
    char f_fmod;
    char s_ronly;
    short s_time[2];
    short pad[48];
} superblock_t;

typedef struct {
    short ino;
    char name[14];
} dir_t;

#endif //MYSHELL_STRUCT_H
