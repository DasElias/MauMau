file(GLOB_RECURSE renderingModel_source_files
    "*.h"
    "*.cpp"
)

add_library(renderingModel ${renderingModel_source_files})
target_link_libraries(renderingModel
    res_cards
    res_cardIndex
    res_avatars
)