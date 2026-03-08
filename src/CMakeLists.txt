target_sources(surm
  PRIVATE
    main.cpp
    util.hpp
)

add_subdirectory(log)
add_subdirectory(stages)
add_subdirectory(surmfile)

