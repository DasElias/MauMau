file(GLOB_RECURSE res_ingame_source_files
    "*.h"
    "*.cpp"
)

add_library(res_ingame ${res_ingame_source_files})