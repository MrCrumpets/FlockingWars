#include <GL/glew.h>
#include "imgui-SFML.h"
#include "imgui.h"

#include <SFML/OpenGL.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Window.hpp>

#include <cstddef> // offsetof, NULL
#include <cassert>

// Supress warnings caused by converting from uint to void* in pCmd->TextureID
#ifdef __clang__
#pragma clang diagnostic ignored "-Wint-to-void-pointer-cast" // warning : cast to 'void *' from smaller integer type 'int'
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"      // warning: cast to pointer from integer of different size
#endif

static bool s_windowHasFocus = true;
static bool s_mousePressed[3] = { false, false, false };

static GLuint       g_FontTexture = 0;
static int          g_ShaderHandle = 0, g_VertHandle = 0, g_FragHandle = 0;
static int          g_AttribLocationTex = 0, g_AttribLocationProjMtx = 0;
static int          g_AttribLocationPosition = 0, g_AttribLocationUV = 0, g_AttribLocationColor = 0;
static unsigned int g_VboHandle = 0, g_VaoHandle = 0, g_ElementsHandle = 0;

namespace
{
    void RenderDrawLists(ImDrawData* draw_data); // rendering callback function prototype

} // anonymous namespace for helper / "private" functions

namespace ImGui
{
    namespace SFML
    {

        void Init(sf::Window& target)
        {
            ImGuiIO& io = ImGui::GetIO();

            // init keyboard mapping
            io.KeyMap[ImGuiKey_Tab] = sf::Keyboard::Tab;
            io.KeyMap[ImGuiKey_LeftArrow] = sf::Keyboard::Left;
            io.KeyMap[ImGuiKey_RightArrow] = sf::Keyboard::Right;
            io.KeyMap[ImGuiKey_UpArrow] = sf::Keyboard::Up;
            io.KeyMap[ImGuiKey_DownArrow] = sf::Keyboard::Down;
            io.KeyMap[ImGuiKey_PageUp] = sf::Keyboard::PageUp;
            io.KeyMap[ImGuiKey_PageDown] = sf::Keyboard::PageDown;
            io.KeyMap[ImGuiKey_Home] = sf::Keyboard::Home;
            io.KeyMap[ImGuiKey_End] = sf::Keyboard::End;
            io.KeyMap[ImGuiKey_Delete] = sf::Keyboard::Delete;
            io.KeyMap[ImGuiKey_Backspace] = sf::Keyboard::BackSpace;
            io.KeyMap[ImGuiKey_Enter] = sf::Keyboard::Return;
            io.KeyMap[ImGuiKey_Escape] = sf::Keyboard::Escape;
            io.KeyMap[ImGuiKey_A] = sf::Keyboard::A;
            io.KeyMap[ImGuiKey_C] = sf::Keyboard::C;
            io.KeyMap[ImGuiKey_V] = sf::Keyboard::V;
            io.KeyMap[ImGuiKey_X] = sf::Keyboard::X;
            io.KeyMap[ImGuiKey_Y] = sf::Keyboard::Y;
            io.KeyMap[ImGuiKey_Z] = sf::Keyboard::Z;

            // init rendering
            io.DisplaySize = static_cast<sf::Vector2f>(target.getSize());
            io.RenderDrawListsFn = RenderDrawLists; // set render callback
        }

        void ProcessEvent(const sf::Event& event)
        {
            ImGuiIO& io = ImGui::GetIO();
            if (s_windowHasFocus) {
                switch (event.type)
                {
                    case sf::Event::MouseButtonPressed: // fall-through
                    case sf::Event::MouseButtonReleased:
                        {
                            int button = event.mouseButton.button;
                            if (event.type == sf::Event::MouseButtonPressed &&
                                    button >= 0 && button < 3) {
                                s_mousePressed[event.mouseButton.button] = true;
                            }
                        }
                        break;
                    case sf::Event::MouseWheelMoved:
                        io.MouseWheel += static_cast<float>(event.mouseWheel.delta);
                        break;
                    case sf::Event::KeyPressed: // fall-through
                    case sf::Event::KeyReleased:
                        io.KeysDown[event.key.code] = (event.type == sf::Event::KeyPressed);
                        io.KeyCtrl = event.key.control;
                        io.KeyShift = event.key.shift;
                        io.KeyAlt = event.key.alt;
                        break;
                    case sf::Event::TextEntered:
                        if (event.text.unicode > 0 && event.text.unicode < 0x10000) {
                            io.AddInputCharacter(static_cast<ImWchar>(event.text.unicode));
                        }
                        break;
                    default:
                        break;
                }
            }

            switch (event.type)
            {
                case sf::Event::LostFocus:
                    s_windowHasFocus = false;
                    break;
                case sf::Event::GainedFocus:
                    s_windowHasFocus = true;
                    break;
                default:
                    break;
            }
        }

