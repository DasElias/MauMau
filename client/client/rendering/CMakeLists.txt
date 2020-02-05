file(GLOB_RECURSE rendering_source_files
    "*.h"
    "*.cpp"
)

add_library(rendering ${rendering_source_files})
target_link_libraries(rendering
	renderingModel
    res_table
    res_ingame
)