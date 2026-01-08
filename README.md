# C-Qt-CampusNavigationSystem
燕山大学校园导航系统（C++ 实现）是数据结构课程实践项目，基于 C++ 原生开发（无第三方依赖），核心依托邻接表存储校园图结构，结合小根堆优化的 Dijkstra 算法实现最短路径规划，支持起点→终点、多途经点串联的路径查询。系统集成完整的用户管理模块，区分学生 / 教师 / 访客身份，实现用户注册 / 登录、信息持久化（二进制文件存储），学生 / 教师可保存规划路径，访客无此权限。Windows 平台下支持路径信息、个性化提示的语音播报，核心功能（路径规划、用户管理）可跨平台运行。项目兼顾数据结构算法落地验证与校园导航实用性，涵盖图、优先队列、哈希表、栈等多种核心数据结构应用，代码模块化设计，易扩展（如新增 A * 算法、双权重路径规划、图形化界面等），是数据结构入门实践的典型案例。
# 燕山大学校园导航系统

<div align="center">
  <p>基于C++实现的校园导航系统，支持最短路径规划、多节点途经点导航、用户认证、语音播报等功能</p>
  <img src="https://img.shields.io/badge/language-C%2B%2B-blue.svg" alt="Language">
  <img src="https://img.shields.io/badge/platform-Windows-green.svg" alt="Platform">
  <img src="https://img.shields.io/badge/license-MIT-yellow.svg" alt="License">
</div>

## 项目简介
本项目是为燕山大学师生及访客设计的校园导航系统，基于**图结构**和**Dijkstra最短路径算法**实现校园内任意地点的路径规划，支持多途经点导航、用户登录注册、路线保存、语音播报等功能。针对不同用户类型（学生/教师/访客）提供个性化提示，适配校园场景的实际需求。

### 开发背景
作为燕山大学大二电子信息工程专业的课程实践项目，旨在将数据结构（图、最短路径算法）、C++面向对象编程、文件IO、跨平台交互等知识点结合，解决校园内导航的实际问题。

### 核心目标
- 实现校园地点间的最短路径计算
- 支持多途经点的自定义路径规划
- 提供用户认证与个性化服务
- 集成语音播报功能，提升使用体验
- 支持路线保存，方便重复使用

## 技术栈
| 技术/知识点                | 说明                                                                 |
|---------------------------|----------------------------------------------------------------------|
| 编程语言                  | C++ (C++11及以上)                                                    |
| 核心数据结构              | 图（邻接表实现）、哈希表、向量（vector）、栈、优先队列                |
| 算法                      | Dijkstra最短路径算法、多节点路径拼接、路径逆序与合并                  |
| 功能模块                  | 文件IO（二进制存储用户数据）、用户认证、语音合成（Windows PowerShell） |
| 开发/运行环境             | Windows（语音功能依赖PowerShell）、GCC/MinGW/VS编译器                |

## 功能特性
### 1. 用户管理
- 支持**学生/教师/访客**三种用户类型注册与登录
- 学生/教师需实名认证（学号/工号+密码），访客免注册直接使用
- 二进制文件持久化存储用户数据，保障数据不丢失
- 展示用户信息（姓名、类型、专业/部门、已保存路线数）

### 2. 路径规划
- 基于Dijkstra算法计算两点间最短路径
- 支持**多途经点**自定义导航（如：教学楼A → 图书馆 → 食堂 → 宿舍）
- 显示路径详情（起点、途经点、终点、总距离、完整路线）
- 支持保存常用路线，方便后续查看

### 3. 语音播报
- 自动播报路径信息（总距离、完整路线）
- 针对不同用户类型播报个性化提示（如学生提示上课时间、教师提示办公区信息）
- 可手动开启/关闭语音功能

### 4. 校园地图管理
- 内置燕山大学核心地点（教学楼、图书馆、食堂、宿舍、办公楼等16个地标）
- 可视化展示校园地图的邻接表结构
- 支持动态添加顶点/边（扩展校园地图）

## 环境要求
### 运行平台
- Windows（语音播报功能依赖Windows PowerShell的语音合成接口，Linux/macOS需适配语音模块）

### 编译环境
- GCC/MinGW（推荐MinGW-w64）
- Visual Studio 2019/2022（兼容）
- 支持C++11及以上标准的编译器

## 安装与运行
### 1. 克隆仓库
```bash
git clone https://github.com/你的用户名/ysu-campus-navigation.git
cd ysu-campus-navigation
```

### 2. 编译代码
#### 方法1：使用GCC/MinGW
```bash
# 编译所有源文件，生成可执行文件
g++ main.cpp Graph.cpp VoiceBroadcast.cpp -o YSU_Campus_Navigation.exe -std=c++11
```

