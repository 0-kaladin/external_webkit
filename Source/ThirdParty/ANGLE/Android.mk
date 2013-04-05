#
# Copyright (C) 2011, 2012, Sony Ericsson Mobile Communications AB
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of the Sony Ericsson Mobile Communications AB nor the
#       names of its contributors may be used to endorse or promote products
#       derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL SONY ERICSSON MOBILE COMMUNICATIONS AB BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

# Use -fno-strict-aliasing so it doesn't need to be global
LOCAL_CFLAGS += -fno-strict-aliasing

# Compiler src
LOCAL_SRC_FILES := \
    src/compiler/BuiltInFunctionEmulator.cpp \
    src/compiler/Compiler.cpp \
    src/compiler/DetectDiscontinuity.cpp \
    src/compiler/DetectRecursion.cpp \
    src/compiler/Diagnostics.cpp \
    src/compiler/DirectiveHandler.cpp \
    src/compiler/ForLoopUnroll.cpp \
    src/compiler/InfoSink.cpp \
    src/compiler/Initialize.cpp \
    src/compiler/InitializeDll.cpp \
    src/compiler/InitializeParseContext.cpp \
    src/compiler/IntermTraverse.cpp \
    src/compiler/Intermediate.cpp \
    src/compiler/MapLongVariableNames.cpp \
    src/compiler/ParseHelper.cpp \
    src/compiler/PoolAlloc.cpp \
    src/compiler/QualifierAlive.cpp \
    src/compiler/RemoveTree.cpp \
    src/compiler/ShaderLang.cpp \
    src/compiler/SymbolTable.cpp \
    src/compiler/ValidateLimitations.cpp \
    src/compiler/VariableInfo.cpp \
    src/compiler/VariablePacker.cpp \
    src/compiler/debug.cpp \
    src/compiler/intermOut.cpp \
    src/compiler/ossource_posix.cpp \
    src/compiler/parseConst.cpp \
    src/compiler/util.cpp \
    src/compiler/timing/RestrictFragmentShaderTiming.cpp \
    src/compiler/timing/RestrictVertexShaderTiming.cpp \
    src/compiler/depgraph/DependencyGraph.cpp \
    src/compiler/depgraph/DependencyGraphBuilder.cpp \
    src/compiler/depgraph/DependencyGraphOutput.cpp \
    src/compiler/depgraph/DependencyGraphTraverse.cpp

# Code generator
LOCAL_SRC_FILES := $(LOCAL_SRC_FILES) \
    src/compiler/CodeGenGLSL.cpp \
    src/compiler/OutputESSL.cpp \
    src/compiler/OutputGLSL.cpp \
    src/compiler/OutputGLSLBase.cpp \
    src/compiler/TranslatorESSL.cpp \
    src/compiler/TranslatorGLSL.cpp \
    src/compiler/VersionGLSL.cpp \

# Generated files
LOCAL_SRC_FILES := $(LOCAL_SRC_FILES) \
    src/compiler/glslang_lex.cpp \
    src/compiler/glslang_tab.cpp

# Preprocessor
LOCAL_SRC_FILES := $(LOCAL_SRC_FILES) \
    src/compiler/preprocessor/DiagnosticsBase.cpp \
    src/compiler/preprocessor/DirectiveHandlerBase.cpp \
    src/compiler/preprocessor/DirectiveParser.cpp \
    src/compiler/preprocessor/ExpressionParser.cpp \
    src/compiler/preprocessor/Input.cpp \
    src/compiler/preprocessor/Lexer.cpp \
    src/compiler/preprocessor/Macro.cpp \
    src/compiler/preprocessor/MacroExpander.cpp \
    src/compiler/preprocessor/Preprocessor.cpp \
    src/compiler/preprocessor/Token.cpp \
    src/compiler/preprocessor/Tokenizer.cpp

# 3rd party
LOCAL_SRC_FILES := $(LOCAL_SRC_FILES) \
    src/third_party/compiler/ArrayBoundsClamper.cpp

