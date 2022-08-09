rmdir /s /q build
mkdir build
cd build
cmake -DCMAKE_CXX_FLAGS="/MP" ..
"C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe" breakout_project.sln  -maxCpuCount:8 /p:Configuration=Release /p:Platform="x64"