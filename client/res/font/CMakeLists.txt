file(GLOB_RECURSE res_font_source_files
    "*.h"
    "*.cpp"
)

add_library(res_font ${res_font_source_files})