#### 方法2：使用Visual Studio
1. 新建空项目，将所有`.h`和`.cpp`文件添加到项目中
2. 设置项目属性：C++标准选择`ISO C++11 标准`
3. 编译并生成可执行文件

### 3. 运行程序
```bash
# Windows终端运行
YSU_Campus_Navigation.exe
```

## 项目目录结构
```
ysu-campus-navigation/
├── Graph.h          # 图结构头文件（顶点、边、最短路径算法声明）
├── Graph.cpp        # 图结构实现（Dijkstra、多节点路径计算等）
├── VoiceBroadcast.h # 语音播报模块头文件
├── VoiceBroadcast.cpp # 语音播报实现（Windows PowerShell语音合成）
├── main.cpp         # 主程序（用户交互、菜单、核心逻辑）
├── user_data.dat    # 用户数据文件（首次运行自动生成）
└── README.md        # 项目说明文档
```

### 文件功能说明
| 文件                     | 核心功能                                                                 |
|--------------------------|--------------------------------------------------------------------------|
| `Graph.h/.cpp`           | 图的邻接表实现、Dijkstra算法、多节点路径计算/拼接、校园地图初始化         |
| `VoiceBroadcast.h/.cpp`  | 语音播报接口、路径文本转语音、用户类型提示语音、语音功能开关             |
| `main.cpp`               | 主程序入口、用户登录/注册、菜单交互、路径规划流程、用户数据持久化         |
| `user_data.dat`          | 二进制存储用户信息（自动生成，请勿手动修改）                             |

## 使用指南
### 1. 首次运行
- 程序检测到无用户数据时，自动进入首次设置流程
- 可选：注册新用户/使用访客账号/使用默认测试账号
  - 测试账号1（学生）：学号`202301001`，密码`123456`
  - 测试账号2（教师）：工号`T1001`，密码`teacher123`
  - 访客账号：免密码直接使用

### 2. 主菜单操作
| 状态       | 功能选项                                                                 |
|------------|--------------------------------------------------------------------------|
| 未登录     | 路径规划、查看校园地图、用户登录、用户注册、退出系统                     |
| 已登录     | 路径规划、查看校园地图、查看用户信息、查看保存路线（学生/教师）、退出登录、退出系统 |

### 3. 路径规划流程
1. 选择「路径规划」功能，程序展示校园地标列表（编号+名称）
2. 输入**起点编号**
3. 选择是否添加途经点（可选多个）
4. 输入**终点编号**
5. 程序自动计算最短路径，展示：起点、途经点、终点、总距离、完整路线
6. （可选）语音播报路径信息（默认开启，可手动关闭）
7. （学生/教师）可选择保存当前路线，方便后续查看

### 4. 语音播报说明
- 语音功能默认开启，仅支持Windows平台
- 路径播报格式：`为您规划的路线总距离XXX米，路线为：A → B → C，已到达终点。`
- 个性化提示：
  - 学生：提示上课提前10分钟、教学楼电梯使用提示
  - 教师：提示办公楼停车区域、会议室位置
  - 访客：提示校史馆位置、咨询处位置

## 核心算法说明
### 1. Dijkstra最短路径算法
- 基于优先队列（小顶堆）实现，时间复杂度`O(E + VlogV)`（V为顶点数，E为边数）
- 计算单源最短路径，记录前驱节点以还原路径
- 处理校园地图中无向带权边（距离为权重）

### 2. 多节点路径拼接
- 将「起点→途经点1→途经点2→…→终点」拆分为多个子路径（起点→途经点1、途经点1→途经点2、…）
- 对每个子路径调用Dijkstra算法，拼接子路径并去重（移除重复的途经点）
- 累加所有子路径距离，得到总距离

## 待优化/扩展功能
- [ ] 适配Linux/macOS平台的语音播报功能（替换PowerShell为espeak等工具）
- [ ] 增加地图可视化界面（如基于EasyX/Qt实现图形化界面）
- [ ] 支持路径偏好设置（如：最短距离/最少步数/避开水域）
- [ ] 增加校园POI（兴趣点）分类（教学楼/食堂/宿舍/场馆）
- [ ] 优化用户数据加密存储（当前为明文二进制存储）

## 作者信息
- 作者：燕山大学 电子信息工程专业 大二学生
- 开发时间：2025年
- 联系方式：（可选添加邮箱/博客地址）

## 许可证
本项目采用MIT许可证开源，详见[LICENSE](LICENSE)文件（可自行添加LICENSE文件）。

```
MIT License

Copyright (c) 2025 燕山大学电子信息工程

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

## 致谢
- 燕山大学数据结构课程组提供的算法指导
- C++标准库、PowerShell语音合成接口等开源工具/接口
