#pragma once
class Constant
{
public:
	static const unsigned int SCR_WIDTH = 1280;
	static const unsigned int SCR_HEIGHT = 720;
#if defined(__APPLE__)
	static const char* cubeVsPath = "../shaders/cube.vs";
	static const char* cubeFsPath = "../shaders/cube.fs";
	static const char* lightCubeVsPath = "../shaders/light_cube.vs";
	static const char* lightCubeFsPath = "../shaders/light_cube.fs";
	static const char* modelVsPath = "../shaders/model.vs";
	static const char* modelFsPath = "../shaders/model.fs";
	static const char* diffuseMapPath = "../resources/textures/container2.png";
	static const char* specularMapPath = "../resources/textures/container2_specular.png";
	static const char* emissiveMapPath = "../resources/textures/matrix.jpeg";
	static const char* objPath = "../resources/models/nanosuit/nanosuit.obj";
	// const char *imgContainerPath = "../resources/textures/container.jpg";
	// const char *imgAwesomefacePath = "../resources/textures/awesomeface.png";
#else
	static inline const char* cubeVsPath = "../../../shaders/cube.vs";
	static inline const char* cubeFsPath = "../../../shaders/cube.fs";
	static inline const char* lightCubeVsPath = "../../../shaders/light_cube.vs";
	static inline const char* lightCubeFsPath = "../../../shaders/light_cube.fs";
	static inline const char* modelVsPath = "../../../shaders/model.vs";
	static inline const char* modelFsPath = "../../../shaders/model.fs";
	static inline const char* diffuseMapPath = "../../../resources/textures/container2.png";
	static inline const char* specularMapPath = "../../../resources/textures/container2_specular.png";
	static inline const char* emissiveMapPath = "../../../resources/textures/matrix.jpeg";
	static inline const char* objPath = "../../../resources/models/nanosuit/nanosuit.obj";
	// const char *imgContainerPath = "../../../resources/textures/container.jpg";
	// const char *imgAwesomefacePath = "../../../resources/textures/awesomeface.png";
#endif
};