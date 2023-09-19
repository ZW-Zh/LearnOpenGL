add_rules("mode.debug", "mode.release")

target("LearnOpenGL")
    set_kind("binary")
    set_languages("c++17")
    add_includedirs("include")
    add_linkdirs("lib")
    add_links("glfw3")
    if is_plat("windows") then 
        add_syslinks("opengl32","kernel32", "User32", "Gdi32", "Shell32")
        set_runtimes("MTd")
    end 
    add_files("src/hello_window.cpp","src/glad.c")
    