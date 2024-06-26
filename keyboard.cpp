#include "keyboard.h"

void printf(const char *);
void printfHex(const uint8_t);

KeyboardEventHandler::KeyboardEventHandler(){};

KeyboardDriver::KeyboardDriver(InterruptManager * manager, KeyboardEventHandler * handler) 
    : InterruptHandler(0x21,manager),dataPort(0x60),commandPort(0x64),handler(handler)
{

}

KeyboardDriver :: ~KeyboardDriver()
{

}

uint32_t KeyboardDriver::HandleInterrupt(uint32_t esp)
{
    uint8_t key = dataPort.Read();

    if(handler==0)
        return esp;

    static bool shift = false;

    char msg[2] = {'\0'};
    switch (key)
    {
        case 0x45:break;

        case 0x1E:if(shift)handler->OnKeyDown ('A');else handler->OnKeyDown ('a');break;
        case 0x30:if(shift)handler->OnKeyDown ('B');else handler->OnKeyDown ('b');break;
        case 0x2E:if(shift)handler->OnKeyDown ('C');else handler->OnKeyDown ('c');break;
        case 0x20:if(shift)handler->OnKeyDown ('D');else handler->OnKeyDown ('d');break;
        case 0x12:if(shift)handler->OnKeyDown ('E');else handler->OnKeyDown ('e');break;
        case 0x21:if(shift)handler->OnKeyDown ('F');else handler->OnKeyDown ('f');break;
        case 0x22:if(shift)handler->OnKeyDown ('G');else handler->OnKeyDown ('g');break;
        case 0x23:if(shift)handler->OnKeyDown ('H');else handler->OnKeyDown ('h');break;
        case 0x17:if(shift)handler->OnKeyDown ('I');else handler->OnKeyDown ('i');break;
        case 0x24:if(shift)handler->OnKeyDown ('J');else handler->OnKeyDown ('j');break;
        case 0x25:if(shift)handler->OnKeyDown ('K');else handler->OnKeyDown ('k');break;
        case 0x26:if(shift)handler->OnKeyDown ('L');else handler->OnKeyDown ('l');break;
        case 0x32:if(shift)handler->OnKeyDown ('M');else handler->OnKeyDown ('m');break;
        case 0x31:if(shift)handler->OnKeyDown ('N');else handler->OnKeyDown ('n');break;
        case 0x18:if(shift)handler->OnKeyDown ('0');else handler->OnKeyDown ('o');break;
        case 0x19:if(shift)handler->OnKeyDown ('P');else handler->OnKeyDown ('p');break;
        case 0x10:if(shift)handler->OnKeyDown ('Q');else handler->OnKeyDown ('g');break;
        case 0x13:if(shift)handler->OnKeyDown ('R');else handler->OnKeyDown ('r');break;
        case 0x1F:if(shift)handler->OnKeyDown ('S');else handler->OnKeyDown ('s');break;
        case 0x14:if(shift)handler->OnKeyDown ('T');else handler->OnKeyDown ('t');break;
        case 0x16:if(shift)handler->OnKeyDown ('U');else handler->OnKeyDown ('u');break;
        case 0x2F:if(shift)handler->OnKeyDown ('V');else handler->OnKeyDown ('v');break;
        case 0x11:if(shift)handler->OnKeyDown ('W');else handler->OnKeyDown ('w');break;
        case 0x2D:if(shift)handler->OnKeyDown ('X');else handler->OnKeyDown ('x');break;
        case 0x15:if(shift)handler->OnKeyDown ('Y');else handler->OnKeyDown ('y');break;
        case 0x2C:if(shift)handler->OnKeyDown ('Z');else handler->OnKeyDown ('z');break;




        case 0x2A: case 0x36:
            shift = true;
            break;
        case 0xAA: case 0xB6:
            shift = false;
            break;
        default:
            if (key < 0x80)
            {
                printf(" keyboard0x");
                printfHex(key);
            }
            break;
    }
    return esp;
}

void KeyboardDriver::Activate()
{
    while(commandPort.Read() & 0x1)
        dataPort.Read();
    commandPort.Write(0xAE);
    commandPort.Write(0x20);
    uint8_t status = (dataPort.Read() | 0x01) & ~0x10;
    commandPort.Write(0x60);
    dataPort.Write(status);

    dataPort.Write(0xF4);
}