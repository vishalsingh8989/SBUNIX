#ifndef _PCI_H
#define _PCI_H

#include <sys/defs.h>

#define PCI_CONFIG_ADDR 0xCF8
#define PCI_CONFIG_DATA 0xCFC
#define PCI_HEADER_TYPE 0x0E
#define PCI_INVALID_VID 0xFFFF
#define PCI_VENDOR_ID   0x2
#define PCI_CLASS       0xA
#define PCI_SUB_CLASS   0xB
#define PCI_SEC_BUS     0x19

void init_pci(void);
void pci_scan_bus(int bus);

#endif
