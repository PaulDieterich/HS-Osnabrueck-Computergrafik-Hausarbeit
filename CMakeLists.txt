
# Der Inhalt dieser Datei kann in /CMakeLists.txt kopiert werden

# Bibliotheken aus dem VS-Projektorder kopieren und in dem CLion-Projekt in einem Unterordner einfügen -> "\libraries"
# Bibliotheken sind bspw. unter "\CGVStudio\thirdparty" zu finden.

# Alle *.cpp und *.h in einen Unterordner verschieben -> "\source"

cmake_minimum_required(VERSION 3.16)

# TODO: Projektnamen anpassen
project(GC_Projekt)
set(CMAKE_CXX_STANDARD 20)

# TODO: Inhalt des source-Ordners auflisten
add_executable(${CMAKE_PROJECT_NAME}
        main.cpp
        source/vector.h
        source/vector.cpp
        source/Aabb.h
        source/Aabb.cpp
        source/Application.h
        source/Application.cpp
        source/BaseModel.h
        source/BaseModel.cpp
        source/BaseShader.h
        source/BaseShader.cpp
        source/Camera.cpp
        source/Camera.h
        source/color.cpp
        source/color.h
        source/ConstantShader.cpp
        source/ConstantShader.h
        source/Framebuffer.cpp
        source/Framebuffer.h
        source/Weapon.h
        source/Wad.h
        source/Weapon.cpp
        source/Wad.cpp
        source/VertexBuffer.h
        source/VertexBuffer.cpp
        source/TriangleSphereModel.h
        source/TriangleSphereModel.cpp
        source/TrianglePlaneModel.h
        source/TrianglePlaneModel.cpp
        source/TriangleBoxModel.h
        source/TriangleBoxModel.cpp
        source/Texture.h
        source/Texture.cpp
        source/ShadowMapGenerator.h
        source/ShaderLightMapper.h
        source/ShadowMapGenerator.cpp
        source/ShaderLightMapper.cpp
        source/rgbimage.h
        source/rgbimage.cpp
        source/Player.h
        source/Player.cpp
        source/Pickup.h
        source/Pickup.cpp
        source/PhongShader.h
        source/PhongShader.cpp
        source/Model.h
        source/Model.cpp
        source/Matrix.h
        source/Matrix.cpp
        source/Map.h
        source/Map.cpp
        source/LinePlaneModel.h
        source/TriangleSphereModel.cpp
        source/LinePlaneModel.cpp
        source/LineBoxModel.h
        source/LineBoxModel.cpp
        source/Lights.h
        source/Lights.cpp
        source/IndexBuffer.h
        source/IndexBuffer.cpp
        source/Game.h
        source/Game.cpp
        )

include_directories(
        source
        libraries/glfw/include
        libraries/glew/include
        libraries/freeimage/include
        libraries/assimp/include
)

find_package(OpenGL REQUIRED)

target_link_libraries(${CMAKE_PROJECT_NAME}
        OpenGL::GL
        OpenGL::GLU
        ../libraries/glfw/lib/glfw3
        ../libraries/glew/lib/glew32
        ../libraries/freeimage/lib/FreeImage
        ../libraries/assimp/lib/assimp-vc140-mt)

# TODO: dll-Dateien aus den lib-Ordnern in den cmake-build-debug ordner kopieren
# - \libraries\glew\lib\glew32.dll -> \cmake-build-debug\glew32.dll
# - \libraries\glfw\lib\glfw3.dll -> \cmake-build-debug\glfw3.dll
# - \libraries\freeimage\lib\FreeImage.dll -> \cmake-build-debug\FreeImage.dll
# - \libraries\assimp\lib\assimp-vc140-mt.dll -> \cmake-build-debug\assimp-vc140-mt.dll