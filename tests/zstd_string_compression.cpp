#include <fstream>
#include <gtest/gtest.h>
#include <magnus.hpp>
#include <sstream>

// Demonstrate some basic assertions.
TEST(ZSTDCompressor, ZSTDStringCompressionTest) {
        std::ifstream zstdfile("../assets/zstd_compression_test.example.zst");
        std::stringstream expected_output;
        expected_output << zstdfile.rdbuf();

        std::string input =
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
            "suffering even in 'advanced' countries.";
        ZSTDCompressor comp = ZSTDCompressor();

        auto compressed_string = comp.compress(input);
        // Put in a newline so that they're equal
        compressed_string.push_back('\n');

        EXPECT_EQ(compressed_string, expected_output.str());
}
