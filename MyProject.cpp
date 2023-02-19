// This has been adapted from the Vulkan tutorial

#include "MyProject.hpp"
using namespace std;
#include "Coordinate.cpp"
#define FIRST_OBJECTS_SIZE 8
#define MAX_TEXTURE_NAME_SIZE 50

const std::string MODEL_PATH = "models/prova1.obj";
const std::string TEXTURE_PATH = "textures/walls.jpg";
const std::string MODEL_PATH1 = "models/quadro3.obj";
vector<Coordinate> objects_coordinates;
vector<string> objects_textures_path;
vector<Coordinate> descriptions_coordinates;
vector<string> descriptions_textures_path;
const std::string MODEL_PATHTERRAIN = "models/terrain.obj";
const std::string TEXTURE_PATHTERRAIN = "textures/terrain.png";
const std::string MODEL_PATHFLOOR = "models/Floor.obj";
const std::string TEXTURE_PATHFLOOR = "textures/floor.png";
const std::string skyt_path = "textures/uv.png";
const std::string skym_path = "models/skybox1.obj";

// The uniform buffer object used in this example
struct globalUniformBufferObject {
	
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
	alignas(16) glm::vec3 lightPos[8];
	alignas(16) glm::vec3 lightColor;
	alignas(16) glm::vec2 coneInOutDecayExp;
	alignas(16) glm::vec3 EyePos;
	alignas(16) glm::vec3 AmbColor;
	alignas(16) glm::vec3 DzColor;
	alignas(16) glm::vec3 DyColor;
	alignas(16) glm::vec3 DxColor;
	
};

struct UniformBufferObject {
	alignas(16) glm::mat4 model;
};

//Questo commento è per testare GitHub
// MAIN ! 
class MyProject : public BaseProject {
protected:
	// Here you list all the Vulkan objects you need:

	// Descriptor Layouts [what will be passed to the shaders]
	DescriptorSetLayout DSLGlobal;
	DescriptorSetLayout DSLObj;
	DescriptorSet DSGlobal;
	// Pipelines [Shader couples]
	Pipeline P1;



	// Models, textures and Descriptors (values assigned to the uniforms)
	Model M1;
	Texture T1;
	DescriptorSet DS1; //instance DSLobj


	Model M2;
	//L'inizializzazione statica DEVE ESSERE CORRETTA, SI DEVE USARE QUELLA DINAMICA
	vector<Texture> TPicture;
	vector<DescriptorSet> DSPicture; //instance DSLobj
	//per ogni quadro dobbiamo aggiungere un descriptor set e siccome cambia la texture anche la texture


	Model MTerrain;
	Texture TTerrain;
	DescriptorSet DSTerrain;

	Model MFloor;
	Texture TFloor;
	DescriptorSet DSFloor;

	vector<Texture> TDesc;
	vector<DescriptorSet> DSDesc;

	// Here you set the main application parameters
	void setWindowParameters() {
		// window size, titile and initial background
		windowWidth = 1200;
		windowHeight = 800;
		windowTitle = "My Project";
		initialBackgroundColor = { 0.68f, 0.8f, 1.0f, 1.0f };

		// Descriptor pool sizes
		uniformBlocksInPool = 4 + objects_coordinates.size() + descriptions_coordinates.size();
		cout << 4+objects_coordinates.size() << "\t" << objects_textures_path.size() << "\t" << descriptions_coordinates.size() << "\t" << descriptions_textures_path.size() << "\n";
		texturesInPool = 3 + objects_textures_path.size() + descriptions_textures_path.size();
		setsInPool = 4 + objects_coordinates.size() + descriptions_coordinates.size();

		// Make the window resizable
		
	}

