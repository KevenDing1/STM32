#STM32F4 Studying Notes

####FSMC
FSMC(Flexible Static Memory Controller，可变静态存储控制器)是STM32系列采用一种新型的存储器扩展技术。在外部存储器扩展方面具有独特的优势，可根据系统的应用需要，方便地进行不同类型大容量静态存储器的扩展。  
是STM32系列中内部集成256 KB以上FlaSh，后缀为xC、xD和xE的高存储密度微控制器特有的存储控制机制。之所以称为“可变”，是由于通过对特殊功能寄存器的设置，FSMC能够根据不同的外部存储器类型，发出相应的数据/地址/控制信号类型以匹配信号的速度，从而使得STM32系列微控制器不仅能够应用各种不同类型、不同速度的外部静态存储器，而且能够在不增加外部器件的情况下同时扩展多种不同类型的静态存储器，满足系统设计对存储容量、产品体积以及成本的综合要求。

####常用置位操作
```C
x |= (1<<y);	//x的第y位置1(y可以等于0)
x &= ~(1<<y);	//x的第y位清0

x |= (3<<y);	//x的第y+1和第y位置11
```


####硬件初始化
启动代码： 启动代码是一段和硬件相关的汇编代码。 是必不可少的！ 这代码主要作用如
下： 1、堆栈（ SP）的初始化； 2、初始化程序计数器（ PC）； 3
、设置向量表异常事件的入口地
址； 4、调用 main 函数。  
ST 公司为 STM32F40x 和 STM32F41x 系列的 STM32F4 提供了一个共同的启动文件，名字
为： startup_stm32f40_41xxx.s。

####AHB总线
由ARM公司推出的AMBA片上总线受到了广大IP开发商和SoC系统集成者的青睐，已成为一种流行的工业标准片上结构。AMBA规范主要包括了AHB(Advanced High performance Bus)系统总线和APB(Advanced Peripheral Bus)外围总线。  
- Advanced High-performance Bus (AHB)
AHB总线用于高性能，高时钟工作频率模块。AHB在AMBA架构中为系统的高性能运行起到了基石作用。AHB为高性能处理器，片上内存，片外内存提供接口，同时桥接慢速外设。
- Advanced System Bus (ASB)
ASB总线主要用于高性能系统模块。ASB是可用于AHB不需要的高性能特性的芯片设计上可选的系统总线。ASB也支持高性能处理器，片上内存，片外内存提供接口和慢速外设。
- Advanced Peripheral Bus (APB)
APB总线用于为慢速外设提供总线技术支持。
APB是一种优化的，低功耗的，精简接口总线，可以技术多种不同慢速外设。由于APB是ARM公司最早提出的总线接口，APB可以桥接ARM体系下每一种系统总线。  

####SysTick
```C 
typedef struct
{
  __IO uint32_t CTRL;                    /*!< Offset: 0x000 (R/W)  SysTick Control and Status Register */
  __IO uint32_t LOAD;                    /*!< Offset: 0x004 (R/W)  SysTick Reload Value Register       */
  __IO uint32_t VAL;                     /*!< Offset: 0x008 (R/W)  SysTick Current Value Register      */
  __I  uint32_t CALIB;                   /*!< Offset: 0x00C (R/ )  SysTick Calibration Register        */
} SysTick_Type;

```
SysTick是MDK定义的一个结构体, 里边包含了CTRL, LOAD, VAL, CALIB四个寄存器.  
在`delay.c`中:
```C
SysTick->CTRL&=~(1<<2);					//SYSTICK使用外部时钟源	
	fac_us=SYSCLK/8;						//不论是否使用OS,fac_us都需要使用
```
原语句相当于`SysTick->CTRL&=0xfffffffb`也就相当于把`CTRL`寄存器第3位置0, 根据`CTRL`寄存器各位定义, 相当于选择了`HCLK/8`(也就是CPU时钟频率的1/8)为SysTick频率.  
```C
static u8  fac_us=0;							//us延时倍乘数			   
static u16 fac_ms=0;							//ms延时倍乘数,在os下,代表每个节拍的ms数
```
fac_us，为 us 延时的基数，也就是延时 1us， SysTick->LOAD 所应
设置的值。   
 Systick 的时钟来自系统时钟 8 分频， 正因为如此，
