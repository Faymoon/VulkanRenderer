function includeVulkan() -- Little function that include Vulkan if LunarG's SDK is installed by MacGamerFR
    filter "system:Windows"
        -- LunarG's Vulkan SDK set a Windows environment variable, so use it
        local envVulkanSDKPath = os.getenv("VK_SDK_PATH")
   
        if (envVulkanSDKPath) then
            local vulkanSDKPath = envVulkanSDKPath:gsub("\\", "/")
            print("Found Vulkan librarie : " .. vulkanSDKPath)
            includedirs { vulkanSDKPath .. "/Include" }
            libdirs { vulkanSDKPath .. "/Lib" }
        else
            print("Vulkan SDK not found !")
        end
    filter {}
end

workspace "VulkanRenderer"
    configurations { "Release", "Debug" }
    location "build/"

project "VulkanRenderer"

    kind "WindowedApp"

    files {
        "include/*.hpp",
        "include/*.inl",
        "src/*.cpp"
    }

    filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"
		targetsuffix "-d"

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"

	filter "platforms:x86"
		architecture "x86"

	filter "platforms:x64"
		architecture "x86_64"

    includeVulkan()

    includedirs { "C:/glfw/include", "C:/glm", "./include"}
    libdirs{ "C:/glfw/lib-vc2015" }

    links { 
        "vulkan-1",
        "glfw3"
     }
