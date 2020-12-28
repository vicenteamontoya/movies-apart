add_subdirectory(ActorGraph)

add_executable (pathfinder.exe pathfinder.cpp)
target_link_libraries(pathfinder.exe PRIVATE actorgraph_dep)
