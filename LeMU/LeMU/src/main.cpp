#include "pch.h"


struct ShaderModule
{
    std::vector<unsigned int> SPIRV;
    VkShaderModule shaderModule;
};



size_t CompileShader(glslang_stage_t stage, const char* shaderSource, ShaderModule& shaderModule)
{

}


int main() {

    // simple lambda to catch potential errors
    glfwSetErrorCallback(
        [](int error, const char* description) 
        {
            fprintf(stderr, "Error: %s\n", description);    
        }
    );

    // GLFW init
    if (!glfwInit()) exit(EXIT_FAILURE);

    
   

    

    return EXIT_SUCCESS;
    
}