系统时钟如果不是 8 的倍数(不能被 8 整除)，则会导致延时函数不准确.


####存储器系统
CM3 的存储器系统支持所谓的“位带”（ bit‐band）操作。通过它，实现了对单
一比特的原子操作。  
支持了位带操作后，可以使用普通的加载/存储指令来对单一的比特进行读写。

在位带区中，每个比特都映射到别名地址区的一个字——这是只有 LSB 有效的字。当一个别名地址被访问时，会先把该地址变换成位带地址。对于读操作，读取位带地址中的一个字，再把需要的位右移到 LSB，并把 LSB 返回。对于写操作，把需要写的位左移至对应的位序号处，然后执行一个原子的“读－改－写”过程。  
注意:当使用位带功能时, 要访问的变量必须用`volatile`来定义. 因为C编译器不知道同一个比特可以有两个地址. 

####sys.h
`FTIR`下降沿触发方式(Falling edge Trigger).  
`RTIR`上升沿触发方式(Raising edge Trigger).

推挽输出和开漏输出
推挽输出:可以输出高,低电平,连接数字器件;  

开漏输出:输出端相当于三极管的集电极. 要得到高电平状态需要上拉电阻才行. 适合于做电流型的驱动,其吸收电流的能力相对强(一般20ma以内).  
推挽结构一般是指两个三极管分别受两互补信号的控制,总是在一个三极管导通的时候另一个截止.
场效应管是电压控制型元器件，只要对栅极施加电压，DS就会导通。
结型场效应管有一个特性就是它的输入阻抗非常大，这意味着：没有电流从控制电路流出，也没有电流进入控制电路。没有电流流入或流出，就不会烧坏控制电路。而双极型晶体管不同，是电流控制性元器件，如果使用开集电路，可能会烧坏控制电路。这大概就是我们总是听到开漏电路而很少听到开集电路的原因吧？因为开集电路被淘汰了。  
NVIC: 嵌套向量中断控制器(Nested Vectored Interrupt Controller)。STM32的中有一个强大而方便的NVIC，它是属于Cortex内核的器件，不可屏蔽中断 (NMI)和外部中断都由它来处理，而SYSTICK不是由 NVIC来控制的。   
STM32中有两个优先级的概念——抢占式优先级和响应优先级，有人把响应优先级称作'亚优先级'或'副优先级'，每个中断源都需要被指定这两种优先级。

Thumb指令可以看做是ARM指令压缩形式的子集，是针对代码密度的问题而提出的，它具有16为的代码密度。Thumb不是一个完整的体系结构，不能指望处理程序只执行Thumb指令而不支持ARM指令集。因此，Thumb指令只需要支持通用功能，必要时，可借助完善的ARM指令集，例如：所有异常自动进入ARM状态。



####锁相环
PLL(Phase Locked Loop)作用:数字芯片有个时钟树的概念，现在比如就是一根导线代替锁相环，芯片外面在时钟的上升沿开始给芯片送入一组数据，芯片内部由于有时钟树的存在，导致了内部时序电路实际使用的时钟是延迟过的，进而产生一个数据漂移的现象。但是有锁相环了，我们可以把时钟树的其中一个分支接入锁相环，使时钟树末梢的相位频率与参考信号保持一致，就不会有数据漂移的现象了。  
以上是锁相环最简单的使用，锁相环还有倍频作用，因为输出的时钟是它自己内部的压控振荡器产生的，若加一个分频器，再与输入参考时钟相比较，就可得到一个频率加N倍的时钟信号，当然相位还是和参考时钟是同步的。


####时钟控制
STM32F4 的时钟设计的比较复杂，各个时钟基本都是可控的，任何外设都有对应的时钟控制开关，这样的设计，对降低功耗是非常有用的，不用的外设不开启时钟，就可以大大降低其功耗。
时钟设置函数:`u8 Sys_Clock_Set(u32 plln,u32 pllm,u32 pllp,u32 pllq)`   
- pllm: 主PLL和音频PLL分频系数(PLL之前的分频)
- plln: 主PLL分频系数(PLL倍频)
- pllp: 系统时钟的主PLL分频系数(PLL之后的分频)
- pllq: USB/SDIO/随机数产生器等的主PLL分频系数(PLL之后的分频)
VCO即压控振荡器，是射频电路的重要组成部分。 射频电路多采用调制解调方式，因此严重依赖本振。


