https://github.com/microsoft/vcpkg
git clone https://github.com/microsoft/vcpkg.git

cd vcpkg && bootstrap-vcpkg.bat
启动脚本执行先决条件检查并下载vcpkg可执行文件。就这么简单，vcpkg已安装并可供使用。

配置VCPKG_ROOT环境变量。

set "VCPKG_ROOT=C:\path\to\vcpkg"
set PATH=%VCPKG_ROOT%;%PATH%


vcpkg.exe install 7zip:x64-windows
vcpkg.exe install libiconv:x64-windows
vcpkg.exe install openssl:x64-windows
vcpkg.exe install ffmpeg:x64-windows

vcpkg.exe install ffmpeg[core,mp3lame,ffmpeg,ffplay,ffprobe,x264,x265,zlib,iconv,nonfree,gpl,opencl,opengl,zmq,ilbc,freetype,vulkan,nvcodec]:x64-windows --recurse

vcpkg.exe install opencv:x64-windows

vcpkg.exe install avcpp:x64-windows

vcpkg.exe install sdl2:x64-windows

vcpkg.exe install nghttp3:x64-windows
vcpkg.exe install pcre:x64-windows
vcpkg.exe install pcre2:x64-windows
vcpkg.exe install libuv:x64-windows
vcpkg.exe install libhv:x64-windows
  
./vcpkg remove zlib:x86-windows libiconv:x86-windows
											
											
FDYTF3CX7PLICVPP