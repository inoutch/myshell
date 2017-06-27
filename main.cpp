#include <vector>
#include "util.h"

using namespace std;

void collectDirectories(vector<dir_t> &directories, const char *root, const inode_t *current) {
    for (int i = 0; i < 8 && current->i_addr[i] != 0; i++) {
        auto b = root + BLOCK_SIZE * current->i_addr[i];
        if ((current->i_mode & ILARG) == 0) {
            size_t total = 0;
            dir_t *d = NULL;
            while (total < BLOCK_SIZE) {
                d = (dir_t *) (b + total);
                if (d->ino == 0) {
                    break;
                }
                directories.push_back(*d);
                total += sizeof(dir_t);
            }
        } else {
            for (int j = 0; j < BLOCK_SIZE / sizeof(inode_t); j++) {
                auto n = (inode_t *) b + sizeof(inode_t) * j;
                if ((n->i_mode & IALLOC) != 0) {
                    break;
                }
                collectDirectories(directories, root, n);
            }
        }
    }
}

inode_t *changeDir(vector<string> &targets, int i, inode_t *currentNode, const char *root, inode_t *inodes) {
    auto target = targets[i];
    vector<dir_t> directories;
    collectDirectories(directories, root, currentNode);

    //  find and replace current node
    for (auto d : directories) {
        if (target == d.name) {
            if ((inodes[d.ino - 1].i_mode & IFDIR) != 0) {
                if (i == targets.size() - 1) {
                    return &inodes[d.ino - 1];
                } else {
                    return changeDir(targets, i + 1, &inodes[d.ino - 1], root, inodes);
                }
            } else {
                cout << target + " is not directory\n";
            }
            return NULL;
        }
    }
    cout << target + " is not found\n";
    return NULL;
}

//  commands
void ls(const vector<string> &args, const inode_t *inode, const char *root, inode_t *inodes) {
    bool optionL = args.size() == 2 && args[1] == "-l";
    vector<dir_t> directories;
    collectDirectories(directories, root, inode);

    //  print directory information
    for (auto d : directories) {
        if (optionL) {
            auto n = inodes[d.ino - 1];
            cout << ((n.i_mode & IFDIR) == 0 ? "-" : "d");
            unsigned short p = (unsigned short) (n.i_mode & 0x1ff);
            printPermission(p >> 6);
            printPermission((p >> 3) & 0x07);
            printPermission(p & 0x07);
            printf(" %8d ", n.i_size1 + (n.i_size0 << 8));
        }
        printf("%s\n", d.name);
    }
}

void cd(const vector<string> &args, inode_t **currentNode, const char *root, inode_t *inodes) {
    if (args.size() < 2) {
        cout << "invalid arguments\n";
        return;
    }

    auto targets = split(args[1], '/');
    auto ret = changeDir(targets, 0, *currentNode, root, inodes);
    if (ret) {
        *currentNode = ret;
    }
}

int main() {
    //  read file
    vector<unsigned char> data;
    if (!readFile(data, "v6root")) {
        return 1;
    }

    //auto bootBlock = &data[BLOCK_SIZE * 0];
    //auto superBlock = (superblock_t *) &data[BLOCK_SIZE * 1];
    auto inodes = (inode_t *) &data[BLOCK_SIZE * 2];
    inode_t *currentNode = &inodes[0];
    string command;

    //  shell system
    while (1) {
        cout << ">>";
        getline(cin, command);
        auto args = split(command, ' ');

        if (args.size() == 0) {
            continue;
        } else if (args[0] == "ls") {
            ls(args, currentNode, (char *) &data[0], inodes);
        } else if (args[0] == "cd") {
            cd(args, &currentNode, (char *) &data[0], inodes);
        } else if (args[0] == "exit") {
            break;
        } else if (args[0] == "p") {
            printf("%p\n", currentNode);
        }
    }
    return 0;
}