        void Update(sf::Window& window, sf::Time dt)
        {
            Update(sf::Mouse::getPosition(window), static_cast<sf::Vector2f>(window.getSize()), dt);
            window.setMouseCursorVisible(!ImGui::GetIO().MouseDrawCursor); // don't draw mouse cursor if ImGui draws it
        }

        void Update(const sf::Vector2i& mousePos, const sf::Vector2f& displaySize, sf::Time dt)
        {
            ImGuiIO& io = ImGui::GetIO();
            io.DisplaySize = displaySize;
            io.DeltaTime = dt.asSeconds();

            if (s_windowHasFocus) {
                io.MousePos = mousePos;
                for (int i = 0; i < 3; ++i) {
                    io.MouseDown[i] = s_mousePressed[i] || sf::Mouse::isButtonPressed((sf::Mouse::Button)i);
                    s_mousePressed[i] = false;
                }
            }

            if (!g_FontTexture)
                CreateDeviceObjects();

            assert(io.Fonts->Fonts.Size > 0); // You forgot to create and set up font atlas (see createFontTexture)
            ImGui::NewFrame();
        }

        void Shutdown()
        {
            ImGui::GetIO().Fonts->TexID = NULL;
            ImGui::Shutdown(); // need to specify namespace here, otherwise ImGui::SFML::Shutdown would be called
        }

        bool CreateDeviceObjects(){
            // Backup GL state
            GLint last_texture, last_array_buffer, last_vertex_array;
            glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
            glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
            glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);

            const GLchar *vertex_shader =
                "#version 330\n"
                "uniform mat4 ProjMtx;\n"
                "in vec2 Position;\n"
                "in vec2 UV;\n"
                "in vec4 Color;\n"
                "out vec2 Frag_UV;\n"
                "out vec4 Frag_Color;\n"
                "void main()\n"
                "{\n"
                "	Frag_UV = UV;\n"
                "	Frag_Color = Color;\n"
                "	gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
                "}\n";

            const GLchar* fragment_shader =
                "#version 330\n"
                "uniform sampler2D Texture;\n"
                "in vec2 Frag_UV;\n"
                "in vec4 Frag_Color;\n"
                "out vec4 Out_Color;\n"
                "void main()\n"
                "{\n"
                "	Out_Color = Frag_Color * texture( Texture, Frag_UV.st);\n"
                "}\n";

            g_ShaderHandle = glCreateProgram();
            g_VertHandle = glCreateShader(GL_VERTEX_SHADER);
            g_FragHandle = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(g_VertHandle, 1, &vertex_shader, 0);
            glShaderSource(g_FragHandle, 1, &fragment_shader, 0);
            glCompileShader(g_VertHandle);
            glCompileShader(g_FragHandle);
            glAttachShader(g_ShaderHandle, g_VertHandle);
            glAttachShader(g_ShaderHandle, g_FragHandle);
            glLinkProgram(g_ShaderHandle);

            g_AttribLocationTex = glGetUniformLocation(g_ShaderHandle, "Texture");
            g_AttribLocationProjMtx = glGetUniformLocation(g_ShaderHandle, "ProjMtx");
            g_AttribLocationPosition = glGetAttribLocation(g_ShaderHandle, "Position");
            g_AttribLocationUV = glGetAttribLocation(g_ShaderHandle, "UV");
            g_AttribLocationColor = glGetAttribLocation(g_ShaderHandle, "Color");

            glGenBuffers(1, &g_VboHandle);
            glGenBuffers(1, &g_ElementsHandle);

            glGenVertexArrays(1, &g_VaoHandle);
            glBindVertexArray(g_VaoHandle);
            glBindBuffer(GL_ARRAY_BUFFER, g_VboHandle);
            glEnableVertexAttribArray(g_AttribLocationPosition);
            glEnableVertexAttribArray(g_AttribLocationUV);
            glEnableVertexAttribArray(g_AttribLocationColor);

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
            glVertexAttribPointer(g_AttribLocationPosition, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, pos));
            glVertexAttribPointer(g_AttribLocationUV, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, uv));
            glVertexAttribPointer(g_AttribLocationColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, col));
