// This has been adapted from the Vulkan tutorial

#include "MyProject.hpp"
using namespace std;
#include "Coordinate.cpp"

const std::string MODEL_PATH = "models/museumTri22.obj";
const std::string TEXTURE_PATH = "textures/walls.jpg";
const std::string MODEL_PATH1 = "models/quadro3.obj";
int numPictures = 8;
const std::string texture_path[] = {"textures/cezan.jpg", "textures/caravaggio.jpg", "textures/botticelli.jpg", "textures/david.jpg", "textures/vangogh.jpg", "textures/cole.jpg", "textures/dalì.jpg", "textures/monet.jpg" };
const std::string MODEL_PATHTERRAIN = "models/terrain.obj";
const std::string TEXTURE_PATHTERRAIN = "textures/terrain.png";
const std::string MODEL_PATHFLOOR = "models/Floor.obj";
const std::string TEXTURE_PATHFLOOR = "textures/floor.png";
const std::string texture_desc[] = { "textures/cez.png", "textures/cardesc.png", "textures/bottdesc.png", "textures/oratdesc.png", "textures/vgdesc.png", "textures/tcdesc.png", "textures/dalidesc.png", "textures/mondesc.png" };

const int num = 8;
const int numd = 8;

// The uniform buffer object used in this example
struct globalUniformBufferObject {
	
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
	alignas(16) glm::vec3 lightPos[8];
	alignas(16) glm::vec3 lightColor;
	alignas(16) glm::vec2 coneInOutDecayExp;
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
	Texture TPicture[num];
	DescriptorSet DSPicture[num]; //instance DSLobj
	//per ogni quadro dobbiamo aggiungere un descriptor set e siccome cambia la texture anche la texture
	

	Model MTerrain;
	Texture TTerrain;
	DescriptorSet DSTerrain;

	Model MFloor;
	Texture TFloor;
	DescriptorSet DSFloor;

	Texture TDesc[numd];
	DescriptorSet DSDesc[numd];
	
