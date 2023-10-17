#include <fstream>
#include <gtest/gtest.h>
#include <magnus.hpp>
#include <sstream>
// Demonstrate some basic assertions.
TEST(LZ4Compressor, LZ4StringCompressionTest) {
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
	LZ4Compressor compressor = LZ4Compressor();

	auto cmpstr = compressor.compress(input);
	auto dmpstr = compressor.decompress(cmpstr, input.size());
	// Compare decompressed string with input to check if they are the same
	EXPECT_EQ(dmpstr, input);
	// Check is compressed string size is less than input
	EXPECT_LT(cmpstr.size(), input.size());
}
