# stock-charts-cpp
Implementación de gráficos bursátiles e indicadores técnicos en pure C++, compatible con cualquier framework de UI e incluye una implementación de Qt Widget.
- Header-Only: No requiere compilación, sigue el patrón de diseño MVVM para facilitar la lectura y modificación.
- Los indicadores (Model/Plugin/Indicator) están implementados imitando a Futubull, y se pueden personalizar para el desarrollo.
- El dibujo abstracto (Graphics/Painter) soporta cualquier framework de UI e interacción con ratón/teclado.
- Proporciona una implementación de Qt Widget que se puede compilar y ejecutar directamente para demostración/pruebas.

Implementation of stock charts and technical indicators in pure C++, supporting any UI framework and providing a Qt Widget implementation.

- Header-Only: No need for compilation, follows the MVVM design pattern for easy reading and modification.
- Indicators (Model/Plugin/Indicator) are realized by imitating Futubull, and can be customized for development.
- Abstract drawing (Graphics/Painter) supports any UI framework and mouse/keyboard interaction.
- Provides a Qt Widget implementation that can be compiled and run directly for demo/testing purposes.

## Uso (Usage)
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

## Notas del autor
- Energía limitada, mantenimiento ocasional, si necesitas ayuda puedes contactarme para resolver dudas.
- `star >= 100` se pueden considerar actualizaciones de funciones como dibujo, superposición, ajuste de derechos, etc.
- Versión de puerto WASM (Web): https://github.com/zxffffffff/stock-charts-wasm.git
- Versión de puerto Dart (Flutter): https://github.com/zxffffffff/stock-charts-dart.git

## Control de parámetros
![image](https://github.com/zxffffffff/stock-charts-cpp/blob/main/doc/stock-chart-0.png)

## Gestión de indicadores (imitando a Futubull)
![image](https://github.com/zxffffffff/stock-charts-cpp/blob/main/doc/stock-chart-1.png)

## Estructura del directorio
![image](https://github.com/zxffffffff/stock-charts-cpp/blob/main/doc/stock-chart-src.png)

## Diagrama de arquitectura
![image](https://github.com/zxffffffff/stock-charts-cpp/blob/main/doc/architecture.png)
