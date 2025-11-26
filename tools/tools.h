#ifndef TOOLS_H
#define TOOLS_H

unsigned int tools_hash_file(const char* path, int* ok);

void tools_main();
void tools_hash();
void tools_compare();
void tools_compress();
void tools_decompress();
void tools_fileinfo();
void tools_hexdump();
void tools_strings();
void tools_decode();
void array_scan();


#endif
