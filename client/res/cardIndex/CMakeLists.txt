file(GLOB_RECURSE res_cardIndex_source_files
    "*.h"
    "*.cpp"
)

add_library(res_cardIndex ${res_cardIndex_source_files})