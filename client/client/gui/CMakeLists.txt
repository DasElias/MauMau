file(GLOB gui_source_files
    "*.h"
    "*.cpp"
)

add_library(gui ${gui_source_files})