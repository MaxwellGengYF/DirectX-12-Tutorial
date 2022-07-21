BuildProject({
    projectName = "DX12_Triangle",
    projectType = "binary",
    event = function()
        add_defines("_XM_NO_INTRINSICS_=1", "NOMINMAX", "UNICODE", "m128_f32=vector4_f32", "m128_u32=vector4_u32")
        add_files("**.cpp")
        add_includedirs("./")
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

    end,
    debugEvent = function()
        add_defines("_DEBUG")
    end,
    releaseEvent = function()
        add_defines("NDEBUG")
    end,
    exception = true
})
