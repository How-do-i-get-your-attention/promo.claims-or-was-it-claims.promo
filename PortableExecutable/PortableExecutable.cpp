#include "PortableExecutable.h"
#include <iostream>
#include <fstream>
#include <iomanip>



void printDosHeader(const IMAGE_DOS_HEADER& dosHeader) {
    std::cout << "DOS Header:" << std::endl;
    std::cout << "e_magic: " << std::hex << dosHeader.e_magic << std::endl;
    std::cout << "e_cblp: " << std::hex << dosHeader.e_cblp << std::endl;
    std::cout << "e_cp: " << std::hex << dosHeader.e_cp << std::endl;
    std::cout << "e_crlc: " << std::hex << dosHeader.e_crlc << std::endl;
    std::cout << "e_cparhdr: " << std::hex << dosHeader.e_cparhdr << std::endl;
    std::cout << "e_minalloc: " << std::hex << dosHeader.e_minalloc << std::endl;
    std::cout << "e_maxalloc: " << std::hex << dosHeader.e_maxalloc << std::endl;
    std::cout << "e_ss: " << std::hex << dosHeader.e_ss << std::endl;
    std::cout << "e_sp: " << std::hex << dosHeader.e_sp << std::endl;
    std::cout << "e_csum: " << std::hex << dosHeader.e_csum << std::endl;
    std::cout << "e_ip: " << std::hex << dosHeader.e_ip << std::endl;
    std::cout << "e_cs: " << std::hex << dosHeader.e_cs << std::endl;
    std::cout << "e_lfarlc: " << std::hex << dosHeader.e_lfarlc << std::endl;
    std::cout << "e_ovno: " << std::hex << dosHeader.e_ovno << std::endl;
    std::cout << "e_oemid: " << std::hex << dosHeader.e_oemid << std::endl;
    std::cout << "e_oeminfo: " << std::hex << dosHeader.e_oeminfo << std::endl;
    std::cout << "e_lfanew: " << std::hex << dosHeader.e_lfanew << std::endl;
}

int main() {
    const std::string filePath = "D:\\How-do-i-get-your-attention\\promo.claims-or-was-it-claims.promo\\PCOrCP\\Services\\Manager.dll";

    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        std::cout << "Failed to open file: " << filePath << std::endl;
        return 1;
    }

    IMAGE_DOS_HEADER dosHeader;
    file.read(reinterpret_cast<char*>(&dosHeader), sizeof(IMAGE_DOS_HEADER));

    if (!file) {
        std::cout << "Failed to read DOS header from file: " << filePath << std::endl;
        return 1;
    }

    printDosHeader(dosHeader);

    return 0;
}
