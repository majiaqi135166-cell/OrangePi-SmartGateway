
# OPi-Smart-Gateway: 基于香橙派的嵌入式温控网关

## 🚀 项目简介
本项目是一个基于 **Orange Pi Zero 3** 开发的嵌入式 Linux 监控系统。通过多线程技术实现了硬件状态采集（CPU温度）、本地逻辑控制（阈值报警/LED闪烁）以及远程 Web 实时监控。

## 🛠️ 核心技术栈
* **语言**: C语言 (C99 标准)
* **并发**: POSIX Threads (Pthread) 实现主从线程分离
* **网络**: 基于 BSD Socket 手写轻量级 HTTP 服务端
* **平台**: Orange Pi Zero 3 (Allwinner H618), Ubuntu/Debian Arm64
* **驱动交互**: Linux Sysfs (Thermal Zone & GPIO)

## 🌟 功能亮点
1. **多线程并发**: 主线程负责高频次采集传感器数据并驱动硬件响应；从线程维护 Web Socket，确保监控请求不阻塞核心控制逻辑。
2. **动态配置**: 支持通过 `config.txt` 或命令行参数实时调整报警阈值（Hot/Warm）。
3. **安全退出机制**: 注册 `SIGINT` 信号，确保程序退出时自动释放 Socket 资源、熄灭 LED 并记录退出日志。
4. **Web 实时仪表盘**: 网页前端利用 `meta-refresh` 实现 2 秒无刷新数据同步。

## 🔍 技术攻关记录
在开发过程中，曾遇到 **TCP 连接堆积导致 Web 服务死锁** 的问题。经过排查发现系 HTTP 协议头格式不规范导致浏览器未能识别 `Connection: close` 指令。通过规范化 Header 结构并引入 `SO_REUSEADDR` 套接字选项，成功解决了高并发刷新下的连接阻塞问题，提升了系统的鲁棒性。

## 📦 如何编译运行
1. **克隆项目**:
   ```bash
   git clone [https://github.com/你的用户名/SmartGateway.git](https://github.com/你的用户名/SmartGateway.git)
   cd SmartGateway
