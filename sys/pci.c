#include<sys/defs.h>
#include<sys/ahci.h>
#include<sys/pci.h>
#include<sys/kprintf.h>
#include<sys/asm_utils.h>

uint64_t *abar;

//static int get_number_of_lowest_set_bit (uint32_t value){
//
//	int pos = 0;
//	uint32_t mask = 0x00000001;
//	while (! (value & mask)){
//		++ pos;
//		mask = mask << 1; }
//	return pos;
//
//}
//
//static int get_number_of_highest_set_bit (uint32_t value){
//
//	int pos = 31;
//	uint32_t mask = 0x80000000;
//	while (! (value & mask))
//	{--pos; mask = mask >> 1; }
//	return pos;
//}

void write_to_data_port(uint32_t address){

	outl (PCI_CONFIG_ADDR, address);
}

uint32_t read_from_cmd_port(){
	return inl(PCI_CONFIG_DATA);
}

void pci_write(uint8_t bus, uint8_t slot,
        uint8_t func, uint8_t offset, int val){

	 uint32_t address = (uint32_t)((bus << 16) | (slot << 11) |
             (func << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));
	 outl (PCI_CONFIG_ADDR, address);
	 outl (PCI_CONFIG_DATA, val);
}


uint16_t pci_read(uint8_t bus, uint8_t slot,
                             uint8_t func, uint8_t offset)
 {
    uint32_t address;
    uint32_t lbus  = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
    uint16_t tmp = 0;

    /* create configuration address as per Figure 1 */
    address = (uint32_t)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));

    /* write address to cmd port */
    write_to_data_port(address);
    /* after write_to_data_port read data from data port
     read in the data
     (offset & 2) * 8) = 0 will choose the first word of the 32 bits register */
    tmp = (uint16_t)((read_from_cmd_port() >> ((offset & 2) * 8)) & 0xffff);
    return (tmp);
 }

