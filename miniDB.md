## miniDB笔记

[toc]

> 本文为阅读[source](https://blog.japinli.top/db_tutorial_trans/)所写的笔记。这里是对应的[仓库](https://github.com/marco-hmc/miniDB_tutorial)。
> 原文质量极高，可惜对应的中文资料中缺乏相关资料，因此翻译成中文。
> 除此，还会提一下复现原文出现哪些坑以及如何避免，并把代码分类好，降低后来者学习难度便是本文意义。
> 估计阅读耗时：10h+

### 1. Getting started

原文作者提出了以下疑问，我在此根据我的理解做了解答。

- 数据在磁盘和内存上的存储格式是怎样的？
  - 
- 何时将数据从内存写入磁盘？
  - 
- 为什么每个表只能有一个主键？
  - 
- 事务回滚是如何进行的？
  - 
- 数据库索引的格式是什么样的？
  - 
- 数据库何时会进行全表扫描？全表扫描是如何工作的？
  - 
- PREPARED 语句是以什么格式保存的？
  - 

### 2. 数据库结构

![image-20220127143330995](https://s2.loli.net/2022/01/27/zd3muTCeROHKhb8.png)

![image-20220127142712258](https://s2.loli.net/2022/01/27/75HuoZUcPtBeGY2.png)

> 左侧为数据库结构，右侧为编译器结构。
> 两者设计思路其实是类似的，即如何解析程序员写的语句。
> 特别是java、python等实际上都是有文中的virtual machine结构。
> 最不同的地方近在于数据库存在B-Tree/Pager等针对存储功能特殊设计的模块。



### 3. 数据库搭建实践

#### 3.1 REPL环境实现

* **Task**

  实现最简单的操作界面(REPL, read-eval-print-loop)，即读取指令-对指令做出反映-打印结果-循环运行，是一个简单的、交互式的编程环境。

* **Code descrption**

  * 识别到`.exit`就退出程序
  
* **Test**

  最终应该能完成以下指令

  ![image-20220128145650041](https://s2.loli.net/2022/01/28/ow4zAnZqSiP9BHa.png)

* **Todo**

  - [x] 添加更多命令

#### 3.2 编译器与虚拟机部分

* **Task**

  在REPL的基础上，根据架构层次设计模块，并完成若干指令的实现。

  编译器：即将输入的操作命令，转变成中间代码

  虚拟机：执行虚拟代码

* **Knowledge Point**

  * `common.c`即基础或者公共属性代码，如操作界面，异常等
  * `complier.c`负责解析输入，并生成指令，即编译器功能
  * `vm.c`负责执行指令，即虚拟机功能

* **Test**

  最终应该能完成以下指令

  ![image-20220128152028425](https://s2.loli.net/2022/01/28/r1F5tUJPaBkINx2.png)

* **Todo**

  - [ ] 不支持负数
  - [ ] 没有输入长度检测，如果输入长度太长可能会溢出

#### 3.3 基本输入判断

* **Task**

  加入基本的输入检测。

  编译器：即将输入的操作命令，转变成中间代码

  虚拟机：执行虚拟代码

* **Knowledge Point**

  * 

* **Test**

  最终应该能完成以下指令

  ![image-20220128153018478](https://s2.loli.net/2022/01/28/GJ7VvMFSHt84Lha.png)

* **Todo**

  - [ ] 前面都是数据存储到内存的，而数据库应该是能够将数据存储到本地

#### 3.4 数据本地存储

* **Task**

  加入基本的输入检测。

  编译器：即将输入的操作命令，转变成中间代码

  虚拟机：执行虚拟代码

* **Knowledge Point**

  * 

* **Test**

  最终应该能完成以下指令

  ![image-20220128153018478](https://s2.loli.net/2022/01/28/GJ7VvMFSHt84Lha.png)

* **Todo**

  - [ ] 前面都是数据存储到内存的，而数据库应该是能够将数据存储到本地





## 附录

* [size_t和ssize_t](https://www.cnblogs.com/tianguiyu/articles/6105128.html)

  简单来说`size_t`有符号整型，`ssize_t`无符号整型

* 