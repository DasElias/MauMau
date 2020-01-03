file(GLOB_RECURSE shared_model_source_files
    "*.h"
    "*.cpp"
)

add_library(shared_model ${shared_model_source_files})