	// Here you set the main application parameters
	void setWindowParameters() {
		// window size, titile and initial background
		windowWidth = 800;
		windowHeight = 600;
		windowTitle = "My Project";
		initialBackgroundColor = {0.68f, 0.8f, 1.0f, 1.0f};
		
		// Descriptor pool sizes
		uniformBlocksInPool = 4+ numd + num;
		texturesInPool = 3+ numd + num;
		setsInPool = 4+ numd + num;
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

		P1.init(this, "shaders/vert.spv", "shaders/frag.spv", {&DSLGlobal, &DSLObj});
		loadModels();
		// Models, textures and Descriptors (values assigned to the uniforms)
		M1.init(this, MODEL_PATH);
		T1.init(this, TEXTURE_PATH);
		DS1.init(this, &DSLObj, {
		
					{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
					{1, TEXTURE, 0, &T1}
				});

		//questo è solo per il primo quadro, M2 è uno, T2 è quanti quadri e DS è quanti quadri
		M2.init(this, MODEL_PATH1);
		for (int i = 0; i < num; i++) {
			TPicture[i].init(this, texture_path[i]);
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

		for (int i = 0; i < numd; i++) {
			TDesc[i].init(this, texture_desc[i]);
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

		for (int i = 0; i < num; i++) {
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
		for (int i = 0; i < numd; i++) {
			DSDesc[i].cleanup();
			TDesc[i].cleanup();
		}
		
		
		
		
	}

	FILE* loadFile(const char* filename) {
		FILE* fpin;
		fopen_s(&fpin, filename, "r");
		if (!fpin) {
			cout << "Errore, impossibile aprire il file: "<<filename << endl;
			exit(0);
		}

		return fpin;

	}

	vector<Coordinate> loadCoordinates() {
		vector<Coordinate> Coordinates(numPictures);
		/*FILE* fp = loadFile("resources\\Coordinates.txt");
		char readBuffer[10];
		float tempCoordinate[3];
		int j =0, count = 0;
		vector<Coordinate> Coordinates(numPictures);
		while (!feof(fp)) {
			fgets(readBuffer, 10, fp);
			if (count % 3 != 0 || count == 0) {
				tempCoordinate[count] = stof(readBuffer);
				count++;
			}
			else if (count % 3 == 0 && count!=0) {
				Coordinates[j] = Coordinate(tempCoordinate[0], tempCoordinate[1], tempCoordinate[2]);
				j++;
				count = 0;
			}

			
		}

		fclose(fp);*/
		Coordinates[0] = Coordinate(0.2f, 1.9f, -0.9f);
		Coordinates[1] = Coordinate(-1.8f, 1.9f, -0.9f);
		Coordinates[2] = Coordinate(-3.8f, 1.9f, -0.9f);
		Coordinates[3] = Coordinate(-5.8f, 1.9f, -0.9f);
		Coordinates[4] = Coordinate(0.2f, 1.9f, 3.1f);
		Coordinates[5] = Coordinate(-1.8f, 1.9f, 3.1f);
		Coordinates[6] = Coordinate(-3.8f, 1.9f, 3.1f);
		Coordinates[7] = Coordinate(-5.8f, 1.9f, 3.1f);
		
		
		return Coordinates;
	}
	/*
	int setVisible(glm::vec3 campos, glm::vec3 coordinate) {
		glm::vec3 c(0.7f, 0.7f, 0.7f);
		glm::vec3 diff = campos - coordinate;
		return std::abs(glm::length(diff)) < std::abs(glm::length(c));
	}*/

	bool setVisible(glm::vec3 campos, glm::vec3 coordinate) {
		glm::vec3 c(1.2f, 1.2f, 1.2f);
		glm::vec3 diff = campos - coordinate;
		return glm::length(diff) < glm::length(c);
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
			for (int i = 0; i < num; i++) {
				
				vkCmdBindDescriptorSets(commandBuffer,
					VK_PIPELINE_BIND_POINT_GRAPHICS,
					P1.pipelineLayout, 1, 1, &DSPicture[i].descriptorSets[currentImage],
					0, nullptr);
				vkCmdDrawIndexed(commandBuffer,
					static_cast<uint32_t>(M2.indices.size()), 1, 0, 0, 0);
			}
		/*
		VkBuffer vertexBuffers2[] = { M2.vertexBuffer };
		VkDeviceSize offsets2[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers2, offsets2);
		vkCmdBindIndexBuffer(commandBuffer, M2.indexBuffer, 0,
			VK_INDEX_TYPE_UINT32);
		
		vkCmdBindDescriptorSets(commandBuffer,
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		P1.pipelineLayout, 1, 1, &DSPicture[0].descriptorSets[currentImage],
				0, nullptr);
		vkCmdDrawIndexed(commandBuffer,
				static_cast<uint32_t>(M2.indices.size()), 1, 0, 0, 0);


		vkCmdBindDescriptorSets(commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			P1.pipelineLayout, 1, 1, &DSPicture[1].descriptorSets[currentImage],
			0, nullptr);
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(M2.indices.size()), 1, 0, 0, 0);
		*/
		
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
		for (int i = 0; i < numd; i++) {

			vkCmdBindDescriptorSets(commandBuffer,
				VK_PIPELINE_BIND_POINT_GRAPHICS,
				P1.pipelineLayout, 1, 1, &DSDesc[i].descriptorSets[currentImage],
				0, nullptr);
			vkCmdDrawIndexed(commandBuffer,
				static_cast<uint32_t>(M2.indices.size()), 1, 0, 0, 0);
		}
	}
	glm::mat3 CamDir = glm::mat3(1.0f);
	glm::vec3 CamPos = glm::vec3(-2.5f, 1.7f, 0.5f);
	glm::vec3 CamAng = glm::vec3(0.0f, 0.0f, 0.0f);
	// Here is where you update the uniforms.
	// Very likely this will be where you will be writing the logic of your application.
	void updateUniformBuffer(uint32_t currentImage) {
		
		 int visible = 0;
		
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
		const float angqd = glm::radians(270.0f);
		const float angqs = glm::radians(90.0f);
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

		if (glfwGetKey(window, GLFW_KEY_U)) {
			visible = 1;
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

		if (!canStep(CamPos.x, CamPos.z)) {
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
		gubo.lightPos[0] = glm::vec3(0.2f, 3.0f, -0.4f); //point lights
		gubo.lightPos[1] = glm::vec3(-1.8f, 3.0f, -0.4f);
		gubo.lightPos[2] = glm::vec3(-3.8f, 3.0f, -0.4f);
		gubo.lightPos[3] = glm::vec3(-5.8f, 3.0f, -0.4f);
		gubo.lightPos[4] = glm::vec3(0.2f, 3.0f, 2.6f);
		gubo.lightPos[5] = glm::vec3(-1.8f, 3.0f, 2.6f);
		gubo.lightPos[6] = glm::vec3(-3.8f, 3.0f, 2.6f);
		gubo.lightPos[7] = glm::vec3(-5.8f, 3.0f, 2.6f);
		gubo.lightColor = glm::vec3(1.0f, 0.96f, 0.934f);
		gubo.coneInOutDecayExp = glm::vec2(0.5f, 1.5f);
		gubo.AmbColor = glm::vec3(0.5f,0.5f,0.5f);
		gubo.DzColor = glm::vec3(0.1f, 0.15f, 0.1f);
		gubo.DyColor = glm::vec3(0.1f, 0.15f, 0.1f);
		gubo.DxColor = glm::vec3(0.1f, 0.15f, 0.1f);

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
		vector<Coordinate> Coordinates = loadCoordinates();
		for (int i = 0; i < numPictures; i++) {
			ubo.model = (glm::translate(glm::mat4(1.0f), Coordinates[i].getPos())
				* glm::rotate(glm::mat4(1.0f), angqd, glm::vec3(0, 1, 0)));
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
		int vis2 = 0;
		//DESC DESTRA
		if (visible == 1 && setVisible(CamPos, glm::vec3(0.2f, 1.9f, -0.7f))) {
			vis2 = 1;
		}
		ubo.model = glm::mat4(float(vis2)) * (glm::translate(glm::mat4(1.0f), glm::vec3(0.2f, 1.9f, -0.7f))
			* glm::rotate(glm::mat4(1.0f), angqd, glm::vec3(0, 1, 0))
			);
		//ubo.model = (glm::translate(glm::mat4(1.0f), glm::vec3(0.1f, 1.7f, -0.5f)) * glm::scale(ubo.model, glm::vec3(0.5 * visible, 0.5 * visible, 0.0)) * glm::rotate(glm::mat4(1.0f), -angq, glm::vec3(0, 1, 0)));
		vkMapMemory(device, DSDesc[0].uniformBuffersMemory[0][currentImage], 0,
			sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DSDesc[0].uniformBuffersMemory[0][currentImage]);

		if (visible == 1 && setVisible(CamPos, glm::vec3(-1.8f, 1.9f, -0.7f))) {
			vis2 = 1;
		}
		ubo.model = glm::mat4(float(vis2)) * (glm::translate(glm::mat4(1.0f), glm::vec3(-1.8f, 1.9f, -0.7f))
			* glm::rotate(glm::mat4(1.0f), angqd, glm::vec3(0, 1, 0)));
		//ubo.model = (glm::translate(glm::mat4(1.0f), glm::vec3(0.1f, 1.7f, -0.5f)) * glm::scale(ubo.model, glm::vec3(0.5 * visible, 0.5 * visible, 0.0)) * glm::rotate(glm::mat4(1.0f), -angq, glm::vec3(0, 1, 0)));
		vkMapMemory(device, DSDesc[1].uniformBuffersMemory[0][currentImage], 0,
			sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DSDesc[1].uniformBuffersMemory[0][currentImage]);

		if (visible == 1 && setVisible(CamPos, glm::vec3(-3.8f, 1.9f, -0.7f))) {
			vis2 = 1;
		}
		ubo.model = glm::mat4(float(vis2)) * (glm::translate(glm::mat4(1.0f), glm::vec3(-3.8f, 1.9f, -0.7f))
			* glm::rotate(glm::mat4(1.0f), angqd, glm::vec3(0, 1, 0))
			);
		//ubo.model = (glm::translate(glm::mat4(1.0f), glm::vec3(0.1f, 1.7f, -0.5f)) * glm::scale(ubo.model, glm::vec3(0.5 * visible, 0.5 * visible, 0.0)) * glm::rotate(glm::mat4(1.0f), -angq, glm::vec3(0, 1, 0)));
		vkMapMemory(device, DSDesc[2].uniformBuffersMemory[0][currentImage], 0,
			sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DSDesc[2].uniformBuffersMemory[0][currentImage]);

		if (visible == 1 && setVisible(CamPos, glm::vec3(-5.8f, 1.9f, -0.7f))) {
			vis2 = 1;
		}
		ubo.model = glm::mat4(float(vis2)) * (glm::translate(glm::mat4(1.0f), glm::vec3(-5.8f, 1.9f, -0.7f))
			* glm::rotate(glm::mat4(1.0f), angqd, glm::vec3(0, 1, 0))
			);

		//ubo.model = (glm::translate(glm::mat4(1.0f), glm::vec3(0.1f, 1.7f, -0.5f)) * glm::scale(ubo.model, glm::vec3(0.5 * visible, 0.5 * visible, 0.0)) * glm::rotate(glm::mat4(1.0f), -angq, glm::vec3(0, 1, 0)));
		vkMapMemory(device, DSDesc[3].uniformBuffersMemory[0][currentImage], 0,
			sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DSDesc[3].uniformBuffersMemory[0][currentImage]);

		//DESC A SIN
		if (visible == 1 && setVisible(CamPos, glm::vec3(0.2f, 1.9f, 2.9f))) {
			vis2 = 1;
		}
		ubo.model = glm::mat4(float(vis2)) * (glm::translate(glm::mat4(1.0f), glm::vec3(0.2f, 1.9f, 2.9f))
			* glm::rotate(glm::mat4(1.0f), angqd, glm::vec3(0, 1, 0))
			);
		//ubo.model = (glm::translate(glm::mat4(1.0f), glm::vec3(0.1f, 1.7f, -0.5f)) * glm::scale(ubo.model, glm::vec3(0.5 * visible, 0.5 * visible, 0.0)) * glm::rotate(glm::mat4(1.0f), -angq, glm::vec3(0, 1, 0)));
		vkMapMemory(device, DSDesc[4].uniformBuffersMemory[0][currentImage], 0,
			sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DSDesc[4].uniformBuffersMemory[0][currentImage]);

		if (visible == 1 && setVisible(CamPos, glm::vec3(-1.8f, 1.9f, 2.9f))) {
			vis2 = 1;
		}
		ubo.model = glm::mat4(float(visible)) * (glm::translate(glm::mat4(1.0f), glm::vec3(-1.8f, 1.9f, 2.9f))
			* glm::rotate(glm::mat4(1.0f), angqd, glm::vec3(0, 1, 0))
			);
		//ubo.model = (glm::translate(glm::mat4(1.0f), glm::vec3(0.1f, 1.7f, -0.5f)) * glm::scale(ubo.model, glm::vec3(0.5 * visible, 0.5 * visible, 0.0)) * glm::rotate(glm::mat4(1.0f), -angq, glm::vec3(0, 1, 0)));
		vkMapMemory(device, DSDesc[5].uniformBuffersMemory[0][currentImage], 0,
			sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DSDesc[5].uniformBuffersMemory[0][currentImage]);


		if (visible == 1 && setVisible(CamPos, glm::vec3(-3.8f, 1.9f, 2.9f))) {
			vis2 = 1;
		}
		ubo.model = glm::mat4(float(visible)) * (glm::translate(glm::mat4(1.0f), glm::vec3(-3.8f, 1.9f, 2.9f))
			* glm::rotate(glm::mat4(1.0f), angqd, glm::vec3(0, 1, 0))
			);
		//ubo.model = (glm::translate(glm::mat4(1.0f), glm::vec3(0.1f, 1.7f, -0.5f)) * glm::scale(ubo.model, glm::vec3(0.5 * visible, 0.5 * visible, 0.0)) * glm::rotate(glm::mat4(1.0f), -angq, glm::vec3(0, 1, 0)));
		vkMapMemory(device, DSDesc[6].uniformBuffersMemory[0][currentImage], 0,
			sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DSDesc[6].uniformBuffersMemory[0][currentImage]);


		if (visible == 1 && setVisible(CamPos, glm::vec3(-5.8f, 1.9f, 2.9f))) {
			vis2 = 1;
		}
		ubo.model = glm::mat4(float(visible)) * (glm::translate(glm::mat4(1.0f), glm::vec3(-5.8f, 1.9f, 2.9f))
			* glm::rotate(glm::mat4(1.0f), angqd, glm::vec3(0, 1, 0))
			);
		//ubo.model = (glm::translate(glm::mat4(1.0f), glm::vec3(0.1f, 1.7f, -0.5f)) * glm::scale(ubo.model, glm::vec3(0.5 * visible, 0.5 * visible, 0.0)) * glm::rotate(glm::mat4(1.0f), -angq, glm::vec3(0, 1, 0)));
		vkMapMemory(device, DSDesc[7].uniformBuffersMemory[0][currentImage], 0,
			sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DSDesc[7].uniformBuffersMemory[0][currentImage]);
	}	
};

// This is the main: probably you do not need to touch this!
int main() {
    MyProject app;

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}