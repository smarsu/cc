# Copyright (c) 2020 smarsufan. All Rights Reserved.

set(__smcc_lib CACHE INTERNAL "__smcc_lib")
function(smcc_library TARGET SRC)
  set(__smcc_lib "${__smcc_lib};${SRC}" CACHE INTERNAL "")
endfunction()

smcc_library(reader reader.cc)
smcc_library(expr expr.cc)
smcc_library(ast ast.cc)
smcc_library(codegen codegen.cc)

add_library(smcc_core ${__smcc_lib})
