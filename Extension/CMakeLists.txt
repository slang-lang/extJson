INCLUDE(${PROJECT_SOURCE_DIR}/CMake/Base.cmake)

SET(SOURCES
    Extension.cpp
)

SET(HEADERS
    Defines.h
    Extension.h
    FromJson.h
    JsonEncode.h
    ToJson.h
)

SET(DEPENDENCIES
	Core
)

SET(MODULES
	json
	slang
)

build_shared_lib(SlangJson "${MODULES}")
