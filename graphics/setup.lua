-- Pre generation

fileName="include/private/specs.h"

specs=[[
#ifndef IMODGP_CONFIG_SPECS
#define IMODGP_CONFIG_SPECS
]]

sessionType=os.getenv("XDG_SESSION_TYPE")
if (sessionType == "wayland") then
    specs=specs.."\n#define XDG_SESSION_TYPE_WAYLAND"

elseif (sessionType =="x11") then
    specs=specs.."\n#define XDG_SESSION_TYPE_X11"
end


-- Checking for GLESv3
reader = io.popen("uname -s")
os_name = reader:read("*a")
reader:close()
libGLESv3 = {
    FreeBSD="/usr/local/lib/libGLESv2.so",
}

libVulkan = {
    FreeBSD="/usr/local/lib/libvulkan.so",
}

function exists(path)

local cmd = string.format([[
#!/bin/sh

if [ -e %s ]; then echo -n 1; exit; fi

echo -n 0;
]], path)

    local reader = io.popen(cmd)
    local value = tonumber(reader:read("*a"))
    reader:close()

    if value == 1 then return true end
    return false
end

if string.match(os_name, "FreeBSD") then

    if exists(libGLESv3.FreeBSD) then
        specs=specs.."\n#define OPENGL_ES3_API_SUPPORTED"
    else
        specs=specs.."\n#define OPENGL_ES3_API_UNSUPPORTED"
    end


    -- TODO: Add vulkan support later
    if exists(libVulkan.FreeBSD) then
    else
    end

    specs=specs.."\n#define VULKAN_API_UNSUPPORTED"
end

specs=specs.."\n#endif"
os.execute("echo ".."'"..specs.."'".." > "..fileName)
