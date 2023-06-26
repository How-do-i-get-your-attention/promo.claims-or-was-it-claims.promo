#include <iostream>
#include <windows.h>
#include <windns.h>

#pragma comment(lib, "dnsapi.lib")

int main() {
    //Get the computer name
    char computerName[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD size = sizeof(computerName);

    if (GetComputerNameA(computerName, &size)) {
        std::cout << "Computer Name: " << computerName << std::endl;
    }
    else {
        std::cout << "Failed to get computer name. Error code: " << GetLastError() << std::endl;
    }


    //GGet nameserver
    PDNS_RECORD dnsRecord = nullptr;
    DNS_STATUS status = DnsQuery(L"How-do-i-get-your-attention.com", DNS_TYPE_NS, DNS_QUERY_STANDARD, nullptr, &dnsRecord, nullptr);

    PDNS_RECORD record = dnsRecord;

    while (record) {
        if (record->wType == DNS_TYPE_NS) {
            // Print the name server
            std::wcout << L"Name Server: " << record->Data.NS.pNameHost << std::endl;
        }
        record = record->pNext;
    }

    // Free the DNS records
    DnsRecordListFree(dnsRecord, DnsFreeRecordList);
    system("pause");
    return 0;
}
