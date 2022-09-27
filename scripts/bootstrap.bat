:source_msvc_env
call vcvarsall.bat amd64 10.0.18362.0

call .\lib\vcpkg\bootstrap-vcpkg.bat

.\lib\vcpkg\vcpkg install zeromq

:cmake -DCMAKE_BUILD_TYPE=RelWithDebugInfo -G "NMake Makefiles" -S . -B ./cmake-build-rd

