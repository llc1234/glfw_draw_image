#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include <GLFW/glfw3.h>


int main() {
    // Initialize GLFW
    if (!glfwInit()) return -1;

    // Create window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Image Drawing", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    stbi_set_flip_vertically_on_load(true);

    // Load image
    int width, height, nrChannels;
    unsigned char* data = stbi_load("image.png", &width, &height, &nrChannels, 0);
    if (!data) {
        std::cout << "ERROR loading image from files!" << std::endl;
        return -1;
    }


    /*
    This code reads an image file ("image.png") from disk into memory,
    then prints its bytes as a C array in hex format.

    Steps:
    1. Open the file in binary mode.
    2. Find the file size.
    3. Allocate a buffer to hold the file data.
    4. Read the entire file into the buffer.
    5. Print each byte as "0xXX," so you can copy-paste it into your code.
    6. Print the total length of the array.
    7. Free the buffer.

    Use this output to embed the image data directly in your program,
    so you can load it later with stbi_load_from_memory() without needing an external file.
    */
    /*
    unsigned char* data = stbi_load_from_memory(image_data, image_data_len, &width, &height, &nrChannels, 0);
    if (!data) {
        std::cout << "ERROR loading image from memory!" << std::endl;
        return -1;
    }

    FILE* f = nullptr;
    if (fopen_s(&f, "image.png", "rb") != 0) {
        perror("Failed to open image.png");
        return -1;
    }


    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    fseek(f, 0, SEEK_SET);

    unsigned char* buffer = (unsigned char*)malloc(len);
    if (!buffer) {
        perror("Failed to allocate memory");
        fclose(f);
        return -1;
    }

    fread(buffer, 1, len, f);
    fclose(f);

    // Print out array
    printf("unsigned char image_data[] = {\n");
    for (int i = 0; i < len; i++) {
        printf("0x%02X", buffer[i]);
        if (i < len - 1) printf(", ");
        if ((i + 1) % 12 == 0) printf("\n");
    }
    printf("\n};\n");
    printf("unsigned int image_data_len = %ld;\n", len);

    free(buffer);
    */


    // Choose correct format
    GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
    GLuint texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        // Enable textures and bind
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture);

        // Draw textured quad
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-0.5f, -0.5f);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(0.5f, -0.5f);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(0.5f, 0.5f);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-0.5f, 0.5f);
        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteTextures(1, &texture);
    glfwTerminate();
    return 0;
}