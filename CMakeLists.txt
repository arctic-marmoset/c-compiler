cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(CCompiler)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS FALSE)

string(TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_UPPER)
if(CMAKE_BUILD_TYPE_UPPER STREQUAL "RELEASE")
    include(CheckIPOSupported)
    check_ipo_supported(RESULT IPO_IS_SUPPORTED)
    if(IPO_IS_SUPPORTED)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
    endif()
endif()

option(CCOMPILER_USE_EXTENSIVE_WARNINGS "Turn warnings up to 11" TRUE)
option(CCOMPILER_TREAT_WARN_AS_ERROR "Treat compiler warnings as errors" FALSE)

if(MSVC)
    string(REGEX REPLACE "[-/]W[1-4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    set(CCOMPILER_WARN_FLAGS /W4 /permissive-)
else()
    set(CCOMPILER_WARN_FLAGS -Wall -Wextra -Wpedantic)
endif()

if(CCOMPILER_TREAT_WARN_AS_ERROR)
    if(MSVC)
        list(APPEND CCOMPILER_WARN_FLAGS /Wx)
    else()
        list(APPEND CCOMPILER_WARN_FLAGS -Werror -pedantic-errors)
    endif()
endif()

if(CCOMPILER_USE_EXTENSIVE_WARNINGS)
    if(MSVC)
        list(APPEND CCOMPILER_WARN_FLAGS
            /w14242 # possible loss of data: conversion from `type1` to `type2`
            /w14254 # possible loss of data: conversion from `type1:field_bits`
                    # to `type2:field_bits`
            /w14263 # member function does not override any base class virtual
                    # member function
            /w14265 # class has virtual functions, but destructor is not
                    # virtual; instances of this class may not be destructed
                    # correctly
            /w14287 # unsigned/negative constant mismatch
            /we4289 # loop control variable declared in the for-loop is used
                    # outside the for-loop scope
            /w14296 # expression is always [true | false]
            /w14311 # pointer truncation from `type1` to `type2`
            /w14545 # expression before comma evaluates to a function which is
                    # missing an argument list
            /w14546 # function call before comma missing argument list
            /w14547 # operator before comma has no effect; expected operator
                    # with side-effect
            /w14549 # operator before comma has no effect; did you intend
                    # 'operator'?
            /w14555 # expression has no effect; expected expression with
                    # side-effect
            /w14619 # pragma warning: there is no warning number `number`
            /w14640 # thread un-safe static member initialization
            /w14826 # conversion from 'type1' to 'type_2' is sign-extended which
                    # may cause unexpected runtime behavior
            /w14928 # illegal copy-initialization; more than one user-defined
                    # conversion has been implicitly applied
        )
    else()
        list(APPEND CCOMPILER_WARN_FLAGS
            -Wshadow
            -Wnon-virtual-dtor
            -Wold-style-cast
            -Wcast-align
            -Wunused
            -Woverloaded-virtual
            -Wconversion
            -Wsign-conversion
            -Wnull-dereference
            -Wformat=2
            -Wfloat-equal
        )
    endif()

    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        list(APPEND CCOMPILER_WARN_FLAGS
            -Wduplicated-cond
            -Wduplicated-branches
            -Wlogical-op
            -Wuseless-cast
        )
    elseif(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
        list(APPEND CCOMPILER_WARN_FLAGS
            -Wshorten-64-to-32
        )
    endif()
endif()

add_executable(compiler
    src/main.cpp
    src/lexer.cpp
    src/parser.cpp
    src/definitions.h
    src/lexer.h
    src/parser.h
    src/symbol_table.h
    src/token.h
    src/token_type.h
    src/syntax/binary_expression.h
    src/syntax/compound_statement.h
    src/syntax/declaration.h
    src/syntax/declaration_reference_expression.h
    src/syntax/expression.h
    src/syntax/function_declaration.h
    src/syntax/literal.h
    src/syntax/parenthesized_expression.h
    src/syntax/primary_expression.h
    src/syntax/return_statement.h
    src/syntax/statement.h
    src/syntax/syntax_node.h
    src/syntax/syntax_type.h
    src/syntax/translation_unit_declaration.h
    src/syntax/variable_declaration.h
)

target_include_directories(compiler
    PRIVATE
    src
)

target_compile_options(compiler PRIVATE ${CCOMPILER_WARN_FLAGS})
