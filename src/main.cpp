#include <iostream>
#include <map>
#include <Plist.hpp>

using namespace std;

const char * path = "d:\\develop\\mapg_last\\bin_full\\res\\levels\\level_1_1.plist ";

int main()
{
    std::map<std::string, boost::any> dict;

    try {
        Plist::readPlist(path, dict);
    } catch (...) {
        std::cout << "Can't read plist:\n" << path;
        return 0;
    }

    auto layersIt = dict.find("Layers");

    if (layersIt != dict.end()) {
        const Plist::array_type * layersPtr = boost::any_cast<Plist::array_type>(&(layersIt->second));
         if (!layersPtr) {
             std::cout << "There are no Layers in plist:\n" << path;
             return 0;
         }

         for (size_t i = 0; i < layersPtr->size(); ++i) {
            const Plist::array_type * layerPtr = boost::any_cast<Plist::array_type>(&layersPtr->at(i));
            if (!layerPtr) {
                std::cout << "Can't read layer #" << i << '\n';
                continue;
            }
            std::cout << "Layer #" << i << ":\n";
            for (size_t j = 0; j < layerPtr->size(); ++j) {
                const Plist::dictionary_type * chip = boost::any_cast<Plist::dictionary_type>(&layerPtr->at(j));
                if (!chip) {
                    std::cout << "Can't read chip #" << j << '\n';
                    continue;
                }

                auto posIt = chip->find("Position");
                if (posIt == chip->end()) {
                    std::cout << "There are no Position in chip #" << j;
                    continue;
                }
                const Plist::string_type * pos = boost::any_cast<Plist::string_type>(&posIt->second);
                std::cout << "chip #" << j << " pos: " << *pos << '\n';
            }

         }
    }

    return 0;
}
