#include <sys/defs.h>
#include <sys/kprintf.h>
#include <sys/asm_utils.h>
#include <sys/utils.h>
#include <sys/vmm.h>
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

uint32_t read(uint16_t bus, uint16_t device, uint16_t function, uint32_t offset)
{
    uint32_t address =
        0x1 << 31
        | ((bus & 0xFF) << 16)
        | ((device & 0x1F) << 11)
        | ((function & 0x07) << 8)
        | (offset & 0xFC);
    outl (PCI_CONFIG_ADDR, address);
    uint32_t result = inl (PCI_CONFIG_DATA);
    return result >> (8* (offset % 4));
}

uint64_t getPCIBarType(uint16_t bus, uint16_t dev, uint16_t func, uint16_t bar, int* type)
{
    uint64_t result = -1;
    uint32_t header_type = read(bus, dev, func, 0x0E) & 0x7F;
    int num_of_bars =  6  -  (header_type*4);
    if(bar >= num_of_bars){
        return result;
    }


    uint32_t bar_value = read(bus, dev, func, 0x10 + 4*bar);
    uint32_t bar_type = (bar_value & 0x1) ? NETIF_IO : NETIF_MEM;
    *type = bar_type;

    //io_base = pciConfigHeader->getPCIBar(PCI_BAR_IO) & ~1;
    //mem_base = pciConfigHeader->getPCIBar( PCI_BAR_MEM) & ~3;

    if(bar_type == NETIF_MEM)
    {
        //kprintf("Memory type\n");

        switch((bar_value >> 1) & 0x3)
        {

            case 0: // 32 Bit Mode
                //kprintf("32 BIT MODE\n");
                break;
            case 1: // 20 Bit Mode
                //kprintf("20 BIT MODE\n");
            case 2: // 64 Bit Mode
                //kprintf("64 BIT MODE\n");
                break;

        }
        result= (bar_value & ~0x1 )- KERNAL_BASE_ADDRESS;

    }
    else{ // NETIF_IO{
           //kprintf("Input Output type\n");
            result= bar_value & ~0x3;

    }


    return result;
}


void pci_check_func(int bus, int dev, int func) {

    int base_class;
    int sub_class;
    int sec_bus;

    base_class = pic_read(bus, dev, func, PCI_CLASS) >> 8;
    sub_class  = pic_read(bus, dev, func, PCI_CLASS) & 0xff;

    klog(IMP, "CLASS: %x, SUB_CLASS: %x\n", base_class, sub_class);

    if(base_class == 0x01 && sub_class == 0x06) {
        klog(BOOTLOG,"AHCI Found!!, bus: %x, dev: %x, func: %x\n", bus, dev, func);
        uint32_t address = (uint32_t)((bus << 16) | (dev << 11) |
                           (func << 8) | 0x24 | ((uint32_t)0x80000000));
        outl(PCI_CONFIG_ADDR, address);
        volatile uint64_t bar = inl(PCI_CONFIG_DATA) & 0xffffffff;
        klog(BOOTLOG,"BAR before Remap: %p\n", bar);

        bar = 0xffffffff;
        outl(PCI_CONFIG_ADDR, address);
        outl(PCI_CONFIG_DATA, bar);
        bar = inl(PCI_CONFIG_DATA) & 0xffffffff;
        klog(INFO, "BAR Range: %p\n", bar);

        //bar = 0x10010000;

        bar = 0xa6000;

        //bar = 0xffffffff800a6000;
        //bar = 0xffffffff800b8000;
        //bar = 0xfc20400;
        outl(PCI_CONFIG_ADDR, address);
        outl(PCI_CONFIG_DATA, bar);
        bar = inl(PCI_CONFIG_DATA) & 0xffffffff;
        klog(BOOTLOG,"BAR after Remap: %p\n", bar);

        abar  = (uint64_t *)kmalloc(PAGE_SIZE*5);
        map_proc(bar, (uint64_t)abar);
        memcpy(abar,(uint64_t*)bar, PAGE_SIZE*5);

        //0xffffffff800a6000;//(uint64_t *) bar;

    }else if(NETWORK_CONTROLLER == base_class && ETHERNET_CONTROLLER == sub_class){
        klog(BOOTLOG,"Ethernet controller Found!!, bus: %x, dev: %x, func: %x\n", bus, dev, func);

        //uint64_t addr;
        int type;

        for(int baridx = 0; baridx < 6 ; baridx++){
            volatile uint64_t bar = getPCIBarType( bus,  dev,  func,  baridx, &type);
            klog(BOOTLOG,"Addr :  %p\n", bar);
            uint32_t address =
            0x1 << 31
            | ((bus & 0xFF) << 16)
            | ((dev & 0x1F) << 11)
            | ((func & 0x07) << 8)
            | ((0x10 + 4*baridx) & 0xFC);

            //outl(PCI_CONFIG_ADDR, address);
            //volatile uint64_t bar = inl(PCI_CONFIG_DATA) & 0xffffffff;
            if(type == NETIF_IO){
                    klog(BOOTLOG,"Map netif IO\n");
                    bar = 0xc8000;
                    //bar = 0xfc20400;
                    outl(PCI_CONFIG_ADDR, address);
                    outl(PCI_CONFIG_DATA, bar);
                    bar = inl(PCI_CONFIG_DATA) & 0xffffffff;

                    klog(BOOTLOG,"After Map netif IO :  %p\n", bar);
            }
        }




    }

    if((base_class == 0x06) && (sub_class == 0x04)) {
        sec_bus = pic_read(bus, dev, func, 0x19) >> 8;
        pci_scan_bus(sec_bus);
    }
}

void pci_scan_dev(int bus, int dev) {
    if(pic_read(bus, dev, 0, PCI_VENDOR_ID) != PCI_INVALID_VID) {
        klog(BOOTLOG, "Device Found -->  \n");
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
    klog(BOOTLOG, "Scanning PCI busses!\n");

    if((pic_read(0, 0, 0, PCI_HEADER_TYPE) & 0x80) == 0) {
        klog(BOOTLOG, "Only 1 Bus connected!\n");
        pci_scan_bus(0);
    }
    else {
        for(int func = 0; func < 8; func++) {
            if (pic_read(0, 0, func, PCI_VENDOR_ID) != PCI_INVALID_VID) {
                klog(BOOTLOG, "Detected bus: %d\n", func);
                pci_scan_bus(func);
            }
        }
    }


}
