add_rules("mode.release", "mode.debug")
function Execute(map, func)
    if map ~= nil then
        func(map)
    end
end
function SetException(enableException)
    if (enableException ~= nil) and (enableException) then
        add_cxflags("/EHsc", {
            force = true
        })
    end
end
function BuildProject(config)
    target(config.projectName)
    
    set_languages("clatest")
    set_languages("cxx20")
    projectType = config.projectType
    if projectType ~= nil then
        set_kind(projectType)
    end
    Execute(config.macros, add_defines)
    Execute(config.files, add_files)
    Execute(config.includePaths, add_includedirs)
    Execute(config.depends, add_deps)
    unityBuildBatch = config.unityBuildBatch
    if (unityBuildBatch ~= nil) and (unityBuildBatch > 0) then
        add_rules("c.unity_build", {
            batchsize = unityBuildBatch
        })
        add_rules("c++.unity_build", {
            batchsize = unityBuildBatch
        })
    end
    if is_mode("release") then
        set_optimize("aggressive")
        if is_plat("windows") then
            set_runtimes("MD")
            add_cxflags("/Zi", "/W0", "/MP", "/Ob2", "/Oi", "/Ot", "/Oy", "/GT", "/GF", "/GS-", "/Gy", "/arch:AVX2",
                "/Gr", "/sdl-", "/GL", "/Zc:preprocessor", "/Gr", "/TP", {
                    force = true
                })
            SetException(config.releaseException)
        end
        Execute(config.releaseMacros, add_defines)
    else
        set_optimize("none")
        if is_plat("windows") then
            set_runtimes("MDd")
            add_cxflags("/Zi", "/W0", "/MP", "/Ob0", "/Oy-", "/GF", "/GS", "/arch:AVX2", "/Gr", "/TP", "/Gr",
                "/Zc:preprocessor", {
                    force = true
                })
            SetException(config.debugException)
        end
        Execute(config.debugMacros, add_defines)
    end
end
add_subdirs("src/")
