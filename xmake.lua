set_arch("x86")
add_requires("lua 5.4.*", {configs = {arch = "x86"}})
add_requires("sol2", {configs = {arch = "x86"}})
add_requires("minhook", {configs = {arch = "x86"}})
add_requires("imgui", {configs = {arch = "x86", dx9 = true, win32 = true}})

target("proxydll")
    set_kind("phony")
    
    on_build(function (target)
        import("core.project.config")        
        local mode = "release"
        
        local args = {"build"}
        table.insert(args, "--target")
        table.insert(args, "i686-pc-windows-msvc")
        
        if mode == "release" then
            table.insert(args, "--release")
        end
        
        os.vrunv("cargo", args, {curdir = "proxydll"})                
        os.cp("proxydll/target/i686-pc-windows-msvc/release/d3d9.dll", "build")
    end)
    
    on_clean(function (target)
        os.vrunv("cargo", {"clean"}, {curdir = "proxydll"})
        os.tryrm("build/d3d9.dll")
    end)

target("kaamoclubmodapi")
    set_kind("shared")
    add_files("modapi/src/*.cpp")
    add_files("modapi/src/Game/*.cpp")
    add_includedirs("modapi/include")
    add_packages("lua", "sol2", "minhook", "imgui")
    add_syslinks("user32", "d3d9", "dbghelp", "dinput8", "dxguid")
    set_languages("c++20")
    --set_symbols("debug")
    --set_optimize("none")

    after_build(function (target)
        os.cp(target:targetfile(), "build")
    end)

    on_clean(function (target)
        os.tryrm("build/.deps")
        os.tryrm("build/.objs")
        os.tryrm("build/windows")
        os.tryrm("build/kaamoclubmodapi.dll")
    end)