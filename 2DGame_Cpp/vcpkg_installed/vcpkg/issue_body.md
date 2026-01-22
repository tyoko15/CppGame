Package: pugixml:x64-windows@1.15#1

**Host Environment**

- Host: x64-windows
- Compiler: MSVC 19.43.34808.0
- CMake Version: 3.31.10
-    vcpkg-tool version: 2025-12-16-44bb3ce006467fc13ba37ca099f64077b8bbf84d
    vcpkg-scripts version: 01e159b519 2026-01-21 (8 hours ago)

**To Reproduce**

`vcpkg install `

**Failure logs**

```
Downloading https://github.com/zeux/pugixml/archive/v1.15.tar.gz -> zeux-pugixml-v1.15.tar.gz
Successfully downloaded zeux-pugixml-v1.15.tar.gz
-- Extracting source C:/vcpkg/downloads/zeux-pugixml-v1.15.tar.gz
CMake Error at scripts/cmake/vcpkg_execute_required_process.cmake:127 (message):
    Command failed: C:/vcpkg/downloads/tools/cmake-3.31.10-windows/cmake-3.31.10-windows-x86_64/bin/cmake.exe -E tar xjf C:/vcpkg/downloads/zeux-pugixml-v1.15.tar.gz
    Working Directory: C:/vcpkg/buildtrees/pugixml/src/v1.15-8a9beec4a5.clean.tmp
    Error code: 1
    See logs for more information:
      C:\vcpkg\buildtrees\pugixml\extract-err.log

Call Stack (most recent call first):
  scripts/cmake/vcpkg_extract_source_archive.cmake:120 (vcpkg_execute_required_process)
  scripts/cmake/vcpkg_extract_source_archive_ex.cmake:8 (vcpkg_extract_source_archive)
  scripts/cmake/vcpkg_from_github.cmake:127 (vcpkg_extract_source_archive_ex)
  ports/pugixml/portfile.cmake:1 (vcpkg_from_github)
  scripts/ports.cmake:206 (include)



```

<details><summary>C:\vcpkg\buildtrees\pugixml\extract-err.log</summary>

```
CMake Error: Problem with archive_write_header(): Invalid empty pathname
CMake Error: Current file: 
CMake Error: Problem extracting tar: C:/vcpkg/downloads/zeux-pugixml-v1.15.tar.gz
```
</details>

**Additional context**

<details><summary>vcpkg.json</summary>

```
{
  "name": "2dgame-cpp",
  "version-string": "0.1",
  "dependencies": [
    "assimp"
  ]
}

```
</details>
