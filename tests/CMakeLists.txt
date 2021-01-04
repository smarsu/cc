# Copyright (c) 2020 smarsufan. All Rights Reserved.

add_executable(test_gettok test_gettok.cc)
target_link_libraries(test_gettok smcc_core)

add_executable(test_ast test_ast.cc)
target_link_libraries(test_ast smcc_core)

add_executable(test_call test_call.cc)
target_link_libraries(test_call smcc_core)
