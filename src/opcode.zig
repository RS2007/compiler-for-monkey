const std = @import("std");

const opCodeType = enum {
    opConstant,
};

const Declaration = struct {
    name: []u8,
    operandWidths: []u32,
};

pub fn getDef(allocator: std.mem.Allocator, byte: u8) !*Declaration {
    const opType: opCodeType = @enumFromInt(byte);
    var decl = try allocator.create(Declaration);
    switch (opType) {
        .opConstant => {
            decl.* = Declaration{
                .name = @as([]u8, @constCast("CONSTANT")),
                .operandWidths = @constCast(&[_]u32{2}),
            };
        },
    }
    return decl;
}

fn make(allocator: std.mem.Allocator, op: opCodeType, operands: []u32) ![]u8 {
    const def = try getDef(allocator, @intFromEnum(op));
    var instrLen: u32 = 1;
    for (def.operandWidths) |w| {
        instrLen += w;
    }
    var offset: u32 = 1;
    var instr = try allocator.alloc(u8, instrLen);
    instr[0] = @intFromEnum(op);
    for (operands, 0..) |operand, i| {
        const width = def.operandWidths[i];
        switch (width) {
            2 => {
                var bytes = @as([2]u8, @bitCast(@as(u16, @intCast(operand))));
                std.mem.reverse(u8, &bytes);
                @memcpy(instr[offset .. offset + 2], &bytes);
                offset += width;
            },
            else => {
                unreachable;
            },
        }
    }
    return instr;
}

test "Basic make instruction" {
    var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
    const allocator = arena.allocator();
    defer arena.deinit();
    const testStruct = struct {
        opCode: opCodeType,
        operands: []u32,
        expected: []u8,
    };
    const tests: []testStruct = @constCast(&[_]testStruct{
        testStruct{
            .opCode = opCodeType.opConstant,
            .operands = @constCast(&[_]u32{65534}),
            .expected = @constCast(&[_]u8{ @intFromEnum(opCodeType.opConstant), 255, 254 }),
        },
    });
    for (tests) |t| {
        const instr = try make(allocator, t.opCode, t.operands);
        _ = try std.testing.expectEqual(instr.len, t.expected.len);
        for (instr, 0..) |byte, i| {
            _ = try std.testing.expectEqual(byte, t.expected[i]);
        }
    }
    //{OpConstant, []int { 65534 }, [] byte { byte (OpConstant), 255 , 254 }},
}
