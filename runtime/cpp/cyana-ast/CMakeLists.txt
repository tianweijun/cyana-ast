cmake_minimum_required(VERSION 3.21)
project(cyana_ast)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "D:/Qt/Qt5.9.0/5.9/mingw53_32/lib/cmake")

# 指定头文件搜索路径
include_directories(${PROJECT_SOURCE_DIR}/lib)
# 指定lib目录
link_directories(${PROJECT_SOURCE_DIR}/lib)

find_package(Qt5 COMPONENTS
        Core
        REQUIRED)

add_executable(cyana_ast main.cpp Ast.cpp Ast.h
        AstType.h Token.cpp Token.h Grammar.cpp Grammar.h
        GrammarType.h GrammarAction.h TokenType.h Logger.cpp
        Logger.h CyanaAstRuntimeException.cpp CyanaAstRuntimeException.h
        RuntimeAutomataAstApplication.cpp RuntimeAutomataAstApplication.h
        PersistentAutomataAstApplication.cpp PersistentAutomataAstApplication.h
        PersistentData.cpp PersistentData.h AstGuiOutputer.cpp AstGuiOutputer.h
        StringUtils.cpp StringUtils.h PersistentObject.cpp PersistentObject.h
        DfaTokenAutomata.cpp DfaTokenAutomata.h TokenDfa.cpp TokenDfa.h
        BacktrackingBottomUpAstAutomata.cpp BacktrackingBottomUpAstAutomata.h
        SyntaxDfa.cpp SyntaxDfa.h
        ByteBuffer.cpp ByteBuffer.h)


target_link_libraries(cyana_ast
        Qt5::Core
        cyana-ast-gui-lib.dll -pthread
        )

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/lib"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>")

if (WIN32)
    set(DEBUG_SUFFIX)
    if (CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt5${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
