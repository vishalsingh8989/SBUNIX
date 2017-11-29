#include <sys/defs.h>
#include <sys/kprintf.h>
#include <sys/asm_utils.h>
#include <sys/pci.h>

uint64_t *abar;

//Note: Template from OSdev
uint16_t pic_read (uint8_t bus, uint8_t slot, uint8_t func,
                   uint8_t offset)
{
    uint32_t address;
    uint32_t lbus  = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
    uint16_t tmp = 0;

    address = (uint32_t)((lbus << 16) | (lslot << 11) |
                         (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));

    outl (PCI_CONFIG_ADDR, address);

    tmp = (uint16_t)((inl (PCI_CONFIG_DATA) >> ((offset & 2) * 8)) & 0xffff);

    return (tmp);
}

void pci_check_func(int bus, int dev, int func) {

    int base_class;
    int sub_class;
    int sec_bus;

    base_class = pic_read(bus, dev, func, PCI_CLASS) >> 8;
    sub_class  = pic_read(bus, dev, func, PCI_CLASS) & 0xff;

    klog(IMP, "CLASS: %x, SUB_CLASS: %x\n", base_class, sub_class);

    if(base_class == 0x01 && sub_class == 0x06) {
        klog(IMP, "AHCI Found!!, bus: %x, dev: %x, func: %x\n", bus, dev, func);
        uint32_t address = (uint32_t)((bus << 16) | (dev << 11) |
                           (func << 8) | 0x24 | ((uint32_t)0x80000000));
        outl(PCI_CONFIG_ADDR, address);
        volatile uint64_t bar = inl(PCI_CONFIG_DATA) & 0xffffffff;
        klog(IMP, "BAR before Remap: %p\n", bar);

        bar = 0xffffffff;
        outl(PCI_CONFIG_ADDR, address);
        outl(PCI_CONFIG_DATA, bar);
        bar = inl(PCI_CONFIG_DATA) & 0xffffffff;
        klog(INFO, "BAR Range: %p\n", bar);

        //bar = 0x10010000;
        bar = 0xa6000;
        //bar = 0xfc20400;
        outl(PCI_CONFIG_ADDR, address);
        outl(PCI_CONFIG_DATA, bar);
        bar = inl(PCI_CONFIG_DATA) & 0xffffffff;
        abar = (uint64_t *) bar;
    }

    if((base_class == 0x06) && (sub_class == 0x04)) {
        sec_bus = pic_read(bus, dev, func, 0x19) >> 8;
        pci_scan_bus(sec_bus);
    }
}

void pci_scan_dev(int bus, int dev) {
    if(pic_read(bus, dev, 0, PCI_VENDOR_ID) != PCI_INVALID_VID) {
        klog(IMP, "Device Found -->  \n");
        pci_check_func(bus, dev, 0);
        if((pic_read(bus, dev, 0, PCI_HEADER_TYPE) & 0x80) != 0) {
            for(int func = 1; func < 8; func++) {
                if(pic_read(bus, dev, func, PCI_VENDOR_ID) != PCI_INVALID_VID)
                    pci_check_func(bus, dev, func);
            }
        }
    }
}

void pci_scan_bus(int bus) {
    for(int dev = 0; dev < 32; dev++) {
        pci_scan_dev(bus, dev);
    }
}

void init_pci ()
{
    klog(INFO, "Scanning PCI busses!\n");

    if((pic_read(0, 0, 0, PCI_HEADER_TYPE) & 0x80) == 0) {
        klog(INFO, "Only 1 Bus connected!\n");
        pci_scan_bus(0);
    }
    else {
        for(int func = 0; func < 8; func++) {
            if (pic_read(0, 0, func, PCI_VENDOR_ID) != PCI_INVALID_VID) {
                klog(IMP, "Detected bus: %d\n", func);
                pci_scan_bus(func);
            }
        }
    }
}
