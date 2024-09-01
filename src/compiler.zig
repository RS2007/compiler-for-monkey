const std = @import("std");
const object = @cImport({
    @cInclude("object.h");
});
const lexer = @cImport({
    @cInclude("lexer.h");
});
const parser = @cImport({
    @cInclude("parser.h");
});

const ByteCode = struct {
    instructions: []u8,
    constants: []object.object_t,
};
const Compiler = struct {
    instructions: []u8,
    constants: []object.object_t,
    allocator: std.mem.Allocator,

    const Self = @This();
    pub inline fn init(allocator: std.mem.Allocator) !*Self {
        const compilerObj = try allocator.create(Self);
        compilerObj.allocator = allocator;
        return compilerObj;
    }

    pub fn compile(self: Self) !*ByteCode {
        const byteCode = try self.allocator.create(ByteCode);
        return byteCode;
    }
};

test "Simple alloc" {
    var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
    const allocator = arena.allocator();
    defer arena.deinit();
    const comp = try Compiler.init(allocator);
    const instructions = try allocator.alloc(u8, 10);
    const objects = try allocator.alloc(object.object_t, 10);
    objects[0] = object.object_t{
        .type = null,
        .inspect = null,
        .refcount = 69,
    };
    comp.instructions = instructions;
    comp.constants = objects;
    std.log.warn("{d}", .{comp.constants[0].refcount});
}

test "Integer arithmetic" {
    const buffer = @as([]u8, @constCast("1+2"));
    var l = lexer.new_lexer(buffer.ptr, buffer.len);
    _ = parser.new_parser(@as([*c]lexer.lexer_t, l));
    std.log.warn("{s}", .{lexer.next_token_lexer(l).*.literal});
    std.log.warn("{s}", .{lexer.next_token_lexer(l).*.literal});
    std.log.warn("{s}", .{lexer.next_token_lexer(l).*.literal});
}
