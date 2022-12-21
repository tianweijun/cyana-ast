cmake_minimum_required(VERSION 3.21)
project(cyana-ast-runtime-lib)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS -fno-exceptions)

SET(CMAKE_BUILD_TYPE "Release")

add_library(cyana-ast-runtime-lib SHARED Ast.cpp Ast.h
        Token.cpp Token.h Grammar.cpp Grammar.h
        GrammarType.h GrammarAction.h TokenType.h Logger.cpp
        Logger.h CyanaAstRuntimeException.cpp CyanaAstRuntimeException.h
        RuntimeAutomataAstApplication.cpp RuntimeAutomataAstApplication.h
        PersistentAutomataAstApplication.cpp PersistentAutomataAstApplication.h
        PersistentData.cpp PersistentData.h
        StringUtils.cpp StringUtils.h PersistentObject.cpp PersistentObject.h
        DfaTokenAutomata.cpp DfaTokenAutomata.h TokenDfa.cpp TokenDfa.h
        BacktrackingBottomUpAstAutomata.cpp BacktrackingBottomUpAstAutomata.h
        SyntaxDfa.cpp SyntaxDfa.h
        ByteBuffer.cpp ByteBuffer.h TokenDfaState.cpp TokenDfaState.h FaStateType.h
        FaStateType.cpp AutomataTmpAst.h AutomataTmpAst.cpp
        SyntaxDfaState.cpp SyntaxDfaState.h ProductionRule.cpp ProductionRule.h
        ByteBufferedInputStream.cpp ByteBufferedInputStream.h TokenType.cpp
        TokenReducingSymbolInputStream.cpp TokenReducingSymbolInputStream.h
        BacktrackingBottomUpBranch.cpp BacktrackingBottomUpBranch.h ReducingSymbol.cpp
        ReducingSymbol.h)