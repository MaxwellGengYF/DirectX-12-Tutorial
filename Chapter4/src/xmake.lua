BuildProject({
    projectName = "DX12_BetterSimpleBox",
    projectType = "binary",
    macros = {"_XM_NO_INTRINSICS_=1", "NOMINMAX", "UNICODE", "m128_f32=vector4_f32", "m128_u32=vector4_u32"},
    debugMacros = {"_DEBUG"},
    releaseMacros = {"NDEBUG"},
    files = {"**.cpp"},
    includePaths = {"./"},
    debugException = true,
    releaseException = true
})
add_links("User32", "kernel32", "Gdi32", "Shell32", "DXGI", "D3D12", "D3DCompiler")
after_build(function(target)
    build_path = ""
    src_path = "shader/"
    if is_mode("release") then
        build_path = "$(buildir)/windows/x64/release/"
    else
        build_path = "$(buildir)/windows/x64/debug/"
    end
    os.cp(src_path .. "*", build_path .. "shader/")
end)
