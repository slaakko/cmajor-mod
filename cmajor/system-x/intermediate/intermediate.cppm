// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.intermediate;

export import cmajor.systemx.intermediate.code;
export import cmajor.systemx.intermediate.codegen;
export import cmajor.systemx.intermediate.compile.unit;
export import cmajor.systemx.intermediate.compute.succ.pred.pass;
export import cmajor.systemx.intermediate.context;
export import cmajor.systemx.intermediate.data;
export import cmajor.systemx.intermediate.dominator.tree.cooper;
export import cmajor.systemx.intermediate.dominator.tree.pass;
export import cmajor.systemx.intermediate.entry.exit.pass;
export import cmajor.systemx.intermediate.error;
export import cmajor.systemx.intermediate.function.pass.driver;
export import cmajor.systemx.intermediate.init.done;
export import cmajor.systemx.intermediate.code.lexer;
export import cmajor.systemx.intermediate.code.parser;
export import cmajor.systemx.intermediate.code.token;
export import cmajor.systemx.intermediate.code.parser.rules;
export import cmajor.systemx.intermediate.linear.scan.reg.allocator;
export import cmajor.systemx.intermediate.metadata;
export import cmajor.systemx.intermediate.module_pass_driver;
export import cmajor.systemx.intermediate.opt.codegen;
export import cmajor.systemx.intermediate.parser;
export import cmajor.systemx.intermediate.pass;
export import cmajor.systemx.intermediate.pass.manager;
export import cmajor.systemx.intermediate.passes;
export import cmajor.systemx.intermediate.reg.allocator;
export import cmajor.systemx.intermediate.simple.assembly.code.generator;
export import cmajor.systemx.intermediate.simple.assembly.codegen.pass;
export import cmajor.systemx.intermediate.simplify.cfg.pass;
export import cmajor.systemx.intermediate.types;
export import cmajor.systemx.intermediate.util;
export import cmajor.systemx.intermediate.verify;
export import cmajor.systemx.intermediate.visit;
export import cmajor.systemx.intermediate.visitor;
export import cmajor.systemx.intermediate.write.cfg.pass;
export import cmajor.systemx.intermediate.write.dominator.tree.pass;
export import cmajor.systemx.intermediate.write.pass;
export import cmajor.systemx.intermediate.writer;
