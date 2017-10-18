#include <sys/defs.h> 
#include <sys/pci.h> 
#include <sys/ahci.h>
#include <sys/kprintf.h>
#include <sys/utils.h>

#define ATA_CMD_READ_DMA_EX 0x25
#define ATA_CMD_WRITE_DMA_EX 0x35
//#define AHCI_BASE 0x400000

//uint64_t __attribute__((aligned(4096))) cmd_list[8192];
//hba_cmd_list_t cmd_list;
//hba_fis_t fis;
hba_port_t * sata_port[32];
hba_mem_t * abar;

/*
void memset(void* dest, int value, int count) 
{
    uint8_t *dest_t = (uint8_t *) dest;
    for(int i = 0; i < count; i++)
        *dest_t++ = value;
}
*/

//Note: from OSdev
void start_cmd(hba_port_t *port) {

    //kprintf("In start, CMD: %x\n", port->cmd);
    while(port->cmd & HBA_PxCMD_CR)
        kprintf("In start, CMD: %x\n", port->cmd);

    port->cmd |= HBA_PxCMD_FRE;
    port->cmd |= HBA_PxCMD_ST;
        
    //kprintf("In start, CMD: %x\n", port->cmd);
}

void stop_cmd(hba_port_t *port) 
{
    //kprintf("In Stop, CMD: %x\n", port->cmd);
    port->cmd &= ~HBA_PxCMD_ST;

    while(1)
    {
        //kprintf("In Stop, CMD: %x\n", port->cmd);
        if(port->cmd & HBA_PxCMD_FR)
            continue;
        if(port->cmd & HBA_PxCMD_CR)
            continue;
        break;
    }
    port->cmd &= ~HBA_PxCMD_FRE;
    port->ie = (uint32_t) -1;
}

int check_type(hba_port_t *port) 
{
    //uint32_t ssts = port->ssts;

    //uint8_t ipm = (ssts >> 8) & 0x0f;
    //uint8_t det = ssts & 0x0f;

    //if(det != 3)
    //    return -1;
    //if(ipm != 1)
    //    return -1;

    return port->sig;
}

void port_rebase(hba_port_t *port, int portno) {

    abar->ghc |= (uint32_t) (1 << 31); //Enable AHCI
    abar->ghc |= (uint32_t) (1);       //Reset HBA
    abar->ghc |= (uint32_t) (1 << 31); //Enable AHCI
    abar->ghc |= (uint32_t) (1 << 1);  //Enable interrupts

    kprintf("Host Capabilities: %x, Global Host Control: %x\n", abar->cap, abar->ghc);
    kprintf("PI: %x, CAP: %x, CMD: %x, SSTS: %x, SERR: %x\n", abar->pi, abar->cap, port->cmd, port->ssts, port->serr_rwc);
    
    stop_cmd(port);

    uint64_t AHCI_BASE = (uint64_t) 0x9000;

    port->clb = AHCI_BASE + (portno<<10);
    memset((void*)(port->clb), 0, 1024);

    port->fb = AHCI_BASE + (32<<10) + (portno<<8);
    memset((void*)(port->fb), 0, 256);

    //kprintf("CMD: %x\n", port->cmd);

    hba_cmd_header_t *cmdheader = (hba_cmd_header_t *) (port->clb);
    for(int i=0; i<32; i++)
    {
        cmdheader[i].prdtl = 8;
        cmdheader[i].ctba = AHCI_BASE + (40<<10) + (portno<<13) + (i<<8);
        memset((void*)cmdheader[i].ctba, 0, 256);
    }

    start_cmd(port);

    port->sctl = 0x301;
    for(int i = 0; i < 20000000; i++);
    port->sctl = 0x300;
    port->cmd |= (HBA_PxCMD_SUD | HBA_PxCMD_POD | HBA_PxCMD_ICC);
    for(int i = 0; i < 20000000; i++);

    port->cmd |= HBA_PxCMD_FRE;
    port->serr_rwc = -1;
    port->is_rwc = -1;
}

void probe_port(hba_mem_t *abar)
{
    //kprintf("ABAR: %p\n", abar);

    uint32_t pi = abar->pi;

    //kprintf("In probe port: %x, pi: %x\n", abar, pi);

    int i = 0;
    while (i < 32) {
        if (pi & 1) {
            int dt = check_type(&abar->ports[i]);
            if(dt == AHCI_DEV_SATA) {
                kprintf("SATA Drive found at port %d\n", i);
                sata_port[i] = &abar->ports[i];
                if (i == 0) {
                    port_rebase((hba_port_t *) &abar->ports[i], i);
                    //kprintf("port rebase done\n");
                    return;
                }
            }
            else if(dt == AHCI_DEV_SATAPI) {
                kprintf("SATAPI Drive found at port %d\n", i);
            }
            else if(dt == AHCI_DEV_SEMB) {
                kprintf("SEMB Drive found at port %d\n", i);
            }
            else if(dt == AHCI_DEV_PM) {
                kprintf("PM Drive found at port %d\n", i);
            }
            else {
               //kprintf("No Drive found at port %d\n", i);
               kprintf("x ");
            }
        }
        pi >>= 1;
        i++;
    }
    kprintf("\n");
}


