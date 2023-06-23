#include "stm32f10x.h"
#include "MySPI.h"
#include "W25Q64_Ins.h"

#define W25Q64_CS_PIN GPIO_Pin_4

/*
FLASH有几个特性：
写使能：在进行写入操作之前，需要先发送写使能指令（W25Q64_WRITE_ENABLE），以允许写入操作。写使能操作应在每次写入之前进行。
页面编程：W25Q64采用页面编程的方式进行数据写入，每个页面的大小为256字节。如果要写入超过一个页面的数据，需要确保数据不跨页面写入。如果数据跨越页面边界，需要分为多次写入。
忙状态检测：在写入操作完成后，需要检测W25Q64的忙状态以确保操作完成。可以通过读取状态寄存器的忙位（BUSY）来判断是否处于忙碌状态。如果忙位为1，表示W25Q64正在执行操作，需要等待操作完成后再进行下一步操作。
FLASH只能1写0，不能0写1，如果需要将0改为1，可以先擦除再写入，这样擦除的数据位ff，也就是全为1，此时写入，1还是1，并且1可以写0，所以在写入数据的时候也需要注意这点，最好在写入前擦除，避免数据写入错误
*/
void W25Q64_Init(void)
{
    MySPI_Init(W25Q64_CS_PIN); // 输入片选线引脚，进行初始化
}

void W25Q64_GetID(uint8_t *Manufacturer, uint16_t *ID)
{
    MySPI_Start(W25Q64_CS_PIN);
    MySPI_SwapByte(W25Q64_JEDEC_ID);
    *Manufacturer = MySPI_SwapByte(W25Q64_DUMMY_BYTE);
    //*ID = MySPI_SwapByte(W25Q64_DUMMY_BYTE) << 8 | MySPI_SwapByte(W25Q64_DUMMY_BYTE);//有问题，输出1740，应该为4017，原因不明

    *ID = MySPI_SwapByte(W25Q64_DUMMY_BYTE) << 8;
    *ID |= MySPI_SwapByte(W25Q64_DUMMY_BYTE);

    MySPI_Stop(W25Q64_CS_PIN);
}

// 发送一次写使能指令，在某些操作后会将写失能，所以在写入数据之前需要写使能，
void W25Q64_WriteEnable(void)
{
    MySPI_Start(W25Q64_CS_PIN);
    MySPI_SwapByte(W25Q64_WRITE_ENABLE);
    MySPI_Stop(W25Q64_CS_PIN);
}

// 等待w25q忙碌结束
// busy位在寄存器1的第0位，1表示忙碌，0表示空闲
// 读取状态寄存器的指令位0x05，该指令规定，如果不结束通信，则一致输出状态寄存器
// 所以可以利用这个机制来实现等待忙碌的函数
void W25Q64_WaitBusy(void)
{
    MySPI_Start(W25Q64_CS_PIN);
    MySPI_SwapByte(W25Q64_READ_STATUS_REGISTER_1); // 发送读状态寄存器1的指令

    uint8_t Timeout = 10000;
    while ((MySPI_SwapByte(W25Q64_DUMMY_BYTE) & 0x01))
        ; // 取出状态寄存器的最低位
    // {
    //     Timeout--;
    //     if (Timeout == 0) // 超时退出机制，注意！！！！！！！！！：timeout时长过短会导致后续操作失败，必须严格确定，否则就死循环等待
    //     {
    //         break;
    //     }
    // };
    MySPI_Stop(W25Q64_CS_PIN);
}

// 页编程指令，也就是写入数据，W25Q64的写入不能跨页写入，如果超过了也边缘，则超出的部分会从这一页的起始位置覆盖，如果有跨页写入的需求，只能自己在代码中手动分批写入
// 需要传入24位的地址，和若干个8位的数据，24位的地址分三次传送，之后发送要写入的数据即可，如果是多个写入的8位数据，则地址会自动+1，依次写入
// 需要注意W25Q64的缓存只有256个字节的缓存，如果写入了超过256个，则超出的部分会自动从第一个覆盖，所以最好不要一次性写入256字节以上的数据
// 此外需要注意传入的参数Count是uint16_t的类型，因为个数不能是0，故八位的而二进制去掉0，只能表示255个，如果需要256个就必须增加二进制位数，即uint16_t
void W25Q64_PageProgram(uint32_t Address, uint8_t *Data, uint16_t Count)
{
    // 写入数据前必须写使能
    W25Q64_WriteEnable();
    MySPI_Start(W25Q64_CS_PIN);
    MySPI_SwapByte(W25Q64_PAGE_PROGRAM);
    MySPI_SwapByte(Address >> 16); // 正数右移高位补0，这里就相当于取出了高8位
    MySPI_SwapByte(Address >> 8);
    MySPI_SwapByte(Address); // 这里需要八位的数据，但是传入了32位的数据，则函数会舍弃高位，留下最低的八位
    for (uint16_t i = 0; i < Count; i++)
    {
        MySPI_SwapByte(Data[i]); // 循环写入数据
    }
    MySPI_Stop(W25Q64_CS_PIN);

    // 事后等待，比起事前等待，好处就是当执行完了该函数后，保证芯片肯定是空闲状态，缺点是阻塞等待，期间不能做其他事情
    // 事前等待的好处就是，写入操作结束后进入的忙状态，可以和其他操作一起执行，这样当再次进入写入操作时，有可能就是空闲状态了，效率利用高
    // 此外，如果是事前等待，必须在读取操作前也加上等待，因为忙碌状态不会响应读取操作，而事后等待能就能避免这个问题
    W25Q64_WaitBusy();
}

// 读取数据，传入指定地址，从这个地址开始连续读Count个字节，读取数据不同于写入缓存，只需要读取电平即可，所以没有个数限制
uint8_t *W25Q64_ReadData(uint32_t Address, uint32_t Count, uint8_t *DataArr)
{
    MySPI_Start(W25Q64_CS_PIN);
    MySPI_SwapByte(W25Q64_READ_DATA); // W25Q64的扇区是4KB
    MySPI_SwapByte(Address >> 16);
    MySPI_SwapByte(Address >> 8);
    MySPI_SwapByte(Address);
    for (uint32_t i = 0; i < Count; i++)
    {
        DataArr[i] = MySPI_SwapByte(W25Q64_DUMMY_BYTE); // 接收数据
    }
    MySPI_Stop(W25Q64_CS_PIN);
    return DataArr;
}

// 擦除扇区，传入指定地址，W25Q64就是擦除该地址所在的扇区中的所有内容
void W25Q64_SectorErase(uint32_t Address)
{
    // 擦除数据前必须写使能
    W25Q64_WriteEnable();
    MySPI_Start(W25Q64_CS_PIN);
    MySPI_SwapByte(W25Q64_SECTOR_ERASE_4KB); // W25Q64的扇区是4KB
    MySPI_SwapByte(Address >> 16);
    MySPI_SwapByte(Address >> 8);
    MySPI_SwapByte(Address);
    MySPI_Stop(W25Q64_CS_PIN);
    W25Q64_WaitBusy();
}
