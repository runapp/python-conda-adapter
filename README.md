# python-conda-adapter
A naive exe to setup conda environments, allowing python to be executed without any settings.

## How to use

- Download & unpack
- Modify `CONDA_BASE_PATH` in `adapter.cpp` to your local conda folder
- Compile with `Release x64`
- Move generated exe file to a folder under PATH
- Optional: Modify `target_cmdline` to other `.exe` files and redo above, to allow "standalone" executing of other commands.

If you don't want to mess up with compiling, install your anaconda to `D:\apps\Anaconda3` and just download the Release files.
