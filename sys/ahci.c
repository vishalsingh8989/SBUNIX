#include<sys/defs.h>
#include<sys/ahci.h>
#include<sys/kprintf.h>

#define RESET_CMD_ST 0xfffffffe
#define RESET_CMD_FRE 0xffffffef


#define HBA_PORT_CMD_ST 0x1
#define HBA_PORT_CMD_FRE 0x10
#define HBA_PORT_CMD_FR 0x4000
#define HBA_PORT_CMD_CR 0x8000


int check_type(hba_port_t port){

	uint32_t ssts =  port.ssts;

	uint8_t ipm = (ssts >> 8) & 0x0F;
	uint8_t det = ssts & 0x0F;
	if (ipm != 0x1  ||  det != 0x3) return ACHI_NO_DEVICE;

	switch (port.sig)
	{
		case AHCI_DEV_SATA:
			return AHCI_DEV_SATAPI;
		case AHCI_DEV_SATAPI:
			return AHCI_DEV_SEMB;
		case AHCI_DEV_SEMB:
			return AHCI_DEV_PM;
		case AHCI_DEV_PM:
			return AHCI_DEV_SATA;
	}
	return ACHI_NO_DEVICE;
}

int scan_ahci(){

	kprintf("AHCI_BASE %p\n", AHCI_BASE  + 0x24);
	hba_mem_t *hba = (hba_mem_t *)(AHCI_BASE + 0x24);
	uint32_t  hba_pi= hba->pi;
	kprintf("port implemtation %p\n", hba_pi);
	uint32_t hba_count = (hba->cap)&0x1f; //32 bits
	kprintf("hba_count: %p\n",hba_count);

	for(int i=0;i<=hba_count;i++){
		if(hba_pi&0x1){
				//kprintf("port : %d\n",i);
		}





		uint32_t cmd_st = (hba->ports[i].cmd)&HBA_PORT_CMD_ST;
		uint32_t cmd_fre = (hba->ports[i].cmd)&HBA_PORT_CMD_FRE;

		int dt = check_type(hba->ports[i]);
		if (dt == AHCI_DEV_SATAPI){
			kprintf("AHCI_DEV_SATAPI found at  %d\n", i);
		}else if (dt == AHCI_DEV_SATA){
			kprintf("AHCI_DEV_SATA drive found at  %d\n", i);
		}else if (dt == AHCI_DEV_SATAPI){
			kprintf("AHCI_DEV_SATAPI drive found at  %d\n", i);
		}else if (dt == AHCI_DEV_SEMB){
			kprintf("AHCI_DEV_SEMB drive found at port %d\n", i);
		}else{
			kprintf("No drive found %d\n", i);
		}

		if(cmd_st!=0||cmd_fre!=0){
						if(cmd_st!=0){
							hba->ports[i].cmd&=RESET_CMD_ST;
						}
						if(cmd_fre!=0){
							(hba->ports[i].cmd)&=RESET_CMD_FRE;
						}
					i--;
					continue;
					}

		hba_pi>>=1;

	}
	return 1;

}
