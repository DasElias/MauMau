file(GLOB_RECURSE res_cards_source_files
    "*.h"
    "*.cpp"
)

add_library(res_cards ${res_cards_source_files})