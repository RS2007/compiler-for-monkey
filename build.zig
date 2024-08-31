const std = @import("std");
const builtin = @import("builtin");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const exe = b.addExecutable(.{
        .name = "mc",
        .target = target,
        .optimize = optimize,
    });

    // Link libc
    exe.linkLibC();

    // Compile the Zig file
    const vm_zig = b.addObject(.{
        .name = "vm",
        .root_source_file = .{ .path = "src/vm.zig" },
        .target = target,
        .optimize = optimize,
    });

    // Add the compiled Zig object to the executable
    exe.addObject(vm_zig);

    const src_dir = "src";
    var dir = std.fs.cwd().openIterableDir(src_dir, .{}) catch @panic("Failed to open src directory");
    defer dir.close();

    var walker = dir.walk(b.allocator) catch @panic("Failed to create directory walker");
    defer walker.deinit();

    var object_files = std.ArrayList(*std.Build.CompileStep).init(b.allocator);
    defer object_files.deinit();

    // Define common C flags
    const c_flags = [_][]const u8{};

    while (walker.next() catch @panic("Failed to iterate over directory")) |entry| {
        if (entry.kind == .file and std.mem.endsWith(u8, entry.basename, ".c")) {
            const full_path = b.pathJoin(&.{ src_dir, entry.path });

            // Create a separate compile step for each .c file
            const obj = b.addObject(.{
                .name = std.mem.concat(b.allocator, u8, &.{ entry.basename, ".o" }) catch @panic("Failed to create object name"),
                .target = target,
                .optimize = optimize,
            });
            obj.addCSourceFile(.{ .file = .{ .path = full_path }, .flags = &c_flags });
            obj.linkLibC();

            // Add include path for vm.h in the root directory
            obj.addIncludePath(.{ .path = "." });

            // Add the object file to our list
            object_files.append(obj) catch @panic("Failed to append object file");
        }
    }

    // Add all object files to the executable
    for (object_files.items) |obj| {
        exe.addObject(obj);
    }

    // Add the same C flags to the executable
    exe.addCSourceFiles(&.{}, &c_flags);

    b.installArtifact(exe);

    const run_cmd = b.addRunArtifact(exe);
    run_cmd.step.dependOn(b.getInstallStep());
    if (b.args) |args| {
        run_cmd.addArgs(args);
    }

    const run_step = b.step("run", "Run the app");
    run_step.dependOn(&run_cmd.step);
}
