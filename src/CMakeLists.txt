target_sources(surm
  PRIVATE
    main.cpp
    util.hpp
)

add_subdirectory(log)
add_subdirectory(stages)
add_subdirectory(surmfile)

target_include_directories(surm
  PRIVATE
    src
    src/log
    src/stages
    src/surmfile)
