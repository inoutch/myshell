#ifndef MYSHELL_PRINT_UTIL_H
#define MYSHELL_PRINT_UTIL_H

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include "struct.h"
#include "define.h"

static inline void printBytes(const char *data, size_t size) {
    const size_t c = 25;
    const size_t r = (size - 1) / c + 1;
    const size_t s = size - (r - 1) * c;

    for (int y = 0; y < r - 1; y++) {
        for (int x = 0; x < c; x++) {
            printf("%02hhx ", data[y * c + x]);
        }
        for (int x = 0; x < c; x++) {
            char ch = data[y * c + x];
            printf("%c", ch >= 0x20 && ch <= 0x7e ? ch : ' ');
        }
        printf("\n");
    }

    //  last column
    for (int x = 0; x < s; x++) {
        printf("%02hhx ", data[(r - 1) * c + x]);
    }
    for (int x = 0; x < c - s; x++) {
        printf("   ");
    }
    for (int x = 0; x < s; x++) {
        char ch = data[(r - 1) * c + x];
        printf("%c", ch >= 0x20 && ch <= 0x7e ? ch : '.');
    }
    for (int x = 0; x < c - s; x++) {
        printf(" ");
    }
    printf("\n");
}

static inline void printSuperBlock(const superblock_t *info) {
    printf("ストレージブロック数 \t= %d\n", info->s_fsize);
    printf("inodeブロック数 \t\t= %d\n", info->s_isize);
}

static inline void printInode(const inode_t *inode, const char *root) {
    printf("ディレクトリからの参照数 \t= %d\n", inode->i_nlink);
    printf("ファイルサイズ\t\t\t= %d\n", inode->i_size0 + (inode->i_size0 << 8));
    printf("ストレージ番号\t\t\t= ");
    for (int i = 0; i < 8; i++) printf("%d ", inode->i_addr[i]);
    printf("\n");

    if ((inode->i_mode & ILARG) == 0) {
        printf("直接参照\n");

        if ((inode->i_mode & IFDIR) != 0) {
            printf("ディレクトリ\n");
            for (int i = 0; i < 8 && inode->i_addr[i] != 0; i++) {
                auto b = root + BLOCK_SIZE * inode->i_addr[i];
                size_t total = 0;
                dir_t *d = NULL;
                while (total < BLOCK_SIZE) {
                    d = (dir_t *) (b + total);
                    if (d->ino == 0) {
                        break;
                    }
                    printf("dir %d: %s\n", d->ino, d->name);
                    total += sizeof(dir_t);
                }
            }
        }
    }
}

static inline void printPermission(int p) {
    char types[3] = {'r', 'w', 'x'};
    for (int i = 2; i >= 0; i--) {
        printf("%c", p & (1 << (i)) ? types[2 - i] : '-');
    }
}

static std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;
    while (getline(ss, item, delim)) {
        if (!item.empty()) {
            elems.push_back(item);
        }
    }
    return elems;
}

static bool readFile(std::vector<unsigned char> &data, const std::string &filename) {
    std::vector<unsigned char> ret;

    std::ifstream fin(filename, std::ios::in | std::ios::binary);
    if (!fin) {
        std::cout << filename + " not found" << std::endl;
        return false;
    }
    unsigned char c;
    while (!fin.eof()) {
        fin.read((char *) &c, sizeof(unsigned char));
        data.push_back(c);
    }
    fin.close();
    return true;
}

#endif //MYSHELL_PRINT_UTIL_H