	// Here you load and setup all your Vulkan objects
	void localInit() {
		// Descriptor Layouts [what will be passed to the shaders]

		DSLObj.init(this, {
			{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT},
			{1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}
			});

		DSLGlobal.init(this, {
			{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS}
			});

		P1.init(this, "shaders/vert.spv", "shaders/frag.spv", { &DSLGlobal, &DSLObj });
		// Models, textures and Descriptors (values assigned to the uniforms)
		M1.init(this, MODEL_PATH);
		T1.init(this, TEXTURE_PATH);
		DS1.init(this, &DSLObj, {

					{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
					{1, TEXTURE, 0, &T1}
			});

		//questo è solo per il primo quadro, M2 è uno, T2 è quanti quadri e DS è quanti quadri
		M2.init(this, MODEL_PATH1);
		TPicture.resize(objects_textures_path.size());
		DSPicture.resize(objects_textures_path.size());
		for (int i = 0; i < objects_textures_path.size(); i++) {
			TPicture[i].init(this, objects_textures_path[i]);
			DSPicture[i].init(this, &DSLObj, {
						{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
						{1, TEXTURE, 0, &TPicture[i]}
				});
		}

		//Terrain
		MTerrain.init(this, MODEL_PATHTERRAIN);
		TTerrain.init(this, TEXTURE_PATHTERRAIN);
		DSTerrain.init(this, &DSLObj, {

					{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
					{1, TEXTURE, 0, &TTerrain}
			});

		//FLOOR
		MFloor.init(this, MODEL_PATHFLOOR);
		TFloor.init(this, TEXTURE_PATHFLOOR);
		DSFloor.init(this, &DSLObj, {
						{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
						{1, TEXTURE, 0, &TFloor}
			});

		//DESCRIPTION
		TDesc.resize(descriptions_textures_path.size());
		DSDesc.resize(descriptions_textures_path.size());
		for (int i = 0; i < descriptions_textures_path.size(); i++) {
			TDesc[i].init(this, descriptions_textures_path[i]);
			DSDesc[i].init(this, &DSLObj, {
						{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
						{1, TEXTURE, 0, &TDesc[i]}
				});
		}

		DSGlobal.init(this, &DSLGlobal, {
					{0, UNIFORM, sizeof(globalUniformBufferObject), nullptr}
			});



	}

	// Here you destroy all the objects you created!		
	void localCleanup() {
		P1.cleanup();
		DSLGlobal.cleanup();
		DSLObj.cleanup();

		DS1.cleanup();
		T1.cleanup();
		M1.cleanup();

		for (int i = 0; i < TPicture.size(); i++) {
			TPicture[i].cleanup();
			DSPicture[i].cleanup();
		}
		M2.cleanup();

		DSTerrain.cleanup();
		TTerrain.cleanup();
		MTerrain.cleanup();

		DSFloor.cleanup();
		TFloor.cleanup();
		MFloor.cleanup();
		for (int i = 0; i < TDesc.size(); i++) {
			DSDesc[i].cleanup();
			TDesc[i].cleanup();
		}
	}

	int enableDesc(glm::vec3 campos, Coordinate coordinate) {
		glm::vec3 c(1.2f, 1.2f, 1.2f);
		glm::vec3 diff = campos - coordinate.getPos();
		if (glm::length(diff) < glm::length(c))
			return 1;
	}


bool isXDoorOpen(glm::vec3 CamPos) {
	return (CamPos.x <= -5.9339f && CamPos.x >= -6.4339) ||
		(CamPos.x >= 0.21609f && CamPos.x <= 0.71609f);
}

bool isZDoorOpen(glm::vec3 CamPos) {
	return (CamPos.z >= 1.63f && CamPos.z <= 2.13f) ||
		(CamPos.z >= -0.4114f && CamPos.z <= 0.0866);
}

bool detectCollision(glm::vec3 campos) {
	const float threshold = 0.1f;
	const std::vector<std::vector<float>> wallPositions = {
		{ -6.9f, -4.9080f, -2.8579f, -0.80891f, 1.2507f },
		{ 3.1456f, 1.1131f, -0.95383f }
	};

	for (const auto& positions : wallPositions) {
		for (float pos : positions) {
			if (std::abs(campos.x - pos) < threshold && !isZDoorOpen(campos)) {
				return true;
			}
			else if (std::abs(campos.z - pos) < threshold && !isXDoorOpen(campos)) {
				return true;
			}
		}
	}

	return false;
}

	// Here it is the creation of the command buffer:
	// You send to the GPU all the objects you want to draw,
	// with their buffers and textures
	void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage) {
				
		//VIEW
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
				P1.graphicsPipeline);
		vkCmdBindDescriptorSets(commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			P1.pipelineLayout, 0, 1, &DSGlobal.descriptorSets[currentImage],
			0, nullptr);

		//MUSEO
		VkBuffer vertexBuffers[] = {M1.vertexBuffer};
		VkDeviceSize offsets[] = {0};
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
		vkCmdBindIndexBuffer(commandBuffer, M1.indexBuffer, 0,
								VK_INDEX_TYPE_UINT32);
		vkCmdBindDescriptorSets(commandBuffer,
						VK_PIPELINE_BIND_POINT_GRAPHICS,
						P1.pipelineLayout, 1, 1, &DS1.descriptorSets[currentImage],
						0, nullptr);		
		vkCmdDrawIndexed(commandBuffer,
					static_cast<uint32_t>(M1.indices.size()), 1, 0, 0, 0);


		//QUADRO 
		
			VkBuffer vertexBuffers2[] = { M2.vertexBuffer };
			VkDeviceSize offsets2[] = { 0 };
			vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers2, offsets2);
			vkCmdBindIndexBuffer(commandBuffer, M2.indexBuffer, 0,
				VK_INDEX_TYPE_UINT32);
			for (int i = 0; i < DSPicture.size(); i++) {
				
				vkCmdBindDescriptorSets(commandBuffer,
					VK_PIPELINE_BIND_POINT_GRAPHICS,
					P1.pipelineLayout, 1, 1, &DSPicture[i].descriptorSets[currentImage],
					0, nullptr);
				vkCmdDrawIndexed(commandBuffer,
					static_cast<uint32_t>(M2.indices.size()), 1, 0, 0, 0);
			}
		

		//TERRAIN

		VkBuffer vertexBuffers3[] = { MTerrain.vertexBuffer };
		VkDeviceSize offsets3[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers3, offsets3);
		vkCmdBindIndexBuffer(commandBuffer, MTerrain.indexBuffer, 0,
			VK_INDEX_TYPE_UINT32);
		vkCmdBindDescriptorSets(commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			P1.pipelineLayout, 1, 1, &DSTerrain.descriptorSets[currentImage],
			0, nullptr);
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(MTerrain.indices.size()), 1, 0, 0, 0);

		//FLOOR
		VkBuffer vertexBuffers4[] = { MFloor.vertexBuffer };
		VkDeviceSize offsets4[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers4, offsets4);
		vkCmdBindIndexBuffer(commandBuffer, MFloor.indexBuffer, 0,
			VK_INDEX_TYPE_UINT32);
		vkCmdBindDescriptorSets(commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			P1.pipelineLayout, 1, 1, &DSFloor.descriptorSets[currentImage],
			0, nullptr);
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(MFloor.indices.size()), 1, 0, 0, 0);
		
		//DESCRIPTION
		VkBuffer vertexBuffers5[] = { M2.vertexBuffer };
		VkDeviceSize offsets5[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers5, offsets5);
		vkCmdBindIndexBuffer(commandBuffer, M2.indexBuffer, 0,
			VK_INDEX_TYPE_UINT32);
		for (int i = 0; i <DSDesc.size(); i++) {
			
			vkCmdBindDescriptorSets(commandBuffer,
				VK_PIPELINE_BIND_POINT_GRAPHICS,
				P1.pipelineLayout, 1, 1, &DSDesc[i].descriptorSets[currentImage],
				0, nullptr);
			vkCmdDrawIndexed(commandBuffer,
				static_cast<uint32_t>(M2.indices.size()), 1, 0, 0, 0);
		}
	}
	glm::mat3 CamDir = glm::mat3(1.0f);
	glm::vec3 CamPos = glm::vec3(0.0, 1.7f, 0.0f);
	glm::vec3 CamAng = glm::vec3(0.0f, 0.0f, 0.0f);
	// Here is where you update the uniforms.
	// Very likely this will be where you will be writing the logic of your application.
	void updateUniformBuffer(uint32_t currentImage) {
		
		bool visible = false;

		static auto startTime = std::chrono::high_resolution_clock::now();
		static float lastTime = 0.0f;

		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float, std::chrono::seconds::period>
			(currentTime - startTime).count();
		float deltaT = time - lastTime;
		lastTime = time;

		const float ROT_SPEED = glm::radians(60.0f);
		const float MOVE_SPEED = 1.25f;
		const float MOUSE_RES = 500.0f;

		static double old_xpos = 0, old_ypos = 0;
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		double m_dx = xpos - old_xpos;
		double m_dy = ypos - old_ypos;
		old_xpos = xpos; old_ypos = ypos;
		glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			CamAng.y += m_dx * ROT_SPEED / MOUSE_RES;
			CamAng.x += m_dy * ROT_SPEED / MOUSE_RES;
		}

