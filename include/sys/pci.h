#ifndef _PCI_H
#define _PCI_H


#define PCI_BUS_COUNT			4 // 255 bususes. but not required for project
#define PCI_DEVICE_PER_BUS		32
#define PCI_FUNC_PER_DEVICE		8
#define PCI_CONFIG_ADDR  		0xCF8
#define PCI_CONFIG_DATA 			0xCFC
#define PCI_HEADER_OFFSET 		0x0E
#define PCI_INVALID_DID			0xFFFF
#define PCI_VENDOR_ID_OFFSET   	0x0
#define PCI_DEVICE_ID_OFFSET   	0x2
#define PCI_CLASS_OFFSET       	0xB
#define PCI_SUB_CLASS_OFFSET   	0xA
#define PCI_INTERFACE_ID_OFFSET  0x9
#define PCI_INTERRUPT_OFFSET     0x3c
#define PCI_SEC_BUS     			0x19

#define BAR_MEM					0
#define BAR_IO					1



int scan_pci();
uint16_t pci_read(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
int is_valid_device(uint8_t bus, uint8_t device, uint8_t function);
void get_device_config(int bus, int dev, int func);
void scan_device(uint8_t bus, uint8_t device);


#endif
