#include <iostream>
#include <magnus.hpp>

int main(int argc, char** argv) {
        ZSTDCompressor comp = ZSTDCompressor();

        std::filesystem::path input = argv[1];
        auto compressed = comp.compress(input);

        auto outfile = input.string() + ".zst";
        std::cout << outfile << std::endl;
        std::filesystem::path comp_file = outfile;

        auto decompressed = comp.decompress(comp_file);

        return 0;
}
