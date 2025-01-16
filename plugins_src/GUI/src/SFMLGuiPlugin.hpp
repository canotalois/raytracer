/*
 * ᗧ···ᗣ···ᗣ··
*/

#include <SFML/Graphics.hpp>
#include "../abstract/AGuiPlugin.hpp"

class SFMLGuiPlugin : public AGuiPlugin {
public:
    SFMLGuiPlugin() = default;

    void initialize() override {

    }

    void execute() override {

    }

    void createWindow(int width, int height, const char* title) override;

    void drawPixels(const uint8_t* pixels, unsigned int width, unsigned int height, unsigned int x, unsigned int y) override;

    void display() override;

    bool windowIsOpen() override;

    void handleEvents() override;

private:
    sf::RenderWindow window;
    sf::Texture texture;
    sf::Sprite sprite;
    std::mutex windowMutex;
    std::thread guiThread;

    static KeyType mapKey(sf::Keyboard::Key key) {
        switch (key) {
            case sf::Keyboard::Z: return KeyType::Z;
            case sf::Keyboard::W: return KeyType::W;
            case sf::Keyboard::S: return KeyType::S;
            case sf::Keyboard::A: return KeyType::A;
            case sf::Keyboard::D: return KeyType::D;
            case sf::Keyboard::Q: return KeyType::Q;
            case sf::Keyboard::E: return KeyType::E;
            case sf::Keyboard::Escape: return KeyType::ESC;
            default: return KeyType::Unknown;
        }
    }
};