int find_cmdslot(hba_port_t *port)
{
    uint32_t slots = (port->sact | port->ci);
    //kprintf("SACT: %x, PI: %x\n", port->sact, port->ci);
    int cmdslots = (abar->cap >> 8) & 0x0f;
    for(int i = 0; i < cmdslots; i++) {
        if((slots & 1) == 0)
            return i;
        slots >>= 1;
    }
    kprintf("Cannot find free command list entry\n");
    return -1;
}

int disk_rw(hba_port_t *port, uint32_t startl, uint32_t starth, uint16_t count, uint8_t *buf, uint8_t rw) 
{
    //if(rw) kprintf("PI: %x, CAP: %x, CMD: %x, SSTS: %x, SERR: %x\n", abar->pi, abar->cap, port->cmd, port->ssts, port->serr_rwc);

    port->is_rwc = (uint32_t) -1; 
    int spin = 0;
    int slot = find_cmdslot(port);
    if(slot == -1)
        return 0;

    hba_cmd_header_t * cmdheader = (hba_cmd_header_t *) port->clb;
    cmdheader += slot;
    cmdheader->cfl = sizeof(fis_reg_h2d_t)/sizeof(uint32_t);
    cmdheader->w = (rw) ? 1 : 0;
    cmdheader->prdtl = (uint16_t)((count-1)>>4) + 1;
    
    hba_cmd_tbl_t * cmdtbl = (hba_cmd_tbl_t *) (cmdheader->ctba);
    memset(cmdtbl, 0, sizeof(hba_cmd_tbl_t) +
                      (cmdheader->prdtl-1)*sizeof(hba_prdt_entry_t));

    int i;
    for(i = 0; i < (cmdheader->prdtl-1); i++)
    {
        cmdtbl->prdt_entry[i].dba = (uint64_t) buf;
        cmdtbl->prdt_entry[i].dbc = 8*1024;
        cmdtbl->prdt_entry[i].i   = 0;
        buf += 4*1024;
        count -= 16;
    }
    cmdtbl->prdt_entry[i].dba = (uint64_t) buf;
    cmdtbl->prdt_entry[i].dbc = count<<9;
    cmdtbl->prdt_entry[i].i   = 0;

    fis_reg_h2d_t *cmdfis = (fis_reg_h2d_t *) (&cmdtbl->cfis);

    cmdfis->fis_type = FIS_TYPE_REG_H2D;
    cmdfis->c = 1;
    cmdfis->command = (rw) ? ATA_CMD_WRITE_DMA_EX : ATA_CMD_READ_DMA_EX;

    cmdfis->lba0 = (uint8_t) startl;
    cmdfis->lba1 = (uint8_t) (startl>>8);
    cmdfis->lba2 = (uint8_t) (startl>>16);
    cmdfis->lba3 = (uint8_t) (startl>>24);
    cmdfis->lba4 = (uint8_t) starth;
    cmdfis->lba5 = (uint8_t) (starth>>8);
    cmdfis->device = 1<<6;
    cmdfis->count = count;

    while((port->tfd & (ATA_STATUS_BSY | ATA_STATUS_DRQ)) && spin < 3000000)
    {
        spin++;
    }
    if(spin == 3000000)
    {
        kprintf("Error, SERR: %x, IS: %x\n", port->serr_rwc,  port->is_rwc);
        if(rw)
            kprintf("Port is hung in Write\n");
        else
            kprintf("Port is hung in Read\n");
        return 0;
    }

    port->ci = 1<<slot;

    while(1)
    {
        if((port->ci & (1<<slot)) == 0)
            break;
        if(port->is_rwc & HBA_PxIS_TFES)
        {
            if(rw)
                kprintf("Write disk error, IS: %x\n", port->is_rwc);
            else
                kprintf("Read disk error, IS: %x\n", port->is_rwc);
            return 0;
        }
    }

    if(port->is_rwc & HBA_PxIS_TFES)
    {
        if(rw)
            kprintf("Write disk error, IS: %x\n", port->is_rwc);
        else
            kprintf("Read disk error, IS: %x\n", port->is_rwc);
        return 0;
    }

    return 1;
}
