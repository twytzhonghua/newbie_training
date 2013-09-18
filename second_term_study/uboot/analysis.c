arm920t 启动流程：

	1.设为svc模式
	2.关看门狗
	3.屏蔽中断
	4.初始化SDRAM
	5.设置栈
	6.时钟
	7.重定位，flash代码拷贝到SDRAM
	8.清BSS段
	9.调用start_arm_boot

	