####内联汇编
THUMB指令不支持汇编内联,采用如下方法实现执行汇编指令WFI .  
```C++
//(sys.c中)
void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}
//开启所有中断
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}

//__ASM宏定义(core_cm4.h中)
#if   defined ( __CC_ARM )
#define __ASM            __asm                                      /*!< asm keyword for ARM Compiler          */  
//...

#elif defined ( __GNUC__ )
  #define __ASM            __asm                                      /*!< asm keyword for GNU Compiler          */   
//...
```
在执行完 WFI 指令之后， STM32F4 就进入待机模式了，系统将停止工作

####IO设置函数
STM32F4 每组通用 I/O 端口包括 4 个 32 位配置寄存器（ MODER、 OTYPER、 OSPEEDR和 PUPDR）、 2 个 32 位数据寄存器（ IDR 和 ODR）、 1 个 32 位置位/复位寄存器 (BSRR)、1 个 32 位锁定寄存器 (LCKR) 和 2 个 32 位复用功能选择寄存器（ AFRH 和 AFRL）等。  

- `MODER`寄存器: GPIO端口模式控制寄存器. 每组IO下有16个IO口, 该寄存器有32位, 每2个位控制1个IO.   
`00`:输入(复位状态); `01`:通用输出模式;`10`:复用功能模式. `11`:模拟模式  
- `OTYPER`寄存器: 控制GPIO的输出类型.该寄存器仅用于输出模式, 在输入模式(MODER[1:0]=00/11时)不起作用. 该寄存器仅低16位有效, 每位控制1个IO口,复位后,各位均为0.   
`0`: 输出推挽; `1`: 输出开漏. 
- `OSPEEDR`寄存器: 用于控制GPIO的输出速度. 仅用于输出模式. 每两个位控制1个IO口.   
`00`: 2MHz; `01`: 25MHz; `10`: 50MHz; `11`: 100MHz或80MHz
- 'PUPDR'寄存器: 用于控制GPIO的上下拉. 每2位控制一个IO口. STM32F1通过`ODR`寄存器控制上下拉的, 而STM32F4则由单独的寄存器PUPDR控制上下拉,使用更灵活.   
`00`: 无上下拉; `01`: 上拉; `10`: 下拉; `11`: 保留.
- `ODR`寄存器: 控制GPIO的输出. 用于设置某个IO输出低电平还是高电平.也仅在输出模式下有效.每1位控制1个IO口, 1位输出高, 0为输出低. 
- `IDR`寄存器: 读取GPIO的输入. 用于读取某个IO的电平, 高16位保留, 低16位每个位对应一个IO,1表示该IO输入的是高电平, 0表示输入的是低电平.   

例如: 设置PortC的第12个IO(PC11)为推挽输出,速度为100MHz, 不带上下拉, 并输出高电平. 则寄存器配置代码如下:
```C
RCC->AHB1ENR|=1<<2; //使能 PORTC 时钟
GPIOC->MODER&=~(3<<(11*2)); //先清除 PC11 原来的设置
GPIOC->MODER|=1<<(11*2); //设置 PC11 为输出模式
GPIOC->OTYPER&=~(1<<11) ; //清除 PC11 原来的设置
GPIOC->OTYPER|=0<<11; //设置 PC11 为推挽输出
GPIOC-> OSPEEDR&=~(3<<(11*2)); //先清除 PC11 原来的设置
GPIOC-> OSPEEDR|=3<<(11*2); //设置 PC11 输出速度为 100Mhz
GPIOC-> PUPDR&=~(3<<(11*2)); //先清除 PC11 原来的设置
GPIOC-> PUPDR|=0<<(11*2); //设置 PC11 不带上下拉
GPIOC->ODR|=1<<11; //设置 PC11 输出 1（高电平）

```
所以要使用某个IO口,首先确定是哪个Port的第几个脚. 然后使能`AHB1ENR`寄存器的对应位. 然后配置GPIO对应Port的寄存器来控制某一位

