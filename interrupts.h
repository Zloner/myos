#ifndef __INTERRUPTS_H__
#define __INTERRUPTS_H__

#include "types.h"
#include "port.h"
#include "gdt.h"

class InterruptManager;

class InterruptHandler
{
protected:
    uint8_t interruptNumber;
    InterruptManager * interruptManager;

public:
    InterruptHandler(uint8_t interruptNumber, InterruptManager *interruptManager);
    ~InterruptHandler();

    virtual uint32_t HandleInterrupt(uint32_t esp);
};

class InterruptManager
{
    friend class InterruptHandler;
protected:
    InterruptHandler *handlers[256];
    static InterruptManager *ActiveInterruptManger;

    struct GateDescriptor
    {
        uint16_t handlerAddressLowBits;
        uint16_t gdt_codeSegmentSelector;
        uint8_t reserved;
        uint8_t access;
        uint16_t handlerAddressHighBits;
    } __attribute__((packed));

    static GateDescriptor interruptDescriptorTable[256];

    struct  InterruptDescriptorTablePointer
    {
        uint16_t size;
        uint32_t base;
    } __attribute__((packed));

    static void SetInterruptDescriptorTableEntry(
        uint8_t interruptNumber,
        uint16_t codeSegmentSelector0ffSet,
        void (*handler)(),
        uint8_t DescriptorPrivilegeLevel,
        uint8_t DescriptorType
    );


    Port8BitSlow picMasterCommand;
    Port8BitSlow picMasterData;
    Port8BitSlow picSlaveCommand;
    Port8BitSlow picSlaveData;

public:
    InterruptManager(GlobalDescriptorTable *gdt);
    ~InterruptManager();

    void Activate();
    void Deactivate();

    static uint32_t handleInterrupt(uint8_t interruptNumber, uint32_t esp);
    uint32_t DoHandleInterrupt(uint8_t InterruptNumber, uint32_t esp);

    static void IgnoreInterruptRequest();

    static void HandleInterruptResquest0x00();
    static void HandleInterruptResquest0x01();
    static void HandleInterruptResquest0x0C();
};

#endif