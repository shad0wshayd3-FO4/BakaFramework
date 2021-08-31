python3 ".\scripts\cmake_generate.py"
cmake -B"build" -S"." -A"x64" --preset vs2022-windows-vcpkg
Read-Host -Prompt "Press Enter to exit"
