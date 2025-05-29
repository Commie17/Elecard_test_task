cmake_minimum_required(VERSION 3.10)
project(YUVOverlayProcessor)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Установка исходников
set(SOURCES
    main.cpp
    overlayProcessor.cpp
    converter.cpp
    imageBmp.cpp
    videoYuv420Reader.cpp
    videoYuv420Writer.cpp
)

# Заголовочные файлы
set(HEADERS
    overlayProcessor.h
    converter.h
    dataContainers.h
    imageBmp.h
    videoYuv420Reader.h
    videoYuv420Writer.h
)

# Создание исполняемого файла
add_executable(yuv_overlay ${SOURCES} ${HEADERS})

