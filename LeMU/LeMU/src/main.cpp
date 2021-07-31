
#include "App.hpp"

// std
#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main() {

    //int width, height, nrChannels;
    //unsigned char* data = stbi_load("textures/texture.jpg", &width, &height, &nrChannels, 3);

    //if (!data)
      //  std::cout << stbi_failure_reason() << std::endl;
        //throw std::runtime_error("fuck you" + std::string(" ") + stbi_failure_reason());

    
    LeMU::FirstApp app{};

    try {
        app.run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
    
}