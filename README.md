# ttLib_wx

This repository contains several classes and functions that have been used in most of the KeyWorks Software projects since the company was formed in 1998. As such, either this repostory or it's older version (`ttLib`) will appear as a sub-module in most repositories owned by **KeyWorks Software**.

This repository is designed to be built and used with the **wxWidgets** library, though this isn't strictly necessary. There is an older version of the repostiory at [ttLib](https://github.com/KeyWorksRW/ttLib) which contains classes and code that can only be used when building solely for Windows (or Wine) without wxWidgets.


In order to include support for **wxString**, the files in this directory will need to be built by your application since the ttLib sub-module won't know where the wxWidgets header files are located. If you are using a `CMakeLists.txt` file, you can add something similar to the following:

```CMake
    include(ttLib/src/wx/ttlib_file_list.cmake)
    add_library(ttLib_wx STATIC ${ttlib_file_list})
    target_precompile_headers(ttLib_wx PRIVATE "your_precompiled_header_here.h")
    target_include_directories(ttLib_wx PRIVATE
        ${widget_dir}/include
        src/
        ttLib/include
    )

    target_link_libraries(your_app PRIVATE ttLib_wx wxWidgets libraries)
```

Alternatively, you could simply include the `ttLib_file_list.cmake` file and then add `${ttlib_file_list}` to your source file list -- but only do that if you are certain you want to use all the classes and source modules.

## License

All KeyWorks Software contributions use a Apache License 2.0 [LICENSE](LICENSE).

[![CodeQL](https://github.com/KeyWorksRW/ttLib/workflows/CodeQL/badge.svg)](https://github.com/KeyWorksRW/ttLib/actions?query=workflow:"CodeQL")