#undef OFFSETOF

            CreateFontsTexture();

            // Restore modified GL state
            glBindTexture(GL_TEXTURE_2D, last_texture);
            glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
            glBindVertexArray(last_vertex_array);

            return true;
        }

        void CreateFontsTexture() {
            // Build texture atlas
            ImGuiIO& io = ImGui::GetIO();
            unsigned char* pixels;
            int width, height;
            io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);   // Load as RGBA 32-bits for OpenGL3 demo because it is more likely to be compatible with user's existing shader.

            // Upload texture to graphics system
            GLint last_texture;
            glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
            glGenTextures(1, &g_FontTexture);
            glBindTexture(GL_TEXTURE_2D, g_FontTexture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

            // Store our identifier
            io.Fonts->TexID = (void *)(intptr_t)g_FontTexture;

            // Restore state
            glBindTexture(GL_TEXTURE_2D, last_texture);
        }

        void InvalidateDeviceObjects() {
            if (g_VaoHandle) glDeleteVertexArrays(1, &g_VaoHandle);
            if (g_VboHandle) glDeleteBuffers(1, &g_VboHandle);
            if (g_ElementsHandle) glDeleteBuffers(1, &g_ElementsHandle);
            g_VaoHandle = g_VboHandle = g_ElementsHandle = 0;

            if (g_ShaderHandle && g_VertHandle) glDetachShader(g_ShaderHandle, g_VertHandle);
            if (g_VertHandle) glDeleteShader(g_VertHandle);
            g_VertHandle = 0;

            if (g_ShaderHandle && g_FragHandle) glDetachShader(g_ShaderHandle, g_FragHandle);
            if (g_FragHandle) glDeleteShader(g_FragHandle);
            g_FragHandle = 0;

            if (g_ShaderHandle) glDeleteProgram(g_ShaderHandle);
            g_ShaderHandle = 0;

            if (g_FontTexture)
            {
                glDeleteTextures(1, &g_FontTexture);
                ImGui::GetIO().Fonts->TexID = 0;
                g_FontTexture = 0;
            }
        }
    } // end of namespace SFML

} // end of namespace ImGui

namespace
{
    // Rendering callback
    void RenderDrawLists(ImDrawData* draw_data)
    {
        GLint last_program, last_texture;
        glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_SCISSOR_TEST);
        glActiveTexture(GL_TEXTURE0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // Handle cases of screen coordinates != from framebuffer coordinates (e.g. retina displays)
        ImGuiIO& io = ImGui::GetIO();
        float fb_height = io.DisplaySize.y * io.DisplayFramebufferScale.y;
        draw_data->ScaleClipRects(io.DisplayFramebufferScale);

        // Setup orthographic projection matrix
        const float ortho_projection[4][4] =
        {
            { 2.0f/io.DisplaySize.x, 0.0f,                   0.0f, 0.0f },
            { 0.0f,                  2.0f/-io.DisplaySize.y, 0.0f, 0.0f },
            { 0.0f,                  0.0f,                  -1.0f, 0.0f },
            {-1.0f,                  1.0f,                   0.0f, 1.0f },
        };
        glUseProgram(g_ShaderHandle);
        glUniform1i(g_AttribLocationTex, 0);
        glUniformMatrix4fv(g_AttribLocationProjMtx, 1, GL_FALSE, &ortho_projection[0][0]);
        glBindVertexArray(g_VaoHandle);

        for (int n = 0; n < draw_data->CmdListsCount; n++)
        {
            const ImDrawList* cmd_list = draw_data->CmdLists[n];
            const ImDrawIdx* idx_buffer_offset = 0;

            glBindBuffer(GL_ARRAY_BUFFER, g_VboHandle);
            glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)cmd_list->VtxBuffer.size() * sizeof(ImDrawVert), (GLvoid*)&cmd_list->VtxBuffer.front(), GL_STREAM_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ElementsHandle);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)cmd_list->IdxBuffer.size() * sizeof(ImDrawIdx), (GLvoid*)&cmd_list->IdxBuffer.front(), GL_STREAM_DRAW);

            for (const ImDrawCmd* pcmd = cmd_list->CmdBuffer.begin(); pcmd != cmd_list->CmdBuffer.end(); pcmd++)
            {
                if (pcmd->UserCallback)
                {
                    pcmd->UserCallback(cmd_list, pcmd);
                }
                else
                {
                    glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
                    glScissor((int)pcmd->ClipRect.x, (int)(fb_height - pcmd->ClipRect.w), (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
                    glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, GL_UNSIGNED_SHORT, idx_buffer_offset);
                }
                idx_buffer_offset += pcmd->ElemCount;
            }
        }

        // Restore modified state
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glUseProgram(last_program);
        glDisable(GL_SCISSOR_TEST);
        glBindTexture(GL_TEXTURE_2D, last_texture);

    }
} // end of anonymous namespace
