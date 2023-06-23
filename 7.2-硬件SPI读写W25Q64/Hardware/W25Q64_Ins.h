#ifndef __W25Q64_H
#define __W25Q64_H

//该文件存放W25Q64的指令码的宏定义
#define W25Q64_WRITE_ENABLE                         0x06    // 写使能
#define W25Q64_WRITE_DISABLE                        0x04    // 写禁止
#define W25Q64_READ_STATUS_REGISTER_1               0x05    // 读状态寄存器 1
#define W25Q64_READ_STATUS_REGISTER_2               0x35    // 读状态寄存器 2
#define W25Q64_WRITE_STATUS_REGISTER                0x01    // 写状态寄存器
#define W25Q64_PAGE_PROGRAM                         0x02    // 页面编程
#define W25Q64_QUAD_PAGE_PROGRAM                    0x32    // 四线程页面编程
#define W25Q64_BLOCK_ERASE_64KB                     0xD8    // 块擦除（64KB）
#define W25Q64_BLOCK_ERASE_32KB                     0x52    // 块擦除（32KB）
#define W25Q64_SECTOR_ERASE_4KB                     0x20    // 扇区擦除（4KB）
#define W25Q64_CHIP_ERASE                           0xC7    // 芯片擦除
#define W25Q64_ERASE_SUSPEND                        0x75    // 擦除暂停
#define W25Q64_ERASE_RESUME                         0x7A    // 擦除恢复
#define W25Q64_POWER_DOWN                           0xB9    // 掉电模式
#define W25Q64_HIGH_PERFORMANCE_MODE                0xA3    // 高性能模式
#define W25Q64_CONTINUOUS_READ_MODE_RESET           0xFF    // 连续读模式复位
#define W25Q64_RELEASE_POWER_DOWN_HPM_DEVICE_ID     0xAB    // 释放掉电/HPM 设备 ID
#define W25Q64_MANUFACTURER_DEVICE_ID               0x90    // 制造商和设备 ID
#define W25Q64_READ_UNIQUE_ID                       0x4B    // 读唯一 ID
#define W25Q64_JEDEC_ID                             0x9F    // JEDEC ID
#define W25Q64_READ_DATA                            0x03    // 读数据
#define W25Q64_FAST_READ                            0x0B    // 快速读取
#define W25Q64_FAST_READ_DUAL_OUTPUT                0x3B    // 快速双输出读取
#define W25Q64_FAST_READ_DUAL_IO                    0xBB    // 快速双线输入/输出读取
#define W25Q64_FAST_READ_QUAD_OUTPUT                0x6B    // 快速四输出读取
#define W25Q64_FAST_READ_QUAD_IO                    0xEB    // 快速四线输入/输出读取
#define W25Q64_OCTAL_WORD_READ_QUAD_IO              0xE3    // 八线输入/输出四线读取

#define W25Q64_DUMMY_BYTE                           0xFF    // 无用数据

#endif