		if (glfwGetKey(window, GLFW_KEY_LEFT)) {
			CamAng.y += deltaT * ROT_SPEED;
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT)) {
			CamAng.y -= deltaT * ROT_SPEED;
		}
		if (glfwGetKey(window, GLFW_KEY_UP)) {
			CamAng.x += deltaT * ROT_SPEED;
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN)) {
			CamAng.x -= deltaT * ROT_SPEED;
		}
		if (glfwGetKey(window, GLFW_KEY_Q)) {
			CamAng.z -= deltaT * ROT_SPEED;
		}
		if (glfwGetKey(window, GLFW_KEY_E)) {
			CamAng.z += deltaT * ROT_SPEED;
		}

		if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
			visible = !visible;
		}


		 CamDir = glm::mat3(glm::rotate(glm::mat4(1.0f), CamAng.y, glm::vec3(0.0f, 1.0f, 0.0f))) *
			glm::mat3(glm::rotate(glm::mat4(1.0f), CamAng.x, glm::vec3(1.0f, 0.0f, 0.0f))) *
			glm::mat3(glm::rotate(glm::mat4(1.0f), CamAng.z, glm::vec3(0.0f, 0.0f, 1.0f)));

		glm::vec3 oldCamPos = CamPos;

		if (glfwGetKey(window, GLFW_KEY_A)) {
			CamPos -= MOVE_SPEED * glm::vec3(glm::rotate(glm::mat4(1.0f), CamAng.y,
				glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(1, 0, 0, 1)) * deltaT;
		}
		if (glfwGetKey(window, GLFW_KEY_D)) {
			CamPos += MOVE_SPEED * glm::vec3(glm::rotate(glm::mat4(1.0f), CamAng.y,
				glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(1, 0, 0, 1)) * deltaT;
		}
		if (glfwGetKey(window, GLFW_KEY_S)) {
			CamPos += MOVE_SPEED * glm::vec3(glm::rotate(glm::mat4(1.0f), CamAng.y,
				glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(0, 0, 1, 1)) * deltaT;
		}
		if (glfwGetKey(window, GLFW_KEY_W)) {
			CamPos -= MOVE_SPEED * glm::vec3(glm::rotate(glm::mat4(1.0f), CamAng.y,
				glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(0, 0, 1, 1)) * deltaT;
		}

		// Detect collision and slide along the wall
	
		if (detectCollision(CamPos)) {
	
				CamPos = oldCamPos;
			}
		



		void* data;
		glm::mat4 CamMat = glm::translate(glm::transpose(glm::mat4(CamDir)), -CamPos);
		UniformBufferObject ubo{};
		globalUniformBufferObject gubo{};
		gubo.view = glm::translate(glm::transpose(glm::mat4(CamDir)), -CamPos); //CamMat
		gubo.proj = glm::perspective(glm::radians(45.0f), //Prj
			swapChainExtent.width / (float)swapChainExtent.height,
			0.1f, 50.0f);;
		gubo.proj[1][1] *= -1;
		gubo.lightPos[0] = glm::vec3(0.2f, 2.9f, -0.1f); //point lights
		gubo.lightPos[1] = glm::vec3(-1.8f, 2.9f, -0.1f);
		gubo.lightPos[2] = glm::vec3(-3.8f, 2.9f, -0.1f);
		gubo.lightPos[3] = glm::vec3(-5.8f, 2.9f, -0.1f);
		gubo.lightPos[4] = glm::vec3(0.2f, 2.9f, 2.3f);
		gubo.lightPos[5] = glm::vec3(-1.8f, 2.9f, 2.3f);
		gubo.lightPos[6] = glm::vec3(-3.8f, 2.9f, 2.3f);
		gubo.lightPos[7] = glm::vec3(-5.8f, 2.9f, 2.3f);
		gubo.lightColor = glm::vec3(1.0f, 0.96f, 0.934f);
		gubo.coneInOutDecayExp = glm::vec2(0.5f, 1.5f);
		gubo.EyePos = CamPos;

		vkMapMemory(device, DSGlobal.uniformBuffersMemory[0][currentImage], 0,
			sizeof(gubo), 0, &data);
		memcpy(data, &gubo, sizeof(gubo));
		vkUnmapMemory(device, DSGlobal.uniformBuffersMemory[0][currentImage]);
		
		

		

		//MUSEO
		ubo.model = glm::mat4(1.0f); 
		vkMapMemory(device, DS1.uniformBuffersMemory[0][currentImage], 0,
							sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DS1.uniformBuffersMemory[0][currentImage]);

		//QUADRI * N
		for (int i = 0; i < objects_coordinates.size(); i++) {
			ubo.model = (glm::translate(glm::mat4(1.0f), objects_coordinates[i].getPos())
					* glm::rotate(glm::mat4(1.0f), objects_coordinates[i].getAngle(), glm::vec3(0, 1, 0)));
			vkMapMemory(device, DSPicture[i].uniformBuffersMemory[0][currentImage], 0,
				sizeof(ubo), 0, &data);
			memcpy(data, &ubo, sizeof(ubo));
			vkUnmapMemory(device, DSPicture[i].uniformBuffersMemory[0][currentImage]);
		}

		

		
		
		//TERRAIN
		ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.9f, 0.0f));
		vkMapMemory(device, DSTerrain.uniformBuffersMemory[0][currentImage], 0,
			sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DSTerrain.uniformBuffersMemory[0][currentImage]);

		//FLOOR
		ubo.model = glm::mat4(1.0f);
		vkMapMemory(device, DSFloor.uniformBuffersMemory[0][currentImage], 0,
			sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DSFloor.uniformBuffersMemory[0][currentImage]);
		
		//qui le ho dichiarate localment anzichè dalla funzione perchè nella funzione le hai hardcodate
		int vis2 = 0;
		
		for (int i = 0; i < descriptions_coordinates.size(); i++) {
			if (visible) {
				vis2 = enableDesc(CamPos, descriptions_coordinates[i]);
			}
			ubo.model = (glm::translate(glm::mat4(vis2), descriptions_coordinates[i].getPos())
					* glm::rotate(glm::mat4(1.0f), descriptions_coordinates[i].getAngle(), glm::vec3(0, 1, 0)));
			vkMapMemory(device, DSDesc[i].uniformBuffersMemory[0][currentImage], 0,
				sizeof(ubo), 0, &data);
			memcpy(data, &ubo, sizeof(ubo));
			vkUnmapMemory(device, DSDesc[i].uniformBuffersMemory[0][currentImage]);
		}
		
		
	}	
};

