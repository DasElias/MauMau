file(GLOB_RECURSE res_table_source_files
    "*.h"
    "*.cpp"
)

add_library(res_table ${res_table_source_files})