void get_device_config(int bus, int dev, int func) {

    int base_class;
    int sub_class;
//    int sec_bus;

    base_class = pci_read(bus, dev, func, PCI_CLASS_OFFSET) >> 8;
    sub_class  = pci_read(bus, dev, func, PCI_SUB_CLASS_OFFSET) & 0xff;

    //kprintf("CLASS: %x, SUB_CLASS: %x\n", base_class, sub_class);

    if(base_class == 0x01 && sub_class == 0x06) {
        kprintf("AHCI Found!!\n");
        uint32_t address = (uint32_t)((bus << 16) | (dev << 11) |
                           (func << 8) | 0x24 | ((uint32_t)0x80000000));
        kprintf("address %p\n", address);
        outl(PCI_CONFIG_ADDR, address);
        volatile uint64_t bar = inl(PCI_CONFIG_DATA) & 0xffffffff;
        kprintf("BAR before Remap: %p\n", bar);

        bar = 0xffffffff;
        outl(PCI_CONFIG_ADDR, address);
        outl(PCI_CONFIG_DATA, bar);
        bar = inl(PCI_CONFIG_DATA) & 0xffffffff;
        kprintf("BAR Range: %p\n", bar);

        //bar = 0x10010000;
        bar = 0x30000000;
        outl(PCI_CONFIG_ADDR, address);
        outl(PCI_CONFIG_DATA, bar);
        bar = inl(PCI_CONFIG_DATA) & 0xffffffff;
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



void get_bar(uint8_t bus, uint8_t device,uint8_t function){
    uint16_t type ; //one bit required

    type = pci_read(bus, device, function, 0x0E) & 0x7F;
    uint32_t bar_offset;
    if(type == 0x00){   //device
    		for(int bar_num  = 0; bar_num < 6 ; bar_num++){ // 6 bars for device
    			bar_offset	      = 0x10 + bar_num*4; //bar start from 0x10 and each bar is 4w size
    			if(bar_num == 0) bar_offset = 0x25;
    			if(bar_num == 1) bar_offset = 0x27;
    			if(bar_num == 2) bar_offset = 0x21;
    			if(bar_num == 3) bar_offset = 0x23;




    			//kprintf("bar offset %d :  %p\n",bar_num,bar_offset);
    			uint8_t bar_type  = (pci_read(bus, device, function, bar_offset) & 0x1) ? BAR_IO : BAR_MEM; //first is always 0 for mem
    			if(bar_type == BAR_MEM){
//				kprintf("BAR is mem.\n");
				//check bit size
				switch ((pci_read(bus, device, function, bar_offset) >> 1) & 0x3){
				//
					case 0:
					{
							uint64_t bar_value = pci_read(bus, device, function ,bar_offset);
							if (bar_value !=0x00){ // 0 not valid
								uint64_t* address = (uint64_t*)(bar_value & ~0x3);
								//abar = (uint32_t *)bar_value;
								kprintf("BAR value is %p, %p\n ", bar_value, address);



							}else{

							}
							break;
					}
					case 2:// 64 bit
						kprintf("BAR 64 bit.\n");
						break;

				}


    			}else{
    				kprintf("BAR is I/O.\n");
    			}
    		}


    }else if(type ==0x01){//PCI bridge
    		kprintf("PCI bridge type not supported\n");

    }else{
    		kprintf("bar type not supported\n");

    }

////    int num_bars =  6-(type*4);
//    kprintf("type is  %d.\n.", type);
////    if(num_bars >= 6)
////        return 0x00;
//	uint64_t bar_value = pci_read(bus, device, function, 0x10 + 4*barnum);
//
//	 //uint8_t type = (bar_value & 0x1) ? BAR_IO : BAR_MEM;
//    //uint8_t bar_type = (temp & 0x1) ? BAR_IO : BAR_MEM;
//	 uint64_t address=0x00;
///*---------------------------------
//    Memory Space BAR Layout
//    ____________________________________________________________________
//   | 31 - 4	                       |   3           |	 2 - 1 |	    0     |
//   | 16-Byte Aligned Base Address  |	 Prefetchable  |	Type	   | Always 0 |
//   ---------------------------------------------------------------------
// */
//    if(type == BAR_MEM){
//    		//kprintf("BAR_MEM found\n.");
//        switch((bar_value >> 1) & 0x3){ //check Type
//
//        		case 0: //for 32bit value
//				address = (bar_value & ~0x3);
//				//kprintf("BAR size is 32 bit\n");
//				break;
//            case 2: //nope its for 64 bit
//            		//address = (uint32_t*) (bar_value & ~0x3); //one bit required
//            		//kprintf("BAR size is 64 bit\n");
//                break;
//            default:
//            		kprintf("BAR size is not 64 bit\n");
//        }
//
//    }else{
//    	//kprintf("BAR_IO found\n.");
//    }





}



// check device templete from osdev.
void scan_device(uint8_t bus, uint8_t device) {

     uint8_t num_of_function = is_valid_device(bus,  device, 0) ?8 : 1; //0th function will tell if device is valid or not.valid device has 8 function and invalid has 1
     for(uint8_t function = 0;  function <  num_of_function ;function++){
    	 	  uint16_t device_id = pci_read(bus, device, function, PCI_DEVICE_ID_OFFSET);
    	 	  if(device_id == PCI_INVALID_DID){
    	 		  	 continue;
    	 	  }
    	      uint16_t vendor_id = pci_read(bus, device, function, PCI_VENDOR_ID_OFFSET);
    	      kprintf("PCI bus %p : Vendor id : %p , Device id : %p. Function : %p \n",bus, vendor_id, device_id, function);

    	    	  get_bar(bus, device, function);

    	      //get_device_config(bus, device, function);
    	      }


    	     // kprintf("value at bar %");


 }



//brute force scan from OSdev
int scan_pci(){
	kprintf("Initialize PCI controller\n");
    uint8_t bus;
    uint8_t device;
    for(bus = 0; bus < PCI_BUS_COUNT; bus++) {
        for(device = 0; device < PCI_DEVICE_PER_BUS; device++) {
        	scan_device(bus, device);
        }
    }
	return 1;

}
