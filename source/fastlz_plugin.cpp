#include "pl/api.hpp"
#include "pl/core/evaluator.hpp"
#include "pl/core/token.hpp"
#include "wolv/types.hpp"
#include <algorithm>
#include <fastlz.h>
#include <functional>
#include <hex/api/content_registry.hpp>
#include <hex/api/plugin_manager.hpp>
#include <hex/helpers/logger.hpp>
#include <hex/plugin.hpp>
#include <string>

// Browse through the headers in lib/libimhex/include/hex/api/ to see what you
// can do with the API. Most important ones are <hex/api/imhex_api.hpp> and
// <hex/api/content_registry.hpp>

// This is the main entry point of your plugin. The code in the body of this
// construct will be executed when ImHex starts up and loads the plugin. The
// strings in the header are used to display information about the plugin in the
// UI.
using hex::log::debug;

using namespace pl::core;

template <typename Func>
void register_fastlz_fn(const std::string &name, Func fn) {
	pl::api::Namespace ns = {"fastlz"};
	// *_to_section(data: []u8, section: std::mem::section)
	hex::ContentRegistry::PatternLanguage::addFunction(
	    ns, name, pl::api::FunctionParameterCount::exactly(2),
	    [fn = std::move(fn)](Evaluator *ctx,
					 const std::vector<Token::Literal> &params)
		  -> std::optional<Token::Literal> {
		    auto in_bytes = params[0].toBytes();
		    constexpr size_t min_input_size = 16;
		    in_bytes.resize(std::max(min_input_size, in_bytes.size()));
		    u64 section_id = u64(params[1].toUnsigned());
		    auto &section = ctx->getSection(section_id);
		    constexpr size_t min_output_size = 66;
		    size_t reserve = std::max(min_output_size, in_bytes.size() * 2);
		    u8 *out_buf = new u8[reserve];
		    int comp = fn(&in_bytes[0], in_bytes.size(), out_buf, reserve);
		    for (int i = 0; i < comp; ++i) {
			    section.push_back(out_buf[i]);
		    }
		    delete[] out_buf;
		    debug(std::to_string(comp));
		    return std::optional<Token::Literal>();
	    });
}

IMHEX_PLUGIN_SETUP("FastLZ Plugin", "NathanSnail",
			 "Adds FastLZ decompression to the pattern language") {
	register_fastlz_fn("compress_to_section", [](u8 *in_bytes, size_t in_size,
								   u8 *out_bytes, size_t _) {
		return fastlz_compress(in_bytes, in_size, out_bytes);
	});

	register_fastlz_fn(
	    "decompress_to_section",
	    [](u8 *in_bytes, size_t in_size, u8 *out_bytes, size_t out_size) {
		    return fastlz_decompress(in_bytes, in_size, out_bytes,
						     out_size);
	    });
}
