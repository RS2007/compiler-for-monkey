const std = @import("std");
const c = @cImport({
    @cInclude("object.h");
    @cInclude("lexer.h");
    @cInclude("parser.h");
});

const ByteCode = struct {
    instructions: std.mem.ArrayList(u8),
    constants: std.mem.ArrayList(c.object_t),

    pub fn disas(allocator: std.mem.Allocator) ![]u8 {
        _ = allocator;
        return "bruh";
    }
};
const Compiler = struct {
    instructions: std.mem.ArrayList(u8),
    constants: std.mem.ArrayList(c.object_t),
    allocator: std.mem.Allocator,

    const Self = @This();
    pub inline fn init(allocator: std.mem.Allocator) !*Self {
        const compilerObj = try allocator.create(Self);
        compilerObj.allocator = allocator;
        return compilerObj;
    }

    fn compileExpression(self: *Self, expression: [*c]c.expression_t) !*ByteCode {
        const bytecode = try self.allocator.create(ByteCode);
        switch (expression.*.type) {
            0 => {
                var integerExpr = @as([*c]c.integer_t, expression);
                var object = try self.allocator.create(c.integer_obj_t);
                object.* = c.integer_obj_t{
                    .object = c.object_t{
                        .type = null,
                        .inspect = null,
                        .refcount = 1,
                    },
                    .value = integerExpr.value,
                };
                try bytecode.constants.append(integerExpr.value);
            },
            4 => {
                var binaryExpr = @as([*c]c.infix_expression_t, expression);
                var lhsByteCode = try compileExpression(binaryExpr.*.left);
                var rhsByteCode = try compileExpression(binaryExpr.*.right);
                try bytecode.instructions.append();
            },
            else => {
                unreachable;
            },
        }
    }
    pub fn compile(self: *Self, program: [*c]c.program_t) !*ByteCode {
        const byteCode = try self.allocator.create(ByteCode);
        for (program.statements) |stmt| {
            var castedStmt = @as([*c]c.statement_t, stmt);
            switch (castedStmt.*.type) {
                3 => {
                    var exprStmt = @as([*c]c.expression_statement_t, castedStmt);
                    self.compileExpression(@as([*c]c.expression_t, exprStmt.expression));
                },
                else => {
                    unreachable;
                },
            }
        }
        return byteCode;
    }
};

test "Simple alloc" {
    var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
    const allocator = arena.allocator();
    defer arena.deinit();
    const comp = try Compiler.init(allocator);
    const instructions = try allocator.alloc(u8, 10);
    const objects = try allocator.alloc(c.object_t, 10);
    objects[0] = c.object_t{
        .type = null,
        .inspect = null,
        .refcount = 69,
    };
    comp.instructions = instructions;
    comp.constants = objects;
    std.log.warn("{d}", .{comp.constants[0].refcount});
}

test "Integer arithmetic" {
    var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
    const allocator = arena.allocator();
    defer arena.deinit();
    const buffer = @as([]u8, @constCast("1+2"));
    var l = c.new_lexer(buffer.ptr, buffer.len);
    var p = c.new_parser(@as([*c]c.lexer_t, l));
    var program = @as([*c]c.program_t, c.parse_program(@as([*c]c.parser_t, p)));
    var compiler = try Compiler.init(allocator);
    var bytecode = try compiler.compile(program);
    std.log.warn("{s}", .{bytecode.disas(allocator)});
    std.log.warn("{s}", .{c.next_token_lexer(l).*.literal});
    std.log.warn("{s}", .{c.next_token_lexer(l).*.literal});
    std.log.warn("{s}", .{c.next_token_lexer(l).*.literal});
}
