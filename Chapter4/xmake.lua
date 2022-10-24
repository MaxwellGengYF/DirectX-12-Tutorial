set_config("toolchain", "clang-cl")
add_rules("mode.release", "mode.debug")
option("is_clang")
add_csnippets("is_clang", "return (__clang__)?0:-1;", {
	tryrun = true
})
option_end()
option("is_msvc")
add_csnippets("is_msvc", "return (_MSC_VER)?0:-1;", {
	tryrun = true
})
option_end()


option_end()
function GetValue(funcOrValue)
	if type(funcOrValue) == 'function' then
		return funcOrValue()
	else
		return funcOrValue
	end
end

--[[
    BuildProject({
        projectName = xxx,
        projectType = xxx,
        unityBuildBatch = n,
        exception = true/false,
        releaseEvent = function()
        end,
        debugEvent = function()
        end
    })
]]
if is_mode("debug") then
	set_targetdir("bin/debug")
else
	set_targetdir("bin/release")
end

function BuildProject(config)
	local projectName = GetValue(config.projectName)
	if projectName == nil then
		return
	end
	target(projectName)
	set_languages("clatest", "cxx20")
	local projectType = GetValue(config.projectType)
	if projectType ~= nil then
		set_kind(projectType)
	end
	if UseUnityBuild then
		local unityBuildBatch = GetValue(config.unityBuildBatch)
		if (unityBuildBatch ~= nil) and (unityBuildBatch > 1) then
			add_rules("c.unity_build", {
				batchsize = unityBuildBatch
			})
			add_rules("c++.unity_build", {
				batchsize = unityBuildBatch
			})
		end
	end
	local value = GetValue(config.exception)
	if (value ~= nil) and value then
		if has_config("is_msvc") then
			add_cxflags("/EHsc")
		else
			add_cxflags("-fexceptions")
		end
	elseif not has_config("is_msvc") then
		add_cxflags("-fno-exceptions")
	end
	set_warnings("none")
	if is_mode("debug") then
		set_optimize("none")
		if is_plat("windows") then
			set_runtimes("MDd")
		end
		if has_config("is_msvc") then
			add_cxflags("/GS", "/Gd");
			-- Not Clang-cl
			if not has_config("is_clang") then
				add_cxflags("/Zc:preprocessor")
			end
		end
		local event = GetValue(config.debugEvent)
		if (type(event) == "function") then
			event()
		end
	else
		set_optimize("fastest")
		if is_plat("windows") then
			set_runtimes("MD")
		end
		if has_config("is_msvc") then
			add_cxflags("/Oy", "/GS-", "/Gd", "/Oi", "/Ot", "/GT", "/Ob2")
			-- Not Clang-cl
			if not has_config("is_clang") then
				add_cxflags("/GL", "/Zc:preprocessor", "/QIntel-jcc-erratum")
			end
		end
		local event = GetValue(config.releaseEvent)
		if (type(event) == "function") then
			event()
		end
	end
end
add_subdirs("src")
