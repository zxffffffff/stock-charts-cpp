# stock-charts-cpp
基于 pure C++ 实现股票图表和技术指标，支持任何 UI 框架并提供 Qt Widget 实现。
- Header-Only：无需编译，遵循MVVM设计模式，方便阅读和修改。
- 指标（lib-chart/Model/Indicator）模仿富途牛牛实现，可定制开发。
- 抽象绘图 (lib-chart/Graphics) 支持任何 UI 框架和鼠标/键盘交互。
- 提供 Qt Widget 实现，可以直接编译和运行 demo/test。

Implementation of stock charts and technical indicators in pure C++, supporting any UI framework and providing a Qt Widget implementation.

- Header-Only: No need for compilation, follows the MVVM design pattern for easy reading and modification.
- Indicators (lib-chart/Model/Indicator) are realized by imitating Futubull, and can be customized for development.
- Abstract drawing (lib-chart/Graphics) supports any UI framework and mouse/keyboard interaction.
- Provides a Qt Widget implementation that can be compiled and run directly for demo/testing purposes.

## 作者说明
- 精力有限，偶尔维护，有需要可以联系我答疑解惑（zxffffffff@outlook.com, 1337328542@qq.com）。
- `star >= 100` 可以考虑更新绘图、叠加、复权等功能。

## 参数控制
![image](https://github.com/zxffffffff/stock-charts-cpp/blob/main/doc/stock-chart-0.png)

## 指标管理（模仿富途牛牛）
![image](https://github.com/zxffffffff/stock-charts-cpp/blob/main/doc/stock-chart-1.png)

## 目录结构
![image](https://github.com/zxffffffff/stock-charts-cpp/blob/main/doc/stock-chart-src.png)

## 架构图
![image](https://github.com/zxffffffff/stock-charts-cpp/blob/main/doc/architecture.png)
