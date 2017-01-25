#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>

namespace sf
{
    class Event;
    class Window;
}

namespace ImGui
{
    namespace SFML
    {
        void Init(sf::Window& target);
        void ProcessEvent(const sf::Event& event);
        void Update(sf::Window& window, sf::Time dt);
        bool CreateDeviceObjects();
        void CreateFontsTexture();
        void Update(const sf::Vector2i& mousePos, const sf::Vector2f& displaySize, sf::Time dt);
        void Shutdown();
    }
}
