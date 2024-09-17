## TinySTL

这是一个简易轻量级的 C++ 标准模板库（STL）实现，包括从内存配置器到容器和一部分算法的一系列组件，以及简单的接口测试。开发任务已经大致完成，与现有标准库接口兼容性良好。

#### 项目介绍

源代码参考自侯捷先生所著《STL源码剖析》中的部分 SGI STL v2.91，这里全部用 C++11 实现，补充了书中缺漏的部分以及新增的特性。

开发博客在知乎与 khyallin.github.io 连载，适合有志于深入研究 C++ 标准库的初学者。由于正在长期维护并修复 bug，请以 Github 而非博客中展示的源代码为准。

#### 开发计划

- 两级内存配置器 allocator，其中内置一个内存池

- 五种不同的迭代器 iterator，使用 traits 编程技巧

- 序列式容器（包括 vector, list, deque）及其拓展（包括 stack, queue, priority_queue）

- 关联式容器的底层实现（包括 rb_tree, hashtable）及其外部接口（包括 set, map, multiset, multimap, unordered_set, unordered_map）

- 部分算法，例如 hash 系列算法与 heap 系列算法

- 使用了部分 std 基础算法（copy, copy_backward等）与适配器（identity, less 等）以提高可读性

#### 使用方法

这是一个 only-header 开源库，运行于 windows11 下的 mingw 8.1.0，您可以下载并引入到自己的项目中。欢迎您报告问题，也许存在内存安全问题、异常安全问题、兼容性问题、不符合 STL 设计理念的行为，或者性能更高的解决方案。
