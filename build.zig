const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    // Add a command-line option to choose the build function
    const build_mode = b.option(
        []const u8,
        "mode",
        "Choose build mode: default or alternative",
    ) orelse "default";

    // Call the appropriate build function based on the mode
    if (std.mem.eql(u8, build_mode, "default")) {
        defaultBuild(b, target, optimize);
    } else if (std.mem.eql(u8, build_mode, "alternative")) {
        alternativeBuild(b, target, optimize);
    } else {
        std.debug.print("Unknown build mode: {s}\n", .{build_mode});
        std.process.exit(1);
    }
}

fn defaultBuild(b: *std.Build, target: std.zig.CrossTarget, optimize: std.builtin.OptimizeMode) void {
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

fn alternativeBuild(b: *std.Build, target: std.zig.CrossTarget, optimize: std.builtin.OptimizeMode) void {
    const test_step = b.step("test", "Run alternative unit tests");
    std.log.warn("Running alternative", .{});

    const tests = b.addTest(.{
        .root_source_file = .{ .path = "src/compiler.zig" },
        .target = target,
        .optimize = optimize,
    });
    tests.addIncludePath(.{ .path = "./src" });
    tests.addCSourceFiles(&[_][]const u8{ "src/lexer.c", "src/object.c", "src/utils.c", "src/token.c", "src/parser.c" }, &[_][]const u8{});
    tests.linkLibC();

    const run_tests = b.addRunArtifact(tests);
    run_tests.has_side_effects = true;
    test_step.dependOn(&run_tests.step);
}
