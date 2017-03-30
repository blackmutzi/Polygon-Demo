TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

unix {

QMAKE_CXXFLAGS += -std=c++11 -Wdeprecated-declarations


LIBS += -llz4 -lglfw -lGL -lglut -lGLU -lXxf86vm -lXext -lX11 -lfreetype -lfreeimage -lboost_system -lboost_thread-mt
LIBS += -lpolycomponent
LIBS += -lpolygon

#Polygon Component - includes
INCLUDEPATH += /usr/include/polygon-component/

#Polygon Engine - includes
INCLUDEPATH += /usr/include/polygon-engine/
INCLUDEPATH += /usr/include/polygon-engine/Editor/Addons/
INCLUDEPATH += /usr/include/polygon-engine/Editor/
INCLUDEPATH += /usr/include/polygon-engine/Container/
INCLUDEPATH += /usr/include/polygon-engine/Component/
INCLUDEPATH += /usr/include/polygon-engine/Manager/
INCLUDEPATH += /usr/include/polygon-engine/Material/
INCLUDEPATH += /usr/include/polygon-engine/GPU/
INCLUDEPATH += /usr/include/polygon-engine/Render/
INCLUDEPATH += /usr/include/polygon-engine/Interface/
INCLUDEPATH += /usr/include/polygon-engine/Render/Modul/
INCLUDEPATH += /usr/include/polygon-engine/FileLoader/


#Self includes
INCLUDEPATH += $$PWD/include/

#Linux Includes
INCLUDEPATH += /usr/include/OIS/
INCLUDEPATH += /usr/include/freetype2/

}

SOURCES += main.cpp \
           Entitys/AnimationEntity.cpp \
           Entitys/Area.cpp \
           Entitys/Block.cpp \
           Entitys/Blockterrain.cpp \
           Entitys/Cube.cpp \
           Entitys/CubeGenerator.cpp \
           Entitys/HeightMapArea.cpp \
           Entitys/Human.cpp \
           Entitys/Sphere.cpp \
           Entitys/Terrain.cpp \
           Scene/TestScene.cpp \
    Manager/ResourceManager.cpp \
    Container/Resource.cpp \
    Technique/SpotLightTech.cpp \
    Frustum.cpp \
    Technique/CascadedShadowTech.cpp \
    Entitys/FrustumEntity.cpp \
    Technique/ShadowMappingTech.cpp \
    Entitys/DisplaceTerrain.cpp \
    Entitys/TerrainGen.cpp \
    Technique/ScreenSpaceReflection.cpp \
    Entitys/Waterterrain.cpp \
    Technique/FresnelTech.cpp \
    Modul/BasicShadowModul.cpp \
    Manager/ContextManager.cpp \
    Container/ContextContainerTest.cpp \
    Modul/DeformationModul.cpp \
    Manager/DeformationManager.cpp \
    Container/Deformation.cpp


HEADERS += \
    include/animationentity.h \
    include/area.h  \
    include/block.h \
    include/blockterrain.h \
    include/cube.h \
    include/cubegenerator.h \
    include/heightmaparea.h \
    include/human.h \
    include/sphere.h \
    include/terrain.h \
    include/testscene.h \
    include/resourcemanager.h \
    include/resource.h \
    include/spotlighttech.h \
    include/frustum.h \
    include/cascadedshadowtech.h \
    include/frustumentity.h \
    include/shadowmappingtech.h \
    include/displaceterrain.h \
    include/terraingen.h \
    include/screenspacereflection.h \
    include/waterterrain.h \
    include/fresneltech.h \
    include/basicshadowmodul.h \
    include/contextmanager.h \
    include/contextcontainertest.h \
    include/deformationmodul.h \
    include/deformationmanager.h \
    include/deformation.h
