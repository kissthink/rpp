### RPP - 国产开源编程语言


##### 为什么需要它？

理由一：目前DIY操作系统一般使用gcc+nasm，RPP支持编译运行，抽象能力强过C，可直接内联汇编，不需要面对难看的AT&T汇编，绝对是您DIY操作系统的利器（支持GRUB从U盘或硬盘引导）。

理由二：RPP以静态类型为主，没有GC，所以理论运行速度和C++一样，最终的效率肯定比动态类型的lua要高，并且不会引起GC停顿。（目前运行速度与luaJIT接近，比tcc略快）

下面是RPP性能测试数据：（双重循环求和，具体测试代码请参考rpp.pdf）

![github](https://github.com/roundsheep/ste/blob/master/pic/rpp_speed.png "github")

理由三：RPP与C/C++互相调用比lua还简单，而且RPP和C++共享进程内存空间，RPP变量和C++变量生命周期相同，不会出现像lua一样的这里变量已经GC了那边还在使用。

理由四：RPP既可以编译运行又可以解释运行，既可以用静态类型又可以用动态类型。语法层支持C++、JS、Lisp、python、asm混合编程，一种语言，五种语法。（目前兼容50%的C++语法，70%的C语法，编译器本身使用C++编写，其90%的源码都可以自我编译）

RPP的整体设计：

![github](https://github.com/roundsheep/ste/blob/master/pic/rpp.png "github")

##### RPP编码风格1：（这是标准C++语法，本段代码可用VC++、G++或RPP进行编译）

		static rbool inherit_proc(tsh& sh,tclass& tci,int level=0)
		{
			if(level++>c_rpp_deep)
				return false;
			if(tci.vfather.empty())
				return true;
			rbuf<tword> v;
			for(int i=0;i<tci.vfather.count();i++)
			{
				rstr cname=tci.vfather[i].vword.get(0).val;
				tclass* ptci=zfind::class_search(sh,cname);
				if(ptci==null)
				{
					ptci=zfind::classtl_search(sh,cname);
					if(ptci==null)
						return false;
				}
				if(!inherit_proc(sh,*ptci,level))
					return false;
				v+=ptci->vword;
			}
			v+=tci.vword;
			tci.vword=v;
			return true;
		}

##### RPP编码风格2：（类似python的无花括号风格）

		bool next_permutation<T>(rbuf<T>& v)  
		{  
			if v.count<=1  
				return false  
			next=v.count-1  
			for  
				temp=next  
				next--  
				if v[next]<v[temp]  
					mid=v.count-1  
					for !(v[next]<v[mid])  
						mid--  
					swap<T>(v[next],v[mid])  
					reverse<T>(v,temp)  
					return true  
				if next==0  
					reverse<T>(v,0)  
					return false  
		} 

##### RPP编码风格3：（类似《算法导论》的伪代码）

		define ← =  
		  
		void insertion_sort(rstr& a)  
		{  
			for j ← 1 to a.count-1  
				key ← a[j]  
				i ← j-1  
				while i>=0 && a.get(i)>key  
					a[i+1] ← a[i]  
					i ← i-1  
				a[i+1] ← key  
		}

##### RPP编码风格4：（Lisp风格，从v1.91开始S表达式用逗号分隔）

		void main()
		{
			int a
			int b
			[int,=,[a,1]]
			[int,=,[b,2]]
			[rf,print,[[int,+,[a,b]]]]
		}

理由五：RPP自由度极高，完全开源，您可以随意DIY运算符的优先级，增加新运算符，增加修改“内置类型”，甚至于int、double这些基础类型都是由用户自定义的，一切由您做主。

理由六：它是C++的山寨版，C++程序员几乎无需学习即可使用，而且RPP所有数据类型和C++二进制兼容，无论是静态链接还是动态链接，想怎样就怎样。

理由七：它以Lisp作为中间层，编译器在运行期可用，程序在运行时可以改变其结构，新的函数可以被引进，因此支持mixin、元编程以及各种动态特性，并且同时支持call_by_name、call_by_need和call_by_value。

理由八：RPP对C++的一些复杂语法进行了简化，单纯的编译速度比C++略快。但是RPP在解释运行的时候并不会一次编译整个程序，而是在函数需要运行的时候才进行即时编译（边运行边编译），所以综合编译速度比C++快一个数量级（仅与VC++和G++对比）。

理由九：RPP的设计目标是简洁、快速、稳定。它的源码结构比lua简单得多，但实现的功能不比lua少，因此RPP将是您实现编译器的绝佳参考。另外嵌入编译器只需要包含一个头文件即可（zmain.h），您再也不用担心各种奇怪的链接错误了。

理由十：RPP支持中文编程。

<br/>
RPP支持多种运行方式，方法如下：（各种运行方式支持的特性请参考rpp.pdf）

##### JIT：

1. cd到bin目录
2. 命令行敲入 rjit example\1.h


##### JIT打包发布（生成独立EXE）：

1. cd到bin目录
2. 命令行敲入 rpack example\1.h
3. 成功后会生成example\1.exe，可脱离RPP环境运行


##### 解释运行：

1. cd到bin目录
2. 命令行敲入 rpp example\1.h

下面是RPP编写的控制台贪食蛇：

![github](https://github.com/roundsheep/ste/blob/master/pic/rpp_snake.png "github")

##### 编译运行（生成EXE）：

1. cd到bin目录
2. 命令行敲入 build_run example\1.h


##### 编译不运行（生成EXE）：

1. cd到bin目录
2. 命令行敲入 build example\1.h


##### 执行语法检查（不生成任何文件）：

1. cd到bin目录
2. 命令行敲入 rcheck example\1.h


##### 裸奔（无操作系统运行）：

1. cd到bin\nasm\
2. 敲入 bin cell.h （成功后会生成 cell.bin）
3. 修改 menu.lst 将引导文件改为 cell.bin
4. 使用 bootice（或其它工具）制作 grub4dos 引导扇区
5. 将 cell.bin、menu.lst、grldr 三个文件拷贝至U盘或硬盘根目录
6. 推荐先使用虚拟机进行测试


##### cocos2dx win32 绑定运行：
（仅v1.83及以下版本支持，新版本正在重构cocos2dx绑定）

1. cd到bin目录
2. 命令行输入 rcc cocos\flappy.h（需要VS2012运行库和最新的opengl驱动）

![github](https://github.com/roundsheep/ste/blob/master/pic/flappy.png "github")

##### cocos2dx android 绑定运行：
（仅v1.83及以下版本支持，新版本正在重构cocos2dx绑定）

1. 安装cocos\rcc.apk

<br/>
##### 中文编程：

1. 命令行敲入 chcp 65001 （因为RPP使用utf8）
2. 然后在命令行标题栏上点击右键，选择"属性"->"字体"
3. 将字体修改为True Type字体"Lucida Console"
4. 点击确定将属性应用到当前窗口
5. 删除bin\rinf\optr.txt
6. 将bin\rinf\optr2.txt重命名为optr.txt
7. 反注释bin\rsrc\basic.h中的第一行 import "chs.h"
8. cd到bin目录
9. 命令行敲入 rpp example\40_1.h


##### 解释运行方式可以关联.h文件：（这样就不用每次敲命令行）

1. 右键点击bin\example下任意一个.h文件，选择“属性”
2. 点击“更改”
3. 点击“浏览”
4. 找到bin\rpp.exe并点击“打开”
5. 选中“始终使用选择的程序打开这种文件”
6. 点击两次“确定”
7. 之后可以直接双击后缀为.h的源文件运行RPP程序

##### 第三方IDE编辑代码：

1. 运行bin\ide\SciTE.exe
2. 点击File->Open
3. 选择bin\example\1.h，点击“打开”
4. 按F5运行程序（F7打包发布）

##### Windows窗体模式：（WinMain）

1. cd到bin目录
2. 命令行敲入 rjit_win example\win.h
3. 或者使用IDE打开win.h，按Ctrl+F5运行或者Ctrl+F7打包发布

![github](https://github.com/roundsheep/ste/blob/master/pic/rpp_win.png "github")

使用scite可以自动补全代码，但不如Visual Assist的智能补全。

Visual Assist智能补全请看视频演示：

http://www.tudou.com/programs/view/40Ez3FuqE10/

<br/>
##### 编译RPP源码：

1. 确保安装了 VS2012 update4 或者 VS2013
2. 打开proj\rpp.sln
3. 按F7，成功后会生成bin\rpp.exe


##### 低版本VC++编译RPP源码：（VS2005、VS2008、VS2010）

1. 修改RPP源码中使用了C++11 move语义的地方（或者回退到RPP v1.87及以下版本）
2. 新建控制台解决方案
3. 添加rpp\rpp.cpp
4. 设置项目属性为多字节字符集
5. 设置运行库为“多线程MT”（可选）
6. 按F7，成功后会生成bin\rpp.exe


##### 自动测试example下所有例子：
（如果您修改了编译器源码，请运行这个批处理）

1. 双击bin\check_all.bat

<br/>
RPP没有协议，任何人可以随意使用、复制、发布、修改、改名。


QQ交流群：34269848   

E-mail：287848066@qq.com