
<h1>将Wine内建自带的通用对话框替换为Linux下zenity的对话框组件</h1>

<h2>Replace Wine's built-in generic dialog boxes with Zenity dialog components.</h2>

使用方法，用x86_64-w64-mingw32编译得到的64位comdlg32.dll放入windows/system32目录下

i686-w64-mingw32编译得到的32位comdlg32.dll放入windows/syswow64目录下

使用wine dll override方式启动exe进行应用 (或用wincfg配置)

<h1>wine builtin:</h1>

WINEDLLOVERRIDES="comdlg32=b" wine taskmgr

<img width="609" height="450" alt="屏幕截图_20250924_201208" src="https://github.com/user-attachments/assets/602f060b-cd80-4fa4-9b16-631d2e31b467" />

<h1>replace with zenity implement:</h1>

WINEDLLOVERRIDES="comdlg32=n" wine taskmgr

<img width="1029" height="438" alt="屏幕截图_20250924_201100" src="https://github.com/user-attachments/assets/bd79e5f4-7350-4d37-8be5-c90e37be5117" />
