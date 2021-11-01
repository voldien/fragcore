// #include <GL/glew.h>
// #include <Rendering/VDRenderingAPICache.h>
// #include <VDSystemInfo.h>

// #include <cstring>

// #define CACHED_TEXTURE_COUNT 128

// typedef struct texture_binding {
//     // unsigned char textureIndex[CACHED_TEXTURE_COUNT];
//     unsigned short textureID[CACHED_TEXTURE_COUNT];
// } TextureBinding;

// TextureBinding textureBinding = {0};
// unsigned int shaderprogram = 0;
// unsigned int framebuffers[2] = {0};
// int viewport[4] = {0};
// float clearcolor[4] = {0};
// unsigned int buffers[32] = {0};
// unsigned int bufferbase[64] = {0};
// char glstates[1024 * 4] = {-1};

// bool cacheEnabled;

// void VDRenderingAPICache::init(unsigned int numThreads) {
//     memset(glstates, -1, sizeof(glstates));

//     VDRenderingAPICache::enable();
// }

// void VDRenderingAPICache::disable() { cacheEnabled = false; }

// void VDRenderingAPICache::enable() { cacheEnabled = true; }

// bool VDRenderingAPICache::isEnabled() { return cacheEnabled == true; }

// bool VDRenderingAPICache::isDisable() { return cacheEnabled == false; }

// void VDRenderingAPICache::setClearColor(float red, float green, float blue,
//                                         float alpha) {
//     glClearColor(red, green, blue, alpha);

//     if ((clearcolor[0] != red) || (clearcolor[1] != green) ||
//         (clearcolor[2] != blue) || (clearcolor[3] != alpha)) {
//         clearcolor[0] = red;
//         clearcolor[1] = green;
//         clearcolor[2] = blue;
//         clearcolor[3] = alpha;
//     }
// }

// void VDRenderingAPICache::setViewport(int x, int y, int width, int height) {
//     if (isDisable()) goto setState;
//     if ((x != viewport[0]) || (y != viewport[1]) || (width != viewport[2]) ||
//         (height != viewport[3])) {
//     } else
//         return;

// /**/
// setState:
//     glViewport(x, y, width, height);
//     viewport[0] = x;
//     viewport[1] = y;
//     viewport[2] = width;
//     viewport[3] = height;
// }

// void VDRenderingAPICache::bindTexture(unsigned int target, int texture,
//                                       unsigned int index) {
//     if (isDisable()) {
//         goto setState;
//     }

//     /**/
//     if (textureBinding.textureID[index] == texture) return;

// setState: /**/
//     glActiveTexture(GL_TEXTURE0 + index);
//     glBindTexture(target, texture);
//     textureBinding.textureID[index] = texture;
// }

// void VDRenderingAPICache::bindTextures(unsigned int first, int count,
//                                        const int* textures) {
//     if (VDSystemInfo::getCompatibility()->sMultiBind) {
//         glActiveTexture(GL_TEXTURE0 + first);
//         glBindTextures(first, count, (const GLuint*)textures);
//         for (int x = first; x < count + first; x++) {
//             textureBinding.textureID[x - first] = first + x;
//         }
//     } else {
//         for (int x = first; x < count + first; x++) {
//             bindTexture(GL_TEXTURE_2D, textures[x], first + x);
//             textureBinding.textureID[x - first] = first + x;
//         }
//     }
// }

// void VDRenderingAPICache::bindProgram(unsigned int program) {
//     // ExGetCurrentThread();

//     if (isDisable()) goto setState;
//     // if(shaderprogram == program)
//     //	return;
//     /**/

// setState:
//     glUseProgram(program);
//     shaderprogram = program;
// }

// unsigned int VDRenderingAPICache::currentBindedProgram() {
//     return shaderprogram;
// }

// void VDRenderingAPICache::bindFramebuffer(unsigned int target,
//                                           unsigned int framebuffer) {
//     switch (target) {
//         case GL_DRAW_FRAMEBUFFER:
//             break;
//         case GL_READ_FRAMEBUFFER:
//             break;
//         case GL_FRAMEBUFFER:
//             break;
//         default:
//             break;
//     }

//     // TODO fix!
//     /*
//     if(target == GL_DRAW_FRAMEBUFFER){
//             if(framebuffers[target - GL_DRAW_FRAMEBUFFER] == framebuffer){
//                     glBindFramebuffer(GL_DRAW_FRAMEBUFFER,framebuffer);
//             }
//     }
//     */

//     glBindFramebuffer(target, framebuffer);
// }

// void VDRenderingAPICache::bindVertexArrayObject(unsigned int array) {
//     glBindVertexArray(array);
// }

// void VDRenderingAPICache::bindBuffer(unsigned int target, unsigned int buffer) {
// setState: /**/
//     glBindBuffer(target, buffer);
// }

// void VDRenderingAPICache::bindBufferBase(unsigned int target,
//                                          unsigned int index,
//                                          unsigned int buffer) {
//     if (isDisable()) goto setState;

// setState: /**/
//     bufferbase[index] = buffer;
//     glBindBufferBase(target, index, buffer);
// }

// void VDRenderingAPICache::setState(unsigned int state, int enable) {
//     int indexpos = state % (sizeof(glstates) / sizeof(glstates[0]));
//     if (isDisable()) goto setState;
//     if (glstates[indexpos] != -1) {
//         if (glstates[indexpos] == enable) {
//             return;
//         }
//     }

// setState:
//     switch (enable) {
//         case 0:
//             glDisable(state);
//             break;
//         case 1:
//             glEnable(state);

//             break;
//     }
//     glstates[indexpos] = enable;
// }

// unsigned int VDRenderingAPICache::getState(unsigned int state) {
//     return glstates[state % (sizeof(glstates) / sizeof(glstates[0]))];
// }