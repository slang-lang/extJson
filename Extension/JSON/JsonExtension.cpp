
// Header
#include "JsonExtension.h"

// Library includes

// Project includes
#include "FromJson.h"
#include "JsonEncode.h"
#include "ToJson.h"

// Namespace declarations


namespace Json {


JsonExtension::JsonExtension()
: AExtension( "extJson", "0.2.1" )
{
    mName = "extJson (using libjsoncpp " + std::string( JSONCPP_VERSION_STRING ) + ")";
}

void JsonExtension::initialize( Slang::Extensions::ExtensionNamespace* /*scope*/ )
{
    // nothing to do here
}

void JsonExtension::provideMethods( Slang::Extensions::ExtensionMethods& methods )
{
    assert( methods.empty() );

    methods.push_back( new FromJson() );
    methods.push_back( new JsonEncode() );
    methods.push_back( new ToJson() );
}


}


extern "C" Slang::Extensions::AExtension* factory( void ) {
	return dynamic_cast<Slang::Extensions::AExtension*>( new Json::JsonExtension() );
}

