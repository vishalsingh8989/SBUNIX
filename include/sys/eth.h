#ifndef _ETH_H_
#define _ETH_H_

#include<sys/defs.h>

#define INTEL_VEND     0x8086  // Vendor ID for Intel
#define E1000_DEV      0x100E  // Device ID for the e1000 Qemu, Bochs, and VirtualBox emmulated NICs
#define E1000_I217     0x153A  // Device ID for Intel I217
#define E1000_82577LM  0x10EA  // Device ID for Intel 82577LM


//DEFS from osdev.org

#define E1000_NUM_RX_DESC 32
#define E1000_NUM_TX_DESC 8

struct e1000_rx_desc {
        volatile uint64_t addr;
        volatile uint16_t length;
        volatile uint16_t checksum;
        volatile uint8_t status;
        volatile uint8_t errors;
        volatile uint16_t special;
} __attribute__((packed));

struct e1000_tx_desc {
        volatile uint64_t addr;
        volatile uint16_t length;
        volatile uint8_t cso;
        volatile uint8_t cmd;
        volatile uint8_t status;
        volatile uint8_t css;
        volatile uint16_t special;
} __attribute__((packed));

static uint8_t read8 (uint64_t p_address);
static uint16_t read16 (uint64_t p_address);
static uint32_t read32 (uint64_t p_address);
static uint64_t read64 (uint64_t p_address);
static void write8 (uint64_t p_address,uint8_t p_value);
static void write16 (uint64_t p_address,uint16_t p_value);
static void write32 (uint64_t p_address,uint32_t p_value);
static void write64 (uint64_t p_address,uint64_t p_value);

#endif
