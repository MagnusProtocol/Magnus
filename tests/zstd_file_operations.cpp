#include <fstream>
#include <gtest/gtest.h>
#include <magnus.hpp>
#include <sstream>

// Demonstrate some basic assertions.
TEST(ZSTDCompressor, ZSTDFileCompressionAndDecompressionTest) {

        std::string expected_output =
            "The Industrial Revolution and its consequences have been a "
            "disaster for the human race. They have greatly increased the "
            "life-expectancy of those of us who live in 'advanced' countries, "
            "but they have destabilized society, have made life unfulfilling, "
            "have subjected human beings to indignities, have led to "
            "widespread psychological suffering (in the Third World to "
            "physical suffering as well) and have inflicted severe damage on "
            "the natural world. The continued development of technology will "
            "worsen the situation. It will certainly subject human beings to "
            "greater indignities and inflict greater damage on the natural "
            "world, it will probably lead to greater social disruption and "
            "psychological suffering, and it may lead to increased physical "
            "suffering even in 'advanced' countries.\n";

        ZSTDCompressor comp = ZSTDCompressor();

        std::filesystem::path input = "../assets/to_compress_example.txt";
        auto compressed = comp.compress(input);

        std::filesystem::path comp_file = input.string() + ".zst";

        auto decompressed = comp.decompress(comp_file);

        std::ifstream zstdfile("../assets/to_compress_example.txt.decomp");
        std::stringstream decompressed_string;
        decompressed_string << zstdfile.rdbuf();

        EXPECT_EQ(expected_output, decompressed_string.str());
}
