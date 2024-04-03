#include "interrupts.h"

void printf(const int8_t *);
void printfHex(const uint8_t);

InterruptHandler::InterruptHandler(uint8_t interruptNumber, InterruptManager *interruptManager)
    : interruptNumber(interruptNumber), interruptManager(interruptManager)
{
    interruptManager->handlers[interruptNumber] = this;
}
InterruptHandler:: ~InterruptHandler()
{
    if(interruptManager->handlers[interruptNumber] = this)
        interruptManager->handlers[interruptNumber] = 0;
}

uint32_t InterruptHandler::HandleInterrupt(uint32_t esp)
{
    return esp;
}

// 定义中断门描述符数组
InterruptManager::GateDescriptor InterruptManager::interruptDescriptorTable[256];

InterruptManager * InterruptManager::ActiveInterruptManger = 0;

// InterruptManager 构造函数
InterruptManager::InterruptManager(GlobalDescriptorTable *gdt)  
    : picMasterCommand(0x20), picMasterData(0x21),picSlaveCommand(0xA0),picSlaveData(0xA1)
{
    // 获取代码段选择子
    uint16_t CodeSegment = gdt->CodeSegmentSelector();

    // 定义中断门类型
    const uint8_t IDT_INTTERRUPT_GATE = 0xE;

    // 初始化中断描述符表中的每个条目
    for (uint16_t i = 0; i < 256;++i)
    {
        handlers[i] = 0;
        SetInterruptDescriptorTableEntry(i, CodeSegment, &IgnoreInterruptRequest, 0, IDT_INTTERRUPT_GATE);
    }

    // 为中断0x20设置一个中断描述符，以及特定的处理程序
    SetInterruptDescriptorTableEntry(0x20, CodeSegment, &HandleInterruptResquest0x00, 0, IDT_INTTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x21, CodeSegment, &HandleInterruptResquest0x01, 0, IDT_INTTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x2C, CodeSegment, &HandleInterruptResquest0x0C, 0, IDT_INTTERRUPT_GATE);

    // 初始化可编程中断控制器 (PIC)
    picMasterCommand.Write(0x11);
    picMasterCommand.Write(0x11);

    picMasterData.Write(0x20);
    picSlaveData.Write(0x28);

    picMasterData.Write(0x04);
    picSlaveData.Write(0x02);

    picMasterData.Write(0x01);
    picSlaveData.Write(0x01);

    picMasterData.Write(0x00);
    picSlaveData.Write(0x00);

    InterruptDescriptorTablePointer idt;
    idt.size = 256 * sizeof(GateDescriptor) - 1;
    idt.base = (uint32_t)interruptDescriptorTable;
    __asm__ volatile("lidt %0": : "m" (idt));
}

// InterruptManager 析构函数
InterruptManager::~InterruptManager()
{

}

// 使用汇编启用中断
void InterruptManager::Activate()
{
    if(ActiveInterruptManger != nullptr)
        ActiveInterruptManger->Deactivate();
    ActiveInterruptManger = this;
    __asm__ volatile("sti");
}

// 使用汇编禁用中断
void InterruptManager::Deactivate()
{
    if(ActiveInterruptManger != this)
    {
        ActiveInterruptManger = 0;
        __asm__ volatile("cli");
    }
}

// 设置中断描述符表中的条目
void InterruptManager::SetInterruptDescriptorTableEntry(
        uint8_t interruptNumber,
        uint16_t codeSegmentSelector0ffSet,
        void (*handler)(),
        uint8_t DescriptorPrivilegeLevel,
        uint8_t DescriptorType)
{
    const uint8_t IDT_DESC_PRESENT = 0x80;
    // 填充中断描述符的详细信息
    interruptDescriptorTable[interruptNumber].handlerAddressLowBits = ((uint32_t)handler) & 0xFFFF;
    interruptDescriptorTable[interruptNumber].handlerAddressHighBits = (((uint32_t)handler) >> 16) & 0xFFFF;
    interruptDescriptorTable[interruptNumber].gdt_codeSegmentSelector = codeSegmentSelector0ffSet;
    interruptDescriptorTable[interruptNumber].access = IDT_DESC_PRESENT | DescriptorType | ((DescriptorPrivilegeLevel & 3) << 5);
    interruptDescriptorTable[interruptNumber].reserved = 0;
}

// 用于处理中断的处理程序
uint32_t InterruptManager::handleInterrupt(uint8_t interruptNumber, uint32_t esp)
{

    if(ActiveInterruptManger!=0)
        return ActiveInterruptManger->DoHandleInterrupt(interruptNumber, esp);
    return esp;

}

uint32_t InterruptManager::DoHandleInterrupt(uint8_t InterruptNumber, uint32_t esp)
{
    if(handlers[InterruptNumber] !=nullptr)
        esp = handlers[InterruptNumber]->HandleInterrupt(esp);
    else if(InterruptNumber != 0x20)
    {
        char msg[] = " unhandled interrupt 0x";
        

        printfHex(InterruptNumber);

        // Port8Bit dataPort(0x60);
        // dataPort.Read();
    }

    if(0x20 <= InterruptNumber && InterruptNumber < 0x30)
    {
        picMasterCommand.Write(0x20);
        if(0x28 <= InterruptNumber)
        {
            picSlaveCommand.Write(0x20);
        }
    }

    return esp;
}