除此之外, 还有两个寄存器`AFRL`和`AFRH`, 它们是用来设置IO引脚复用和映射的. STM32F4每个IO引脚通过一个复用器连接到板载外设/模块,该复用器一次仅允许一个外设的复用功能(Alternat Function)连接到IO引脚.复用功能选择,就是通过AFRL和AFRH来控制的. AFRL控制0~7IO, AFRH控制8~15IO.

AFRL和AFRH被定义为`AFR[2]`. `AFR[0]`代表AFRL, `AFR[1]`代表AFRH.  
U3_RX(串口3的接收引脚), 在`PB11/PC11/PD9`上面都有这个复用功能, 这就要通过对应IO的复用功能选择器来选择,可以选择3个脚里任意一个作为U3_RX,只需要设置对应引脚所在GPIO的AFRL/AFRH即可,而且,没有选择作为 U3_RX 复用的另外两个 IO 口，还是可以用来作为普通 IO 输出或者其他复用功能输出的。


####中断管理
CM4内核支持256个中断, 其中包含16个内核中断和240个外部中断. 并且具有 256级的可编程中断设置.  
STM32F40XX/STM32F41XX只是用了CM4内核的一部分,共有92个中断.包括10个内核中断和82个可屏蔽中断. 具有16级可编程的中断优先级.

NVIC: Nested Vectored Interrupt Controller   
嵌套向量中断控制器  
ISER[8] 全称是： Interrupt Set-Enable Registers，这是一个中断使能寄存器组  
`__IO uint32_t ISER[8];		 /*!< Interrupt Set Enable Register */`    
CM4内核支持256个中断,这里用8个32位寄存器来控制. 每个位控制一个中断. 但是STM32F40XX可屏蔽中断只有82个,所以有用的就是`ISER[0~2]`, 共可以表示96个中断.但只用了82个. 0~81分别对应ISER[0]0~31位,ISER[1]0~31,ISER[2]0~17.要使能某个中断,必须设置相应的ISER位为1.   
ICER 全称: Interrupt Clear-Enable Registers. 用来清除某个中断的使能的. 写1有效.    
ISPR 全称: Interrupt Set-Pending Registers 中断挂起控制寄存器组. 通过置1,可以将正在进行的中断挂起, 而去执行同级或者更高级的中断.   
ICPR: 中断解挂.置1有效  
IABR:中断激活标志位寄存器组(Interrupt Active Bit Register). 只读寄存器, 通过它可以知道当前在执行的中断是哪一个(置1). 中断执行完后由硬件清0.  
前边的寄存器的对应位和ISER是一样的.
IP[240](Interrupt Priority Registers)是一个中断优先级控制的寄存器组. STM32中断分组与这个寄存器组密切相关.  
IP寄存器组由240个8bit寄存器组成.每个可屏蔽中断占用8bit. 这样可以总共表示240个可屏蔽中断.而STM32F40XX只用了其中的82个,IP[81]~IP[0]分别对应中断81~0. 而每个可屏蔽中断占用的8bit并未全部使用. 而是只用了高4位.

STM32F4中断分组: 中断共分5组,组0~4. 分组设置由`SCB->AIRCR`寄存器的`bit10~8`定义.  
SCB: System Control Block register  
AIRCR: Application Interrupt and Reset Control Register


组 	| AIRCR[10:8] 	| bit[7:4]分配情况 | 分配结果 						   
-------|---------------|------------------|-----------------------------------
0		| 111		 	| 	0:4 		   | 0 位抢占优先级， 4 位响应优先级   
1	 	| 110			|   1:3 		   | 1 位抢占优先级， 3 位响应优先级   
2	 	| 101	 		|   2:2   		   | 2 位抢占优先级， 2 位响应优先级   
3	 	| 100			|   3:1 		   | 3 位抢占优先级， 1 位响应优先级   
4	 	| 011			|   4:0			   | 4 位抢占优先级， 0 位响应优先级   








  


