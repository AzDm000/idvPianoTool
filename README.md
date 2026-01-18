# idvPianoTool

以学习windows api为目的开发的一款第五人格钢琴辅助演奏工具。支持将 FL Studio 的工程文件（.flp）直接转换为游戏内按键信号。

---

## 环境准备
请确保以下文件位于**同一目录下**：
* `idvPianoTool.exe` (核心程序)
* `flp_exporter.exe` (FLP 转换组件)
* 你的曲谱文件（`.flp` 或 `.json`）

---

## 使用方式
* 右键 `idvPianoTool.exe`并选择 **[以管理员身份运行]**
* 根据指引输入含扩展名的文件名（例如 `test.flp` 或 `out.json`）。
* 程序会自动识别并聚焦至第五人格游戏进程。
* **注意**：程序切屏至游戏后将在约 **1 秒内** 开始播放播放按键。

---

## 注意事项
* 程序仅会提取flp工程文件中**第一个**样式下的音符数据，所以请确认：
* 在 FL Studio 中仅使用 **1 个样式 (Pattern)** 与 **1 种乐器**。
* 编曲时控制在 **3 个音阶** 以内（游戏内乐器最高仅支持3个音阶）。
* 程序每轮处理 `.flp` 时会自动在根目录生成 `out.json` 并覆盖旧文件，如需保留曲谱json文件请及时重命名备份。

---

## 重要提示
> [!IMPORTANT]
> **关于按键失灵**：
> 经经验总结，第五人格PC客户端每次打开钢琴界面后，如果在游戏内进行了鼠标点击操作，会概率造成屏幕右侧部分按键暂时失灵。离开演奏界面再进入可以刷新状态。

---

## 鸣谢
本工具的 `flp_exporter.exe` 组件基于 **PyFLP** 开发：
* **项目链接**：[demberto/PyFLP](https://github.com/demberto/PyFLP)

---

## 附 json格式示例

```json
[
    {
        "pitch": "A5",
        "start_ms": 0.0,
        "end_ms": 238.1,
        "duration_ms": 238.1
    },
    {
        "pitch": "D#6",
        "start_ms": 238.1,
        "end_ms": 476.19,
        "duration_ms": 238.1
    },
    {
        "pitch": "E6",
        "start_ms": 476.19,
        "end_ms": 714.29,
        "duration_ms": 238.1
    }
]
```
