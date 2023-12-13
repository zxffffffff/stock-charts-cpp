# stock-charts-cpp
基于 pure C++ 实现股票图表和技术指标，支持任何 UI 框架并提供 Qt Widget 实现。
- Header-Only：无需编译，遵循MVVM设计模式，方便阅读和修改。
- 指标（Model/Plugin/Indicator）模仿富途牛牛实现，可定制开发。
- 抽象绘图 (Graphics/Painter) 支持任何 UI 框架和鼠标/键盘交互。
- 提供 Qt Widget 实现，可以直接编译和运行 demo/test。

Implementation of stock charts and technical indicators in pure C++, supporting any UI framework and providing a Qt Widget implementation.

- Header-Only: No need for compilation, follows the MVVM design pattern for easy reading and modification.
- Indicators (Model/Plugin/Indicator) are realized by imitating Futubull, and can be customized for development.
- Abstract drawing (Graphics/Painter) supports any UI framework and mouse/keyboard interaction.
- Provides a Qt Widget implementation that can be compiled and run directly for demo/testing purposes.

## 使用(Usage)
```cpp
// stock
kStock = std::make_shared<StockCore>(Candlestick());

// model
auto model = std::make_shared<ChartModel>(kStock);
model->addPlugin<PluginIndicator>();

// viewmodel
auto vm = std::make_shared<ChartViewModel>(model);
vm->addLayer<LayerBG>();
vm->addLayer<LayerStock>();
vm->addLayer<LayerSuperimposition>();
vm->addLayer<LayerIndicator>();
vm->addLayer<LayerPainting>();
vm->addLayer<LayerCrossLine>();
vm->addLayer<LayerTitle>();

// view(Qt)
auto view = new ChartViewQt(ui.kchartWidget);
view->init(vm);
```

## 作者说明
- 精力有限，偶尔维护，有需要可以联系我答疑解惑（zxffffffff@outlook.com, 1337328542@qq.com）。
- `star >= 100` 可以考虑更新绘图、叠加、复权等功能。
- WASM（Web）移植版本：https://github.com/zxffffffff/stock-charts-wasm.git
- Dart（Flutter）移植版本：https://github.com/zxffffffff/stock-charts-dart.git

## 参数控制
![image](https://github.com/zxffffffff/stock-charts-cpp/blob/main/doc/stock-chart-0.png)

## 指标管理（模仿富途牛牛）
![image](https://github.com/zxffffffff/stock-charts-cpp/blob/main/doc/stock-chart-1.png)

## 目录结构
![image](https://github.com/zxffffffff/stock-charts-cpp/blob/main/doc/stock-chart-src.png)

## 架构图
![image](https://github.com/zxffffffff/stock-charts-cpp/blob/main/doc/architecture.png)
