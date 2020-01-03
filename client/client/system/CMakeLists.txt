file(GLOB system_source_files
    "*.h"
    "*.cpp"
)

add_library(system ${system_source_files})