file(GLOB_RECURSE res_menu_source_files
    "*.h"
    "*.cpp"
)

add_library(res_menu ${res_menu_source_files})