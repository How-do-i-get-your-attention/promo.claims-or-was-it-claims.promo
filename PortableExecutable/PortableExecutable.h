#pragma once
#include <cstdint>   // For fixed-size integer types (e.g., uint16_t, uint32_t)

struct IMAGE_DOS_HEADER {
    uint16_t e_magic;        // Magic number
    uint16_t e_cblp;         // Bytes on the last page of the file
    uint16_t e_cp;           // Pages in the file
    uint16_t e_crlc;         // Relocations
    uint16_t e_cparhdr;      // Size of header in paragraphs
    uint16_t e_minalloc;     // Minimum extra paragraphs needed
    uint16_t e_maxalloc;     // Maximum extra paragraphs needed
    uint16_t e_ss;           // Initial (relative) SS value
    uint16_t e_sp;           // Initial SP value
    uint16_t e_csum;         // Checksum
    uint16_t e_ip;           // Initial IP value
    uint16_t e_cs;           // Initial (relative) CS value
    uint16_t e_lfarlc;       // File address of relocation table
    uint16_t e_ovno;         // Overlay number
    uint16_t e_res[4];       // Reserved words
    uint16_t e_oemid;        // OEM identifier (for e_oeminfo)
    uint16_t e_oeminfo;      // OEM information; e_oemid specific
    uint16_t e_res2[10];     // Reserved words
    uint32_t e_lfanew;       // File address of new exe header
};
