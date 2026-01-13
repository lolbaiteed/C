const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const mode = b.standardOptimizeOption(.{});

    const module = b.addModule("main", .{
        .target = target,
        .optimize = mode,
        .link_libc = true,
    });

    module.addCSourceFiles(.{
        .files = &.{
            "./src/lib/async.c",
            "./test.c",
        },
        .flags = &.{
            "-Wall",
            "-Wextra",
            "-pedantic",
        },
    });

    const exe = b.addExecutable(.{
        .name = "app",
        .root_module = module
    });

    b.installArtifact(exe);
}