FILE* loadFile(const char* filename) {
	FILE* fpin;
	fopen_s(&fpin, filename, "r");
	if (!fpin) {
		cout << "Errore, impossibile aprire il file: " << filename << endl;
		return NULL;
	}

	return fpin;
}

vector<Coordinate> loadCoordinates(const char* filePath) {
	vector<Coordinate> coordinates;
	coordinates.reserve(FIRST_OBJECTS_SIZE);
	float tmpCoordinates[3], tmpAngle;
	FILE* fp = loadFile(filePath);
	while (!feof(fp)) {
		fscanf(fp, "%f %f %f %f", &tmpCoordinates[0], &tmpCoordinates[1], &tmpCoordinates[2], &tmpAngle);
		coordinates.push_back(Coordinate(tmpCoordinates, tmpAngle));
	}
	fclose(fp);
	return coordinates;
}

vector<string> loadTextures(const char* filePath) {
	vector<string> texturesName;
	texturesName.reserve(FIRST_OBJECTS_SIZE);
	FILE* fp = loadFile(filePath);
	char tmpTextureName[MAX_TEXTURE_NAME_SIZE];
	int count = 0;
	while (fscanf(fp, "%s", tmpTextureName) != EOF) {
		texturesName.push_back(tmpTextureName);
		count++;
	}
	fclose(fp);
	return texturesName;
}

// This is the main: probably you do not need to touch this!
int main() {
	objects_coordinates = loadCoordinates("resources/Coordinates.txt");
	objects_textures_path = loadTextures("resources/Textures.txt");
	descriptions_coordinates = loadCoordinates("resources/Coordinates2.txt");
	descriptions_textures_path = loadTextures("resources/Textures2.txt");
    MyProject app;
	
    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}