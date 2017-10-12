#include<sys/defs.h>
#include<sys/ahci.h>
#include<sys/pci.h>
#include<sys/kprintf.h>
#include<sys/asm_utils.h>

uint64_t *abar;


void dsleep()
{
	//inefficient sleep
	for(uint64_t sl = 0 ; sl <1 ;sl++ );
	}
void write_to_data_port(uint32_t address, uint64_t val){

	outl (PCI_CONFIG_ADDR, address);
	outl(PCI_CONFIG_DATA, val);
}

uint32_t read_from_cmd_port(){
	return inl(PCI_CONFIG_DATA);
}

//void pci_write(uint8_t bus, uint8_t slot,
//        uint8_t func, uint8_t offset, int val){
//
//	 uint32_t address = (uint32_t)((bus << 16) | (slot << 11) |
//             (func << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));
//	 outl (PCI_CONFIG_ADDR, address);
//	 outl (PCI_CONFIG_DATA, val);
//}


/*Calculate address using bus, device function and then offset the punch that address to PCI_CONFIG_ADDR using outl.
 * read result from PCI_CONFIG_DATA using intl - > in long , out long.
 */
uint16_t pci_read (uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset){
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

void get_device_config(int bus, int dev, int func) {

    int base_class;
    int sub_class;
    //int sec_bus;

    base_class = pci_read(bus, dev, func, 0xA) >> 8;
    sub_class  = pci_read(bus, dev, func, 0xA) & 0xff;

    //kprintf("CLASS: %x, SUB_CLASS: %x\n", base_class, sub_class);

    if(base_class == 0x01 && sub_class == 0x06) {
        kprintf("AHCI Found!!\n");
        uint32_t address = (uint32_t)((bus << 16) | (dev << 11) |
                           (func << 8) | 0x24 | ((uint32_t)0x80000000));
        kprintf("address : %p\n", address);
        outl(PCI_CONFIG_ADDR, address);
        volatile uint64_t bar = inl(PCI_CONFIG_DATA) & 0xffffffff;
        kprintf("BAR before Remap: %p\n", bar);

        //bar = 0x10000000;
        bar = 0x8000000;

        /*
         * any address between 640kb - 1mb
         */
        //bar = 0xa5000;
        //bar  = 0xa6000;

        outl(PCI_CONFIG_ADDR, address);
        outl(PCI_CONFIG_DATA, bar);
        bar = inl(PCI_CONFIG_DATA) & 0xffffffff;
        kprintf("BAR after Remap: %p\n", bar);
        abar = (uint64_t *) bar;
        kprintf("ABAR: %p\n", abar);
    }

//    if((base_class == 0x06) && (sub_class == 0x04)) {
//        sec_bus = pci_read(bus, dev, func, 0x19) >> 8;
//        pci_scan_bus(sec_bus);
//    }
}

int is_valid_device(uint8_t bus, uint8_t device, uint8_t function){
	return (pci_read(bus, device, function, PCI_HEADER_OFFSET)&(1<< 7)) != 0;
}

// check device templete from osdev.
void scan_device(uint8_t bus, uint8_t device) {

     uint16_t num_of_function = is_valid_device(bus,  device, 0) ?8 : 1; //0th function will tell if device is valid or not.valid device has 8 function and invalid has 1
     for(uint16_t function = 0;  function <  num_of_function ;function++){
    	 	  uint16_t device_id = pci_read(bus, device, function, PCI_DEVICE_ID_OFFSET);
    	 	  if(device_id != PCI_INVALID_DID){  //legends has it that some functions are not contiguous.So, break removed and print added.
    	 		 dsleep();
    	 		 uint16_t vendor_id = pci_read(bus, device, function, PCI_VENDOR_ID_OFFSET);
    	 		 kprintf("PCI bus : %x, Device: %x Function %x,", bus, device, function);
    	 		 kprintf("Vendor  : %p ,Device : %p\n" , vendor_id, device_id);
    	 		 get_device_config(bus, device, function);
    	 	  }

    	      }
 }

//brute force scan from OSdev
int scan_pci(){
	cls();
	kprintf("Initialize PCI controller\n");
	kprintf("Starting brute force scan of %d PCI bus and %d devices per bus.\n", PCI_BUS_COUNT, PCI_DEVICE_PER_BUS);
    uint8_t bus;
    uint8_t device;
    for(bus = 0; bus < PCI_BUS_COUNT; bus++) {
        for(device = 0; device < PCI_DEVICE_PER_BUS; device++) {
        	scan_device(bus, device);
        }
    }



	return 1;

}
