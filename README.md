# python-conda-adapter

A naive exe to setup conda environments, allowing python to be executed without any settings.

简单的python包装器。在python启动前设置好conda变量，无需提前activate base环境。

## How to use

- Download & unpack
- Modify `CONDA_BASE_PATH` in `adapter.cpp` to your local conda folder
- Compile with `Release x64`
- Move generated exe file to a folder under PATH
- Optional: Modify `target_cmdline` to other `.exe` files and redo above, to allow "standalone" executing of other commands.

If you don't want to mess up with compiling, install your anaconda to `D:\apps\Anaconda3` and just download the Release files.

## 使用说明

下载解压，根据需要修改代码，编译（推荐Release x64），挪到一个文件夹下，把这个文件夹加到系统的PATH里。

现在随便打开一个cmd，输入python，试试`import numpy`应该不会报错了。

懒得编译可以直接下载Release里的文件，但conda必须安装到`D:\apps\Anaconda3`。
