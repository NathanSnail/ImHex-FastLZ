#include "pl/api.hpp"
#include "pl/core/token.hpp"
#include <hex/api/content_registry.hpp>
#include <hex/api/plugin_manager.hpp>
#include <hex/helpers/logger.hpp>
#include <hex/plugin.hpp>
#include <iostream>
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

IMHEX_PLUGIN_SETUP("FastLZ Plugin", "NathanSnail",
			 "Adds FastLZ decompression to the pattern language") {
	pl::api::Namespace ns = {"fastlz"};
	hex::ContentRegistry::PatternLanguage::addFunction(
	    ns, "debug_type", pl::api::FunctionParameterCount::exactly(1),
	    [](Evaluator *evaluator, const std::vector<Token::Literal> &params)
		  -> std::optional<Token::Literal> {
		    debug(std::to_string((int)params[0].getType()));
		    return std::optional<Token::Literal>();
	    });
	hex::ContentRegistry::PatternLanguage::addFunction(
	    // size buffer section
	    ns, "to_section", pl::api::FunctionParameterCount::exactly(2),
	    [](Evaluator *evaluator, const std::vector<Token::Literal> &params)
		  -> std::optional<Token::Literal> {
		    debug("Got called");
		    auto compressed_bytes = params[0].toBytes();
		    std::string x = "";
		    for (auto y : compressed_bytes) {
			    x += std::to_string(y) + ", ";
		    }
		    debug(x);
		    return std::optional<Token::Literal>();
	    });
}
