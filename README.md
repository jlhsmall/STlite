# STLite-2020
CS147 assignment, ACM Class, SJTU

## 实现要求

要求实现与C++标准库类似的数据结构，包括迭代器等。框架接口在已本仓库中给出，只需要实现`.hpp`文件内所要求的内容即可。

请注意，在本作业中，只允许使用`cstdio`，`cstring`，`iostream`，`cmath`四个C++标准库，如需使用其他功能请自行实现。在下发代码中有`exceptions.hpp`和`utility.hpp`两个辅助文件，你需要使用其中的异常处理和`sjtu::pair`完成STLite的实现，但不允许修改其中的任何内容。

本作业中的四个容器在接口和复杂度要求上与STL基本一致，除了：

#### 与STL的不同之处

- `sjtu::priority_queue`要求以最高$O(\log n)$的时间复杂度实现合并操作。
- `sjtu::deque`要求支持以不超过$O(\sqrt n)$时间完成**任意位置**的插入和删除，但随机访问的时间复杂度要求降低为$O(\sqrt n)$。
- `sjtu::deque`的迭代器在首尾操作后也会失效。

因此，如果对内容有任何疑问，可以参考STL，或是在这里提出issue，谢谢！


## 评测方法

测试数据将全部下发（见本仓库），供调试使用。最终测试将会在OJ上进行，并进行code review。

## 截止日期

`priority_queue`(A)/`vector`(B)：3月25日（第四周周三）23:00前

其余内容：4月22日（第九周周三）23:00前。
（爷学一下git