set_policy("build.ccache", false)
add_rules("mode.release", "mode.debug")
function GetValue(funcOrValue)
    if type(funcOrValue) == 'function' then
        return funcOrValue()
    else
        return funcOrValue
    end
end

function SetException(enableException)
    local value = GetValue(enableException)
    if (value ~= nil) and value then
        add_cxflags("/EHsc", {
            force = true
        })
    end
end
function BuildProject(config)
    local projectName = GetValue(config.projectName)
    if projectName == nil then
        return
    end
    target(projectName)
    set_languages("clatest")
    set_languages("cxx20")
    local projectType = GetValue(config.projectType)
    if projectType ~= nil then
        set_kind(projectType)
    end
    local unityBuildBatch = GetValue(config.unityBuildBatch)
    if (unityBuildBatch ~= nil) and (unityBuildBatch > 0) then
        add_rules("c.unity_build", {
            batchsize = unityBuildBatch
        })
        add_rules("c++.unity_build", {
            batchsize = unityBuildBatch
        })
    end
    SetException(config.exception)
    if is_mode("release") then
        set_optimize("aggressive")
        if is_plat("windows") then
            set_runtimes("MD")
        end
        add_cxflags("/Zi", "/W0", "/Ob2", "/Oi", "/Ot", "/Oy", "/GT", "/GF", "/GS-", "/Gy", "/arch:AVX2", "/Gd",
            "/sdl-", "/GL", "/Zc:preprocessor", "/TP", {
                force = true
            })
        local event = GetValue(config.releaseEvent)
        if (type(event) == "function") then
            event()
        end
    else
        set_optimize("none")
        if is_plat("windows") then
            set_runtimes("MDd")
        end
        add_cxflags("/Zi", "/W0", "/Ob0", "/Oy-", "/GF", "/GS", "/arch:AVX2", "/TP", "/Gd", "/Zc:preprocessor", {
            force = true
        })
        local event = GetValue(config.debugEvent)
        if (type(event) == "function") then
            event()
        end
    end
    local event = GetValue(config.event)
    if (type(event) == "function") then
        event()
    end
end
add_subdirs("src/")
