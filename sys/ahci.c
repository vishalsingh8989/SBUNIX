#include <sys/defs.h> 
#include <sys/pci.h> 
#include <sys/ahci.h>
#include <sys/kprintf.h>

#define ATA_CMD_READ_DMA_EX 0x25
#define ATA_CMD_WRITE_DMA_EX 0x35
#define AHCI_BASE 0x400000

hba_port_t * sata_port[32];
hba_mem_t * abar;

void memset(void* dest, int value, int count) 
{
    uint8_t *dest_t = (uint8_t *) dest;
    for(int i = 0; i < count; i++)
        *dest_t++ = value;

}

//Note: from OSdev
void start_cmd(hba_port_t *port) {

    while(port->cmd & HBA_PxCMD_CR)
        kprintf("in start\n");

    port->cmd |= HBA_PxCMD_FRE;
    port->cmd |= HBA_PxCMD_ST;
}

void stop_cmd(hba_port_t *port) 
{
    port->cmd &= ~HBA_PxCMD_ST;

    while(1)
    {
        kprintf("in stop\n");
        if(port->cmd & HBA_PxCMD_FR)
            continue;
        if(port->cmd & HBA_PxCMD_CR)
            continue;
        break;
    }

    port->cmd &= ~HBA_PxCMD_FRE;

}

int check_type(hba_port_t *port) 
{
    return port->sig;
}

void port_rebase(hba_port_t *port, int portno) {
    stop_cmd(port);

    port->clb = AHCI_BASE + (portno<<10);
    //port->clbu = 0;
    memset((void*)(port->clb), 0, 1024);

    port->fb =AHCI_BASE + (32<<10) + (portno<<8);
    //port->fbu = 0;
    memset((void*)(port->fb), 0, 256);

    hba_cmd_header_t *cmdheader = (hba_cmd_header_t *) (port->clb);
    for(int i=0; i<32; i++)
    {
        cmdheader[i].prdtl = 8;
        cmdheader[i].ctba = AHCI_BASE + (40<<10) + (portno<<13) + (i<<8);
        //cmdheader[i].ctbau = 0;
        memset((void*)cmdheader[i].ctba, 0, 256);
    }

    start_cmd(port);
}


void probe_port(hba_mem_t *abar)
{
    kprintf("ABAR: %p\n", abar);

    uint32_t pi = abar->pi;

    kprintf("In probe port: %x, pi: %x\n", abar, pi);

    int i = 0;
    while (i < 32) {
        if (pi & 1) {
            int dt = check_type(&abar->ports[i]);
            if(dt == AHCI_DEV_SATA) {
                kprintf("SATA Drive found at port %d\n", i);
                sata_port[i] = &abar->ports[i];
                //port_rebase((hba_port_t *) abar->ports, i);
                kprintf("port rebase done\n");
                return;
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
                kprintf("No Drive found at port %d\n", i);
            }
        }
        pi >>= 1;
        i++;
    }
}


int find_cmdslot(hba_port_t *port)
{
    uint32_t slots = (port->sact | port->ci);
    int cmdslots = (abar->cap >> 8) & 0x0f;
    for(int i = 0; i < cmdslots; i++) {
        if((slots & 1) == 0)
            return i;
        slots >>= 1;
    }
    kprintf("Cannot find free command list entry");
    return -1;
}

int disk_rw(hba_port_t *port, uint32_t startl, uint32_t starth, uint16_t count, uint64_t *buf, uint8_t rw) 
{
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
        cmdtbl->prdt_entry[i].i   = 1;
        buf += 4*1034;
        count -= 16;
    }
    cmdtbl->prdt_entry[i].dba = (uint64_t) buf;
    cmdtbl->prdt_entry[i].dbc = count<<9;
    cmdtbl->prdt_entry[i].i   = 1;

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

    while((port->tfd & (ATA_STATUS_BSY | ATA_STATUS_DRQ)) && spin < 1000000)
    {
        spin++;
    }
    if(spin == 1000000)
    {
        kprintf("Port is hung\n");
        return 0;
    }

    port->ci = 1<<slot;

    while(1)
    {
        if((port->ci & (1<<slot)) == 0)
            break;
        if(port->is_rwc & HBA_PxIS_TFES)
        {
            kprintf("Read/Write disk error\n");
            return 0;
        }
    }

    if(port->is_rwc & HBA_PxIS_TFES)
    {
        kprintf("Read/Write disk error\n");
        return 0;
    }

    return 1;
}
