file(GLOB_RECURSE glad_source_files
    "*.h"
    "*.cpp"
)


add_library(glad ${glad_source_files})