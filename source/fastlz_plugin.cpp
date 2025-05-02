#include <algorithm>
#include <array>
#include <bit>
#include <fastlz.h>
#include <hex/api/content_registry.hpp>
#include <hex/api/plugin_manager.hpp>
#include <hex/helpers/logger.hpp>
#include <hex/plugin.hpp>
#include <pl/api.hpp>
#include <pl/core/evaluator.hpp>
#include <pl/core/token.hpp>
#include <pl/patterns/pattern.hpp>
#include <string>
#include <wolv/types.hpp>

// Browse through the headers in lib/libimhex/include/hex/api/ to see what you
// can do with the API. Most important ones are <hex/api/imhex_api.hpp> and
// <hex/api/content_registry.hpp>

// This is the main entry point of your plugin. The code in the body of this
// construct will be executed when ImHex starts up and loads the plugin. The
// strings in the header are used to display information about the plugin in the
// UI.
using hex::log::debug;

using namespace pl::core;

template <typename T> void push_bytes(std::vector<u8> bytes, const T &data) {
	auto data_bytes = std::bit_cast<std::array<u8, sizeof(T)>>(data);
	for (auto byte : data_bytes) {
		bytes.push_back(byte);
	}
}

IMHEX_PLUGIN_SETUP("FastLZ Plugin", "NathanSnail",
			 "Adds FastLZ decompression to the pattern language") {
	pl::api::Namespace ns = {"fastlz"};
	// compress_to_section(data: []u8, section: std::mem::section) -> void
	hex::ContentRegistry::PatternLanguage::addFunction(
	    ns, "compress_to_section",
	    pl::api::FunctionParameterCount::exactly(2),
	    [](Evaluator *ctx, const std::vector<Token::Literal> &params)
		  -> std::optional<Token::Literal> {
		    auto in_bytes = params[0].toBytes();
		    constexpr size_t min_input_size = 16;
		    in_bytes.resize(std::max(min_input_size, in_bytes.size()));

		    constexpr size_t min_output_size = 66;
		    size_t reserve = std::max(min_output_size, in_bytes.size() * 2);
		    u8 *out_bytes = new u8[reserve];
		    int comp =
			  fastlz_compress(&in_bytes[0], in_bytes.size(), out_bytes);

		    u64 section_id = u64(params[1].toUnsigned());
		    auto &section = ctx->getSection(section_id);

		    push_bytes(section, comp);
		    push_bytes(section, in_bytes.size());
		    for (int i = 0; i < comp; ++i) {
			    section.push_back(out_bytes[i]);
		    }
		    delete[] out_bytes;
		    return std::optional<Token::Literal>();
	    });

	/* decompress_to_section(compressed: std::mem::section,
	 *                       addr: std::mem::section,
	 *                       output: std::mem::section) -> new addr */
	hex::ContentRegistry::PatternLanguage::addFunction(
	    ns, "decompress_to_section",
	    pl::api::FunctionParameterCount::exactly(3),
	    [](Evaluator *ctx, const std::vector<Token::Literal> &params)
		  -> std::optional<Token::Literal> {
		    u64 in_section_id = params[0].toUnsigned();
		    u64 addr = params[1].toUnsigned();
		    u64 out_section_id = u64(params[2].toUnsigned());

		    auto &out_section = ctx->getSection(out_section_id);

		    u32 compressed_size;
		    ctx->readData(addr, &compressed_size, sizeof(compressed_size),
					in_section_id);
		    addr += sizeof(compressed_size);
		    u32 decompressed_size;
		    ctx->readData(addr, &decompressed_size,
					sizeof(decompressed_size), in_section_id);
		    addr += sizeof(decompressed_size);

		    u8 *in_buf = new u8[compressed_size];
		    u8 *out_buf = new u8[decompressed_size];

		    ctx->readData(addr, in_buf, compressed_size, in_section_id);
		    addr += compressed_size;

		    int comp = fastlz_decompress(in_buf, compressed_size, out_buf,
							   decompressed_size);

		    for (int i = 0; i < comp; ++i) {
			    out_section.push_back(out_buf[i]);
		    }

		    delete[] in_buf;
		    delete[] out_buf;
		    return std::optional<Token::Literal>{(u128)addr};
	    });

	hex::ContentRegistry::PatternLanguage::addFunction(
	    ns, "main_section", pl::api::FunctionParameterCount::exactly(0),
	    [](Evaluator *ctx, const std::vector<Token::Literal> &params)
		  -> std::optional<Token::Literal> {
		    return std::optional<Token::Literal>{
			  (u128)pl::ptrn::Pattern::MainSectionId};
	    });
}
