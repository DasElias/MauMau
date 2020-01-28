file(GLOB_RECURSE res_avatars_source_files
    "*.h"
    "*.cpp"
)

add_library(res_avatars ${res_avatars_source_files})