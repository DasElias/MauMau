file(GLOB_RECURSE nanovg_source_files
    "*.h"
	"*.c"
)


add_library(nanovg ${nanovg_source_files})