file(GLOB_RECURSE renderingModel_source_files
    "*.h"
    "*.cpp"
)

add_library(renderingModel ${renderingModel_source_files})