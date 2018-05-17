
set(SOURCES
    main.c
    buffer.c
)

add_executable(buffer_test ${SOURCES})
target_link_libraries(buffer_test syrup-buffer unity)
add_test(buffer_test buffer_test)
